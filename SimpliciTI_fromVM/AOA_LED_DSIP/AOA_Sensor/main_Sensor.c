#include <math.h>
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "vlo_rand.h"
#include "math.h"
#include "AOA_msg.h"


#ifndef APP_AUTO_ACK
#error ERROR: Must define the macro APP_AUTO_ACK for this application.
#endif


static linkID_t sLinkID = 0;


// Symbols defined in the link command file 
extern unsigned char NWRK_ADDR_0;
extern unsigned char NWRK_ADDR_1;
extern unsigned char NWRK_ADDR_2;
extern unsigned char NWRK_ADDR_3;

extern unsigned short CAL_ANGL;  // symbold address defined in the linker file 
extern unsigned short CAL_SPEED; // located in flash Info memory segment D



// This ifdef section and a main could be moved out of the app into a different exectble 
// to safe space. Calibration (espectially the MAC address part only needs to be don once 
//
//#define CAL_MODE_ONLY
#ifdef CAL_MODE_ONLY


#define  AngOffs    (524) 
#define  SpeedOffs  (533)

// Functions to write calibartion values into info flash memory segments
void setCPU_FLASH_clock ( void )
{
    // if Protected info segment (SegmentA) containg the RC oscillator cal values is erased
    // lock up CPU here... Do not proceed to flash as timing would be completely wrong
    if ( CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF )
    {
        BSP_Init(); 
        BSP_TURN_ON_LED2();
        while(1)
        ;
    }
    WDTCTL = WDTPW + WDTHOLD;        // stop Watchdog 
    BCSCTL1 = CALBC1_1MHZ;           // Set DCO to 1MHz 
    DCOCTL = CALDCO_1MHZ;            // for FLASH operations 
    FCTL2 = FWKEY + FSSEL0 + FN1;    // MCLK/3 for Flash Timing Generator   
}
void erase_FlashInfoMem (void * addr)
{
   unsigned short * ptr;
   
   ptr =  (unsigned short *)addr;
   setCPU_FLASH_clock(); 
 
   FCTL1 = FWKEY + ERASE;   // Set Erase  bit
   FCTL3 = FWKEY;           // Clear LOCK bits
   *ptr = 0;              // Dummy write to start erase
   FCTL1 = FWKEY;           // Clear ERASE bit
   FCTL3 = FWKEY + LOCK;    // Set LOCK bit    
  
}
void wrt_FlashInfoMem(void * addr, unsigned short dat)
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
      
}

// Function to fabricate a random MAC address and store it in the top of the protected info memory segment
// returns with CPU running in slow clock mode 
void createRandomMACAddress()
{
  unsigned int rand, rand2;
  
  if(NWRK_ADDR_0  ==  NWRK_ADDR_1 == NWRK_ADDR_2 == NWRK_ADDR_3 != 0xFF ) // Already programmed
    return;
    
  setCPU_FLASH_clock();
  do
  {
    rand = TI_getRandomIntegerFromVLO();    // first byte can not be 0x00 of 0xFF
  }
  while( (rand & 0xFF00)==0xFF00 || (rand & 0xFF00)==0x0000 );
  
  rand2 = TI_getRandomIntegerFromVLO();
  
 
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits (LOCKA toggles)
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
   
  NWRK_ADDR_0 =(rand>>8) & 0xFF;            // Write MAC address into protected info segment A
  NWRK_ADDR_1 =rand & 0xFF;
  NWRK_ADDR_2 =(rand2>>8) & 0xFF; 
  NWRK_ADDR_3 =rand2 & 0xFF; 
  
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit ( LockA toggles 
}

void  ProgramFlashCalValues( void )
{

    // check MAC addreess and only program once for the life of the device ever
    createRandomMACAddress(); // create random MAC address and stick it into protected space starting at 0x10f0..

    // TODO: need to actually measure these offsets and then program them 
    erase_FlashInfoMem(&CAL_SEG_START);
    wrt_FlashInfoMem(&CAL_ANGL, AngOffs ); 
    wrt_FlashInfoMem(&CAL_SPEED, SpeedOffs);
   
   BSP_Init(); 
   BSP_TURN_ON_LED1();
    while(1);
}

#endif // CAL_MODE_ONLY


//#define NOT_NEEDED
#ifdef NOT_NEEDED

void 
Setup_TimerA( unsigned int ms_delay )
{
    // 
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = ms_delay*12;                     //  
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode 
}  

void Disable_TimerA( void )
{
    TACCTL0 =0;                             // disable interrupt -- timer
}

void
waitFor_TimerA()
{
	__bis_SR_register(LPM3_bits+GIE);  // LPM3 with interrupts enabled   I.E go to sleep until Timer A wakes up
}
#endif // NOT_NEEDED


void TxData( uint8_t *message, int size )
{
     volatile int rc;
     unsigned short  noAck = 0;
   
     BSP_TURN_ON_LED1();  // Blink green while transmit is in progress
     while ((rc = SMPL_SendOpt(sLinkID,message, size, SMPL_TXOPTION_ACKREQ)) != SMPL_SUCCESS)
     {
		BSP_Delay(120);  // usecs in between tries 
        if (++noAck > 25 )  // if it fails to send more than n times I assume the AP is gone and I go and reboot 
        {
        	BSP_TURN_OFF_LED1();  //  green off
            WDTCTL = 0;        //  reboot -- this will create an Watchdog reset due to invalid Password 
        }
        
        if (noAck > 4 )
        	BSP_TURN_ON_LED2();  // Red On
     }

     BSP_TURN_OFF_LED1();  
     BSP_TURN_OFF_LED2();  // Red On
}     
unsigned short  
ADC_measure( unsigned short In) 
{		
	ADC10CTL1 = In;	// internal ADC osc ~5Mhz, No clock divider, SH from StartConversion bit, Single conversion, straight binary format
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled  -- wait for ADC complete 
	return ADC10MEM;
}

float p_static_hPa;
float p_ram_hPa;
float p_AOA_hPa;
float CAS;
float P_Alt;

 
#define AVERAGE_SAMPLING 400
#define RUNNING_AVERAGE 8 
#define RAM_SENSOR_ADC_OFFSET 525
#define ANGLE_SENSOR_ADC_OFFSET 547

typedef struct tag_running_avg
{
    short P_ANG;
    short P_RAM;
    short P_STAT;
} t_avg_struct;
 
// Todo chcne msg[] to f 



static void 
Measure_loop()
{
  short i;
  static unsigned short r_avg_ndx; // can not intit bceasue this compiler initializes it each time the function enters even though it's static 
  short AOA;
  static t_avg_struct run_avg[RUNNING_AVERAGE] = {0};

  long  p_ram_adc_acc, p_angle_adc_acc;// p_static_adc_acc;  // using a long for average accumulator
  short p_ram_adc, p_angle_adc;// p_static_adc;
  AOA_msg  msg; 
  volatile unsigned short tmp1;


// !! must feed the watchdog !!

// !!  WDTCTL = WDTPW + WDTHOLD;        // stop Watchdog 
  
  ADC10AE0 = 0x17;			  // A0,A1,A2, A4 are analog inputs 
 
  while (1)
  {  // average multiple readings -- summing 10 adc bits into Ulong 
 	 //     BSP_TOGGLE_LED2();
 
      // Sensors output voltage is between 0 and 5V with an offet around 2.5V for no pressure
      // sensors voltage is /2 so for the positive pressure range we see 1.25 to 2.5V on the
      // analog inputs.  The ADC is using it's internal Vref set to 2.5V for full range 
      ADC10CTL0 = SREF_1 + REFON + REF2_5V+ ADC10SHT_3 + ADC10ON + ADC10IE ; // Reference: VRef & GND, Vref =ON, Vref =2.5V, 64 clock S&H, Interrupt on completion

         
	  p_ram_adc_acc = p_angle_adc_acc = 0; //p_static_adc_acc = 0;
      for( i = 0 ; i<AVERAGE_SAMPLING; i++ )
      {
           p_ram_adc_acc    += ( ADC_measure(INCH_0));		// Input P2.0, A0	 device 7002DP +- 2kPa 
 //          p_static_adc_acc += ADC_measure(INCH_1);     // input P2.1, A1    Barometer device 6115 1.5kPa to 115kPa
           p_angle_adc_acc  += ( ADC_measure(INCH_2));     // input P2.2, A2    device 7002DP +- 2kPa
      }
     
      p_ram_adc =  p_ram_adc_acc / AVERAGE_SAMPLING;
 //     p_static_adc = p_static_adc_acc / AVERAGE_SAMPLING;
      p_angle_adc = p_angle_adc_acc / AVERAGE_SAMPLING;
     
     
// todo : use calibrated values from EEPROM -- Make Calibration function     
      p_ram_adc -= RAM_SENSOR_ADC_OFFSET;  // Zero pressure BIAS to make +/- pressure
      p_angle_adc -= ANGLE_SENSOR_ADC_OFFSET;  

     
#ifdef RUNNING_AVERAGE    
// ~~~~~~~~  Sum and compute running average 
   
        if (r_avg_ndx >= RUNNING_AVERAGE) // reset buffer index
            r_avg_ndx = 0;
            
        // fill next position in average buffer    
        run_avg[r_avg_ndx].P_ANG = p_angle_adc;
        run_avg[r_avg_ndx].P_RAM = p_ram_adc;
        r_avg_ndx++;
 //       run_avg[r_avg_ndx++].P_STAT = p_static_adc;
        
        // sum up all the running average samples 
        p_angle_adc_acc = p_ram_adc_acc = 0;//p_static_adc_acc = 0;
        for (i=0; i < RUNNING_AVERAGE; i++ )
        {
            p_angle_adc_acc     += run_avg[i].P_ANG;
            p_ram_adc_acc       += run_avg[i].P_RAM;
            
  //          p_static_adc_acc    += run_avg[i].P_STAT;
        }
        // calc the linear average
        p_ram_adc    = p_ram_adc_acc / RUNNING_AVERAGE;
        p_angle_adc  = p_angle_adc_acc / RUNNING_AVERAGE;
  //      p_static_adc = p_static_adc_acc / RUNNING_AVERAGE; 
        
 #endif       
 // ----  Calculate final values after average of average 
        // calc atmospheric pressure  according to sensors transfer function
        // P in Kpa = ( (Vout/Vcc) + 0.095)/ 0.009;
        // or when already corrected for ratiometric at 5V 
        // P in Kpa  = (Vout+0.475)/0.045;   Or P in Hpa = (Vout+0.475) / 0.0045 

        // sensor adc already compensated input divider and ratiometric to 5V, 10bit ADC
 //       p_static_hPa = ( p_static_adc  * 5.0) / 1024.0;   // calculate the voltage on the sensor  
 //       p_static_hPa += 0.475f;                           // offset 
 //       p_static_hPa /= 0.0045f;                          // scale 
 
        // see http://en.wikipedia.org/wiki/Altimeter
        //PressureAltitude = (1 - Math.Pow((p_Baro_hPa / 1013.25), 0.19026)) * 288.15;
        //PressureAltitude /= 0.00198122;
 //       P_Alt = (1 - pow((p_static_hPa / 1013.25), 0.19026)) * 288.15;  
 //       P_Alt /= 0.00198122;
        
        // calculate calibrated airspeed from pitot pressure
        // http://en.wikipedia.org/wiki/Calibrated_airspeed
        p_ram_hPa = ((float)(p_ram_adc) * 0.0475); // cal factor 0.0475 hPa per adc bit
        CAS = pow((p_ram_hPa / 1013.25) + 1, (2.0 / 7.0)) - 1.0;
        CAS = sqrt(CAS * 5.0) * 761.21;  // 661.48 == knots, 761.21 = miles

          // reduce the angle pressure by the speed to normalize the AOA reading
        if (p_ram_adc > 20 && p_angle_adc > 1)       // only when we are mooving and have a positive AOA
            AOA = (1000L * p_angle_adc) / p_ram_adc; // p_ram and p_angle have the same sensor sensitivity so we can direcly divide 
        else
            AOA = 0;
 
     
    
  
  //````````````````````````````````````transmit the data ~~~~~~~~~~~~~~~~
  
  
  
      // fill the message buffer with the values to transmit  
 	  msg.AOA = AOA; 
      msg.CAS = CAS;
     // msg.p_Alt =  (short) P_Alt;
      msg.p_Alt =0;
	  msg.p_RamAdc = p_ram_adc ;		
	  msg.p_AoaAdc = p_angle_adc ;
	     
      TxData( (uint8_t *)&msg, sizeof(msg));
 
  }
}



/*------------------------------------------------------------------------------
* ADC10 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	ADC10CTL0 &= ~ENC;						// Disable so that control registers can be modified for next measurment.
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

/*------------------------------------------------------------------------------
* Timer A0 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  __bic_SR_register_on_exit(LPM3_bits);        // Clear LPM3 bit from 0(SR)
}
	
	
	
static ioctlLevel_t pwr_lvl;

void main (void)
{  
// this is sensor end (EP)	
    addr_t MacAddr;
    freqEntry_t Freq;
    volatile smplStatus_t ret;
    
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

#ifdef ADC__TEST_CODE
  
  ADC10CTL0 = SREF_1 + REFON + REF2_5V+ ADC10SHT_3 + ADC10ON + ADC10IE ;
  
  ADC10AE0 |= 0x01;                         // P2.0 ADC option select
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  for (;;)
  {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    if (ADC10MEM < 0x1FF)
      P1OUT &= ~0x01;                       // Clear P1.0 LED off
    else
      P1OUT |= 0x01;                        // Set P1.0 LED on
  }
#endif

#ifdef CAL_MODE_ONLY    // Must erase Info memory segment with programmer first 
   ProgramFlashCalValues(   );  // sets Cal values and never returns 
#endif  
  // if the Network address in the Protected memory section is no set yet, then the default address as 
  // file nwk_config.dat is used
  // Symbols NWRK_ADDR_n (aka MAC address) are defined in the linkfile and are in memory starting at 0x10f0

     // Note: The default MAC address as defined in the nwk_config.dat file by "THIS_DEVICE_ADDRESS" is not used
    if (( NWRK_ADDR_0 ==  NWRK_ADDR_1 == NWRK_ADDR_2 == NWRK_ADDR_3) != 0xff )
    {
        MacAddr.addr[0] = NWRK_ADDR_0;
        MacAddr.addr[1] = NWRK_ADDR_1;
        MacAddr.addr[2] = NWRK_ADDR_2;
        MacAddr.addr[3] = NWRK_ADDR_3;
        SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &MacAddr);
    }
    // else use the default MAC address as defined in the nwk_config.dat file 
    

    BSP_Init();	// must be after creatRandomAddress() becuse the clock is set to 1Mhz in there for flash write 
    BSP_TURN_ON_LED1();


     
    /* Keep trying to join (a side effect of successful initialization) until
     * successful. Toggle LEDS to indicate that joining has not occurred.
     */
    // Init joins this device to an access point within range that has the same Join Token and Link Token as this apllication
    // Default DEFAULT_JOIN_TOKEN and DEFAULT_LINK_TOKEN are defined in smpl_nwk_config.dat file.
    // In exchange for a matching join token sendt via the broadcast addreess this device learns the acesspoints address and it's LINK token
#define WAITFOR_LINK
#ifdef WAITFOR_LINK
    while ( SMPL_Init(NULL) != SMPL_SUCCESS )
    {
    
        BSP_TOGGLE_LED2();  // Toggle LEDS while waiting for link
        BSP_TOGGLE_LED1();  
        BSP_Delay(64000);  // usecs
        BSP_Delay(64000);  // usecs

    }
    BSP_TURN_OFF_LED1(); 
    BSP_TURN_OFF_LED2(); 
    
    // radio parameters are in file D:\MSP430_CCS_CC2500_prj\SimpliciTI_fromVM\Components\mrfi\smartrf\CC2500\smartrf_CC2500.h
   
    //channel changing  doesn't work unless frequency agility is defined in the config.dat file 
    Freq.logicalChan = 3; 
    ret = SMPL_Ioctl( IOCTL_OBJ_FREQ, IOCTL_ACT_SET, &Freq);
    
    pwr_lvl = IOCTL_LEVEL_2; // max
    SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SETPWR, &pwr_lvl  );
 
    if (SMPL_Link( &sLinkID) != SMPL_SUCCESS)
    {
        BSP_TURN_ON_LED1();  
        BSP_TURN_ON_LED2(); 
        BSP_Delay(64000);  // usecs
  		BSP_Delay(64000);  // usecs  
  		BSP_Delay(64000);  // usecs
  		BSP_Delay(64000);  // usecs
        WDTCTL = 0;      // reboot -- this will create an Watchdog reset due to invalid Password    
    }
     

#endif
    
    Measure_loop();  // never returns -- except when connections fails and a reboot is issued 

}
