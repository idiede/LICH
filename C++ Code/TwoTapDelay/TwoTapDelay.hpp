#ifndef TwoTapDelay_hpp
#define TwoTapDelay_hpp

#include "Patch.h"
#include "DcFilter.hpp"
#include "CircularBuffer.hpp"
#include "TapTempo.hpp"
#include "SmoothValue.h"


class TwoTapDelay : public Patch {
private:
     CircularBuffer* delayBufferOne;
     CircularBuffer* delayBufferTwo;
	 static const int TRIGGER_LIMIT = (1<<17);
	 TapTempo<TRIGGER_LIMIT> tempoOne;
	 TapTempo<TRIGGER_LIMIT> tempoTwo;
     int delay_1, delay_2, ratio;
     SmoothFloat time_1;
	 SmoothFloat time_2;
     SmoothFloat drop;// =1
     SmoothFloat feedback;
     StereoDcFilter dc;
	 
public:
 TwoTapDelay() : 
    delay_1(0), delay_2(0), tempoOne(getSampleRate()*60/120), tempoTwo(getSampleRate()*60/120)
	{
	registerParameter(PARAMETER_A, "Rate 1");
    registerParameter(PARAMETER_B, "Rate 2");
    registerParameter(PARAMETER_C, "Feedback");
    registerParameter(PARAMETER_D, "Dry/wet");
	delayBufferOne = CircularBuffer::create(TRIGGER_LIMIT);
    delayBufferTwo = CircularBuffer::create(TRIGGER_LIMIT);		
}
 
 ~TwoTapDelay(){
	  CircularBuffer::destroy(delayBufferOne);
      CircularBuffer::destroy(delayBufferTwo);
	}
	
	float delayTimeOne(int ratio){
         float time = tempoOne.getPeriod();
         time = max(0.0001, min(0.9999, time));
      return time;
  }
  float delayTimeTwo(int ratio){
         float time = tempoTwo.getPeriod();
         time = max(0.0001, min(0.9999, time));
      return time;
  }
	void processAudio(AudioBuffer &buffer){
	   int speed_1 = getParameterValue(PARAMETER_A)*4096;//for now no taptempo
	   int speed_2 = getParameterValue(PARAMETER_B)*4096;//for now no taptempo
	   drop = 1.0;
	   ratio = 1.0;
	   feedback = getParameterValue(PARAMETER_C);//only one feedback for both
	   float wet = getParameterValue(PARAMETER_D);
	   float dry = 1.0-wet;
	   int size = buffer.getSize();
	   tempoOne.clock(size);
	   tempoTwo.clock(size);
       tempoOne.setSpeed(speed_1);
	   tempoTwo.setSpeed(speed_2);
       time_1 = delayTimeOne(ratio);
	   time_2 = delayTimeTwo(ratio);   
       FloatArray left = buffer.getSamples(LEFT_CHANNEL);
       FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
	  // FloatArray temp_left = buffer.getSamples(LEFT_CHANNEL);
	   //FloatArray temp_right = buffer.getSamples(RIGHT_CHANNEL);
	   
	   int newDelay1 = time_1*(delayBufferOne->getSize()-1);
       int newDelay2 = time_2*(delayBufferTwo->getSize()-1);
	   
	   dc.process(buffer); // remove DC offset
	   
	for(int n=0; n<size; n++){
      float x1 = n/(float)size;
      float x0 = 1.0-x1;
      float dly1 = delayBufferOne->read(delay_1)*x0 + delayBufferOne->read(newDelay1)*x1;
	  float dly2 = delayBufferTwo->read(delay_2)*x0 + delayBufferTwo->read(newDelay2)*x1;
   
      //write delay line
      delayBufferOne->write(feedback*dly1 + drop*left[n]);
      delayBufferTwo->write(feedback*dly2 + drop*right[n]);//combine at end
	  
      
	  left[n] = dly1*wet + left[n]*dry;//(temp_left[n] + temp_right[n]) * 0.5;
	  right[n] = dly2*wet + right[n]*dry;//(temp_left[n] + temp_right[n]) * 0.5;	  
    }
	
}

};//class
#endif