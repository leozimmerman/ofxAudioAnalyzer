
#pragma once

//taken from essentia/src/algorithms/tonal/multipitchklapuri.cpp

#include "ofxAudioAnalyzerAlgorithms.h"

class ofxAAMultiPitchKlapuriAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    void setup(ofxAAPitchSalienceFunctionPeaksAlgorithm* saliencePeaksPtr,
               ofxAAOneVectorOutputAlgorithm* spectrumPtr,
               int sampleRate);
    void compute();
    
    vector<float>& getPitches();
    
    int frequencyToCentBin(Real frequency);
    Real getWeight(int centBin, int harmonicNumber);
    
private:
    
    ofxAAPitchSalienceFunctionPeaksAlgorithm* _saliencePeaksAlgthm;
    ofxAAOneVectorOutputAlgorithm* _spectrumAlgthm;
    
    Real _binResolution;
    int _binsInSemitone;
    std::vector<Real> _centSpectrum;
    Real _sampleRate;
    int _binsInOctave;
    Real _referenceTerm;
    Real _referenceFrequency;
    int _numberBins;
    Real _centToHertzBase;
    int _numberHarmonicsMax;
    vector<Real> nearestBinWeights;
    
    vector<float> pitches;
    
};
