#include <string.h>
#include "bsp.h"
#include "mrfi.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_frame.h"
#include "nwk.h"
#include "MAX_7313.h"
#include "AOA_msg.h"

#ifndef APP_AUTO_ACK
#error ERROR: Must define the macro APP_AUTO_ACK for this application.
#endif

extern  short CAL_SEG_START;// Flash memory addresses
extern  short CAL_STALL;  // symbold address defined in the linker file 
extern  short CAL_CRUISE; // located in flash Info memory segment D -- must be short

/* reserve space for the maximum possible peer Link IDs */

// WARNING -- The compiler does not initialize the arrey below --
// init = {0,} only inits the first location to 0 , not the entire array
// init a number of locations is dangereous since we don't know how 
// long the array is
// needs a for-loop init 

static linkID_t sLID[NUM_CONNECTIONS] = {0,0};
static uint8_t  sNumCurrentPeers = 0;

/* work loop semaphores */
static volatile uint8_t sPeerFrameSem = 0;
static volatile uint8_t sJoinSem = 0;

static ioctlLevel_t pwr_lvl;
uint8_t   msg[MAX_APP_PAYLOAD]; // fromsmpl_nwk_config.dat -- 10 bytes

#define wrt_flash
#ifdef wrt_flash

// Functions to write calibartion values into info flash memory segments
static void 
setCPU_FLASH_clock ( void )
{
    // if Protected info segment (SegmentA) containg the RC oscillator cal values is erased
    // lock up CPU here... Do not proceed to flash as timing would be completely wrong
    if ( CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF )
    {
        BSP_Init(); 
        BSP_TURN_ON_LED2();
        while(1)			// lock up
        ;
    }
    WDTCTL = WDTPW + WDTHOLD;        // stop Watchdog 
    BCSCTL1 = CALBC1_1MHZ;           // Set DCO to 1MHz 
    DCOCTL = CALDCO_1MHZ;            // for FLASH operations 
    FCTL2 = FWKEY + FSSEL0 + FN1;    // MCLK/3 for Flash Timing Generator   
}

// this erases the entire memory segment of where the address is located
static void 
erase_FlashInfoMem (void * addr)
{
   unsigned short * ptr;
   
   ptr =  (unsigned short *)addr;
   setCPU_FLASH_clock(); 
 
   FCTL1 = FWKEY + ERASE;   // Set Erase  bit
   FCTL3 = FWKEY;           // Clear LOCK bits
   *ptr = 0;              // Dummy write to start erase
   FCTL1 = FWKEY;           // Clear ERASE bit
   FCTL3 = FWKEY + LOCK;    // Set LOCK bit    
   
    BCSCTL1 = 0x8d;      // set clock back to normal after Flash writes
    DCOCTL = 0x6b; 
  
}
static void 
wrt_FlashInfoMem(void * addr, unsigned short dat)
{
    unsigned short * ptr;
       
    ptr =  (unsigned short *)addr;
    
    if (* ptr != 0xffff)     // protect FLASH against multiple writes 
        return ;
        
    setCPU_FLASH_clock();  
     
    FCTL3 = FWKEY ;                 // Clear LOCK bit
    FCTL1 = FWKEY + WRT;            // Set WRT bit for write operation
    *ptr = dat;                     // Write the word
    FCTL1 = FWKEY;                  // Clear WRT bit
    FCTL3 = FWKEY + LOCK;           // Set LOCK  bit    
    
    BCSCTL1 = 0x8d;      // set clock back to normal after Flash writes
    DCOCTL = 0x6b; 
      
}


static void  
ProgramdummyFlashCalValues( void )
{
	
    erase_FlashInfoMem(&CAL_SEG_START); // This erases the entire Info segment
	
    wrt_FlashInfoMem(&CAL_STALL, 900 ); // only writes if location is erased
    wrt_FlashInfoMem(&CAL_CRUISE, 0); // ""
    
}

#endif 


/* Runs in ISR context. Reading the frame should be done in the */
/* application thread not in the ISR thread. */
static uint8_t 
RX_Frame_IndicationCallBack(linkID_t linkID)
{
  if (linkID)
     sPeerFrameSem++;
   else
     sJoinSem++;

  /* leave frame to be read by application. */
  return 0;
}



void main (void)
{
// this is display end	
	
  freqEntry_t Freq;
 // smplStatus_t ret;
  AOA_msg * p = (AOA_msg * ) msg;    
  bspIState_t intState;
  // volatile short aoa , cas;
  volatile unsigned short tmp;
  ioctlRadioSiginfo_t signal_info;
  unsigned char display_RSSI_only = 0; 
  unsigned char Cal_mode =0;
  short i;
  volatile smplStatus_t ret_stat;
  unsigned char sw1, sw2;

 
  BSP_Init();

  BSP_TURN_ON_LED2(); // the red one
  
 // ProgramdummyFlashCalValues();
  
   // special mode to display the rssi on the AOA led bar display 
  if ( BSP_BUTTON1() )
    display_RSSI_only = 1;

  MAX7315_Init( 14 ); // MAX brightness
  MAX7313_Read_Switches(&sw1, &sw2);
  
  if (sw1 || CAL_STALL < 0 || CAL_CRUISE < 0) // calibration mode 
  {
  	 erase_FlashInfoMem(&CAL_SEG_START); // This erases the entire Info segment
  	 Cal_mode =1;
  	 for (i =1; i < 10 ; i++ ) // some blinking to indicate Calibration mode
  	 {
	  	MAX7315_DisplayDot(1, 1);
		BSP_Delay(64000);  // usecs
		MAX7315_DisplayDot(12, 1);
		BSP_Delay(64000);  // usecs
  	 }
	
  }
  else 
  {
	  // led selftest 
	  for (i =1; i < NumLEDs+1 ; i++ )
	  {
	  	MAX7315_DisplayDot(i, 0);
	  	BSP_Delay(64000);  // usecs
	  	BSP_Delay(64000);  // usecs
	  }
	  for (; i>=0 ; i--)
	  {
	  	MAX7315_DisplayDot(i, 1);
	  	BSP_Delay(64000);  // usecs
	  	BSP_Delay(64000);  // usecs
	  }
  }

 
 // now init the simpliciti stack
  ret_stat = SMPL_Init(RX_Frame_IndicationCallBack);       // Supplies the callback functions address to the Init
  
  // This doesn't work unless Freq_agility is turned on 
  Freq.logicalChan = 3; 
  ret_stat = SMPL_Ioctl( IOCTL_OBJ_FREQ, IOCTL_ACT_SET, &Freq);
    
  pwr_lvl = IOCTL_LEVEL_2; //set tx powet to max
  ret_stat = SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SETPWR, &pwr_lvl  );
 
 
  BSP_TURN_OFF_LED2();
   

    

  /* main work loop */
  while (1)
  {
    /* Wait for the Join semaphore to be set by the receipt of a Join frame from a
     * device that is an End Device.
     *
     */
    if (sJoinSem && (sNumCurrentPeers < NUM_CONNECTIONS)) // NUM_CONNECTIONS  set to 1 in nwk_config.dat
    {
        BSP_TURN_ON_LED2();   // Indicate join request 
        // listen for a new connection 
        if (SMPL_SUCCESS == SMPL_LinkListen(&sLID[sNumCurrentPeers]))
        {
            sNumCurrentPeers++;  
        }

        BSP_ENTER_CRITICAL_SECTION(intState);
        sJoinSem--;
        BSP_EXIT_CRITICAL_SECTION(intState);
        
       	BSP_Delay(64000);  // usecs
       	BSP_Delay(64000);  // usecs
        BSP_TURN_OFF_LED2();
    }

   
    
    /* Have we received a frame on one of the ED connections?
     * No critical section -- it doesn't really matter much if we miss a poll
     */
    if (sPeerFrameSem)
    {
      short    rssi;
      uint8_t   len;
      const uint8_t   peer =0;
	
	
	// only one peer,  the first linked is allowed in the Join handling above 
      /* process all frames waiting */
      BSP_TURN_ON_LED1(); 
      //for (peer=0; peer < sNumCurrentPeers; peer++)
      if (sLID[peer])	// if peer 0 has been linked then receive its frame
      {
        if (SMPL_SUCCESS == SMPL_Receive(sLID[peer], msg, &len))
        {
        
          BSP_ENTER_CRITICAL_SECTION(intState);
          sPeerFrameSem--;
          BSP_EXIT_CRITICAL_SECTION(intState);
        
          
          if (display_RSSI_only )
          {
            MAX7315_DisplayDot(0, 1);
            signal_info.lid =sLID[peer];            
            SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SIGINFO, &signal_info);
            rssi =  signal_info.sigInfo.rssi; // reduce to fit 12 bit led display 
            // rssi is in -db -- lowest signal that still receives a packetis about -100
            // all the way close in  about -38
            // so I first add 100 to get the numbers in the positive range 0-60
            // then divide by 5 (4) to get it in the 12 bit led range 
            rssi +=100; // 
            rssi /=3;     // adjust range to fit 12 bit led bar
			if (rssi <0 )
				rssi =0;          
            if (rssi > 12 ) 
            	rssi = 12; // limit -- top most red led == max signal
           //   rssi = 13 - rssi;     // invert
           
            MAX7315_DisplayDot(rssi, 1);
          }
          else
          {
          	if (Cal_mode)
          	{
           		  MAX7315_DisplayDot(8, 1);
          		  MAX7313_Read_Switches(&sw1, &sw2);
          		  if (sw1)
          		  {
          			wrt_FlashInfoMem(&CAL_CRUISE, p->AOA );
          		  }
          		  else if (sw2)
          		  {
          		  	wrt_FlashInfoMem(&CAL_STALL, p->AOA ); // only writes if location is erased
          		  }
          		
          		if (CAL_STALL >0 && CAL_CRUISE >0 )
          			Cal_mode =0;
          	}
          	else
          	{
	            //tmp = (p->p_AoaAdc * 1000l) /p->p_RamAdc; 
	            // the divisor below needs to be calibrated so that the display shows top red at the max angle of attack. 
	            short tmp_aoa = p->AOA - CAL_CRUISE;
	            tmp_aoa /= ( (CAL_STALL - CAL_CRUISE )/9);
	            tmp_aoa++;
	            MAX7315_DisplayDot(tmp_aoa, 0);
	          	}
          }

        }
      }
      BSP_TURN_OFF_LED1(); 
    }
  }

}






