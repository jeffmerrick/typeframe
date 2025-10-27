/*****************************************************************************
  | File        :   Power_Management_Lib.h
  | Author      :   Waveshare team 
  | Info        :   Hardware underlying interface
  ----------------
  | This version:   V1.1
  | Date        :   2022-12-20
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
#ifndef _Power_Management_Lib_H
#define _Power_Management_Lib_H

#include "DEV_Config.h"
#include "INA219.h"
#include "PCF85063A.h"
#include "pico/multicore.h"

#define Power_off() Power_State_Crtl(false)
#define Power_on()  Power_State_Crtl(true)
#define one_day_sec ((24*3600))

typedef struct {
    bool Power_State;
    bool Running_State;
    bool Change_State;
    bool Rtc_State;
    bool Rtc_Change_State;
    float Vout_Voltage;
    float Vout_Current;
    float Vin_Voltage;
    
} Power_All_State;


bool Check_Runing_State_By_GPIO(void);
bool Check_Runing_State_By_CURRENT(uint16_t Shutdown_Current);

void Power_State_Crtl(bool State);

void Shutdown(void);
bool Wait_For_Boot(void);

bool Power_RTC_Set_Alarm(datetime_t *Alarm_Time);

bool Power_init(void);
bool Power_RTC_init(datetime_t *RTC_Init_Time);
bool Power_on_by_Period_Time_Init(datetime_t *Now_Time,datetime_t *Power_On_Time,datetime_t *Power_Off_Time);
bool Power_on_by_Cycle_Init(datetime_t *Now_Time,uint32_t power_on_keep_time,uint32_t power_off_keep_time,bool State);
bool Power_Ctrl_By_Period_Time(void);

bool Power_Ctrl_By_Button(void);

bool Power_On_Period_Time(datetime_t *Power_On_Time);

Power_All_State Power_State_Get_All(void);



#endif