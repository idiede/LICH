#ifndef DualStereoPan_h
#define DualStereoPan_h

#include "Patch.h"
#include "FloatArray.h"

class DualStereoPan : public Patch {
	
	private: 
		 FloatArray left_duplicate;
		 FloatArray right_duplicate;
		 
	
    public:
	   DualStereoPan() {
	
		 registerParameter(PARAMETER_A, "Gain 1");
         registerParameter(PARAMETER_B, "Gain 2");
         registerParameter(PARAMETER_C, "Pan 1");
         registerParameter(PARAMETER_D, "Pan 2");
		 left_duplicate = FloatArray::create(getBlockSize());//becomes right channel
         right_duplicate = FloatArray::create(getBlockSize());
		 
	}
	~DualStereoPan(){
		
		FloatArray::destroy(left_duplicate);
		FloatArray::destroy(right_duplicate);
	}
	// we need to create and destroy buffers outside of process audio

		void processAudio(AudioBuffer &buffer) {

		float gain_1 = getParameterValue(PARAMETER_A);
	    float gain_2 = getParameterValue(PARAMETER_B);
		float pan_1 = getParameterValue(PARAMETER_C);
	    float pan_2 = getParameterValue(PARAMETER_D);
		
		FloatArray left = buffer.getSamples(LEFT_CHANNEL);
		left_duplicate.copyFrom(left);
        FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
		right_duplicate.copyFrom(right);//becomes left channel

		for (int i = 0; i < buffer.getSize(); ++i) {
			
		    
            left[i] = left[i] * gain_1;
			left_duplicate[i] = left_duplicate[i] * gain_1;
			right[i] = right[i] * gain_2;
			right_duplicate[i] = right_duplicate[i] * gain_2;//right channel to be mixed with left
			right[i] = (right[i] * pow(pan_2, 0.75))
			          + (left_duplicate[i] * pow(pan_1,0.75));
			
		    left[i] = (left[i] * pow(1.-pan_1, 0.75))
			          + (right_duplicate[i] * pow(1.-pan_2, 0.75));

		}
	}
};
#endif