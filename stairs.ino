/***********************************************************************
(c) 2016 Goobyalus

Given some distribution of lights and a sensor on each end, display a
wave transmitting across the lights when a sensor is activated.

This code does not account for:
* special behavior when wave is already in motion and sensor is acivated
* multiple waves at once
* flipping waves in different directions

This code has not yet been tested.
***********************************************************************/

#include <math.h>
//TODO: Pin types?

#define REVERSE_WAVEFORM_ON_REVERSE_WAVE 0  //TODO: feature unimplemented
#define OOB(x, low, high) ( (x) < (low) || (x) > (high) )
#define NUM_LIGHTS 4
#define SPEED_MAGNITUDE 1
#define WAVE_LOW_BOUND -127.0
#define WAVE_HIGH_BOUND 127.0
#define WAVE_LENGTH (WAVE_HIGH_BOUND - WAVE_LOW_BOUND)
#define BOTTOM_BOUND 0.0
#define TOP_BOUND 100.0
#define WAVE_OOB_FWD ( ( wave_position + WAVE_HIGH_BOUND ) < BOTTOM_BOUND \
                                || ( wave_position + WAVE_LOW_BOUND  ) > TOP_BOUND )
#if REVERSE_WAVEFORM_ON_REVERSE_WAVE == 0
#define WAVE_OOB WAVE_OOB_FWD
#else
#define WAVE_OOB ( wave_speed > 0 ) ? WAVE_OOB_FWD : ( ( ) )  //TODO
#endif
                                
      
//const double PI = atan(1.0);

int wave_speed;  // positive, negative, zero
int wave_position; 
double light_positions[NUM_LIGHTS];
int light_pins[NUM_LIGHTS];  // map lights to pins
int sensor_bottom_pin;
int sensor_top_pin;


// Return the value of a waveform at the position x.
// Bounded by WAVE_LOW_BOUND and WAVE_HIGH_BOUND -- 0's outside
int waveform ( double x ) {
  if ( OOB( x, WAVE_LOW_BOUND, WAVE_HIGH_BOUND ) ) {
    return 0;
  }
  return ( 1 + cos(x * PI / 127) ) * 255 / 2;
}

int sensor_bottom() {
  return digitalRead(sensor_bottom_pin);
}

int sensor_top() {
  return digitalRead(sensor_top_pin);
}

void setup() {
  wave_speed = 0;
  wave_position = 0;
  // Initialize light positions
  for ( int i = 0 ; i < NUM_LIGHTS; i++ ) {
    // equally distribute 
    light_positions[i] = BOTTOM_BOUND + (i * ( BOTTOM_BOUND - TOP_BOUND ) / ( NUM_LIGHTS - 1) );
  }
  // Initialize light pins
  light_pins[0] = 11;
  light_pins[1] = 10;
  light_pins[2] = 9;
  light_pins[3] = 6;
  for ( int i = 0 ; i < sizeof(light_pins); i++) {
    pinMode(light_pins[i], OUTPUT);
  }
  
  // Initialize sensor pins
  sensor_bottom_pin = 7;
  sensor_top_pin = 8;
  pinMode(sensor_bottom_pin, INPUT);
  pinMode(sensor_top_pin, INPUT);
}

void loop() {
  
  // Note: Speed of this loop, and SPEED_MAGNITUDE, determine speed of wave
  //       This code does not create multiple waves
  //     Does not incorporate special case for during wave 
  
  if ( sensor_bottom() && wave_speed <= 0 ) {   
    // start wave at bottom
    wave_position = BOTTOM_BOUND - WAVE_HIGH_BOUND;
    wave_position -= SPEED_MAGNITUDE;  // account for 1at advance wave
    // move wave up
    wave_speed = SPEED_MAGNITUDE;   
    
  } else if ( sensor_top() && wave_speed >= 0 ) { 
    // start wave at top
    wave_position = TOP_BOUND - WAVE_LOW_BOUND;
    wave_position += SPEED_MAGNITUDE;  // account for 1at advance wave
    // move wave down
    wave_speed = 0 - SPEED_MAGNITUDE;
  }
  
  // Stop wave if out of bounds
  // TODO: Does not work because OOB is too strict and always OOB 
  //if ( WAVE_OOB ) {
  //  wave_speed = 0;
  //} 
  
  // Advance wave by advancing time and updating all light values
  if ( wave_speed ) { // Saves processing time while wave is not active
    wave_position += wave_speed;
    for (int i = 0 ; i < NUM_LIGHTS; i++ ) {
      analogWrite( light_pins[i], waveform( light_positions[i] - wave_position) );
    }
  }
  delay(1);
}

/*
Idea
  lights have coordinates
  bounded waveform travels from origin sensor to termination
*/
