/***************************************************************************************************
ExploreEmbedded Copyright Notice
****************************************************************************************************
* File:   01-TaskSwitching
* Version: 15.0
* Author: ExploreEmbedded
* Website: http://www.exploreembedded.com/wiki
* Description: File contains the free rtos example to demonstarte the task switching.

This code has been developed and tested on ExploreEmbedded boards.
We strongly believe that the library works on any of development boards for respective controllers.
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider
buying the ExploreEmbedded boards.

The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php

EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION
RELATED TO UPDATES.


Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies
and that both those copyright notices and this permission notice appear in supporting documentation.
**************************************************************************************************/

#include <Arduino_FreeRTOS.h>
#include <Wire.h> 
#include <LiquidCrystal.h>


#define DHT_ER  0
#define DHT_OK  1
#define DHT_DATA  13

byte HNhietDo, LNhietDo, HDoAm, LDoAm;
byte DHT_GetTemHumi(byte &tem1, byte &tem2, byte &humi1, byte &humi2);
//------------------------------------------------------------------
const byte add_DS1307 = 0x68;
const byte byte_data = 7;
int second, minute, hour, day, wday, month, year;
int giay, phut, gio, thu, ngay, thang, nam;
//----------------------------------------------------------------------
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int relay_dao_trung = 12;
int relay_den_nhiet_do = 11;
int relay_den_do_am = 10;
int relay_quat_hut = 9;



void setup()
{

	Serial.begin(9600);
	Serial.println(F("In Setup function"));

	xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
	xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
	xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);

	Wire.begin();
	setTime(5, 59, 55, 1, 28, 5, 17);// 00h:00p:00s // thu-00n-00t-0000N
	lcd.begin(16, 4);
	pinMode(relay_dao_trung, OUTPUT);
	pinMode(relay_den_nhiet_do, OUTPUT);
	pinMode(relay_den_do_am, OUTPUT);
	pinMode(relay_quat_hut, OUTPUT);
}


void loop()
{
	// DO nothing
}


static void MyTask1(void* pvParameters)
{
	while (1)
	{
		Serial.println(F("Task1"));
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

static void MyTask2(void* pvParameters)
{
	while (1)
	{
		Serial.println(F("Task2"));
		vTaskDelay(150 / portTICK_PERIOD_MS);
	}
}

static void MyIdleTask(void* pvParameters)
{
	while (1)
	{

		Wire.beginTransmission(add_DS1307);
		Wire.write((byte)0x00);
		Wire.endTransmission();
		Wire.requestFrom(add_DS1307, byte_data);

		giay = bcd2dec(Wire.read() & 0x7f);
		phut = bcd2dec(Wire.read());
		gio = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
		thu = bcd2dec(Wire.read());
		ngay = bcd2dec(Wire.read());
		thang = bcd2dec(Wire.read());
		nam = bcd2dec(Wire.read());
		nam = nam + 2000;

	}
}

int bcd2dec(byte num)
{
	return ((num / 16 * 10) + (num % 16));
}
int dec2bcd(byte num)
{
	return ((num / 10 * 16) + (num % 10));
}