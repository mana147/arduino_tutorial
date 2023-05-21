#include "SimpleKalmanFilter.h" // thư viện bộ lọc .
//-----------------------------------
SimpleKalmanFilter bo_loc(1, 1, 0.09); 
// hàm dựn của bộ lọc , để 1 /1 / 0.09 là đẹp nhất để bộ lọc bám sát tín hiệu đầu vào 
unsigned int set_OCR1A;
int u_kalman1;
byte brightness = 0;   
const int LED = 3;    // chân 5 hỗ trợ phát xung PWM
//-----------------------------------
void setup()
{
	pinMode(A1,INPUT);
	pinMode(A2, INPUT);
	Serial.begin(9600);
	/*
	TCCR1A = 0;
	TCCR1B = 0;
	// set lai 2 thanh ghi ;
	DDRB |= (1 << PB1);
	// đầu ra pin 9 / PB1 / OC1A / PCINT1
	TCCR1A |= (1 << WGM11) | (1 << COM0A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	TCCR1B |= (1 << CS10) | (1 << CS11);
	
	ICR1 = ;
	OCR1A = 0;
	*/
}
	
void set(unsigned int x)
{
	if (set_OCR1A != x)
	{
		OCR1A = x;
		set_OCR1A = OCR1A;
	}
	else
	{
		return;
	}
}

void loop()
{
	int value1 = analogRead(A1);// đọc giá trị từ chân A1
	value1 = constrain(value1, 0, 500); // giới hạn thông số đọc từ chân cảm biến từ 0 đến 514
	Serial.print(value1); // print qua serial vào máy tính để xem 
	Serial.print(",");    // (",") để cách các số ra xem cho dễ  
	u_kalman1 = bo_loc.updateEstimate(value1); // đưa giá trị vào bộ lọc
	Serial.print(u_kalman1); // in giá trị qua bộ lọc 
	Serial.print(",");
	if (u_kalman1 >= 0) // đưa ra trị đã qua lọc vào sét điều kiện 
	{
		if (u_kalman1 >= '0' && u_kalman1 <= '500') // giới hạn trong khoảng 0 đến 500
		{
			brightness = u_kalman1; // gán giá trị vào một biến khác 
		}
	}
	//set(u_kalman1);
	byte value = map(brightness, 0 , 500, 0 , 225 );// chuyển đổi thang đo từ 0 đến 500
	analogWrite(LED, brightness);
	int value2 = analogRead(A2); // đưa xung ra 
	//Serial.print(brightness); // in ra màn hình 
	//brightness = 0;  // đưa biến về  0 để đọc lại từ đầu còn giá trị vẫn nhận vào bình thường
	Serial.print(value2); // print qua serial vào máy tính để xem 
	Serial.print(",");
	Serial.println(); // cái này khỏi nói ai cũng biết  
	delay(10);
}