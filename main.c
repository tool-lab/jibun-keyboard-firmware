/*******************************************************************************
  Main Demo File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Main demo file for keyboard project.  Entry point for compiler.

  Description:
    Main demo file for keyboard project.  Entry point for compiler.  Also
    calls the system initailization and handles system issues like calling the
    demo code appropriately and handing device level events from the USB stack.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* Standard C includes */
#include <stdint.h>

/* Microchip library includes */
#include "system.h"
#include "system_config.h"

#include "usb.h"
#include "usb_device_hid.h"

#include "usb_keycode.h"

// Tool Labs
// LED制御はしないため以下はコメントアウト
// #include "app_led_usb_status.h"
#include "app_device_keyboard.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************
volatile signed int SOFCounter = 0;

// Tool Labs
// キーデータ格納配列
unsigned char keyCode[NumberOfSwitches];
unsigned char keyModifier[NumberOfSwitches];
unsigned char shiftedKeyCode[NumberOfSwitches];
unsigned char shiftedKeyModifier[NumberOfSwitches];

// Tool Labs
// キーデータ初期設定
// 1つのキーを2バイトで設定。最初のバイトはキー値、次のバイトは修飾キー値
// プリント基板のキー番号1〜12番まで順番に2バイトずつ定義する

// Shiftキーオフのデータ
// Key1〜Key4
__EEPROM_DATA(KEYCODE_1, MODIFIER_NONE, KEYCODE_2, MODIFIER_NONE, KEYCODE_3, MODIFIER_NONE, KEYCODE_4, MODIFIER_NONE);
// Key5〜Key8
__EEPROM_DATA(KEYCODE_5, MODIFIER_NONE, KEYCODE_6, MODIFIER_NONE, KEYCODE_7, MODIFIER_NONE, KEYCODE_8, MODIFIER_NONE);
// Key9〜Key12
__EEPROM_DATA(KEYCODE_9, MODIFIER_NONE, KEYCODE_0, MODIFIER_NONE, KEYCODE_Delete, MODIFIER_NONE, KEYCODE_ReturnEnter, MODIFIER_NONE);

// Shiftキーオンのデータ
// Key1〜Key4
__EEPROM_DATA(KEYCODE_X, MODIFIER_Command, KEYCODE_C, MODIFIER_Command, KEYCODE_V, MODIFIER_Command, KEYCODE_4, MODIFIER_NONE);
// Key5〜Key8
__EEPROM_DATA(KEYCODE_5, MODIFIER_NONE, KEYCODE_6, MODIFIER_NONE, KEYCODE_7, MODIFIER_NONE, KEYCODE_8, MODIFIER_NONE);
// Key9〜Key12
__EEPROM_DATA(KEYCODE_9, MODIFIER_NONE, KEYCODE_0, MODIFIER_NONE, KEYCODE_Delete, MODIFIER_NONE, KEYCODE_ReturnEnter, MODIFIER_NONE);

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

int main(void)
{
    SYSTEM_Initialize( SYSTEM_STATE_USB_START );
    
    USBDeviceInit();
    USBDeviceAttach();

    while(1)
    {
        SYSTEM_Tasks();

        #if defined(USB_POLLING)
        /* Check bus status and service USB interrupts.  Interrupt or polling
         * method.  If using polling, must call this function periodically.
         * This function will take care of processing and responding to SETUP
         * transactions (such as during the enumeration process when you first
         * plug in).  USB hosts require that USB devices should accept and
         * process SETUP packets in a timely fashion.  Therefore, when using
         * polling, this function should be called regularly (such as once every
         * 1.8ms or faster** [see inline code comments in usb_device.c for
         * explanation when "or faster" applies])  In most cases, the
         * USBDeviceTasks() function does not take very long to execute
         * (ex: <100 instruction cycles) before it returns. */
        USBDeviceTasks();
        #endif

        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to.  So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }

        /* If we are currently suspended, then we need to see if we need to
         * issue a remote wakeup.  In either case, we shouldn't process any
         * keyboard commands since we aren't currently communicating to the host
         * thus just continue back to the start of the while loop. */
        if( USBIsDeviceSuspended()== true )
        {
            //Check if we should assert a remote wakeup request to the USB host,
            //when the user presses the pushbutton.
            if(BUTTON_IsPressed(BUTTON_USB_DEVICE_REMOTE_WAKEUP) == 0)
            {
                //Add code here to issue a resume signal.
            }

            /* Jump back to the top of the while loop. */
            continue;
        }

        /* Run the keyboard demo tasks. */
        APP_KeyboardTasks();
    }//end while
}//end main


/*******************************************************************
 * Function:        bool USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, uint16_t size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  uint16_t size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch((int)event)
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            // Tool Labs
            // LED制御はしないため以下はコメントアウト
            // APP_LEDUpdateUSBStatus();
            if(SOFCounter < 32767)
            {
                SOFCounter++;
            }
            else
            {
                SOFCounter = 0;
            }
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            // APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific handler for suspend events for
            //possible further action (like optionally going reconfiguring the application
            //for lower power states and going to sleep during the suspend event).  This
            //would normally be done in USB compliant bus powered applications, although
            //no further processing is needed for purely self powered applications that
            //don't consume power from the host.
            SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND);
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            // APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific resume from suspend handler (ex: to
            //restore I/O pins to higher power states if they were changed during the 
            //preceding SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND) call at the start
            //of the suspend condition.
            SYSTEM_Initialize(SYSTEM_STATE_USB_RESUME);
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the keyboard
             * demo code. */
            APP_KeyboardInit();
            break;
        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckHIDRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}

/*******************************************************************************
 End of File
*/
