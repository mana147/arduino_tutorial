#include "SimpleKalmanFilter.h"

SimpleKalmanFilter bo_loc_x(1, 1, 0.009);
SimpleKalmanFilter bo_loc_y(1, 1, 0.009);
SimpleKalmanFilter bo_loc_z(1, 1, 0.009);

int u_kalman1;
int u_kalman2;
int u_kalman3;

int set_balanced_X = 530;
int set_balanced_Y = 530;
int set_balanced_Z = 466;

int error_X , per_error_X ; // sai số trước và sai số sau
int error_Y , per_error_Y ;
int error_Z , per_error_Z ;

int dError_X;
int dError_Y;
int dError_Z;

float integral_X = 0;
float integral_Y = 0;
float integral_Z = 0;


float TGLM = 0.015;
float k3 = 500; 

float k_p = 1;
float k_i = 1;
float k_d = 1;

float k_p_z = 1;
float k_i_z = 0;
float k_d_z = 0;

int power_X;
int power_Y;
int power_Z;
//-----------------------
int en_a = 11;
int inA = 13;
int inB = 12;

int en_b = 10;
int inC = 9;
int inD = 8;
//-----------------------

void setup()
{
  Serial.begin(9600);

  pinMode(en_a, OUTPUT);
  pinMode(inA, OUTPUT);
  pinMode(inB , OUTPUT);

  pinMode(en_b, OUTPUT);
  pinMode(inC, OUTPUT);
  pinMode(inD , OUTPUT);

}

void loop()
{

  int value1 = analogRead(A0);
  int value2 = analogRead(A1);
  int value3 = analogRead(A2);

  u_kalman1 = bo_loc_x.updateEstimate(value1);
  u_kalman1 = constrain( u_kalman1 , 0 , 800 );

  u_kalman2 = bo_loc_y.updateEstimate(value2);
  u_kalman2 = constrain( u_kalman2 , 0 , 800 );

  u_kalman3 = bo_loc_z.updateEstimate(value3);
  u_kalman3 = constrain( u_kalman3 , 0 , 800 );


  //value_kalman_T = constrain(value_kalman_T, 0, 800);
  //------------------------------------------------

  error_X = set_balanced_X - u_kalman1 ;  // value_kalman_X; // tinh sai số giữa giá trị đặt vào và giá trị đo được
  error_Y = set_balanced_Y - u_kalman2 ;   // value_kalman_Y;
  error_Z = set_balanced_Z - u_kalman3 ;

  dError_X = error_X - per_error_X ;
  dError_Y = error_Y - per_error_Y ;
  dError_Z = error_Z - per_error_Z ;

  integral_X = integral_X + error_X * TGLM ; // i_part = i_part + ( k_i * thời gian lấy mẫu * error ) / 1000
  integral_Y = integral_Y + error_Y * TGLM ;
  integral_Z = integral_Z + error_Z * TGLM ;

  power_X = (error_X * k_p) + (dError_X * k_d) + (integral_X * k_i) ;
  power_Y = (error_Y * k_p) + (dError_Y * k_d) + (integral_Y * k_i) ;
  power_Z = (error_Z * k_p_z) + (dError_Z * k_d_z) + (integral_Z * k_i_z) ;

  int tele_P_X = power_X ;
  int tele_P_Y = power_Y ;
  int tele_P_Z = power_Z ;
  
  // ----- begin -x/+x ------

  if ( power_X > 0 )
  {
    digitalWrite(inD, HIGH);
    digitalWrite(inC, LOW);

    if (power_X > 255) { power_X = 255 ; }

    if (u_kalman1 == set_balanced_X ) { power_X = 0 ; integral_X = 0 ;}

    if (u_kalman3 > k3 ) { power_X = 0 ; integral_X = 0 ; }

    analogWrite(en_b, power_X );
  }

 else
  {
    power_X = - power_X;
    digitalWrite(inD, LOW);
    digitalWrite(inC, HIGH);

    if (power_X > 255) { power_X = 255 ; }

    if ( u_kalman1 == set_balanced_X ) { power_X = 0 ; integral_X = 0 ;}

    if (u_kalman3 > k3 ) { power_X = 0 ; integral_X = 0 ; }

    analogWrite(en_b, power_X );
  }

  // ----- begin -y/+y -----------------

  if (power_Y > 0 )
  {

    digitalWrite(inB, HIGH);
    digitalWrite(inA, LOW);

    if (power_Y > 255) { power_Y =255 ; }

    if (u_kalman2 == set_balanced_Y  ) { power_Y = 0 ; integral_Y = 0; }

    if (u_kalman3 > k3 ) { power_Y = 0 ; integral_Y = 0 ; }

    analogWrite(en_a, power_Y);
  }
 else
  {
    power_Y = - power_Y ; 

    digitalWrite(inB, LOW);
    digitalWrite(inA, HIGH);

    if( power_Y > 255) { power_Y = 255 ; }

    if( u_kalman2 == set_balanced_Y ) { power_Y = 0 ; integral_Y = 0; }

    if (u_kalman3 > k3 ) { power_Y = 0 ; integral_Y = 0 ; }

    analogWrite(en_a, power_Y);

  }

  // ------------------------
  per_error_X = error_X; // update error value
  per_error_Y = error_Y;
  per_error_Z = error_Z;

  /*
    char text[40];
    sprintf(text, " %d , %d , %d , %d ", tele_K_X , tele_K_Y , tele_P_X , tele_P_Y );
    Serial.println(text);
  */

  Serial.print (" X = ");
  Serial.print (u_kalman1);

  Serial.print (" Y = ");
  Serial.print (u_kalman2);

  Serial.print (" Z = ");
  Serial.print (u_kalman3);

//------------------------
  Serial.print (" power_X = ");
  Serial.print (tele_P_X);

  Serial.print (" power_Y = ");
  Serial.print (tele_P_Y);

  Serial.print (" power_Z = ");
  Serial.print (tele_P_Z);

  Serial.println();
  delay (0);
}