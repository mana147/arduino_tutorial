#include "Work_scheduler.h"
#include "Timer.h"


const int ledpin13 = 13;
const int ledpin12 = 12;
const int ledpin11 = 11;
const int ledpin10 = 10;

//----------------------------------------------------

Work_scheduler *job_1;
Work_scheduler *job_2;
Work_scheduler *job_3;
Work_scheduler *job_4;
Work_scheduler *job_5;

//----------------------------------------------------
void bat_tat_led_1(int pin)
{
	analogWrite (pin, 25);
}

void bat_tat_led_2(int pin)
{
	static boolean wasLightedUp = false;
	digitalWrite(pin, !wasLightedUp ? HIGH : LOW);
	wasLightedUp = !wasLightedUp;
}

void bat_tat_led_3(int pin)
{
	static boolean wasLightedUp = false;
	digitalWrite(pin, !wasLightedUp ? HIGH : LOW);
	wasLightedUp = !wasLightedUp;
}

void bat_tat_led_4(int pin)
{
	static boolean wasLightedUp = false;
	digitalWrite(pin, !wasLightedUp ? HIGH : LOW);
	wasLightedUp = !wasLightedUp;
}


void printToSerial() 
{
	static unsigned long counter = 0;
	counter++;
	Serial.print(" i love arduino ");
	Serial.println(counter);
}

//----------------------------------------------------

void setup()
{
	Serial.begin(9600);

	pinMode(ledpin13, OUTPUT);
	pinMode(ledpin12, OUTPUT);
	pinMode(ledpin11, OUTPUT);
	pinMode(ledpin10, OUTPUT);

	Timer::getInstance()->khoi_tao();

	job_1 = new Work_scheduler(ledpin13 ,  , bat_tat_led_1);

	job_2 = new Work_scheduler(ledpin12 , 500UL , bat_tat_led_2);

	job_3 = new Work_scheduler(ledpin11,  250UL, bat_tat_led_3);

	job_4 = new Work_scheduler(ledpin10,  125UL, bat_tat_led_4);

}
//----------------------------------------------------

void loop()
{
	Timer::getInstance()->cap_nhat();

	job_1->tinh_thoi_gian_de_lam_viec();

	job_2->tinh_thoi_gian_de_lam_viec();

	job_3->tinh_thoi_gian_de_lam_viec();

	job_4->tinh_thoi_gian_de_lam_viec();

	Timer::getInstance()->reset_diem();

}
