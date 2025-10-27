/*****************************************************************************
  | File        :   Power_Management_HAT.c
  | Author      :   Typeframe (based on Waveshare)
  | Info        :   Simplified Power Management HAT (B) firmware - Button Control Only
******************************************************************************/
#include "DEV_Config.h"
#include "Power_Management_Lib.h"
#include "MP28167.h"

struct repeating_timer timer;
Power_All_State Read_State;

static bool Timer_Callback(struct repeating_timer *t)
{
    Read_State = Power_State_Get_All();
    // Print power state info to UART (for debugging)
    printf("Power_State : %d\r\n", Read_State.Power_State);
    printf("Running_State : %d\r\n", Read_State.Running_State);
    printf("Vin_Voltage(V) : %.2f\r\n", Read_State.Vin_Voltage);
    printf("Vout_Voltage(V) : %.2f\r\n", Read_State.Vout_Voltage);
    printf("Vout_Current(MA) : %.2f\r\n", Read_State.Vout_Current);
    return true;
}

void Button_Ctr_Init(void)
{
    Power_init();
    MP28167_Default_Config();
    add_repeating_timer_ms(1500, Timer_Callback, NULL, &timer);
}

void Button_Ctr_Loop(void)
{
    Power_Ctrl_By_Button();
}

int main()
{
    Button_Ctr_Init();
    while (true)
    {
        Button_Ctr_Loop();
    }
    return 0;
}
