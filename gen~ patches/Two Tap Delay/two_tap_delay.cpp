#include "two_tap_delay.h"

namespace two_tap_delay {

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
	Delay m_delay_8;
	Delay m_delay_6;
	Delay m_delay_7;
	Delay m_delay_5;
	int __exception;
	int vectorsize;
	t_sample m_y_3;
	t_sample m_Exp_13;
	t_sample m_Push_12;
	t_sample m_D_14;
	t_sample __m_slide_18;
	t_sample __m_slide_15;
	t_sample m_C_11;
	t_sample m_B_10;
	t_sample m_A_9;
	t_sample m_y_4;
	t_sample m_x_2;
	t_sample m_x_1;
	t_sample __m_slide_21;
	t_sample samplerate;
	t_sample __m_slide_24;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_x_1 = ((int)0);
		m_x_2 = ((int)0);
		m_y_3 = ((int)0);
		m_y_4 = ((int)0);
		m_delay_5.reset("m_delay_5", ((int)48000));
		m_delay_6.reset("m_delay_6", ((int)48000));
		m_delay_7.reset("m_delay_7", ((int)48000));
		m_delay_8.reset("m_delay_8", ((int)48000));
		m_A_9 = ((int)0);
		m_B_10 = ((int)0);
		m_C_11 = ((int)0);
		m_Push_12 = ((int)0);
		m_Exp_13 = ((int)0);
		m_D_14 = ((int)0);
		__m_slide_15 = 0;
		__m_slide_18 = 0;
		__m_slide_21 = 0;
		__m_slide_24 = 0;
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
		t_sample iup_16 = (1 / maximum(1, abs(((t_sample)0.25))));
		t_sample idown_17 = (1 / maximum(1, abs(((t_sample)0.25))));
		t_sample iup_19 = (1 / maximum(1, abs(((t_sample)0.25))));
		t_sample idown_20 = (1 / maximum(1, abs(((t_sample)0.25))));
		t_sample mul_1712 = (m_B_10 * ((int)48000));
		t_sample iup_22 = (1 / maximum(1, abs(((int)3000))));
		t_sample idown_23 = (1 / maximum(1, abs(((int)3000))));
		t_sample mul_1726 = (m_A_9 * ((int)48000));
		t_sample iup_25 = (1 / maximum(1, abs(((int)3000))));
		t_sample idown_26 = (1 / maximum(1, abs(((int)3000))));
		t_sample min_27 = (-0.999);
		t_sample min_28 = (-0.999);
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			__m_slide_15 = fixdenorm((__m_slide_15 + (((m_C_11 > __m_slide_15) ? iup_16 : idown_17) * (m_C_11 - __m_slide_15))));
			t_sample slide_1700 = __m_slide_15;
			t_sample clamp_1721 = ((slide_1700 <= ((int)0)) ? ((int)0) : ((slide_1700 >= ((t_sample)0.999)) ? ((t_sample)0.999) : slide_1700));
			__m_slide_18 = fixdenorm((__m_slide_18 + (((m_D_14 > __m_slide_18) ? iup_19 : idown_20) * (m_D_14 - __m_slide_18))));
			t_sample slide_1699 = __m_slide_18;
			t_sample clamp_1716 = ((slide_1699 <= ((int)0)) ? ((int)0) : ((slide_1699 >= ((t_sample)0.999)) ? ((t_sample)0.999) : slide_1699));
			__m_slide_21 = fixdenorm((__m_slide_21 + (((mul_1712 > __m_slide_21) ? iup_22 : idown_23) * (mul_1712 - __m_slide_21))));
			t_sample slide_1711 = __m_slide_21;
			t_sample tap_1710 = m_delay_8.read_linear(slide_1711);
			t_sample fold_1707 = fold(tap_1710, (-0.99), ((t_sample)0.99));
			t_sample mix_1753 = (fold_1707 + (clamp_1721 * (m_y_4 - fold_1707)));
			t_sample mix_1705 = mix_1753;
			t_sample sub_1701 = (slide_1711 - ((int)113));
			t_sample tap_1714 = m_delay_7.read_linear(sub_1701);
			t_sample fold_1708 = fold(tap_1714, (-0.99), ((t_sample)0.99));
			t_sample mix_1754 = (fold_1708 + (clamp_1721 * (m_y_3 - fold_1708)));
			t_sample mix_1706 = mix_1754;
			__m_slide_24 = fixdenorm((__m_slide_24 + (((mul_1726 > __m_slide_24) ? iup_25 : idown_26) * (mul_1726 - __m_slide_24))));
			t_sample slide_1715 = __m_slide_24;
			t_sample tap_1728 = m_delay_6.read_linear(slide_1715);
			t_sample fold_1724 = fold(tap_1728, (-0.99), ((t_sample)0.99));
			t_sample mix_1755 = (fold_1724 + (clamp_1721 * (m_x_2 - fold_1724)));
			t_sample mix_1722 = mix_1755;
			t_sample mul_1703 = ((mix_1705 + mix_1722) * ((t_sample)0.499));
			t_sample mix_1756 = (in2 + (clamp_1716 * (mul_1703 - in2)));
			t_sample clamp_1717 = ((mix_1756 <= min_27) ? min_27 : ((mix_1756 >= ((t_sample)0.999)) ? ((t_sample)0.999) : mix_1756));
			t_sample out2 = clamp_1717;
			t_sample sub_1702 = (slide_1715 - ((int)113));
			t_sample tap_1730 = m_delay_5.read_linear(sub_1702);
			t_sample fold_1725 = fold(tap_1730, (-0.99), ((t_sample)0.99));
			t_sample mix_1757 = (fold_1725 + (clamp_1721 * (m_x_1 - fold_1725)));
			t_sample mix_1723 = mix_1757;
			t_sample mul_1704 = ((mix_1706 + mix_1723) * ((t_sample)0.499));
			t_sample mix_1758 = (in1 + (clamp_1716 * (mul_1704 - in1)));
			t_sample clamp_1719 = ((mix_1758 <= min_28) ? min_28 : ((mix_1758 >= ((t_sample)0.999)) ? ((t_sample)0.999) : mix_1758));
			t_sample out1 = clamp_1719;
			t_sample y2_next_1731 = fixdenorm(mix_1705);
			t_sample y1_next_1732 = fixdenorm(mix_1706);
			t_sample x2_next_1733 = fixdenorm(mix_1722);
			t_sample x1_next_1734 = fixdenorm(mix_1723);
			m_delay_8.write((m_y_4 + in2));
			m_delay_7.write((m_y_3 + in1));
			m_delay_6.write((m_x_2 + in2));
			m_delay_5.write((m_x_1 + in1));
			m_y_4 = y2_next_1731;
			m_y_3 = y1_next_1732;
			m_x_2 = x2_next_1733;
			m_x_1 = x1_next_1734;
			m_delay_5.step();
			m_delay_6.step();
			m_delay_7.step();
			m_delay_8.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_A(t_param _value) {
		m_A_9 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_B(t_param _value) {
		m_B_10 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_C(t_param _value) {
		m_C_11 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_Push(t_param _value) {
		m_Push_12 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_Exp(t_param _value) {
		m_Exp_13 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_D(t_param _value) {
		m_D_14 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
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
		case 0: *value = self->m_A_9; break;
		case 1: *value = self->m_B_10; break;
		case 2: *value = self->m_C_11; break;
		case 3: *value = self->m_D_14; break;
		case 4: *value = self->m_Exp_13; break;
		case 5: *value = self->m_Push_12; break;
		
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
	// initialize parameter 0 ("m_A_9")
	pi = self->__commonstate.params + 0;
	pi->name = "A";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_A_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_B_10")
	pi = self->__commonstate.params + 1;
	pi->name = "B";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_B_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_C_11")
	pi = self->__commonstate.params + 2;
	pi->name = "C";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_C_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_D_14")
	pi = self->__commonstate.params + 3;
	pi->name = "D";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_D_14;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_Exp_13")
	pi = self->__commonstate.params + 4;
	pi->name = "Exp";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_Exp_13;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_Push_12")
	pi = self->__commonstate.params + 5;
	pi->name = "Push";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_Push_12;
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


} // two_tap_delay::
