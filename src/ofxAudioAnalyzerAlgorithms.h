# pragma once


#include "ofMain.h"

#include "essentia/algorithmfactory.h"
#include "essentia/essentiamath.h" // for the isSilent function
#include "essentia/pool.h"

using namespace std;
using namespace essentia;
using namespace standard;



class ofxAABaseAlgorithm{
    
public:
    
    float getValue(){
        return floatValue;
    }
    
    float getValueDb(){
        //returns floatValue in a logaritmic scale
        //0.000001 to 1 -->  -6 to 0
        return log10(floatValue);
    }
    
    float getValueNormalized(float min, float max, bool doClamp=false){
        return ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
    }
    
    float getValueDbNormalized(float min, float max, bool doClamp=false){
        return ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
    }
    
    float getSmoothedValue(float smthAmnt){
        smoothedFloatValue =  smoothedFloatValue * smthAmnt + (1-smthAmnt) * floatValue;
        return smoothedFloatValue;
    }
    
    float getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp=false){
        float normVal = ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
        smoothedNormFloatValue =  smoothedNormFloatValue * smthAmnt + (1-smthAmnt) * normVal;
        return smoothedNormFloatValue;
    }
    
    float getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp=false){
        float normVal = ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
        smoothedNormFloatValueDb =  smoothedNormFloatValueDb * smthAmnt + (1-smthAmnt) * normVal;
        return smoothedNormFloatValueDb;
    }
    
    bool getIsActive(){return isActivated;}
    
    void setActive(bool state){isActivated = state;}
    void setValueZero(){floatValue = 0.0;}
    
    void compute(){
        if(isActivated){
            algorithm->compute();
        }
    }
    
    void castValueToFloat(){
        if(isActivated)
            floatValue = (float) realValue;
        else
            floatValue = 0.0;
    }
    
    void deleteAlgorithm(){delete algorithm;}
    
    
    Algorithm* algorithm;
    
    Real realValue;
    
    
private:
    bool isActivated = TRUE;
    float floatValue = 0.0;
    float smoothedFloatValue = 0.0;
    float smoothedNormFloatValue = 0.0;
    float smoothedNormFloatValueDb = 0.0;
    
};
//---------------------------------------------------------------------
class ofxAAOneVectorOutputAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void assignFloatValuesSize(int size, int val){
        floatValues.assign(size, val);
    }
    
    void castValuesToFloat(bool logarithmic){
        
        for (int i=0; i<realValues.size(); i++){
            if(getIsActive()){
                if(logarithmic)
                    floatValues[i] = log10((float) realValues[i]);
                else
                    floatValues[i] = (float) realValues[i];
            }else{
                if(logarithmic)
                    floatValues[i] = log10(0.001);
                else
                    floatValues[i] = 0.0;
            }
        }
    
    }
    
    void updateLogRealValues(){
        logRealValues.resize(realValues.size());
        for (int i=0; i<realValues.size(); ++i)
            logRealValues[i] = amp2db(realValues[i]);
    
    }
    
    int getBinsNum(){return floatValues.size();}
    
    
    vector<float> floatValues;
    vector<Real> realValues;
    vector<Real> logRealValues;
    
};

//---------------------------------------------------------------------
class ofxAACartToPolAlgorithm : public ofxAABaseAlgorithm{
public:
    vector<Real> magnitudes;
    vector<Real> phases;
    
};
//---------------------------------------------------------------------
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
    
    void castValuesToFloat(){
        if(getIsActive()){
            pitchFloatVal = (float) pitchRealVal;
            confidenceFloatVal = (float) confidenceRealVal;
        }
        else{
            pitchFloatVal = confidenceFloatVal = 0.0;
        }
        
    }
    
    float getPitchValue(){return pitchFloatVal;}
    float getConfidenceValue(){return confidenceFloatVal;}
    
    float getSmoothedPitchValue(float smthAmnt){
        smoothedPitchFloatValue =  smoothedPitchFloatValue * smthAmnt + (1-smthAmnt) * pitchFloatVal;
        return smoothedPitchFloatValue;
    }
    
    float getSmoothedConfidenceValue(float smthAmnt){
        smoothedConfidenceFloatValue =  smoothedConfidenceFloatValue * smthAmnt + (1-smthAmnt) * confidenceFloatVal;
        return smoothedConfidenceFloatValue;
    }
 
    Real pitchRealVal, confidenceRealVal;

private:
    float pitchFloatVal, confidenceFloatVal;
    float smoothedPitchFloatValue = 0.0;
    float smoothedConfidenceFloatValue = 0.0;

};
//---------------------------------------------------------------------
class ofxAATuningFrequencyAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    void castValuesToFloat(){
        if(getIsActive()){
            freqFloatVal = (float) freqRealVal;
            centsFloatVal = (float) centsRealVal;
        }
        else{
            freqFloatVal = centsFloatVal = 0.0;
        }
        
    }
    
    float getFreqValue(){return freqFloatVal;}
    float getCentsValue(){return centsFloatVal;}
    
    Real freqRealVal,   centsRealVal;
    
    private:
    float freqFloatVal, centsFloatVal;
    
};