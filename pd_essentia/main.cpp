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

#include <essentia/algorithmfactory.h>
#include <essentia/essentiamath.h>
#include <essentia/pool.h>

using std::vector;

using namespace std;
using namespace essentia;
using namespace essentia::standard;

namespace Helper {
    void printVectorFloat(const vector<float> &vecIn)
    {
        for(int i=0; i<vecIn.size(); i++)
            std::cout << vecIn[i] << "\n";
    }
    void printVectorInt(const vector<int> &vecIn)
    {
        for(int i=0; i<vecIn.size(); i++)
            std::cout << vecIn[i] << "\n";
    }
}

using std::vector;

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


class pd_essentia : public flext_dsp
{
    FLEXT_HEADER(pd_essentia, flext_dsp)
    
    int fft_counter;
    


    pd_essentia()
    {
        AddInSignal("In");
        AddOutSignal("Out");
        AddOutList("FFT");
        
        FLEXT_ADDBANG(0,my_bang);
        
        
    }
    
    ~pd_essentia()
    {
        

    }
    
    virtual void m_signal(int n, t_sample *const *insigs, t_sample *const *outsigs)
    {
        const t_sample *in = insigs[0];
        t_sample *out = outsigs[0];
        while(n--) {
            *(out++) = *(in++);
        }
    }
    
    
    AtomList floatVectorToList(const std::vector<float>& floatVector) {
        AtomList listOut(sizeof(floatVector));
        
        for(int i=0; i<floatVector.size(); i++) {
            t_atom value;
            SetFloat(value, floatVector[i]);
            
            listOut[i] = value;
        }
        
        return listOut;
    }
    
    AtomList stringVectorToList(const std::vector<string>& stringVector) {
        AtomList listOut(sizeof(stringVector));
        
        for(int i=0; i<stringVector.size(); i++) {
            t_atom value;
            SetString(value, stringVector[i].c_str());
            
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
        
        //
        string audioFilename = "/Users/carthach/Dev/Pd/User_Libraries/in.wav";
        string outputFilename = "/Users/carthach/Dev/Pd/User_Libraries/out.sig";
        
        
        // register the algorithms in the factory(ies)
        essentia::init();
        
        Pool pool;
        
        /////// PARAMS //////////////
        int sampleRate = 44100;
        int frameSize = 2048;
        int hopSize = 1024;
        
        // we want to compute the MFCC of a file: we need the create the following:
        // audioloader -> framecutter -> windowing -> FFT -> MFCC
        
        AlgorithmFactory& factory = standard::AlgorithmFactory::instance();
        
        Algorithm* audio = factory.create("MonoLoader",
                                          "filename", audioFilename,
                                          "sampleRate", sampleRate);
        
        Algorithm* fc    = factory.create("FrameCutter",
                                          "frameSize", frameSize,
                                          "hopSize", hopSize);
        
        Algorithm* w     = factory.create("Windowing",
                                          "type", "blackmanharris62");
        
        Algorithm* spec  = factory.create("Spectrum");
        Algorithm* mfcc  = factory.create("MFCC");
        
        
        
        
        /////////// CONNECTING THE ALGORITHMS ////////////////
        cout << "-------- connecting algos ---------" << endl;
        
        // Audio -> FrameCutter
        vector<Real> audioBuffer;
        
        audio->output("audio").set(audioBuffer);
        fc->input("signal").set(audioBuffer);
        
        // FrameCutter -> Windowing -> Spectrum
        vector<Real> frame, windowedFrame;
        
        fc->output("frame").set(frame);
        w->input("frame").set(frame);
        
        w->output("frame").set(windowedFrame);
        spec->input("frame").set(windowedFrame);
        
        // Spectrum -> MFCC
        vector<Real> spectrum, mfccCoeffs, mfccBands;
        
        spec->output("spectrum").set(spectrum);
        mfcc->input("spectrum").set(spectrum);
        
        mfcc->output("bands").set(mfccBands);
        mfcc->output("mfcc").set(mfccCoeffs);
        
        
        
        /////////// STARTING THE ALGORITHMS //////////////////
        cout << "-------- start processing " << audioFilename << " --------" << endl;
        
        audio->compute();
        
        while (true) {
            
            // compute a frame
            fc->compute();
            
            // if it was the last one (ie: it was empty), then we're done.
            if (!frame.size()) {
                break;
            }
            
            // if the frame is silent, just drop it and go on processing
            if (isSilent(frame)) continue;
            
            w->compute();
            spec->compute();
            mfcc->compute();
            
            pool.add("lowlevel.mfcc", mfccCoeffs);
            
        }
        
        // aggregate the results
        Pool aggrPool; // the pool with the aggregated MFCC values
        const char* stats[] = { "mean", "var", "min", "max" };
        
        Algorithm* aggr = AlgorithmFactory::create("PoolAggregator",
                                                   "defaultStats", arrayToVector<string>(stats));
        
        aggr->input("input").set(pool);
        aggr->output("output").set(aggrPool);
        aggr->compute();
        
        // write results to file
        cout << "-------- writing results to file " << outputFilename << " ---------" << endl;
        
        Algorithm* output = AlgorithmFactory::create("YamlOutput",
                                                     "filename", outputFilename);
        output->input("pool").set(aggrPool);
        output->compute();
        
        AtomList names = stringVectorToList(aggrPool.descriptorNames());
        
        ToOutList(1, names);
        
        
        delete audio;
        delete fc;
        delete w;
        delete spec;
        delete mfcc;
        delete aggr;
        delete output;
        
        essentia::shutdown();
        
        

    }
    

    
    FLEXT_CALLBACK(my_bang);
};
FLEXT_NEW_DSP("pd_essentia~", pd_essentia)

