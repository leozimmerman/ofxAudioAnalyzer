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
#include "ofxAAMultiPitchKlapuriAlgorithm.h"
#include "ofxAAOnsetsAlgorithm.h"


//----------------------------------
//for scaling values:
#define DB_MIN -6
#define DB_MAX 0
#define MFCC_MAX_ESTIMATED_VALUE 300.0
//----------------------------------
//Vars for algorithm creation:
#define MELBANDS_BANDS_NUM 24
#define DCT_COEFF_NUM 13
#define PITCH_SALIENCE_FUNC_NUM 10
#define TRISTIMULUS_BANDS_NUM 3

#define HPCP_SIZE 12
#define HPCP_MIN_FREQ 40.0//hz
#define HPCP_MAX_FREQ 5000.0//hz

#define PEAKS_MAXPEAKS_NUM 10000
#define PEAKS_MIN_FREQ 40.0//hz
#define PEAKS_MAX_FREQ 5000.0//hz
//----------------------------------

class ofxAudioAnalyzerUnit
{

public:
    
    ofxAudioAnalyzerUnit(int sampleRate, int bufferSize){
        setup(sampleRate, bufferSize);
    }
    ~ofxAudioAnalyzerUnit(){
        exit();
    }
    void setup(int sampleRate, int bufferSize);
    void exit();

    void analyze(const vector<float> &  inBuffer);
    
    void setActive(ofxAAAlgorithm algorithm, bool state);
    
    void setMaxEstimatedValue(ofxAAAlgorithm algorithm, float value);
    
    float getValue(ofxAAAlgorithm algorithm, float smooth=0.0, bool normalized=false);
    bool getOnsetValue();
    vector<float>& getValues(ofxAAAlgorithm algorithm, float smooth=0.0);
    vector<SalienceFunctionPeak>& getPitchSaliencePeaksRef(float smooth=0.0);
    int getBinsNum(ofxAAAlgorithm algorithm);

    int   getPitchFreqAsMidiNote(float smooth=0.0);
    string getPitchFreqAsNoteName(float smooth=0.0);
    
    

    //Onsets:
    void resetOnsets();
    void setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
    ofxAAOnsetsAlgorithm* getOnsetsAlgorithmPtr(){return &onsets;}

    //PitchSalienceFunction
    void setSalienceFunctionPeaksParameters(int maxPeaks);
    
    
    
   
private:
    //Utils:
    int pitchToMidi(float pitch);
    string midiToNoteName(int midiNote);
    
    //Algorithms:
    vector<Real> audioBuffer;

    //algorithms with return value func
    ofxAABaseAlgorithm rms;
    ofxAABaseAlgorithm energy;
    ofxAABaseAlgorithm power;
    ofxAAPitchDetectAlgorithm pitchDetect;
    ofxAABaseAlgorithm pitchSalience;
    //ofxAATuningFrequencyAlgorithm tuning;
    ofxAABaseAlgorithm inharmonicity;
    ofxAABaseAlgorithm hfc;
    ofxAABaseAlgorithm centroid;
    ofxAABaseAlgorithm spectralComplex;
    ofxAABaseAlgorithm dissonance;
    ofxAABaseAlgorithm rollOff;
    ofxAABaseAlgorithm oddToEven;
    ofxAABaseAlgorithm strongPeak;
    ofxAABaseAlgorithm strongDecay;
    
    ofxAAOneVectorOutputAlgorithm spectrum;
    ofxAAOneVectorOutputAlgorithm melBands;
    ofxAAOneVectorOutputAlgorithm dct;//MFCC
    ofxAAOneVectorOutputAlgorithm hpcp;
    ofxAAOneVectorOutputAlgorithm tristimulus;
    //MultiPitch
    ofxAAPitchSalienceFunctionPeaksAlgorithm pitchSalienceFunctionPeaks;
    ofxAAMultiPitchKlapuriAlgorithm multiPitchKlapuri;
    
    //Algorithms for internal functionality:
    ofxAAOneVectorOutputAlgorithm dcremoval;
    ofxAAOneVectorOutputAlgorithm window;
    ofxAAFftAlgorithm fft;
    ofxAACartToPolAlgorithm cartesian2polar;
    ofxAAPeaksAlgorithm spectralPeaks;
    ofxAAPeaksAlgorithm harmonicPeaks;
    ofxAAOneVectorOutputAlgorithm pitchSalienceFunction;
    ofxAAOnsetsAlgorithm onsets;
    
    //--------
    int framesize;
    int hopsize;
    int sr;
    int zeropadding;
    Real framerate;
    
    //Max estimated values (for normalizing)
    float maxEnergyEstimatedValue;
    float maxHfcEstimatedValue;
    float maxSpecCompEstimatedValue;
    float maxCentroidEstimatedValue;
    float maxOddToEvenEstimatedValue;
    float maxStrongPeakEstimatedValue;
    float maxStrongDecayEstimatedValue;

    
};


