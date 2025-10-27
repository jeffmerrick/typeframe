#include "PCF85063A.h"

static uint8_t decToBcd(int val);
static int bcdToDec(uint8_t val);

const unsigned char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov","Dec"};

void PCF85063A_Init()
{
	// Initiate Normal Mode, RTC Run, NO reset, No correction , 24hr format, Internal load capacitane 12.5pf
	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL;

	DEV_I2C_Write_Byte(PCF85063A_ADDRESS, RTC_CTRL_1_ADDR, Value);
}

void PCF85063A_Reset()
{
	uint8_t Value = RTC_CTRL_1_DEFAULT|RTC_CTRL_1_CAP_SEL|RTC_CTRL_1_SR;
	DEV_I2C_Write_Byte(PCF85063A_ADDRESS, RTC_CTRL_1_ADDR, Value);
}

void PCF85063A_Set_Time(datetime_t time)
{

	uint8_t buf[4] = {RTC_SECOND_ADDR,
					  decToBcd(time.sec),
					  decToBcd(time.min),
					  decToBcd(time.hour)};
	DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 4);
}

void PCF85063A_Set_Date(datetime_t time)
{
	uint8_t buf[5] = {RTC_DAY_ADDR,
					  decToBcd(time.day),
					  decToBcd(time.dotw),
					  decToBcd(time.month),
					  decToBcd(time.year - YEAR_OFFSET)};
	DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 5);
}

void PCF85063A_Set_All(datetime_t time)
{
	uint8_t buf[8] = {RTC_SECOND_ADDR,
					  decToBcd(time.sec),
					  decToBcd(time.min),
					  decToBcd(time.hour),
					  decToBcd(time.day),
					  decToBcd(time.dotw),
					  decToBcd(time.month),
					  decToBcd(time.year - YEAR_OFFSET)};
	DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 8);
}

void PCF85063A_Read_now(datetime_t *time)
{
	uint8_t bufss[7] = {0};
	DEV_I2C_Read_nByte(PCF85063A_ADDRESS, RTC_SECOND_ADDR, bufss, 7);
	time->sec = bcdToDec(bufss[0] & 0x7F);
	time->min = bcdToDec(bufss[1] & 0x7F);
	time->hour = bcdToDec(bufss[2] & 0x3F);
	time->day = bcdToDec(bufss[3] & 0x3F);
	time->dotw = bcdToDec(bufss[4] & 0x07);
	time->month = bcdToDec(bufss[5] & 0x1F);
	time->year = bcdToDec(bufss[6])+YEAR_OFFSET;
}

//Enable Alarm and Clear Alarm flag
void PCF85063A_Enable_Alarm()
{
	uint8_t Value = RTC_CTRL_2_DEFAULT | RTC_CTRL_2_AIE;
	Value &= ~RTC_CTRL_2_AF;
	DEV_I2C_Write_Byte(PCF85063A_ADDRESS,RTC_CTRL_2_ADDR,Value);
}
//Get Alarm flay
uint8_t PCF85063A_Get_Alarm_Flag()
{
	uint8_t Value =DEV_I2C_Read_Byte(PCF85063A_ADDRESS,RTC_CTRL_2_ADDR);
	//printf("Value = 0x%x",Value);
	Value&=RTC_CTRL_2_AF;
	return Value;
}


void PCF85063A_Set_Alarm(datetime_t time)
{

	uint8_t buf[6] ={
		RTC_SECOND_ALARM,
		decToBcd(time.sec)&(~RTC_ALARM),
		decToBcd(time.min)&(~RTC_ALARM),
		decToBcd(time.hour)&(~RTC_ALARM),
		//decToBcd(time.day)&(~RTC_ALARM),
		//decToBcd(time.dotw)&(~RTC_ALARM)
		RTC_ALARM, 	//disalbe day
		RTC_ALARM	//disalbe weekday
	};
	DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 6);
}
void PCF85063A_Read_Alarm(datetime_t *time)
{
	uint8_t bufss[6] = {0};
	DEV_I2C_Read_nByte(PCF85063A_ADDRESS, RTC_SECOND_ALARM, bufss, 7);
	time->sec = bcdToDec(bufss[0] & 0x7F);
	time->min = bcdToDec(bufss[1] & 0x7F);
	time->hour = bcdToDec(bufss[2] & 0x3F);
	time->day = bcdToDec(bufss[3] & 0x3F);
	time->dotw = bcdToDec(bufss[4] & 0x07);
}



// Convert normal decimal numbers to binary coded decimal
static uint8_t decToBcd(int val)
{
	return (uint8_t)((val / 10 * 16) + (val % 10));
}
// Convert binary coded decimal to normal decimal numbers
static int bcdToDec(uint8_t val)
{
	return (int)((val / 16 * 10) + (val % 16));
}