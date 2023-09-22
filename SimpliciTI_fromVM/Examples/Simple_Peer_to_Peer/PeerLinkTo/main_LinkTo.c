#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"

#include "app_remap_led.h"


static uint8_t  sTxTid=0, sRxTid=0;
static linkID_t sLinkID1 = 0;


#define SPIN_ABOUT_A_SECOND  NWK_DELAY(1000)



void toggleLED(uint8_t which)
{
  if (1 == which)
  {
    BSP_TOGGLE_LED1();
  }
  else if (2 == which)
  {
    BSP_TOGGLE_LED2();
  }
  return;
}

/* handle received frames. */
static uint8_t 
sRxCallback(linkID_t port)
{
  uint8_t msg[2], len, tid;

  /* is the callback for the link ID we want to handle? */
  if (port == sLinkID1)
  {
    /* yes. go get the frame. we know this call will succeed. */
     if ((SMPL_SUCCESS == SMPL_Receive(sLinkID1, msg, &len)) && len)
     {
       /* Check the application sequence number to detect
        * late or missing frames... 
        */
       tid = *(msg+1);
       if (tid)
       {
         if (tid > sRxTid)
         {
           /* we're good. toggle LED in the message */
           toggleLED(*msg);
           sRxTid = tid;
         }
       }
       else
       {
         /* the wrap case... */
         if (sRxTid)
         {
           /* we're good. toggle LED in the message */
           toggleLED(*msg);
           sRxTid = tid;
         }
       }
       /* drop frame. we're done with it. */
       return 1;
     }
  }
  /* keep frame for later handling. */
  return 0;
}


static void   // the transmitter
linkTo()
{
  uint8_t  msg[2];

  toggleLED(2);
	
  while (SMPL_SUCCESS != SMPL_Link(&sLinkID1))
  {
    /* blink LEDs until we link successfully */
    toggleLED(2);
    toggleLED(1);
  }


  /* we're linked. turn off red LED. received messages will toggle the green LED. */
  BSP_TURN_OFF_LED2();
  BSP_TURN_OFF_LED1();


  /* turn on RX. default is RX off. */
  SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0);

  /* put LED to toggle in the message */
  msg[0] = 2;  /* toggle red  in the listener*/
  while (1)
  {
    /* put the sequence ID in the message */
    msg[1] = ++sTxTid;
    BSP_TURN_ON_LED1();
    SMPL_Send(sLinkID1, msg, sizeof(msg));
    NWK_DELAY(50);
    BSP_TURN_OFF_LED1();
    NWK_DELAY(50);
  }
}


void 
main (void)
{
`  BSP_Init();


  /* This call will fail because the join will fail since there is no Access Point 
   * in this scenario. But we don't care -- just use the default link token later. 
   * We supply a callback pointer to handle the message returned by the peer. 
   */
  SMPL_Init(sRxCallback);

 
  /* never coming back... */
  linkTo();

  /* but in case we do... */
  while (1) ;
}

