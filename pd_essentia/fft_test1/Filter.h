//
//  Filter.h
//  fft_test1~
//
//  Created by Cárthach Ó Nuanáin on 07/05/2014.
//
//

#ifndef __fft_test1___Filter__
#define __fft_test1___Filter__
#include <flext.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using std::vector;

class Filter{
public:
    int ffts, fs, bands;
    float fmin, fmax;
    bool equal;
    
    
    float a;
    

    
    Filter();
    Filter(int ffts, int fs, int bands, float fmins, float fmax, bool equal);
    
    void setup();
    ~Filter();
    
    //Methods
    vector<float> frequencies(int bands, float fmin, float fmax, float a);
    
    vector<float> triang(int start, int mid, int stop, bool equal);
};

#endif /* defined(__fft_test1___Filter__) */
