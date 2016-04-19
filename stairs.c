/***********************************************************************
(c) 2016 Goobyalus

Given some distribution of lights and a sensor on each end, display a
wave transmitting across the lights when a sensor is activated.
***********************************************************************/

#include <math.h>
//TODO: Pin types?

#define OOB(x, low, high) ( (x) < (low) || (x) > (high) )
#define NUM_LIGHTS 4
#define SPEED_MAGNITUDE 10
#define WAVE_LOW_BOUND -127.0
#define WAVE_HIGH_BOUND 127.0
#define WAVE_LENGTH (WAVE_HIGH_BOUND - WAVE_LOW_BOUND)
#define BOTTOM_BOUND 0.0
#define TOP_BOUND 1000.0
#define WAVE_OOB ( ( wave_position + WAVE_HIGH_BOUND ) < BOTTOM_BOUND \
                                || ( wave_position + WAVE_LOW_BOUND  ) > TOP_BOUND )
			
const double PI = atan(1.0);

int wave_speed;  // positive, negative, zero
int wave_position; 
float light_positions[NUM_LIGHTS];  // 
int light_pins[NUM_LIGHTS];  // map lights to pins


// Return the value of a waveform at the position x.
// Bounded by WAVE_LOW_BOUND and WAVE_HIGH_BOUND -- 0's outside
int waveform ( double x ) {
	if ( OOB( x, WAVE_LOW_BOUND, WAVE_HIGH_BOUND ) ) {
		return 0;
	}
	return ( 1 + cos(x * PI / 127) ) * 255 / 2;
}

int sensor_bottom() {
	//TODO
}

int sensor_top() {
	//TODO
}

void setup() {
	wave_speed = 0;
	wave_position = 0;
	// Initialize light positions
	for ( int i = 0 ; i < NUM_LIGHTS; i++ ) {
		// equally distribute 
		light_positions = BOTTOM_BOUND + (i * ( BOTTOM_BOUND - TOP_BOUND ) / ( NUM_LIGHTS - 1) );
	}
	//TODO: Initialize light pins
	light_pins[0] = 0;
	light_pins[1] = 0;
	light_pins[2] = 0;
	light_pins[3] = 0;
}

void loop() {
	
	// Note: Speed of this loop, and SPEED_MAGNITUDE, determine speed of wave
	//       This code does not create multiple waves
	//		 Does not incorporate special case for during wave 
	
	if ( sensor_bottom() ) {		
		// start wave at bottom
		wave_position = BOTTOM_BOUND - WAVE_HIGH_BOUND;
		wave_position -= SPEED_MAGNITUDE;  // account for 1at advance wave
		// move wave up
		wave_speed = SPEED_MAGNITUDE;		
		
	} else if ( sensor_top() ) {	
		// start wave at top
		wave_position = TOP_BOUND - WAVE_LOW_BOUND;
		wave_position += SPEED_MAGNITUDE;  // account for 1at advance wave
		// move wave down
		wave_speed = 0 - SPEED_MAGNITUDE;
	}
	
	// Stop wave if out of bounds
	if ( WAVE_OOB ) ) {
		wave_speed = 0;
	}	
	
	// Advance wave by advancing time and updating all light values	
	if ( wave_speed ) {	// Saves processing time while wave is not active
		wave_position += wave_speed;
		for (int i = 0 ; i < NUM_LIGHTS ) {
			analogWrite( light_pins[i], waveform( light_positions[i] - wave_position) );
		}
	}
}

/*
Idea
	lights have coordinates
	bounded waveform travels from origin sensor to termination
*/
