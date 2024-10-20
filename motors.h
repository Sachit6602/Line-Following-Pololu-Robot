#ifndef _MOTORS_H
#define _MOTORS_H

# define LPPIN 10
# define LDPIN 16
# define RPPIN 9
# define RDPIN 15

# define FWD LOW
# define REV HIGH


// Class to operate the motor(s).
class Motors_c {
  public:

    // Constructor, must exist.
    Motors_c() {

    } 

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
    void init() {
    pinMode(LPPIN, OUTPUT);
    pinMode(LDPIN, OUTPUT);
    pinMode(RPPIN, OUTPUT);
    pinMode(RDPIN, OUTPUT);

    // Set initial direction (HIGH/LOW)
    // for the direction pins.
    digitalWrite(LDPIN, HIGH);
    digitalWrite(RDPIN, HIGH);

    }

    // Write a function to operate
    // your motor(s)
    // ...

    void setMotorDir( char a, char b) {
      digitalWrite(LDPIN, a);
      digitalWrite(RDPIN, b);;

    }

    void setMotorPower( float left_pwm, float right_pwm ) {
      analogWrite( LPPIN, left_pwm );
      analogWrite( RPPIN, right_pwm );

    }
    
};



#endif