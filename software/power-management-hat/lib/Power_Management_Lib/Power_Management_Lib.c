/*****************************************************************************
  | File        :   Power_Management_Lib.c
  | Author      :   Waveshare team
  | Info        :   Hardware underlying interface
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

#include "Power_Management_Lib.h"

Power_All_State power_all_state;
datetime_t *Time_Pointer[3];
uint32_t keep_time[2];

uint16_t Power_Wait_Time_S = 60;
uint16_t Wait_Boot_Time_S = 90;
uint16_t Shutdown_Time_Ms = 1000;
uint16_t Power_Off_Time_Ms = 8000;

static struct repeating_timer timer;
/******************************************************************************
    function: Timer_Callback
    brief : The Timer Callback Handles STA_LED blink events
    parameter:
        t   : The timer struct
******************************************************************************/
static bool Timer_Callback(struct repeating_timer *t)
{
    // Debug("Repeat at %lld\n", time_us_64());
    static bool State = 0;
    DEV_Digital_Write(LED_STA_PIN, State);
    State = !State;

    return true;
}
/******************************************************************************
    function: Time_To_Int
    brief : Convert time to seconds of the day
    parameter:
        Time   : The datetime_t struct
******************************************************************************/
uint32_t Time_To_Int(datetime_t *Time)
{
    uint32_t Cnt = (Time->hour * 3600) + (Time->min * 60) + (Time->sec);
    return Cnt;
}

/******************************************************************************
    function: Int_To_Time
    brief : Increment the time and convert to datetime_t struct
    parameter:
        Time   : The datetime_t struct
        Inc_time: The number of seconds increased
******************************************************************************/
bool Inc_To_Time(datetime_t *Time, uint32_t Inc_time)
{
    uint32_t last_time;
    uint32_t Cnt = Time_To_Int(Time);
    if (Inc_time > one_day_sec)
    {
        Debug("error:Inc_time too long!!! Inc_time = %d \r\n",Inc_time);
        return 0;
    }
    if (Inc_time < 60)
    {
        Debug("error:Inc_time too short!!! Inc_time = %d \r\n",Inc_time);
        return 0;
    }
    last_time = Cnt + Inc_time;
    if (last_time > one_day_sec)
    {
        Time->day += 1 ;
        last_time -= one_day_sec;
    }
    Time->hour = last_time / 3600 % 24;
    Time->min = last_time / 60 % 60;
    Time->sec = last_time % 60;
    return 1;
}

/******************************************************************************
    function: Check_Runing_State_By_GPIO
    brief : Raspberry PI running status is detected by GPIO
    parameter:
******************************************************************************/
bool Check_Runing_State_By_GPIO(void)
{
    power_all_state.Running_State = DEV_Digital_Read(PI_RUNNING_PIN);
    return power_all_state.Running_State;
}
/******************************************************************************
    function: Check_Runing_State_By_CURRENT
    brief : Raspberry PI running status is detected by CURRENT
    parameter:
        Shutdown_Current    : Current after shutdown
******************************************************************************/
bool Check_Runing_State_By_CURRENT(uint16_t Shutdown_Current)
{
    power_all_state.Vout_Current = INA219_getCurrent_mA(); // Vout_Current in mA
    if (Shutdown_Current > power_all_state.Vout_Current)
    {
        return false;
    }
    return true;
}
/******************************************************************************
    function: Power_State_Crtl
    brief : Turn on or off the power supply according to the state
    parameter:
        State   : Power on when True, power off otherwise
******************************************************************************/
void Power_State_Crtl(bool State)
{
    DEV_Digital_Write(POWER_CTRL_PIN, State);
    power_all_state.Change_State = true;
    power_all_state.Power_State = State;
    Debug("Now Power_State is %d.\r\n", power_all_state.Power_State);
}
/******************************************************************************
    function: Shutdown
    brief : Press the button for a long time to turn off the power or wait for the PI to shutdown
    parameter:
******************************************************************************/
void Shutdown(void)
{
    uint16_t Cnt_Time = 0;
    uint16_t Wait_Time_S = Power_Wait_Time_S;
    DEV_Digital_Write(PI_SHUTDOWN_PIN, 1);
    Debug("Shutdown instructions have been sent.\r\n");
    while (DEV_Digital_Read(PI_RUNNING_PIN) && (Cnt_Time < Wait_Time_S))
    {
        Cnt_Time++;
        DEV_Delay_ms(1000);
    }
    Debug("Raspberry PI has been detected offline and and About to power off.\r\n");
    DEV_Delay_ms(2000);
    DEV_Digital_Write(PI_SHUTDOWN_PIN, 0);
    Power_State_Crtl(false);
}
/******************************************************************************
    function: Wait_For_Boot
    brief : Wait for the Raspberry PI to start up and check by GPIO
    parameter:
******************************************************************************/
bool Wait_For_Boot(void)
{

    uint16_t Cnt_Time;
    uint16_t Wait_Time_S = Wait_Boot_Time_S;
    Power_State_Crtl(true);
    Debug("Wait Raspberry PI boot.\r\n");
    for (Cnt_Time = 0; !DEV_Digital_Read(PI_RUNNING_PIN) && (Cnt_Time < Wait_Time_S * 10); Cnt_Time++)
    {
        DEV_Delay_ms(100);
    }
    if (Cnt_Time >= Wait_Time_S * 10)
    {
        Debug("Raspberry PI was not boot at the specified time.\r\n");
        return false;
    }
    Debug("Raspberry PI boot successfully.\r\n");
    return true;
}
/******************************************************************************
    function: Power_RTC_Set_Alarm
    brief : Set an RTC alarm
    parameter:
        Alarm_Time : Time of the next RTC alarm
******************************************************************************/
bool Power_RTC_Set_Alarm(datetime_t *Alarm_Time)
{
    if (Alarm_Time != NULL)
    {
        PCF85063A_Enable_Alarm();
        PCF85063A_Set_Alarm(*Alarm_Time);
        return true;
    }
    return false;
}
/******************************************************************************
    function: Power_init
    brief : Initialize the components involved in power control
    parameter:
******************************************************************************/
bool Power_init(void)
{
    DEV_Module_Init();
    INA219_begin();
    PCF85063A_Init();
    add_repeating_timer_ms(power_all_state.Power_State ? 200 : 600, Timer_Callback, NULL, &timer);
}
/******************************************************************************
    function: Power_RTC_init
    brief : Initialize the RTC
    parameter:
        RTC_Init_Time : RTC initialization time
******************************************************************************/
bool Power_RTC_init(datetime_t *RTC_Init_Time)
{
    if (RTC_Init_Time != NULL)
    {
        PCF85063A_Reset();
        DEV_Delay_ms(5);
        PCF85063A_Init();
        PCF85063A_Set_All(*RTC_Init_Time);
        return true;
    }
    else
    {
        PCF85063A_Init();
        return true;
    }
    return false;
}
/******************************************************************************
    function: Power_on_by_Period_Time
    brief : GPIO interrupt callback,Handles the RTC alarm event
    parameter:
        gpio    :   The pin that triggers the interrupt
        events  :   The event that triggers the interrupt
******************************************************************************/
void Power_on_by_Period_Time(uint gpio, uint32_t events)
{
    if ((gpio == RTC_INT_PIN) && (events == GPIO_IRQ_EDGE_FALL))
    {
        if (PCF85063A_Get_Alarm_Flag())
        {
            if (power_all_state.Rtc_State == false)
            {
                PCF85063A_Set_Alarm(*Time_Pointer[2]);
                power_all_state.Rtc_State = true;
                power_all_state.Rtc_Change_State = true;
            }
            else
            {
                PCF85063A_Set_Alarm(*Time_Pointer[1]);
                power_all_state.Rtc_State = false;
                power_all_state.Rtc_Change_State = true;
            }
            PCF85063A_Enable_Alarm();
            Debug("Alarm!!! Now Power_State is %d\r\n", power_all_state.Rtc_State);
        }
        else
        {
            Debug("Other RTC INT\r\n");
        }
    }
}

/******************************************************************************
    function: Power_on_by_Period_Time
    brief : GPIO interrupt callback,Handles the RTC alarm event
    parameter:
        gpio    :   The pin that triggers the interrupt
        events  :   The event that triggers the interrupt
******************************************************************************/
void Power_on_by_Cycle(uint gpio, uint32_t events)
{
    datetime_t Next_Alarm_Time;
    if ((gpio == RTC_INT_PIN) && (events == GPIO_IRQ_EDGE_FALL))
    {
        if (PCF85063A_Get_Alarm_Flag())
        {

            PCF85063A_Read_now(&Next_Alarm_Time);
            Inc_To_Time(&Next_Alarm_Time, power_all_state.Rtc_State ? keep_time[1] : keep_time[0]);
            PCF85063A_Set_Alarm(Next_Alarm_Time);
            PCF85063A_Enable_Alarm();
            power_all_state.Rtc_State = !power_all_state.Rtc_State;
            power_all_state.Rtc_Change_State = true;
            Debug("Alarm!!! Now Power_State is %d\r\n", power_all_state.Rtc_State);
        }
        else
        {
            Debug("Other RTC INT\r\n");
        }
    }
}

/******************************************************************************
    function: Power_on_by_Period_Time_Init
    brief : Initialization Period_Time
    parameter:
        Now_Time        :   The Current time
        Power_On_Time   :   The Power On time
        Power_Off_Time  :   The Power Off time
******************************************************************************/
bool Power_on_by_Period_Time_Init(datetime_t *Now_Time, datetime_t *Power_On_Time, datetime_t *Power_Off_Time)
{
    uint32_t Time_Cnt[3];
    Power_RTC_init(Now_Time);

    Time_Pointer[0] = Now_Time;
    Time_Pointer[1] = Power_On_Time;
    Time_Pointer[2] = Power_Off_Time;

    for (uint8_t x = 0; x < 3; x++)
    {
        Time_Cnt[x] = Time_To_Int(Time_Pointer[x]);
    }
    cancel_repeating_timer(&timer);
    DEV_Digital_Write(LED_STA_PIN, 1);
    if (Time_Cnt[1] < Time_Cnt[2])
    {
        if ((Time_Cnt[0] < Time_Cnt[1]) || (Time_Cnt[0] > Time_Cnt[2]))
        {
            PCF85063A_Set_Alarm(*Power_On_Time);
            Shutdown();
        }
        else
        {
            PCF85063A_Set_Alarm(*Power_Off_Time);
            Wait_For_Boot();
        }
    }
    else
    {
        if ((Time_Cnt[0] < Time_Cnt[1]) && (Time_Cnt[0] > Time_Cnt[2]))
        {
            PCF85063A_Set_Alarm(*Power_On_Time);
            Shutdown();
        }
        else
        {
            PCF85063A_Set_Alarm(*Power_Off_Time);
            Wait_For_Boot();
        }
    }
    add_repeating_timer_ms(power_all_state.Power_State ? 200 : 600, Timer_Callback, NULL, &timer);
    PCF85063A_Enable_Alarm();
    power_all_state.Rtc_State = power_all_state.Power_State;
    power_all_state.Rtc_Change_State = false;
    gpio_set_irq_enabled_with_callback(RTC_INT_PIN, GPIO_IRQ_EDGE_FALL, true, Power_on_by_Period_Time);
    return power_all_state.Power_State;
}

bool Power_on_by_Cycle_Init(datetime_t *Now_Time, uint32_t power_on_keep_time, uint32_t power_off_keep_time, bool State)
{
    datetime_t Next_Alarm_Time;
    keep_time[0]=power_on_keep_time;
    keep_time[1]=power_off_keep_time;
    Power_RTC_init(Now_Time);
    PCF85063A_Read_now(&Next_Alarm_Time);
    Inc_To_Time(&Next_Alarm_Time, State ? power_on_keep_time : power_off_keep_time);
    PCF85063A_Set_Alarm(Next_Alarm_Time);
    PCF85063A_Enable_Alarm();
    power_all_state.Rtc_State = State;
    power_all_state.Rtc_Change_State = false;
    cancel_repeating_timer(&timer);
    DEV_Digital_Write(LED_STA_PIN, 1);
    gpio_set_irq_enabled_with_callback(RTC_INT_PIN, GPIO_IRQ_EDGE_FALL, true, Power_on_by_Cycle);
    if (State)
    {
        Wait_For_Boot();
    }
    else
    {
        Shutdown();
    }
    add_repeating_timer_ms(power_all_state.Power_State ? 200 : 600, Timer_Callback, NULL, &timer);
}

/******************************************************************************
    function: Power_Ctrl_By_Period_Time
    brief : The power supply is controlled by RTC time
    parameter:
******************************************************************************/
bool Power_Ctrl_By_Period_Time(void)
{
    if (power_all_state.Rtc_Change_State)
    {
        cancel_repeating_timer(&timer);
        DEV_Digital_Write(LED_STA_PIN, 1);
        if (power_all_state.Rtc_State)
        {
            Wait_For_Boot();
        }
        else
        {
            Shutdown();
        }
        power_all_state.Rtc_Change_State = false;
        
        add_repeating_timer_ms(power_all_state.Power_State ? 200 : 600, Timer_Callback, NULL, &timer);
    }
    return power_all_state.Power_State;
}
/******************************************************************************
    function: Power_Ctrl_By_Period_Time
    brief : The power supply is controlled by Button
    parameter:
******************************************************************************/
bool Power_Ctrl_By_Button(void)
{
    uint32_t Press_Time = 0;

    if (DEV_Digital_Read(PWR_KEY_PIN) == 0)
    {
        DEV_Delay_ms(2);
        cancel_repeating_timer(&timer);
        DEV_Digital_Write(LED_STA_PIN, 1);
        for (Press_Time = 0; !DEV_Digital_Read(PWR_KEY_PIN); Press_Time++)
        {
            DEV_Delay_ms(1);
        }
        Debug("Press_Time = %d ms\r\n", Press_Time);
        if (power_all_state.Power_State)
        {
            if (Press_Time > Power_Off_Time_Ms)
            {
                // Mandatory Shutdown Raspberry Pi.
                Debug("Mandatory Shutdown Raspberry Pi. \r\n");
                Power_State_Crtl(false);
            }
            else if (Press_Time > Shutdown_Time_Ms)
            {
                // Wait Raspberry Pi Shutdown
                Debug("Wait Raspberry Pi Shutdown. \r\n");
                Shutdown();
            }
            else
            {
                // User Program
                power_all_state.Change_State = true;
            }
        }
        else
        {
            // Wait Raspberry Pi boot.
            Debug("Power on Raspberry Pi. \r\n");
            Wait_For_Boot();
        }
    }
    if (power_all_state.Change_State == true)
    {
        power_all_state.Change_State = false;
        cancel_repeating_timer(&timer);
        add_repeating_timer_ms(power_all_state.Power_State ? 200 : 600, Timer_Callback, NULL, &timer);
    }

    return power_all_state.Power_State;
}

/******************************************************************************
    function: Power_State_Get_All
    brief : Gets all current running states
    parameter:
******************************************************************************/
Power_All_State Power_State_Get_All(void)
{
    uint16_t adc_read = DEC_ADC_Read();
    power_all_state.Vin_Voltage = adc_read * 3.3 * (297 / 27) / 4096.0;
    power_all_state.Vout_Voltage = INA219_getBusVoltage_V(); // voltage on V- (load side)
    power_all_state.Vout_Current = INA219_getCurrent_mA();   // Vout_Current in mA
    Check_Runing_State_By_GPIO();
    PCF85063A_Get_Alarm_Flag();
    return power_all_state;
}
