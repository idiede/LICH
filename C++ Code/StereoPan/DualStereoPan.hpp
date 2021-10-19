#ifndef DualStereoPan_h
#define DualStereoPan_h

#include "Patch.h"

class DualStereoPan : public Patch {
	
public:
	DualStereoPan() {
		 registerParameter(PARAMETER_A, "Gain 1");
         registerParameter(PARAMETER_B, "Gain 2");
         registerParameter(PARAMETER_C, "Pan 1");
         registerParameter(PARAMETER_D, "Pan 2");

	}

		void processAudio(AudioBuffer &buffer) {

		float gain_1 = getParameterValue(PARAMETER_A);
	    float gain_2 = getParameterValue(PARAMETER_B);
		float pan_1 = getParameterValue(PARAMETER_C);
	    float pan_2 = getParameterValue(PARAMETER_D);
		FloatArray left = buffer.getSamples(LEFT_CHANNEL);
		FloatArray left_duplicate = buffer.getSamples(LEFT_CHANNEL);//becomes right channel

		FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
		FloatArray right_duplicate = buffer.getSamples(RIGHT_CHANNEL);//becomes left channel

		for (int i = 0; i < buffer.getSize(); ++i) {
			
		    
            left[i] = left[i] * gain_1;
			left_duplicate[i] = left[i];//left channel to be mixed with right
			right[i] = right[i] * gain_2;
			right_duplicate[i] = right[i];//right channel to be mixed with left
			right[i] = (right[i] * pow(pan_1, 0.75))
			           + (left_duplicate[i] * pow(pan_1,0.75));
			
		    left[i] = (left[i] * pow(1.-pan_1, 0.75))
			           + (right_duplicate[i] * pow(1.-pan_1, 0.75));

		}
	}
};
#endif