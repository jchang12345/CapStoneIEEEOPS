//as of now this is just a mere copy of project 5. make sure i can detect walls, using analog read if necessray. i can techincally bring this home for some tests later.
//serial monitor to double check and use prints for sure.

//the new plan is to no longer rely on IR2, that guy is no longer salvageable. 
//also, we can remap so values are nicer, but for now, will completely ignore left sensor. coorection left and turning left will mean looking at the right sensor only, if it drops too much while turning,
//then we have to turn back to the left. if im approaching a wall in ir led1, i will have to turn right or left.
//1. turn right is if irled 3 is signalling NOTHING THERE
//2, turn left is if irled 3 is signalling SOMETHING THERE and ir 1 is tryign to turn

 //pray in the 8.1 x2 batteries



//pray in the 8.2V

 
#define IRLED1 A5 //left
//#define IRLED2 A3 //the A5 and A3 for analog, but the digital ones don't need it
#define IRReceiver1 // only working w/ 1 IR led and 1 Receiver.
#define IRLED2 A6 //left
#define IRLED3 A4 //the A5 and A3 for analog, but the digital ones don't need it
#define IRReceiver2 // only working w/ 1 IR led and 1 Receiver.
//#define IRLED2 A3 //the A5 and A3 for analog, but the digital ones don't need it
#define IRReceiver3 // only working w/ 1 IR led and 1 Receiver.

#define LeftMotor 10  // Enable Pin for motor 1 , using A4 analog write , 1-2 enable pin, should go to A3 
#define RightMotor 6  // Enable Pin for motor 2, using A3 , 3-4 enable pin , should go to A 6

#define Right1 2  // Control pin 1 for motor 1, D8 in1
#define Right2 3  // Control pin 2 for motor 1, D9 in2 
#define Left1 5  // Control pin 1 for motor 2, D2 in 3
#define Left2 4  // Control pin 2 for motor 2, D3 in4 
double KI = 3, KD = 3, KP = 5; //You will be tuning these constants
double integralRight;
double pErrorRight;
int dt = 2;

double integralLeft;
double pErrorLeft;

//TURNING CONSTANTS DELAY for turning
int RightTurnDelay=1225 ; //90degree right turn
//9V 1st guess: 820 //Oldcode 975
int LeftTurnDelay=1260; //90 degree left turn
//9V 1st guess: 835 //Old code 1000
//INIT SPEEDS
int LeftMotorSpeed=125;
int RightMotorSpeed=153;

 // analogWrite(LeftMotor, 125);
  //analogWrite(RightMotor, 153);

//default, LeftWheel>>>>RightWheel 

//PID SPEEDUP CONFIGS RIGHT TURN
//want your right wheel to be > than ur left wheel.
int RHSRightTurnSpeed=133;
//9V 1st guess: 120 //Old code: 133
int LHSRightTurnSpeed=100;
//9V 1st guess: 90 //Old code 100

//PID SPEEDUP CONFIGS LEFT TURN
//want your left wheel to be > than ur right wheel.

int RHSLeftTurnSpeed=100;
//9V 1st guess:88//Old code 100
int LHSLeftTurnSpeed=150;
//9V 1st guess:130 //Old code 150




int idealRight = 22;
int idealLeft = 29;
int errorRight;
int errorLeft;
int derivativeRight;
int derivativeLeft;



int countstuck=0;


//A PID controller
/*
  what im realizing here is...
  1. values from IR sensor reads are diff
  2. values from motor for turning are also going to be diff
  (IOW testing these only really possible if in lab...)
*/


bool theEnd=false;


//if too close to one of the walls, we need to turn and check if its too close with some kind of mapping.
int countleft = 0;
int countright = 0;
//#define IRReceiver2
int zerofront;
int zerorear;
//pulse width modulation analog for digital ports : 3,5,6,9,10,11
//analogWrite goes from 0-255
//analogRead goes from 0-1023 though, so we need to map that later.
void setup() {


  pinMode(Right1, OUTPUT);
  pinMode(Right2, OUTPUT);
  pinMode(Left1, OUTPUT);
  pinMode(Left2, OUTPUT);


  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(IRLED1, INPUT);
  pinMode(IRLED2, INPUT);
  pinMode(IRLED3, INPUT);

  //pinMode(IRRECEIVER1, INPUT); we don't define receivers.
  //pinMode(IRRECEIVER1, INPUT);
 
  //analogWrite(LeftMotor, 80);
  //   analogWrite(RightMotor, 100);
 
  //560 is the highest value consistently ish ive been getting.

}
//LED should get brighter the closer your hand is to the IR receiver.
void loop()
{
  // if(countleft>=300)
/*  {
    Serial.println("RATIO OF COUNT LEFT TO COUNT RIGHT IS");
    Serial.println("COUNTLEFT COUNT");
    Serial.println(countleft);
    Serial.println("COUNTRIGHT COUNT");
    Serial.println(countright);
  }
*/
if(theEnd) //this is good, when it reaches a dead end, he completelySTOPPSSSS
{
  Serial.println("STOPPING CAR BECAUSE APPROACHING WALL BUT PATH EXISTS ON BOTH SIDES");
      digitalWrite(Right1, LOW);
      digitalWrite(Right2, LOW);
      digitalWrite(Left1, HIGH);
      digitalWrite(Left2, HIGH);
      //end of the maze bool condition 
      delay(2000);
      return;
}
  analogWrite(LeftMotor, LeftMotorSpeed);
  analogWrite(RightMotor, RightMotorSpeed);
  //high is to activiate and turn it


  digitalWrite(Right1, HIGH);
  digitalWrite(Right2, LOW);

  digitalWrite(Left1, HIGH);
  digitalWrite(Left2, LOW);

  int IR1 = analogRead(IRLED1);
  int  LED1V = map(IR1, 0, 1023, 255, 0); // starts at 196 -> 18->20 is too close, 50 is turn. dont have to worry about ideal for front.
  //int sum = 0;
 int IR2=analogRead(IRLED2);



  int  LED2V = map(IR2, 0, 1023, 255, 0); //starts at 188->78-85 is too close, 108 is probably an ideal.
  int IR3 = analogRead(IRLED3); //starts at 194, -> 20, too close, 50 is ideal
  int  LED3V = map(IR3, 0, 1023, 255, 0);
  //243 is intial value.
  
  //129, 135
//int LED1V=IR1;
//int LED2V=IR2;
///int LED3V=IR3;

//Serial.println("IR1 is ");
 // Serial.println(LED1V);
  //Serial.println("IR2 is ");
  //Serial.println(LED2V);

  if (LED1V <= 15) //front
  {    
analogWrite(LeftMotor, LeftMotorSpeed);
  analogWrite(RightMotor, RightMotorSpeed);
  //high is to activiate and turn it


  digitalWrite(Right1, HIGH);
  digitalWrite(Right2, LOW);

  digitalWrite(Left1, HIGH);
  digitalWrite(Left2, LOW);
    //delay();
    
    //delay 2000 and pause first 
    if(LED2V<50&&LED3V<50)
    {

     //  Serial.println("STOPPING CAR BECAUSE APPROACHING WALL BUT PATH EXISTS ON BOTH SIDES");
      digitalWrite(Right1, LOW);
      digitalWrite(Right2, LOW);
      digitalWrite(Left1, HIGH);
      digitalWrite(Left2, HIGH);
      //end of the maze bool condition 
      theEnd=true;
      delay(2000);
      return;
    }
    if (LED2V <80&&LED3V>165) //should we put a while instead?
    //instead of checking if led3V > led2V, check if LED 3 is greater than a certain value, MAKE SURE LED3V IS SMALL. LED 2 had to be small at first b/c wall approach, and we must turn right.
    { //sometimes, the value is at 112...
      analogWrite(LeftMotor, 85);
    analogWrite(RightMotor, 95);
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    
       int LED2prev=map(analogRead(IRLED2),0,1023,255,0);


int count =0;
 delay(120);
    while (map(analogRead(IRLED2), 0, 1023, 255, 0) > LED2prev)
    {
    //  Serial.println("In while loop for left sensor");
      LED2prev =0;
      for(int i =0; i < 5; i++)
      LED2prev += map(analogRead(IRLED2),0,1023,255,0);
      LED2prev /= 5;
      count++;
      if(count>1000)
      {
        //jolt back
        analogWrite(LeftMotor,150);
        analogWrite(RightMotor,173);
          digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);

    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);
        count=0;
      }
    }

    delay(120);
    while (map(analogRead(IRLED2), 0, 1023, 255, 0) < LED2prev)
    {
    //  Serial.println("In while loop for left sensor");
      LED2prev =0;
      for(int i =0; i < 5; i++)
      LED2prev += map(analogRead(IRLED2),0,1023,255,0);
      LED2prev /= 5;
      count++;
      if(count>1000)
      {
        //jolt back
        analogWrite(LeftMotor,150);
        analogWrite(RightMotor,173);
          digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);

    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);
        count=0;
      }
    }
       
   /* while (map(analogRead(IRLED1), 0, 1023, 255, 0) < 105)
    {
     // Serial.println("In while loop for front sensor");
    }
   while (map(analogRead(IRLED2), 0, 1023, 255, 0) > 127)
    {
    //  Serial.println("In while loop for right sensor");
    }
    */
    //Serial.println("TURN RighT"); //right sensor
    analogWrite(LeftMotor, 0);
    analogWrite(RightMotor, 0);
   
    }
    else if (LED3V < 80&&LED2V>165)  //else if LED 3 is greater than a certain value, for turn left, you want to make sure that the right LED value is BIGGGGGG
    {


       analogWrite(LeftMotor, 85);
    analogWrite(RightMotor, 95);
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);

    int LED3prev=map(analogRead(IRLED3),0,1023,255,0);
    /* 
    while (map(analogRead(IRLED1), 0, 1023, 255, 0) < 105)
    {
      Serial.println("In while loop for front sensor");
    }*/


int count=0;
    
    delay(120);
    while (map(analogRead(IRLED3), 0, 1023, 255, 0) > LED3prev)
    {
    //  Serial.println("In while loop for left sensor");
      LED3prev =0;
      for(int i =0; i < 5; i++)
      LED3prev += map(analogRead(IRLED3),0,1023,255,0);
      LED3prev /= 5;
      count++;
      if(count>1000)
      {
        //jolt back
        analogWrite(LeftMotor,150);
        analogWrite(RightMotor,173);
          digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);

    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);
        count=0;
      }
    }

    delay(120);
    while (map(analogRead(IRLED3), 0, 1023, 255, 0) < LED3prev)
    {
    //  Serial.println("In while loop for left sensor");
      LED3prev =0;
      for(int i =0; i < 5; i++)
      LED3prev += map(analogRead(IRLED3),0,1023,255,0);
      LED3prev /= 5;
    }
    Serial.println("TURN LEfT"); //right sensor
    analogWrite(LeftMotor, 0);
    analogWrite(RightMotor, 0);

  count++;
      if(count>1000)
      {
        //jolt back
        analogWrite(LeftMotor,150);
        analogWrite(RightMotor,173);
          digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);

    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);
        count=0;
      }
    }
/*
  errorLeft = idealLeft - LED2V; //correct to the left as a result because we are too close to the right here.

  errorRight = idealRight - LED3V-errorLeft; //correct to the left as a result because we are too close to the right here.
  derivativeRight = (pErrorRight - errorRight) / dt;
  integralRight = errorRight * dt;
  pErrorRight = errorRight;
  //Serial.print("Error right: ");
  //Serial.println(errorRight);
  double CorrectionRight = KP * errorRight + KI * integralRight + KD * derivativeRight;

    


 
  //Serial.println("CORRECTIONRIGHT VALUE ");
  //Serial.println(CorrectionRight);
  //turn right move forwars a bit. if neg

//  Serial.println("LED3V VALUE IS :");
 // Serial.println(LED3V);
 // Serial.println("CORRECTIONLEFT VALUE ");
//  Serial.println(CorrectionLeft);
  //turn left move  forard a bit.


  if (CorrectionRight < 0) //idk have to find out what this value is.
  { //idea is correctionright tooooooo big, we know we need to turn back
    //by turning left . if too small too negative, turn back by turn right

    //Serial.println("PID TURN LEFT");  //too close to right.
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(RightMotor, 100+CorrectionRight);
    analogWrite(LeftMotor, 80-CorrectionRight);

   // delay(80);
    countleft++;

    //delay(500);
  }
//Serial.println("CORRECTIONLEFT VALUE ");
  //Serial.println(CorrectionLeft);
  if(CorrectionRight>0)
  {
  //  Serial.println("PID TURN RIGHT");  //too close to left

    countright++;
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(LeftMotor, 80+CorrectionRight);
    analogWrite(RightMotor, 100-CorrectionRight); //slow down right motor.
    //high is to activiate and turn it
   // delay(80);
  }
   */
   

    
  }

  //129, 135
  //PID CONTROL RIGHT


  else if(LED3V<140&&LED2V<140) //do pid
  { //both sensors should be like under 90. but in the case they AINT UNDER 90, one side of the wall is MISSING.
    /*

3) If both left/right walls are missing, pretend that you are straight and go forward with the current speed for a 
set amount of time until you encounter both walls/one wall again :D (we might take this out if not enough teams can get it done)
     */

//a condition to check for is if there are open walls. how do i counter that problem?

    
    //coorectionright computation
    
  //coorectionright computation
  errorRight = idealRight - LED3V; //correct to the left as a result because we are too close to the right here.
  derivativeRight = (pErrorRight - errorRight) / dt;
  integralRight = errorRight * dt;
  pErrorRight = errorRight;
  double CorrectionRight = KP * errorRight + KI * integralRight + KD * derivativeRight;

    errorLeft = idealLeft - LED2V; //correct to the left as a result because we are too close to the right here.
  derivativeLeft = (pErrorLeft - errorLeft) / dt;
  integralLeft = errorRight * dt;
  pErrorLeft = errorLeft;
  double CorrectionLeft = KP * errorLeft + KI * integralLeft + KD * derivativeLeft;

 
 // Serial.println("CORRECTIONRIGHT VALUE ");
 // Serial.println(CorrectionRight);
  //turn right move forwars a bit. if neg

//  Serial.println("LED3V VALUE IS :");
//  Serial.println(LED3V);
 // Serial.println("CORRECTIONLEFT VALUE ");
//  Serial.println(CorrectionLeft);
  //turn left move  forard a bit.


  if (CorrectionRight > CorrectionLeft) //idk have to find out what this value is.
  { //idea is correctionright tooooooo big, we know we need to turn back
    //by turning left . if too small too negative, turn back by turn right

  //  Serial.println("PID TURN LEFT");  //too close to right.
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(RightMotor, 113);
    analogWrite(LeftMotor, 0);

    delay(35);
    countleft++;

    //delay(500);
  }
//Serial.println("CORRECTIONLEFT VALUE ");
  //Serial.println(CorrectionLeft);
  if(CorrectionLeft>CorrectionRight)
  {
   // Serial.println("PID TURN RIGHT");  //too close to left

    countright++;
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(LeftMotor, 105);
    analogWrite(RightMotor, 0); //slow down right motor.
    //high is to activiate and turn it
    delay(35);
  }//inside this part of pid i should also be checking led 1 for reals though tbh based on how i pid or else make sure when i turn back to normal, I AINT LEANING AGAINST NO WALL. and tryna ram a wall.





    }

else if(LED3V>140&&LED2V<140) //missing wall on the right side, do pid only on the left
{
   errorLeft = idealLeft - LED2V; //correct to the left as a result because we are too close to the right here.
  derivativeLeft = (pErrorLeft - errorLeft) / dt;
  integralLeft = errorRight * dt;
  pErrorLeft = errorLeft;
  double CorrectionLeft = KP * errorLeft + KI * integralLeft + KD * derivativeLeft;

 //Serial.println("CORRECTIONLEFT VALUE ");
  //Serial.println(CorrectionLeft);
  if(CorrectionLeft>0)
  {
   /* Serial.println("PID TURN RIGHT");  //too close to left

    countright++;
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(LeftMotor, 105);
    analogWrite(RightMotor, 0); //slow down right motor.
    //high is to activiate and turn it
    delay(100);
    */
    delay(25);
  }//inside this part of pid i should also be checking led 1 for reals though tbh based on how i pid or else make sure when i turn back to normal, I AINT LEANING AGAINST NO WALL. and tryna ram a wall.

}
else if(LED2V>140&&LED3V<140) //missing a wall on the left side, do pid only on the right.
{
    //coorectionright computation
  errorRight = idealRight - LED3V; //correct to the left as a result because we are too close to the right here.
  derivativeRight = (pErrorRight - errorRight) / dt;
  integralRight = errorRight * dt;
  pErrorRight = errorRight;
  double CorrectionRight = KP * errorRight + KI * integralRight + KD * derivativeRight;
if (CorrectionRight > 0) //idk have to find out what this value is.
  { //idea is correctionright tooooooo big, we know we need to turn back
    //by turning left . if too small too negative, turn back by turn right
/*
    Serial.println("PID TURN LEFT");  //too close to right.
    digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);

    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
    analogWrite(RightMotor, 113);
    analogWrite(LeftMotor, 0);

    delay(100);
    countleft++;
*/
    //delay(500);
        delay(25);

  }
}


    
    //inside this part of pid i should also be checking led 1 for reals though tbh based on how i pid or else make sure when i turn back to normal, I AINT LEANING AGAINST NO WALL. and tryna ram a wall.

  //129, 135
///delay(1000);









}





//based on my test runs, 70-80s means im hitting a wall in my room. will need to calibrate what
//the real numbers are in the lab with their lighting and their typo walls. but for testing
//purposes in my room, ill just have it register 70-80s is a wall.
// put your main code here, to run repeatedly:
/*
   /This code  will turn Motor A clockwise for 2 sec.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(2000);
  //This code will turn Motor A counter-clockwise for 2 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(2000);

  //This code will turn Motor B clockwise for 2 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(2000);
  //This code will turn Motor B counter-clockwise for 2 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(2000);

  //And this code will stop motors
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
*/
//how to turn
//stop
/*
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
  //cw 1 turn right
  digitalWrite(Right1, HIGH);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
        //stop
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
  //ccw 1
  digitalWrite(Right1, LOW);
    digitalWrite(Right2, HIGH);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
        //stop
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
  //cw 2
  digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, HIGH);
    digitalWrite(Left2, LOW);
        delay(3000);
        //stop
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);
        //ccw 2
  digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, HIGH);
        delay(3000);
        //stop
    digitalWrite(Right1, LOW);
    digitalWrite(Right2, LOW);
    digitalWrite(Left1, LOW);
    digitalWrite(Left2, LOW);
        delay(3000);

*/


/*if(LED2V<=80) //left, turn right
  {
  analogWrite(LeftMotor, 62);
  analogWrite(RightMotor, 62);
  Serial.println("APPROACHING WALL from LED 2PLEASE TURN");
  digitalWrite(Right1, HIGH);
  digitalWrite(Right2, LOW);
  digitalWrite(Left1, LOW);
  digitalWrite(Left2, HIGH);
  //turn right

  }
*/

// change direction, goes backwards...

/* digitalWrite(LeftMotor, LOW);
  digitalWrite(RightMotor, LOW);

  delay(1000);

  digitalWrite(LeftMotor, HIGH);
  digitalWrite(RightMotor, HIGH);

  digitalWrite(Right1, LOW);
  digitalWrite(Right2, HIGH);
  digitalWrite(Left1, LOW);
  digitalWrite(Left2, HIGH);
*/



