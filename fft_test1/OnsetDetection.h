//
//  OnsetDetection.h
//  fft_test1~
//
//  Created by Cárthach Ó Nuanáin on 07/05/2014.
//
//

#ifndef __fft_test1___OnsetDetection__
#define __fft_test1___OnsetDetection__

#include <iostream>
#include <flext.h>
#include <fftw3.h>
#include "Filter.h"

class OnsetDetection{
public:    
    Filter filter;

    void computeOnsets();
    
    OnsetDetection();
    ~OnsetDetection();
};

#endif /* defined(__fft_test1___OnsetDetection__) */
