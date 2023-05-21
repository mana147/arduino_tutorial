#include "SimpleKalmanFilter.h"

SimpleKalmanFilter bo_loc(1,1,0.009);

int u_kalman1;


void setup() 
{
	Serial.begin(9600);
}

void loop() 
{
	int value1 = analogRead(A1);

	u_kalman1 = bo_loc.updateEstimate(value1);
	Serial.print(u_kalman1);

	Serial.println();
}