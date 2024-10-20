#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"


#define LED_PIN 13  // Pin to activate the orange LED
boolean led_state;  // Variable to "remember" the state
                    // of the LED, and toggle it.
#define Buzzer 6

int a;
int count;
int l=47;
float dl, dr;
int le0,le1;
float tetha, last_tetha, angle, ia, ra, p;
float xr;
float x1,y1,cc;

Motors_c motors;
LineSensor_c sensors;
Encoders_c encoders;

// put your setup code here, to run once:

void setup() {

  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");

  // Set LED pin as an output
  pinMode( LED_PIN, OUTPUT );
  pinMode(Buzzer, OUTPUT);

  // Set initial state of the LED
  led_state = false;
  motors.init();
  sensors.init();
  encoders.setupEncoder0();
  encoders.setupEncoder1();

  a= 1;
  count=0;
  le0=0;
  le1=0;
  tetha=0;
  x1=0;
  y1=0;
  angle=0;
}

void movefwd(){
   motors.setMotorDir(FWD, FWD);
   motors.setMotorPower(20, 20);
}

void rev(){
   motors.setMotorDir(REV, REV);
   motors.setMotorPower(20, 20);
}

void idle(){
   motors.setMotorDir(REV, REV);
   motors.setMotorPower(0, 0);
}

void turnleft(){
   motors.setMotorDir(FWD, FWD);
   motors.setMotorPower(0, 20);
}

void turnright(){
   motors.setMotorDir(FWD, FWD);
   motors.setMotorPower(20, 0);
}

void uturn(){
   motors.setMotorDir(FWD, REV);
   motors.setMotorPower(30, 30);
}
void delaymillis(unsigned long x)
{
  unsigned long presentMillis = millis();
  while(millis() - presentMillis <= x)
  {
    
  }
}

void returnhome(){
   motors.setMotorDir(FWD, FWD);
   motors.setMotorPower(51, 50);
}


// put your main code here, to run repeatedly:
void loop() {

  // Using an if statement to toggle a variable
  // with each call of loop()
  if( led_state == true ) {
    led_state = false;
  } else {
    led_state = true;
  }
  a = a+1;
 
  char OnLine;
  int DN[5];
  
  // We use the variable to set the
  // debug led on or off on the 3Pi+
  digitalWrite( LED_PIN, led_state );

  for(int i=0;i<5;i++){
    DN[i]=sensors.ReadSensors(i);
    
  }
  dl = encoders.encodl() - le0; //delta left
  dr = encoders.encodr() - le1; //delta right

  le0 = encoders.encodl(); 
  le1 = encoders.encodr();

  xr = ((dl*0.28)+(dr*0.28))/2.0; //X(r)
  tetha = (((dl*0.28)-(dr*0.28)))/(2.0*17); //Theta(r)
  x1 = x1+ (xr*cos(angle)); //X1
  y1 = y1+ (xr*sin(angle)); //Y2
  angle = angle + tetha;
  //Serial.print(x1);
  //Serial.print(",");
  //Serial.print(y1);
  //Serial.print("//");
  //Serial.print(angle);
  //Serial.println(",");
  last_tetha =  tetha;
  //Serial.println(encoders.encodl());
  //Serial.println(encoders.encodr());

  if(DN[2]>=1000 && count<=1){
    count = count+1;
    delaymillis(300);
  }
  else if(count == 0 || count == 1){
    movefwd();
  }

  else if(count == 2){
  if(DN[0]>=1000 && DN[1]>=1000 && DN[2]>=1000 && DN[3]>=1000){
    rev();
    delaymillis(300);
    idle();
    delaymillis(1000);
    turnleft();
    delaymillis(800);
  }
  else if(DN[1]>=1800 && DN[2]>=1800 && DN[3]>=1800){
    turnleft();
  }
  else if(DN[4]>=1000 && DN[1]<=1000 && DN[2]<=1000 && DN[3]<=1000 && DN[0]<=1000){
    rev();   
    delaymillis(300);
    idle();
    delaymillis(1000);
    turnright();
    delaymillis(800);
  }
  else if(DN[1]<=1000 && DN[2]>=1000 && DN[3]>=1000){
    turnright();
  }
  else if(DN[1]>=1000 && DN[2]>=1000 && DN[3]<=1000){
    turnleft();
  }
  else if(DN[1]>=1000 && DN[2]>=1000 && DN[3]>=1000){
    movefwd();
  }
  else if(DN[0]>=1800 && DN[4]>=1800){
    turnleft();
  }
  
  else if(DN[0]>=1000 && DN[4]>=1000){
    rev();    
    delaymillis(300);
    idle();
    delaymillis(1000);
    turnleft();
    delaymillis(800);
  }
  
  else{
    if(DN[0]<=1000 && DN[4]<=1000 && DN[1]<=1000 && DN[2]<=1000 && DN[3]<=1000){
      if(x1>380){
        idle();
        delay(1000);
        count = 3;
      }
      else{
      motors.setMotorDir(FWD, FWD);
      motors.setMotorPower(15, 15);
      delaymillis(750);
      uturn();
      delaymillis(850);
      }
    }
  }
  }
  else if(count==3){
    idle();
    cc = sqrt((x1*x1)+(y1*y1)); //Pythagoras theorem
    ia = (acos(x1/cc)); //Cos Inverse to find angle 
    if(y1>0){
      ra = -1.8+ia; //Finding angle to be turned
    }
    else if(y1<0){
      ra = 1.8-ia; //Finding angle to be turned
    }
    count=4;
  } 
  else if(count==4){
    if(y1>0){
      p = (ra/-1.8)*100;
      int del = 1000*(p/100); //rotate angle time
      int rh = (cc/50)*470; //return home time
      motors.setMotorDir(FWD, REV);
      motors.setMotorPower(30, 30);
      delaymillis(del);
      returnhome();
      delaymillis(rh);
      idle();
      count =5;
    }
    else if(y1<0){
      p = (ra/1.8)*100;
      int del = 1000*(p/100); //rotate angle time
      int rh = (cc/50)*455; //return home time
      motors.setMotorDir(REV, FWD);
      motors.setMotorPower(30, 30);
      delaymillis(del);
      returnhome();
      delaymillis(rh);
      idle();
      count=5;
    }  
  }
  else if(count==5){
    Serial.println("Done");
  }
  

}
