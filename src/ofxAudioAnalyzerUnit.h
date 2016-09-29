/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * ofxAudioAnalyzer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 * ---------------------------------------------------------------
 *
 * This project uses Essentia, copyrighted by Music Technology Group - Universitat Pompeu Fabra
 * using GNU Affero General Public License.
 * See http://essentia.upf.edu for documentation.
 *
 */

#pragma once

#include "ofMain.h"

#include <iostream>
#include "algorithmfactory.h"
#include "essentiamath.h"
#include "pool.h"

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
    void analyze(const vector<float> &  inBuffer);
    void exit();
    
    void resetOnsets();
    
    int getSampleRate() {return _samplerate;}
    int getBufferSize() {return _framesize;}
    
    void setActive(ofxAAAlgorithm algorithm, bool state);
    void setMaxEstimatedValue(ofxAAAlgorithm algorithm, float value);
    void setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
    void setSalienceFunctionPeaksParameters(int maxPeaks);
    
    float getValue(ofxAAAlgorithm algorithm, float smooth=0.0, bool normalized=false);
    vector<float>& getValues(ofxAAAlgorithm algorithm, float smooth=0.0);
    vector<SalienceFunctionPeak>& getPitchSaliencePeaksRef(float smooth=0.0);
    bool getIsActive(ofxAAAlgorithm algorithm);
    bool getOnsetValue();
    
    int getBinsNum(ofxAAAlgorithm algorithm);
    float getMaxEstimatedValue(ofxAAAlgorithm algorithm);
    
    ofxAAOnsetsAlgorithm* getOnsetsAlgorithmPtr(){return &onsets;}
    
    int   getPitchFreqAsMidiNote(float smooth=0.0);
    string getPitchFreqAsNoteName(float smooth=0.0);
    
   
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
    int _samplerate;
    int _framesize;
    
    int hopsize;
    int zeropadding;
    Real framerate;
    
    //Max estimated values (for normalizing)
//    float maxEnergyEstimatedValue;
//    float maxPitchFreqEstimatedValue;
//    float maxHfcEstimatedValue;
//    float maxSpecCompEstimatedValue;
//    float maxCentroidEstimatedValue;
//    float maxRollOffEstimatedValue;
//    float maxOddToEvenEstimatedValue;
//    float maxStrongPeakEstimatedValue;
//    float maxStrongDecayEstimatedValue;

    
};


