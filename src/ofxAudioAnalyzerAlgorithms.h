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

# pragma once

#include "ofMain.h"

#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h" // for the isSilent function
#include "essentia/pool.h"

using namespace std;
using namespace essentia;
using namespace standard;


enum ofxAAAlgorithm{
    RMS,
    ENERGY,
    POWER,
    PITCH_FREQ,
    PITCH_CONFIDENCE,
    PITCH_SALIENCE,
    INHARMONICITY,
    HFC,
    CENTROID,
    SPECTRAL_COMPLEXITY,
    DISSONANCE,
    ROLL_OFF,
    ODD_TO_EVEN,
    STRONG_PEAK,
    STRONG_DECAY,
    SPECTRUM,
    MEL_BANDS,
    MFCC,
    HPCP,
    MULTI_PITCHES,
    PITCH_SALIENCE_FUNC_PEAKS,
    TRISTIMULUS,
    ONSETS
    
};

//OSC INDEXES
#define POWER_OSC_IDX 0
#define PITCH_FREQ_OSC_IDX 1
#define PITCH_CONF_OSC_IDX 2
#define PITCH_SALIENCE_OSC_IDX 3
#define HFC_OSC_IDX 4
#define CENTROID_OSC_IDX 5
#define SPEC_COMP_OSC_IDX 6
#define INHARMONICITY_OSC_IDX 7
#define DISSONANCE_OSC_IDX 8
#define ROLLOFF_OSC_IDX 9
#define ODDTOEVEN_OSC_IDX 10
#define ONSETS_OSC_IDX 11



class ofxAABaseAlgorithm{
    
public:
    
    
    void init();
    
    float getValue();
    float getValueDb();
    
    ///Gets the value normalized from 0 to maxEstimatedValue with Clamping
    float getValueNormalized();
    
    float getValueNormalized(float min, float max, bool doClamp=TRUE);
    float getValueDbNormalized(float min, float max, bool doClamp=TRUE);
    float getSmoothedValue(float smthAmnt);
    
    ///Gets the value normalized and smoothed from 0 to maxEstimatedValue with Clamping
    float getSmoothedValueNormalized(float smthAmnt);
    
    float getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    float getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    
    float getMaxEstimatedValue();
    
    bool getIsActive();
    
    void setActive(bool state);
    void setValueZero();
    void setMaxEstimatedValue(float value);
    
    void compute();
    
    void castValueToFloat();
    
    void deleteAlgorithm();
    
    Algorithm* algorithm;
    Real realValue;
    
    
protected:
    
    bool isActivated;
    
private:
    
    float floatValue;
    float smoothedFloatValue;
    float smoothedNormFloatValue;
    float smoothedNormFloatValueDb;
    
    float maxEstimatedValue;
    
};
//---------------------------------------------------------------------
class ofxAAOneVectorOutputAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void init();
    
    void initAndAssignSize(int size, int initValues);
    
    void assignFloatValuesSize(int size, int val);
    
    void castValuesToFloat(bool logarithmic);
    
    void updateLogRealValues();
    
    int getBinsNum();
    vector<float>& getValues();
    vector<float>& getSmoothedValues(float smthAmnt);
    
    vector<Real> realValues;
    vector<Real> logRealValues;
    
private:
    
    vector<float> floatValues;
    vector<float> smoothedFloatValues;
    
};


//---------------------------------------------------------------------
struct SalienceFunctionPeak{
    float bin;//cents
    float value;
    
    SalienceFunctionPeak(){
        bin = 0.0;
        value = 0.0;
    }
};

class ofxAAPitchSalienceFunctionPeaksAlgorithm : public ofxAABaseAlgorithm{
public:
    
    void init();
    
    void castValuesToFloat();
    
    vector<SalienceFunctionPeak>& getPeaks();
    vector<SalienceFunctionPeak>& getSmoothedPeaks(float smthAmnt);
    
    ///change to maxPeaks!!!!
    void setMaxPeaksNum(int maxBins){maxPeaksNum = maxBins;}
    
    vector<Real> realSalienceBins;
    vector<Real> realSalienceValues;
    
private:
    vector<SalienceFunctionPeak> peaks;
    vector<SalienceFunctionPeak> smoothedPeaks;
    
    bool limitPeaksNum;
    
    int maxPeaksNum;
};


//---------------------------------------------------------------------
class ofxAACartToPolAlgorithm : public ofxAABaseAlgorithm{
public:
    vector<Real> magnitudes;
    vector<Real> phases;
    
};
//---------------------------------------------------------------------
//class used for SpectralPeaks & HarmonicPeaks
class ofxAAPeaksAlgorithm : public ofxAABaseAlgorithm{
public:
    vector<Real> frequencies;
    vector<Real> magnitudes;
    
};

//---------------------------------------------------------------------
class ofxAAFftAlgorithm : public ofxAAOneVectorOutputAlgorithm{
    public:
    vector<complex<Real> > fftRealValues;

};
//---------------------------------------------------------------------
class ofxAAPitchDetectAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void init();
    
    void castValuesToFloat();
    
    float getPitchValue();
    float getPitchValueNormalized();
    float getConfidenceValue();
    
    float getSmoothedPitchValue(float smthAmnt);
    float getSmoothedPitchValueNormalized(float smthAmnt);
    float getSmoothedConfidenceValue(float smthAmnt);
    
    float getMaxPitchEstimatedValue(){return pitchMaxEstimatedValue;}
    
    void setMaxPitchEstimatedValue(float value);
    
    Real pitchRealVal, confidenceRealVal;

private:
    
    float pitchFloatVal, confidenceFloatVal;
    
    float smoothedPitchFloatValue;
    float smoothedNormPitchFloatValue;
    float smoothedConfidenceFloatValue;
    
    float pitchMaxEstimatedValue;

};

//---------------------------------------------------------------------
class ofxAATuningFrequencyAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    void castValuesToFloat();
    
    float getFreqValue();
    float getCentsValue();
    
    Real freqRealVal,   centsRealVal;
    
    private:
    float freqFloatVal, centsFloatVal;
    
};
//---------------------------------------------------------------------
