/*
 TUTORIAL CODE AVR FOR ATMEGA 32 .
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#include "myLCD.h"

#ifndef cbi
	#define cbi(port, bit) 	   (port) &= ~(1 << (bit))
#endif
#ifndef sbi
	#define sbi(port, bit) 	   (port) |=  (1 << (bit))
#endif

// định nghĩa các đường điều khiển motor

#define MOTOR_DDR DDRD
#define MOTOR_PORT PORTD
#define MOTOR_DIR 6
#define MOTOR_EN 7
#define Sampling_time      25   // thời gian lấy mẫu
#define inv_Sampling_time  40   // 1/thơi gian lấy mẫu
#define PWM_Period         8000 // 8000 cycles = 1ms , f = 8 MHz

volatile long int Pulse , pre_Pulse;

volatile long int rSpeed , // for speed control
					 Err ,
			     pre_Err ,
				 Kp = 8  ,
				 Kd = 10 ,
				 Ki = 1  ;

volatile long int pPart = 0, // PID gains
				  iPart = 0,
				  dPart = 0;

volatile long int Ctrl_Speed = 5 ; // vận tốc điều khiển

volatile long int Output ;

volatile unsigned char sample_count = 0;

//--------------------PID----------------------
void Motor_Speed_PID (long int des_Speed)
{
	rSpeed = Pulse - pre_Pulse ; // tinh van toc trong sampling time
	pre_Pulse = Pulse ; 		 // lưu lại giá trị Pulse : so xung
	Err = des_Speed - abs(rSpeed);// tinh error lỗi

	// ----------các thành phần của PID ------------
	pPart = Kp * Err ;
	dPart = Kd * ( Err - pre_Err ) * inv_Sampling_time ;
	iPart = iPart + ( Ki * Sampling_time * Err / 1000 ) ;
   Output = Output + ( pPart + dPart + iPart ) ;
    // công thức được biến đổi vì là điều khiển vận tốc

	// saturation

	if ( Output >= PWM_Period )
	{
		Output = PWM_Period - 1;
	}

	if ( Output <= 0 )
	{
		Output = 1;
	}
	OCR1A = Output ; // gan duty cycles cho OCR1A : update PWM
	pre_Err = Err ; // lưu lại giá trị Err
}

int main()
{
	const char dis[5] ; // biến tạm hiển thị LCD
	Pulse = 0;
	// encoder va cac chan nhan toc do
	DDRB = 0x00 ; // set PORTB
	PORTB = 0xFF ; // điện trở kéo lên
	// Motor
	MOTOR_DDR = 0xF0;
	sbi(MOTOR_PORT,MOTOR_DIR);
	// ngat ngoai cho encoder
	MCUCSR |= ( 0 << ISC2 ); // ngat INT2 là ngắt cạnh xuống
	GICR |= ( 1 << INT2 ) ;  // Enable INT2
	// ----------------------------------------
	// dung timer 2 làm bộ định thì 25ms sampling time
	TCCR2 = ( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 );
	// CS22 = 1 , CS21 = 1 , CS20 = 1  Prescaler = 1024
	TCNT2 = 60 ;
	// gán giá trị khởi tạo cho T/C2 để được 25ms (trường hợp f = 8 MHz )
	TIMSK = ( 1 << TOIE2 ); // cho phep ngắt khi có tràn T/C2
	//---------------------------------------------------
	// dùng time 1 làm PWM generator , Fast PWM mode 114 : ICR1 chua time period
	TCCR1A = ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM11 );
	TCCR1B = ( 1 << WGM13 ) | ( 1 << WGM12 ) | ( 1 << CS10 );
	//----------------------------------------------------------
	//---------------- LCD -----------------------
	init_LCD();
	clr_LCD();
	print_LCD("Tutorial AVR 32");
	move_LCD(2,1);
	print_LCD("DC servo Motor");
	_delay_ms(1000);
	clr_LCD();
	print_LCD("Actual : /25ms");
	move_LCD(2,1);
	print_LCD("Desired : /25ms");
	//------------------ LCD -----------------
	// khoi dong gia tri PWM
	OCR1A = 1 ;
	ICR1 = PWM_Period ;
	sbi(MOTOR_PORT,MOTOR_EN); // khoi dong motor
	sei();
	//-----------------------------------------
	while (1)
	{
		if (sample_count >= 10 ) // hien thi moi 250ms
		{
			Ctrl_Speed = (PINB >> 4 ) & 0x0F; // doc switches
			//------- in van toc hien tai --------
			sprintf(dis ,"%i" , rSpeed );
			move_LCD( 1 , 9 );
			print_LCD(" ");
			move_LCD( 1 , 9 );
			print_LCD(dis);
			//------- in van toc mong muon --------
			sprintf(dis ,"%i" , Ctrl_Speed );
			move_LCD( 2 , 9 );
			print_LCD(" ");
			move_LCD( 2 , 9 );
			print_LCD(dis);
			sample_count = 0;
		}
	}
}

ISR (TIMER2_OVF_vect)
{
	TCNT2 = 60 ;
	sample_count ++ ;
	Motor_Speed_PID(Ctrl_Speed);
}
ISR (INT2_vect)
{
	if(bit_is_set(PINB,0)) Pulse++;
	else Pulse--;
}

















