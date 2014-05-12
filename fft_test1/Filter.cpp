//
//  Filter.cpp
//  fft_test1~
//
//  Created by Cárthach Ó Nuanáin on 07/05/2014.
//
//

#include "Filter.h"




Filter::Filter()
{
    ffts = 1024;
    fs = 44100;
    bands=24;
    fmin=27.5;
    fmax=16000;
    equal=false;

    setup();

}

Filter::Filter(int ffts, int fs, int bands, float fmins, float fmax, bool equal)
{
    this->ffts = ffts;
    this->fs = fs;
    this->bands = bands;
    this->fmin = fmins;
    this->fmax = fmax;
    this->equal = equal;
    
    setup();

}

void Filter::setup()
{
    //Clamp fmax
    if (fmax > fs / 2.0)
        fmax = fs / 2.0;
    
    vector<float> freqs = frequencies(bands, fmin, fmax, 440);
    
    Helper::printVectorFloat(freqs);
    
    float factor = (fs / 2.0) /ffts;
    
    vector<int> freqsRound;
    
    for(int i=0;i<freqs.size(); i++) {
        int roundedValue = (int)roundf(freqs[i]/factor);
        freqsRound.push_back(roundedValue);
    }
    
    // using default comparison:
    std::vector<int>::iterator it;
    it = std::unique (freqsRound.begin(), freqsRound.end());
    
    freqsRound.resize( std::distance(freqsRound.begin(),it) );
    
    
    for (std::vector<int>::iterator it = freqsRound.begin() ; it != freqsRound.end();)
    {
        if(*it > ffts)
            freqsRound.erase(it);
        else
            ++it;
    }
    
    bands = freqsRound.size() - 2;
    
    //Todo add assert
}

Filter::~Filter()
{
    
}

vector<float> Filter::frequencies(int bands, float fmin, float fmax, float a)
{
    vector<float> frequenciesOut;
    
    float factor = powf(2.0, (1.0/(float)bands));
    
    float freq = a;

    frequenciesOut.push_back(freq);
    
    while(freq <= fmax) {
        freq *= factor;
        frequenciesOut.push_back(freq);
    }
    
    freq = a;
    
    while (freq >= fmin){
        freq /= factor;
        frequenciesOut.push_back(freq);
    }
    
    std::sort(frequenciesOut.begin(), frequenciesOut.end());
    
    
    return frequenciesOut;
}

//Need some matrices here - TNT?
vector<float> triang(int start, int mid, int stop, bool equal)
{
    float height = 1.0;

    if(equal)
        height = 2.0 / float(stop - start);
    
    int array_size = stop - start;

}





