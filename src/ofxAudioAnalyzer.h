#pragma once

#include "ofMain.h"

#include <iostream>
#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h" // for the isSilent function
#include "essentia/pool.h"

using namespace std;
using namespace essentia;
using namespace standard;

#include "ofxAudioAnalyzerAlgorithms.h"

//----------------------------

#define MELBANDS_BANDS_NUM 24
#define DCT_COEFF_NUM 10

#define HPCP_SIZE 12
#define HPCP_MIN_FREQ 40.0//hz
#define HPCP_MAX_FREQ 5000.0//hz

#define PEAKS_MAXPEAKS_NUM 10000
#define PEAKS_MIN_FREQ 40.0//hz
#define PEAKS_MAX_FREQ 5000.0//hz


#define ENERGY_MAX_ESTIMATED_VALUE 100
#define HFC_MAX_ESTIMATED_VALUE 1000.0
#define SPEC_COMP_MAX_ESTIMATED_VALUE 30.0
#define CENTROID_MAX_ESTIMATED_VALUE 7000.0

#define DB_MIN -6
#define DB_MAX 0

//----------------------------------

class ofxAudioAnalyzer
{
    public:

        void setup(int bufferSize, int sampleRate);
        void exit();
        void analyze(float * iBuffer, int bufferSize);
    
        void resetOnsets();
    
        ///---------------------------
    
    ///--------
    
        float getRms(float smooth=0.0);
        float getEnergy(float smooth=0.0);
        float getPower(float smooth=0.0);
    
        float getPitchFreq(float smooth=0.0);
        int   getPitchFreqAsMidiNote(float smooth=0.0);
        string getPitchFreqAsNoteName(float smooth=0.0);
        float getPitchConfidence(float smooth=0.0);
        float getMelodySalience(float smooth=0.0);
    
        float getTuningFreq();
        float getTuningCents();
        
        float getInharmonicity(float smooth=0.0);
        float getHfc(float smooth=0.0);
        float getSpectralComplex(float smooth=0.0);
        float getCentroid(float smooth=0.0);
    
        float getHfcNormalized(float smooth=0.0);
        float getSpectralComplexNormalized(float smooth=0.0);
        float getCentroidNormalized(float smooth=0.0);
        
        bool getIsOnset();
    
        int getSpectrumBinsNum();
        int getMelBandsBinsNum();
        int getMfccBinsNum();
        int getHpcpBinsNum();
        
        vector<float>& getSpectrumRef();
        vector<float>& getMelBandsRef();
        vector<float>& getDctRef();
        vector<float>& getHpcpRef();
    
    
        //--------------------------------------------
    
        void setOnsetTreshold(float val);
        void setOnsetAlpha(float val);
    


    
        //For storing results casted to Floats ---------------------
    
    

     private:
    
        vector<Real> audioBuffer;
    
        ofxAABaseAlgorithm rms;
        ofxAABaseAlgorithm energy;
        ofxAABaseAlgorithm power;
        ofxAAPitchDetectAlgorithm pitchDetect;
        ofxAABaseAlgorithm pitchSalience;
        ofxAATuningFrequencyAlgorithm tuningFreq;
        ofxAABaseAlgorithm inharmonicity;
        ofxAABaseAlgorithm hfc;
        ofxAABaseAlgorithm centroid;
        ofxAABaseAlgorithm spectralComplex;
        ofxAAOneVectorOutputAlgorithm spectrum;
        ofxAAOneVectorOutputAlgorithm melBands;
        ofxAAOneVectorOutputAlgorithm dct;//MFCC
        ofxAAOneVectorOutputAlgorithm hpcp;
        ofxAAOneVectorOutputAlgorithm dcremoval;
        ofxAAOneVectorOutputAlgorithm window;
        ofxAAFftAlgorithm fft;
        ofxAACartToPolAlgorithm cartesian2polar;
        ofxAAPeaksAlgorithm peaks;
        ofxAAPeaksAlgorithm harmonicPeaks;
        ofxAABaseAlgorithm onsetHfc;
        ofxAABaseAlgorithm onsetComplex;
        ofxAABaseAlgorithm onsetFlux;
    
        //Onset detection------------
        bool onsetEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux);
        bool isOnset;
        Real silenceTreshold, alpha;
        bool addHfc, addComplex, addFlux;
        bool doOnsets;
    
        int framesize;
        int hopsize;
        int sr;
        int zeropadding;
        Real framerate;
    
        int detecBufferSize;
        vector<vector<Real> > detections;
        vector<Real> detection_sum;
        Real hfc_max, complex_max, flux_max;
    
        //Utils
        int pitchToMidi(float pitch);
        string midiToNoteName(int midiNote);
};


