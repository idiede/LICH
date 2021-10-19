#include "stereo_tap_delay.h"

namespace stereo_tap_delay {

/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2016 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing (at) cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	Delay m_delay_3;
	Delay m_delay_4;
	Delta __m_delta_12;
	PlusEquals __m_pluseq_13;
	int __exception;
	int vectorsize;
	t_sample m_A_11;
	t_sample __m_count_14;
	t_sample m_Exp_10;
	t_sample __m_latch_15;
	t_sample __m_slide_16;
	t_sample m_D_9;
	t_sample m_C_8;
	t_sample m_B_7;
	t_sample m_x_1;
	t_sample samplerate;
	t_sample __m_slide_19;
	t_sample m_x_2;
	t_sample m_Push_6;
	t_sample m_history_5;
	t_sample __m_slide_22;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_x_1 = ((int)0);
		m_x_2 = ((int)0);
		m_delay_3.reset("m_delay_3", ((int)48000));
		m_delay_4.reset("m_delay_4", ((int)48000));
		m_history_5 = ((int)0);
		m_Push_6 = ((int)0);
		m_B_7 = ((int)0);
		m_C_8 = ((int)0);
		m_D_9 = ((int)0);
		m_Exp_10 = ((int)0);
		m_A_11 = ((int)0);
		__m_delta_12.reset(0);
		__m_pluseq_13.reset(0);
		__m_count_14 = 0;
		__m_latch_15 = 0;
		__m_slide_16 = 0;
		__m_slide_19 = 0;
		__m_slide_22 = 0;
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) {
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) {
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) {
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample clamp_35 = ((m_B_7 <= ((int)0)) ? ((int)0) : ((m_B_7 >= ((t_sample)0.9)) ? ((t_sample)0.9) : m_B_7));
		t_sample clamp_30 = ((m_C_8 <= ((int)0)) ? ((int)0) : ((m_C_8 >= ((int)1)) ? ((int)1) : m_C_8));
		t_sample add_4 = (m_A_11 + ((t_sample)0.001));
		t_sample mul_3 = (add_4 * ((int)7));
		t_sample ceil_2 = ceil(mul_3);
		t_sample iup_17 = (1 / maximum(1, abs(((int)3000))));
		t_sample idown_18 = (1 / maximum(1, abs(((int)3000))));
		t_sample expr_26 = safepow(m_D_9, ((t_sample)0.75));
		t_sample expr_25 = safepow((((int)1) - m_D_9), ((t_sample)0.75));
		t_sample iup_20 = (1 / maximum(1, abs(((t_sample)0.1))));
		t_sample idown_21 = (1 / maximum(1, abs(((t_sample)0.1))));
		t_sample iup_23 = (1 / maximum(1, abs(((t_sample)0.1))));
		t_sample idown_24 = (1 / maximum(1, abs(((t_sample)0.1))));
		int min_25 = (-1);
		int min_26 = (-1);
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			t_sample delta_13 = __m_delta_12(m_Push_6);
			t_sample gtep_12 = ((delta_13 >= ((int)1)) ? delta_13 : 0);
			t_sample plusequals_11 = __m_pluseq_13.post(gtep_12, m_history_5, 0);
			int eq_10 = (plusequals_11 == ((int)2));
			__m_count_14 = (m_history_5 ? 0 : (fixdenorm(__m_count_14 + plusequals_11)));
			t_sample counter_6 = __m_count_14;
			int counter_7 = ((int)0);
			int counter_8 = ((int)0);
			__m_latch_15 = ((eq_10 != 0) ? counter_6 : __m_latch_15);
			t_sample latch_5 = __m_latch_15;
			t_sample gen_66 = latch_5;
			t_sample history_9_next_65 = fixdenorm(eq_10);
			t_sample div_1 = safediv(gen_66, ceil_2);
			__m_slide_16 = fixdenorm((__m_slide_16 + (((div_1 > __m_slide_16) ? iup_17 : idown_18) * (div_1 - __m_slide_16))));
			t_sample slide_29 = __m_slide_16;
			t_sample tap_41 = m_delay_4.read_linear(slide_29);
			t_sample fold_38 = fold(tap_41, (-1), ((int)1));
			t_sample mix_82 = (fold_38 + (clamp_35 * (m_x_2 - fold_38)));
			t_sample mix_36 = mix_82;
			t_sample tap_43 = m_delay_3.read_linear(slide_29);
			t_sample fold_39 = fold(tap_43, (-0.99), ((t_sample)0.99));
			t_sample mix_83 = (fold_39 + (clamp_35 * (m_x_1 - fold_39)));
			t_sample mix_37 = mix_83;
			__m_slide_19 = fixdenorm((__m_slide_19 + (((expr_25 > __m_slide_19) ? iup_20 : idown_21) * (expr_25 - __m_slide_19))));
			t_sample slide_18 = __m_slide_19;
			t_sample mul_24 = (mix_37 * slide_18);
			__m_slide_22 = fixdenorm((__m_slide_22 + (((expr_26 > __m_slide_22) ? iup_23 : idown_24) * (expr_26 - __m_slide_22))));
			t_sample slide_19 = __m_slide_22;
			t_sample mul_20 = (mix_36 * slide_19);
			t_sample mix_84 = (mix_37 + (((t_sample)0.5) * (mul_24 - mix_37)));
			t_sample mix_85 = (in1 + (clamp_30 * (mix_84 - in1)));
			t_sample clamp_33 = ((mix_85 <= min_25) ? min_25 : ((mix_85 >= ((int)1)) ? ((int)1) : mix_85));
			t_sample out1 = clamp_33;
			t_sample mix_86 = (mix_36 + (((t_sample)0.5) * (mul_20 - mix_36)));
			t_sample mix_87 = (in2 + (clamp_30 * (mix_86 - in2)));
			t_sample clamp_31 = ((mix_87 <= min_26) ? min_26 : ((mix_87 >= ((int)1)) ? ((int)1) : mix_87));
			t_sample out2 = clamp_31;
			t_sample x2_next_80 = fixdenorm(mix_36);
			t_sample x1_next_81 = fixdenorm(mix_37);
			m_history_5 = history_9_next_65;
			m_delay_4.write((m_x_2 + in2));
			m_delay_3.write((m_x_1 + in1));
			m_x_2 = x2_next_80;
			m_x_1 = x1_next_81;
			m_delay_3.step();
			m_delay_4.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_Push(t_param _value) {
		m_Push_6 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_B(t_param _value) {
		m_B_7 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_C(t_param _value) {
		m_C_8 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_D(t_param _value) {
		m_D_9 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_Exp(t_param _value) {
		m_Exp_10 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_A(t_param _value) {
		m_A_11 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	
} State;


///
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs

int gen_kernel_numins = 2;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 6; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1", "in2" };
const char *gen_kernel_outnames[] = { "out1", "out2" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_A(value); break;
		case 1: self->set_B(value); break;
		case 2: self->set_C(value); break;
		case 3: self->set_D(value); break;
		case 4: self->set_Exp(value); break;
		case 5: self->set_Push(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_A_11; break;
		case 1: *value = self->m_B_7; break;
		case 2: *value = self->m_C_8; break;
		case 3: *value = self->m_D_9; break;
		case 4: *value = self->m_Exp_10; break;
		case 5: *value = self->m_Push_6; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(6 * sizeof(ParamInfo));
	self->__commonstate.numparams = 6;
	// initialize parameter 0 ("m_A_11")
	pi = self->__commonstate.params + 0;
	pi->name = "A";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_A_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_B_7")
	pi = self->__commonstate.params + 1;
	pi->name = "B";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_B_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_C_8")
	pi = self->__commonstate.params + 2;
	pi->name = "C";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_C_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_D_9")
	pi = self->__commonstate.params + 3;
	pi->name = "D";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_D_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_Exp_10")
	pi = self->__commonstate.params + 4;
	pi->name = "Exp";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_Exp_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_Push_6")
	pi = self->__commonstate.params + 5;
	pi->name = "Push";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_Push_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // stereo_tap_delay::
