//
//  FFTW_Wrapper.cpp
//  fft_test1~
//
//  Created by Cárthach Ó Nuanáin on 08/05/2014.
//
//

#include "FFTW_Wrapper.h"

FFTW_Wrapper::FFTW_Wrapper()
{
    MYFFT_SIZE = 512;
    
    data_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*MYFFT_SIZE);
    fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*MYFFT_SIZE);
    
    my_plan = fftw_plan_dft_1d(MYFFT_SIZE, data_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
}

FFTW_Wrapper::~FFTW_Wrapper(){
    fftw_destroy_plan(my_plan);
    fftw_free(data_in);
    fftw_free(fft_out);
}

void FFTW_Wrapper::computeFFT()
{
//    fftw_forget_wisdom(); //NEED THIS FOR MAX/MSP
    fftw_execute(my_plan);
}