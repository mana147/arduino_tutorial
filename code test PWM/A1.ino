unsigned int Gia_tri_moi;
void setup()
{
	TCCR1A = 0;
	TCCR1B = 0;
	// RESET lại 2 thanh ghi
	DDRB |= (1 << PB1);
	// Đầu ra PB1 là OUTPUT ( pin 9)
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// chọn Fast PWM, chế độ chọn TOP_value tự do  ICR1
	TCCR1A |= (1 << COM1A1);
	// So sánh thường( none-inverting)
	TCCR1B |= (1 << CS10);
	// P_clock=16mhz
	// mỗi P_clock bằng 1/16mhz= 0.0625 us
	// Hay 16  (P_clock ) bằng  1 us.
	// F_pwm=p_clock/50001=319.993hz
	OCR1A = 8480;
	// Value=8480 , tương đương xung có độ rộng 8484*0.0625us=530us (0.53ms)
	// Value=38550, tương đương xung có độ rộng 38550*0.0625us=2410us (2,41ms)
	ICR1 = 65535;
	// xung răng cưa tràn sau 65536 P_clock, tương đương (6,5 ms)
}

void set(unsigned int x)
{
	if (Gia_tri_moi != x) {
		OCR1A = x;
		Gia_tri_moi = OCR1A;
	}
	else {
		return; // thoát ngay
	}
	// x : 8480 - 38550
	//Độ rộng: 0.53ms - 2.41 ms
	// Góc lệch: 0 ->180 độ
}

void loop()
{
	set(8480); // 0 độ
	delay(2000);
	set(38550); // 180 độ
	delay(2000);
}