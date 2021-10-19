#ifndef StereoTapDelay_hpp
#define StereoTapDelay_hpp

#include "Patch.h"
#include "DcFilter.hpp"
#include "CircularBuffer.hpp"
#include "TapTempo.hpp"
#include "SmoothValue.h"


class StereoTapDelay : public Patch {
private:
     CircularBuffer* delayBufferLeft;
     CircularBuffer* delayBufferRight;
	 static const int TRIGGER_LIMIT = (1<<17);
	 TapTempo<TRIGGER_LIMIT> tempo;
     int delay_left, delay_right, ratio;
     SmoothFloat time;
     SmoothFloat drop;// =1
     SmoothFloat feedback;
     StereoDcFilter dc;
	 
public:
 StereoTapDelay() : 
    delay_left(0), delay_right(0), tempo(getSampleRate()*60/120)
	{
	registerParameter(PARAMETER_A, "Rate");
    registerParameter(PARAMETER_B, "Feedback");
    registerParameter(PARAMETER_C, "Dry/Wet");
    registerParameter(PARAMETER_D, "Pan");
	delayBufferLeft = CircularBuffer::create(TRIGGER_LIMIT);
    delayBufferRight = CircularBuffer::create(TRIGGER_LIMIT*2);
}
 
 ~StereoTapDelay(){
	  CircularBuffer::destroy(delayBufferLeft);
      CircularBuffer::destroy(delayBufferRight);
	}
	float delayTime(int ratio){
         float time = tempo.getPeriod();
         time = max(0.0001, min(0.9999, time));
      return time;
  }
	void processAudio(AudioBuffer &buffer){
	   int speed = getParameterValue(PARAMETER_A)*4096;//for now no taptempo
	   drop = 1.0;
	   ratio = 1.0;
	   feedback = getParameterValue(PARAMETER_B);
	   float wet = getParameterValue(PARAMETER_C);
	   float dry = 1.0-wet;
	   float pan = getParameterValue(PARAMETER_D);
	   int size = buffer.getSize();
	   tempo.clock(size);
       tempo.setSpeed(speed);
       time = delayTime(ratio);
	
	   //delayBufferLeft = CircularBuffer::create(size);
       //delayBufferRight = CircularBuffer::create(size);
       
	   
       FloatArray left = buffer.getSamples(LEFT_CHANNEL);
       FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
	   int newDelayL = time*(delayBufferLeft->getSize()-1);//*time = 1
       int newDelayR = time*(delayBufferRight->getSize()-1);
	   dc.process(buffer); // remove DC offset
	   
	for(int n=0; n<size; n++){
      float x1 = n/(float)size;
      float x0 = 1.0-x1;
      float dly = delayBufferLeft->read(delay_left)*x0 + delayBufferLeft->read(newDelayL)*x1;
      //float rdly = delayBufferRight->read(delay_right)*x0 + delayBufferRight->read(newDelayR)*x1;
      //write delay line
      delayBufferRight->write(feedback*dly + drop*left[n]);
      delayBufferLeft->write(feedback*dly + drop*right[n]);
	  
	  	/*  left[i] = ((left[i] * pow(1.-pan_1, 0.75))
			           + (right[i] * pow(1.-pan_1, 0.75))*0.5);
			right[i] = ((right[i] * pow(pan_1, 0.75))
			           + (left[i] * pow(pan_1,0.75))*0.5);
        */
					   
      left[n] = dly*wet * pow(1.-pan, 0.75) + left[n]*dry;
      right[n] = dly*wet * pow(pan, 0.75) + right[n]*dry;
    }
	
	/*for(int i=0; i<buffer.getSize(); i++){
      float x1 = i/(float)buffer.getSize();
      float x0 = 1.0-x1;//we don't have time only rate and feedback
      float ldly = delayBufferLeft->read(delay_left)*x0;// + delayBufferLeft->read(newDelayL)*x1;
      float rdly = delayBufferRight->read(delay_left)*x0;// + delayBufferRight->read(newDelayR)*x1;
      // stereo pan
      //delayBufferRight->write(feedback*ldly + left[i]);//this mixing left and right?
      //delayBufferLeft->write(feedback*rdly + right[i]);
	*/  
	/*  left[i] = ((left[i] * pow(1.-pan_1, 0.75))
			           + (right[i] * pow(1.-pan_1, 0.75))*0.5);
			right[i] = ((right[i] * pow(pan_1, 0.75))
			           + (left[i] * pow(pan_1,0.75))*0.5);
    
					   
      left[i] = (ldly*wet * pow(1.-pan, 0.75)) + (rdly* wet * pow(1.-pan, 0.75)) + left[i]*dry;
      right[i] = (rdly*wet * pow(pan, 0.75)) + (rdly* wet * pow(pan, 0.75)) + right[i]*dry;
	*/  
	  
    //}
}

};//class
#endif