#ifndef StereoPan_hpp
#define StereoPan_hpp

#include "Patch.h"

class StereoPan : public Patch {
	
public:
	StereoPan() {
		 registerParameter(PARAMETER_A, "Gain 1");
         registerParameter(PARAMETER_B, "Gain 2");
         registerParameter(PARAMETER_C, "Pan 1");
	}

		void processAudio(AudioBuffer &buffer) {

		float gain_1 = getParameterValue(PARAMETER_A);
	    float gain_2 = getParameterValue(PARAMETER_B);
		float pan_1 = getParameterValue(PARAMETER_C);
		FloatArray left = buffer.getSamples(LEFT_CHANNEL);
		FloatArray right = buffer.getSamples(RIGHT_CHANNEL);

		for (int i = 0; i < buffer.getSize(); ++i) {
					    
            left[i] = left[i] * gain_1;
            right[i] = right[i] * gain_2;
			
			left[i] = ((left[i] * pow(1.-pan_1, 0.75))
			           + (right[i] * pow(1.-pan_1, 0.75))*0.5);
			right[i] = ((right[i] * pow(pan_1, 0.75))
			           + (left[i] * pow(pan_1,0.75))*0.5);

		}
	}
};
#endif