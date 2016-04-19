#define NUM_LIGHTS 4
#define SPEED_MAGNITUDE 10
#define WAVE_LOW_BOUND -127
#define WAVE_HIGH_BOUND 127
#define WAVE_LENGTH (WAVE_HIGH_BOUND - WAVE_LOW_BOUND)
#define BOTTOM_BOUND 0
#define TOP_BOUND 255
#define WAVE_OOB ( ( wave_position + WAVE_HIGH_BOUND ) < BOTTOM_BOUND \
				|| ( wave_position + WAVE_LOW_BOUND  ) > TOP_BOUND )

int wave_speed;  // positive, negative, zero
int wave_position; 
int light_positions[NUM_LIGHTS];  // 
int light_pins[NUM_LIGHTS];  // map lights to pins

// Return the value of a waveform at the position x.
// Should return value in [0, 255]
int waveform ( int x ) {
	if (x < -127 || x > 127) {
		return 0;
	}
	x = x*2*3.1415/128; // Convert to radians
	return cos() //TODO
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
	//TODO: Initialize light positions
	//TODO: Initialize light pins
}

void loop() {
	
	// Note: Speed of this loop, and SPEED_MAGNITUDE, determine speed of wave
	//       Does not create multiple waves

	//TODO: Does not incorporate special case for during wave 
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
	if ( WAVE_OOB ) {
		wave_speed = 0;
	}	
	
	// Advance wave
	wave_position += wave_speed;
	for (int i = 0 ; i < NUM_LIGHTS ) {
		analogWrite( light_pins[i], waveform( light_positions[i] - wave_position) );
	}

}

/*
Idea
	lights have coordinates
	bounded waveform travels from origin sensor to termination
	reverse direction if in procession

*/
