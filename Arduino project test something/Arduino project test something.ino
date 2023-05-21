// ... khai báo thư viện ...
#include <Kalman.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>

// ... khai báo biến và hàm ...

void rpm_fan(); // hàm con tần số servo

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // khai báo mảng Lcd

volatile int Hz_servo = 0; // biến dùng để lưu tần số servo
volatile int speed_servo = 0;  // biến tốc độ 
unsigned long last_millis = 0;

void setup()
{
	attachInterrupt(1, rpm_fan, FALLING);
	lcd.begin(16, 2);
}

void loop()
{
	if (millis() - last_millis >= 1000) // cập nhật trạng thái mỗi giây
	{
		noInterrupts();// tạm ngừng tất cả các interrupts

		speed_servo = Hz_servo * 60; // tính tốc độ quạt

		lcd.print(speed_servo);
		lcd.setCursor(0, 1);
		lcd.print(Hz_servo);

		Hz_servo = 0;
		last_millis = millis();
		interrupts();
	}
}

void rpm_fan()
{
	Hz_servo ++;
}
