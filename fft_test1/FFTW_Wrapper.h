//
//  FFTW_Wrapper.h
//  fft_test1~
//
//  Created by Cárthach Ó Nuanáin on 08/05/2014.
//
//

#ifndef __fft_test1___FFTW_Wrapper__
#define __fft_test1___FFTW_Wrapper__

#include <iostream>
#include <fftw3.h>
#include <flext.h>

class FFTW_Wrapper {
public:
    int MYFFT_SIZE;
    
    fftw_complex *data_in , *fft_out;
    fftw_plan my_plan;

    void computeFFT();
    
    FFTW_Wrapper();
    ~FFTW_Wrapper();
};

#endif /* defined(__fft_test1___FFTW_Wrapper__) */
