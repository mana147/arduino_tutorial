#include <TimerOne.h>
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter bo_loc_x(1, 1, 0.009);
SimpleKalmanFilter bo_loc_y(1, 1, 0.009);
SimpleKalmanFilter bo_loc_z(1, 1, 0.009);
SimpleKalmanFilter bo_loc_t(1, 1, 0.009);

int u_kalman_x;
int u_kalman_y;
int u_kalman_z;
int u_kalman_t;

double set_balanced_X = 470;
double set_balanced_Y = 470;
double set_balanced_Z = 500;
double set_balanced_T = 500;

double error_X , per_error_X ; // sai số trước và sai số sau
double error_Y , per_error_Y ;
double error_Z , per_error_Z ;
double error_T , per_error_T ;

double dError_X;
double dError_Y;
double dError_Z;
double dError_T;

double integral_X = 0;
double integral_Y = 0;
double integral_Z = 0;
double integral_T = 0;

double TGLM = 1;
double inv_TGLM = 1; 

double k_p = 5;
double k_i = 0.001;
double k_d = 50;

double power_X;
double power_Y;
double power_Z;
double power_T;

//-----------------------
int en_a = 3;
int inA = 2;
int inB = 4;

int en_b = 6;
int inC = 5;
int inD = 7;
//-----------------------

double hall_x[5];
double hall_y[5];

double Max_x , Max_y , Min_x , Min_y ;

double sum_x = 0 ;
double sum_y = 0 ;

double AveHall_x = 0 ;
double AveHall_y = 0 ;

void setup()
{
  Serial.begin(9600);

  pinMode(en_a, OUTPUT);
  pinMode(inA, OUTPUT);
  pinMode(inB , OUTPUT);

  pinMode(en_b, OUTPUT);
  pinMode(inC, OUTPUT);
  pinMode(inD , OUTPUT);

  Timer1.initialize(0);
  Timer1.attachInterrupt(PID);

  //------------------------------
  TCCR1A = 0;
  TCCR1B = 0;
  /
}

void loop()
{
  //
}

void PID ()
{
    /*
  int value1 = analogRead(A0);
  int value2 = analogRead(A1);
  int value3 = analogRead(A2);
  int value4 = analogRead(A3);

  u_kalman_x = bo_loc_x.updateEstimate(value1);
  u_kalman_x = constrain( u_kalman_x , 0 , 800 );

  u_kalman_y = bo_loc_y.updateEstimate(value2);
  u_kalman_y = constrain( u_kalman_y , 0 , 800 );

  u_kalman_z = bo_loc_z.updateEstimate(value3);
  u_kalman_z = constrain( u_kalman_z , 0 , 800 );

  u_kalman_t = bo_loc_t.updateEstimate(value4);
  u_kalman_t = constrain( u_kalman_t , 0 , 800 );
   */
  //------------------------------------------------

  hall_x [0] = analogRead( A0 );
  Max_x = hall_x[0];
  Min_x = Max_x ;

  hall_y [0] = analogRead( A1 );
  Max_y = hall_y[0];
  Min_y = Max_y;

  //------------------------
  hall_x[1] = analogRead(A0);
  if(hall_x[1] > Max_x)
  {
      Max_x = hall_x[1];
  }
  else if(hall_x[1]<Min_x)
  {
      Min_x = hall_x [1];
  }

  hall_y[1] = analogRead(A1);
  if(hall_y[1] > Max_y)
  {
      Max_y = hall_y[1];
  }
  else if(hall_y[1]<Min_y)
  {
      Min_y = hall_y [1];
  }

  //------------------------
  hall_x[2] = analogRead(A0);
  if(hall_x[2] > Max_x)
  {
      Max_x = hall_x[2];
  }
  else if(hall_x[2]<Min_x)
  {
      Min_x = hall_x [2];
  }

  hall_y[2] = analogRead(A1);
  if(hall_y[2] > Max_y)
  {
      Max_y = hall_y[2];
  }
  else if(hall_y[2]<Min_y)
  {
      Min_y = hall_y [2];
  }

   //------------------------
  hall_x[3] = analogRead(A0);
  if(hall_x[3] > Max_x)
  {
      Max_x = hall_x[3];
  }
  else if(hall_x[3]<Min_x)
  {
      Min_x = hall_x [3];
  }

  hall_y[3] = analogRead(A1);
  if(hall_y[3] > Max_y)
  {
      Max_y = hall_y[3];
  }
  else if(hall_y[3]<Min_y)
  {
      Min_y = hall_y [3];
  }

  //------------------------
  hall_x[4] = analogRead(A0);
  if(hall_x[4] > Max_x)
  {
      Max_x = hall_x[4];
  }
  else if(hall_x[4]<Min_x)
  {
      Min_x = hall_x [4];
  }

  hall_y[4] = analogRead(A1);
  if(hall_y[4] > Max_y)
  {
      Max_y = hall_y[4];
  }
  else if(hall_y[4]<Min_y)
  {
      Min_y = hall_y [4];
  }

  //------------------------
  sum_x = hall_x [0] + hall_x [1] + hall_x [2] + hall_x [3] + hall_x [4] ;
  sum_y = hall_y [0] + hall_y [1] + hall_y [2] + hall_y [3] + hall_y [4] ;

  AveHall_x = (sum_x - Min_x - Max_x) / 3;
  AveHall_y = (sum_y - Min_y - Max_y) / 3;

  error_X = set_balanced_X - AveHall_x ;  // value_kalman_X; // tinh sai số giữa giá trị đặt vào và giá trị đo được
  error_Y = set_balanced_Y - AveHall_y ;   // value_kalman_Y;
  //error_Z = set_balanced_Z - u_kalman_z ;
  //error_T = set_balanced_T - u_kalman_t ;

  dError_X = ( error_X - per_error_X ) * inv_TGLM;
  dError_Y = ( error_Y - per_error_Y ) * inv_TGLM;
  //dError_Z = ( error_Z - per_error_Z ) * inv_TGLM;
  //dError_T = ( error_T - per_error_T ) * inv_TGLM;

  integral_X += ( error_X + per_error_X ) * TGLM ; // i_part = i_part + ( k_i * thời gian lấy mẫu * error ) / 1000
  integral_Y += ( error_Y + per_error_Y ) * TGLM ;
  //integral_Z = integral_Z + error_Z * TGLM ;
  //integral_T = integral_T + error_T * TGLM ;

  power_X = (error_X * k_p) + (dError_X * k_d) + (integral_X * k_i) ;
  power_X = constrain(power_X, -999 , 999 );

  power_Y = (error_Y * k_p) + (dError_Y * k_d) + (integral_Y * k_i) ;
  power_Y = constrain(power_Y, -999 , 999 );

  //power_Z = (error_Z * k_p) + (dError_Z * k_d) + (integral_Z * k_i) ;
  //power_Z = constrain(power_Z, -999 , 999 );

  //power_T = (error_T * k_p) + (dError_T * k_d) + (integral_T * k_i) ;
  //power_T = constrain(power_T, -999 , 999 );

  double tele_P_X = power_X ;
  double tele_P_Y = power_Y ;
  //int tele_P_Z = power_Z ;
  //int tele_P_T = power_T ;

  // ----- begin -x/+x --------------------------------
  //--------------------------------------------------

  if ( power_X > 0 ) 
  {
    digitalWrite(inD, HIGH);
    digitalWrite(inC, LOW);
  }
 else
  {
    power_X = - power_X;
    digitalWrite(inD, LOW);
    digitalWrite(inC, HIGH);
  }

  if( power_X > 250) { power_X = 250 ; }

  if( AveHall_x > 510 ) { power_X = 0; integral_X = 0; dError_X = 0 ; }

  analogWrite(en_b, power_X );

  // ----- begin -y/+y -----------------
  //------------------------------------

  if (power_Y > 0 )
  {
    digitalWrite(inB, HIGH);
    digitalWrite(inA, LOW);
  }
 else
  {
    power_Y = - power_Y ; 
    digitalWrite(inB, LOW);
    digitalWrite(inA, HIGH);
  }
  
  if (power_Y > 250) { power_Y = 250 ; }

  if ( AveHall_y > 510 ) { power_Y = 0 ; integral_Y = 0; dError_Y = 0 ; }

  analogWrite(en_a, power_Y);
  // ------------------------
  per_error_X = error_X; // update error value
  per_error_Y = error_Y;
  
  //per_error_Z = error_Z;
  //per_error_T = error_T;

  Serial.print (" X = ");
  Serial.print (AveHall_x);

  Serial.print (" Y = ");
  Serial.print (AveHall_y);

  // Serial.print (" Z = ");
  //Serial.print (u_kalman_z);

  //Serial.print (" T = ");
  //Serial.print (u_kalman_t);

//------------------------
  Serial.print (" power_X = ");
  Serial.print (power_X);

  Serial.print (" power_Y = ");
  Serial.print (power_Y);

  //Serial.print (" power_Z = ");
  //Serial.print (tele_P_Z);

  //Serial.print (" power_T = ");
  //Serial.print (tele_P_T);

  Serial.println();
}