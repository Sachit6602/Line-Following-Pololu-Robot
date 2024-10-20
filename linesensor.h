// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

# define EMIT_PIN    11    // Documentation says 11.
# define DN1 12
# define DN2 A0   // Complete for DN1 pin
# define DN3 A2
# define DN4 A3
# define DN5 A4

int lpin[5]={
        DN1,
        DN2,
        DN3,
        DN4,
        DN5
      };

// Class to operate the linesensor(s).
class LineSensor_c {
  public:
  
    // Constructor, must exist.
    LineSensor_c() {

    } 

    void init(){
      pinMode( EMIT_PIN, INPUT ); // Set EMIT as an input (off)
      pinMode( DN1, INPUT );
      pinMode( DN2, INPUT ); // Set line sensor pin to input
      pinMode( DN3, INPUT );
      pinMode( DN4, INPUT );
      pinMode( DN5, INPUT );

      
    }

    int ReadSensors(int n){
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, HIGH );
      pinMode( lpin[n], OUTPUT );
      digitalWrite( lpin[n], HIGH );
      delayMicroseconds( 10 );

     pinMode( lpin[n], INPUT );
     unsigned long start_time = micros();
      while( digitalRead( lpin[n] ) == HIGH ) {
      // Do nothing here (waiting).
     }
     unsigned long end_time = micros();
      pinMode( EMIT_PIN, INPUT );
      unsigned long DN = end_time - start_time;

      return (float)DN;

    }
};



#endif
