/*
 flext tutorial - simple 2
 
 Copyright (c) 2002,2003 Thomas Grill (xovo@gmx.net)
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "license.txt," in this distribution.
 
 -------------------------------------------------------------------------
 
 This is an example of a simple object doing a float addition
 */

// include flext header
#include <flext.h>
#include "math.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "OnsetDetection.h"
#include "Filter.h"
#include "FFTW_Wrapper.h"



using std::vector;

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


class fft_test1 : public flext_dsp
{
    FLEXT_HEADER(fft_test1, flext_dsp)
    
    FFTW_Wrapper fftw;
    Filter filter;


    int fft_counter;

    fft_test1()
    {
        AddInSignal("In");
        AddOutSignal("Out");
        AddOutList("FFT");
        
        FLEXT_ADDBANG(0,my_bang);
        
        int fft_counter = 0;
        
        int sr = Samplerate();
        int SFFRAMES = 2048;
        
        filter.ffts = SFFRAMES / 2;
        
        
        std::string result;
        std::stringstream sstm;
        sstm << filter.ffts;
        result = sstm.str();
        
        
        post(result.c_str());
        
    }
    
    ~fft_test1()
    {

    }
    
    virtual void m_signal(int n, t_sample *const *insigs, t_sample *const *outsigs)
    {
        const t_sample *in = insigs[0];
        t_sample *out = outsigs[0];
        while(n--) {
            fftw.data_in[fft_counter++][0] = *in;
            *(out++) = *(in++);
        }
        
        if(fft_counter >= fftw.MYFFT_SIZE)
        {
            fftw.computeFFT();
            fft_counter = 0;
        }
    }
    
    void my__other_bang() {
        AtomList fftToMax(fftw.MYFFT_SIZE);


        for(int i=0; i<fftw.MYFFT_SIZE; i++) {
            t_atom sample;
            
            float real = fftw.fft_out[i][0];
            float img = fftw.fft_out[i][1];
            
            float realSquare = powf(real, 2.0);
            float imgSquare = powf(img, 2.0);
            
            float magSquare = realSquare + imgSquare;
            float mag = sqrtf(magSquare);
            
            SetFloat(sample, mag);
            
            fftToMax[i] = sample;
        }
        
        ToOutList(1, fftToMax);
    }
    
    AtomList vectorToList(const std::vector<float>& floatVector) {
        AtomList listOut(sizeof(floatVector));
        
        for(int i=0; i<fftw.MYFFT_SIZE; i++) {
            t_atom value;
            SetFloat(value, floatVector[i]);
            
            listOut[i] = value;
        }
        
        return listOut;
    }
    
    void my_bang() {
        AtomList fftToMax(10);
        
        
        for(int i=0; i<10; i++) {
            t_atom sample;
            

            
            SetFloat(sample, 5.0);
            
            fftToMax[i] = sample;
        }
        
        std::cout << "Hello\n";
        
        ToOutList(1, fftToMax);
    }
    

    
    FLEXT_CALLBACK(my_bang);
};
FLEXT_NEW_DSP("fft_test1~", fft_test1)

