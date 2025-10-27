/*****************************************************************************
  | File        :   MP28167.h
  | Author      :   Waveshare team 
  | Info        :   MP28167 
  ----------------
  | This version:   V1.0
  | Date        :   2022-10-01
  | Info        :

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documnetation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to  whom the Software is
  furished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

******************************************************************************/

#ifndef _MP28167_H_
#define _MP28167_H_

#include "DEV_Config.h"
#include <stdint.h>
#include <stdlib.h> //itoa()
#include <stdio.h>

//MP28167_ADDRESS
#define MP28167_ADDRESS   (0X60)

#define DC_VREF_L_ADDR      (0X00)
#define DC_VREF_H_ADDR      (0X01)
#define DC_VREF_GO_ADDR     (0X02)
#define DC_IOUT_KIM_ADDR    (0X03)
#define DC_CTL1_ADDR        (0X04)
#define DC_CTL2_ADDR        (0X05)
#define DC_STATUS_ADDR      (0X09)
#define DC_INTERRUPT_ADDR   (0X0A)
#define DC_MASK_ADDR        (0X0B)

#define DC_ID1_ADDR         (0X0C)
#define DC_MFR_ID_ADDR      (0X27)
#define DC_DEV_ID_ADDR      (0X28)
#define DC_IC_REV_ADDR      (0X29)

// Registar Overview - CTL1
#define DC_CTL1_EN          (0X80)
#define DC_CTL1_DIS         (0X00)

#define DC_CTL1_OCP_OVP_HIPCUP      (0X40)
#define DC_CTL1_OCP_OVP_LATCH_OFF   (0X00)

#define DC_CTL1_DISCHG_EN   (0X20)
#define DC_CTL1_DISCHG_DIS  (0X00)

#define DC_CTL1_MODE_PWM    (0X10)
#define DC_CTL1_MODE_AUTO   (0X00)

#define DC_CTL1_FREQ_750KHZ (0X04)
#define DC_CTL1_FREQ_500KHZ (0X00)

// Registar Overview - CTL2
#define DC_CTL2_VOUT_COMP_200MV (0XC0)
#define DC_CTL2_VOUT_COMP_120MV (0X80)
#define DC_CTL2_VOUT_COMP_60MV  (0X40)
#define DC_CTL2_VOUT_COMP_0MV   (0X00)

#define DC_CTL2_SS_4MS4         (0X30)
#define DC_CTL2_SS_3MS5         (0X20)
#define DC_CTL2_SS_2MS2         (0X10)
#define DC_CTL2_SS_1MS1         (0X00)

// Registar Overview - STATUS
#define DC_STATUS_PG        (0X80)
#define DC_STATUS_OTP       (0X40)
#define DC_STATUS_OTW       (0X20)
#define DC_STATUS_CC_CV     (0X10)


void MP28167_Config(uint8_t reg,uint8_t config);
void MP28167_Default_Config();


#endif /* INC_MP28167_H_ */
