/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#include <xc.h>
#include <stdbool.h>
#include "buttons.h"

extern unsigned char keyCode[12];
extern unsigned char keyModifier[12];

/*** Button Definitions *********************************************/

// Tool Labs
// スイッチ定義を追加
// プリント基板のバージョンごとの設定をする

#if defined (STANDARD_VERSION)

#define S1_PORT  PORTCbits.RC0
#define S2_PORT  PORTCbits.RC4
#define S3_PORT  PORTBbits.RB7
#define S4_PORT  PORTCbits.RC1
#define S5_PORT  PORTCbits.RC5
#define S6_PORT  PORTBbits.RB6
#define S7_PORT  PORTCbits.RC2
#define S8_PORT  PORTCbits.RC6
#define S9_PORT  PORTBbits.RB5
#define S10_PORT PORTCbits.RC3
#define S11_PORT PORTCbits.RC7
#define S12_PORT PORTBbits.RB4

#define BUTTON_PRESSED      0
#define BUTTON_NOT_PRESSED  1

#endif

#if defined (LITE_VERSION)

#define S1_PORT  PORTCbits.RC5
#define S2_PORT  PORTCbits.RC4
#define S3_PORT  PORTCbits.RC3
#define S4_PORT  PORTCbits.RC6
#define S5_PORT  PORTCbits.RC7
#define S6_PORT  PORTBbits.RB7
#define S7_PORT  PORTCbits.RC0
#define S8_PORT  PORTCbits.RC1
#define S9_PORT  PORTCbits.RC2
#define S10_PORT PORTBbits.RB4
#define S11_PORT PORTBbits.RB5
#define S12_PORT PORTBbits.RB6

#define BUTTON_PRESSED      0
#define BUTTON_NOT_PRESSED  1

#endif

// Full版はシュミットトリガインバータを入れているのでBポートは1がON、
// Cポートは0がONになるので、BUTTON_PRESSEDは定義しないで
// 判定の部分はハードコードする
#if defined (FULL_VERSION)

#define S1_PORT  PORTCbits.RC5
#define S2_PORT  PORTCbits.RC4
#define S3_PORT  PORTCbits.RC3
#define S4_PORT  PORTCbits.RC6
#define S5_PORT  PORTCbits.RC7
#define S6_PORT  PORTBbits.RB7
#define S7_PORT  PORTCbits.RC0
#define S8_PORT  PORTCbits.RC1
#define S9_PORT  PORTCbits.RC2
#define S10_PORT PORTBbits.RB4
#define S11_PORT PORTBbits.RB5
#define S12_PORT PORTBbits.RB6

#endif

/*********************************************************************
* Function: bool BUTTON_IsPressed(BUTTON button);
*
* Overview: Returns the current state of the requested button
*
* PreCondition: button configured via BUTTON_SetConfiguration()
*
* Input: BUTTON button - enumeration of the buttons available in
*        this demo.  They should be meaningful names and not the names 
*        of the buttons on the silkscreen on the board (as the demo 
*        code may be ported to other boards).
*         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
*
* Output: TRUE if pressed; FALSE if not pressed.
*
********************************************************************/
bool BUTTON_IsPressed(BUTTON button)
{

#if defined (STANDARD_VERSION) || (LITE_VERSION)
    switch(button)
    {
        case BUTTON_S1:
            return ( (S1_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S2:
            return ( (S2_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S3:
            return ( (S3_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S4:
            return ( (S4_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S5:
            return ( (S5_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S6:
            return ( (S6_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S7:
            return ( (S7_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S8:
            return ( (S8_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S9:
            return ( (S9_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S10:
            return ( (S10_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S11:
            return ( (S11_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_S12:
            return ( (S12_PORT == BUTTON_PRESSED) ? true : false);
        case BUTTON_NONE:
            return false;
    }
#endif

// Full版はCポートは0がON、Bポートはシュミットトリガインバータが入っているため
// 1がONになるため、判定はハードコードしてしまっています。
#if defined (FULL_VERSION)
    switch(button)
    {
        case BUTTON_S1:
            return ( (S1_PORT == 0) ? true : false);
        case BUTTON_S2:
            return ( (S2_PORT == 0) ? true : false);
        case BUTTON_S3:
            return ( (S3_PORT == 0) ? true : false);
        case BUTTON_S4:
            return ( (S4_PORT == 0) ? true : false);
        case BUTTON_S5:
            return ( (S5_PORT == 0) ? true : false);
        case BUTTON_S6:
            return ( (S6_PORT == 1) ? true : false);
        case BUTTON_S7:
            return ( (S7_PORT == 0) ? true : false);
        case BUTTON_S8:
            return ( (S8_PORT == 0) ? true : false);
        case BUTTON_S9:
            return ( (S9_PORT == 0) ? true : false);
        case BUTTON_S10:
            return ( (S10_PORT == 1) ? true : false);
        case BUTTON_S11:
            return ( (S11_PORT == 1) ? true : false);
        case BUTTON_S12:
            return ( (S12_PORT == 1) ? true : false);
        case BUTTON_NONE:
            return false;
    }
#endif
    
    return false;
}

/*********************************************************************
* Function: void BUTTON_Enable(BUTTON button);
*
* Overview: Returns the current state of the requested button
*
* PreCondition: button configured via BUTTON_SetConfiguration()
*
* Input: BUTTON button - enumeration of the buttons available in
*        this demo.  They should be meaningful names and not the names
*        of the buttons on the silkscreen on the board (as the demo
*        code may be ported to other boards).
*         i.e. - ButtonIsPressed(BUTTON_SEND_MESSAGE);
*
* Output: None
*
********************************************************************/
void BUTTON_Enable(BUTTON button)
{
    // Tool Labs
    // すべてのポートをデジタルポートに設定
    ANSEL  = 0x00;
    ANSELH = 0x00;

    // Tool Labs
    // すべてのポートを入力ポートに設定
    TRISB  = 0xf0;
    TRISC  = 0xff;
    
// Tool Labs
// 以下は使用予定がないためコメントアウト
#if 0    
    switch(button)
    {
        case BUTTON_S1:
            break;

        case BUTTON_NONE:
            break;
    }
#endif
    
}
