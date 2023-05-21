#include "SimpleKalmanFilter.h"

SimpleKalmanFilter bo_loc_X(1, 1, 0.09);
SimpleKalmanFilter bo_loc_Y(1, 1, 0.09);

int value_kalman_X;
int value_kalman_Y;

int value_NOT_X;
int value_NOT_Y;

int value_1A;
int value_1B;

int set_balanced_X = 167;
int set_balanced_Y = 150;

int error_X , per_error_X ; // sai số trước và sai số sau 
int error_Y , per_error_Y ;

int dError_X; 
int dError_Y;

float integral_X;
float integral_Y;

float k_p = 2.5;
float k_i = 0.1;
float k_d = 7.5;

int power_X;
int power_Y;


void setup()
{
	Serial.begin(9600);
	pinMode(7, OUTPUT);
	//-----------------
	TCCR1A = 0;
	TCCR1B = 0;
	//-----------------------
	DDRB |= (1 << PB1) | (1 << PB2);
	//-----------------------
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	//-----------------------
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);
	//-----------------------
	TCCR1B |= (1 << CS10);
	//-----------------------
	OCR1A = 0;
	OCR1B = 0;
	//-----------------------
	value_1A = OCR1A;
	value_1B = OCR1B;
	ICR1 = 500;

}

void set_1A_pin_9(unsigned int x) // pin 9
{
	if (value_1A != x)
	{
		OCR1A = x;
		value_1A = OCR1A;
	}
	else
	{
		return;
	}
}

void set_1B_pin_10(unsigned int x) // pin 10
{
	if (value_1B != x)
	{
		OCR1B = x;
		value_1B = OCR1B;
	}
	else
	{
		return;
	}
}

void loop()
{
	int value1 = analogRead(A1);
	int value2 = analogRead(A2);

	value_kalman_X = bo_loc_X.updateEstimate(value1);
	value_kalman_X = constrain(value_kalman_X, 0, 510);
	int tele_K_X = value_kalman_X;

	value_kalman_Y = bo_loc_Y.updateEstimate(value2);
	value_kalman_Y = constrain(value_kalman_Y, 0, 510);
	int tele_K_Y = value_kalman_Y;

	//------------------------------------------------
	
	error_X = set_balanced_X - value_kalman_X ;  // value_kalman_X; // tinh sai số giữa giá trị đặt vào và giá trị đo được
	error_Y = set_balanced_Y - value_kalman_Y ;  // value_kalman_Y;
	
	dError_X = error_X - per_error_X ;
	dError_Y = error_Y - per_error_Y ;

	integral_X = integral_X + error_X * 0.01  ; // i_part = i_part + ( k_i * thời gian lấy mẫu * error ) / 1000
	integral_Y = integral_Y + error_Y * 0.01  ;

	power_X = (error_X * k_p) + (integral_X * k_i) + (dError_X * k_d);
	power_Y = (error_Y * k_p) + (integral_Y * k_i) + (dError_Y * k_d);

	int tele_P_X = power_X ;
	int tele_P_Y = power_Y ;

	
	//--------------------------------------------------

	if ( power_X > 0 )
	{
		digitalWrite(11,HIGH);

		if (power_X > 240 )
		{
			power_X = 240;
		}

		analogWrite(6, power_X);
	}
	else
	{
		power_X = - power_X;

		digitalWrite(11, LOW);

		if (power_X >  240 )
		{
			power_X = 240 ;
		}

		analogWrite(6, power_X);

	}

	if ( value_kalman_X > 350);
	{
		power_X = 0;
		integral_X = 0;
	}

	//------ end X ----------------

	// ---------------------------

	if (power_Y > 0)
	{
		digitalWrite(7, HIGH);

		if (power_Y > 240)
		{
			power_Y = 240;
		}

		analogWrite( 5 , power_Y );
	}
	else
	{
		power_Y = -power_Y;

		digitalWrite(7, LOW);

		if (power_Y >  240)
		{
			power_Y = 240;
		}

		analogWrite( 5 , power_Y );

	}

	if (value_kalman_Y > 350);
	{
		power_Y = 0;
		integral_Y = 0;
	}
	//-------- end Y ----------------
	//-------------------------------

	per_error_X = error_X; // update error value
	per_error_Y = error_Y;

	char text[40];
	//sprintf(text, " %d , %d , %d ,%d ", tele_K_X , tele_K_Y , tele_P_X , tele_P_Y );
	sprintf(text, " %d , %d ", tele_P_X, tele_P_Y);
	Serial.println(text);
}