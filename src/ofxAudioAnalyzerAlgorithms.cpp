
#include "ofxAudioAnalyzerAlgorithms.h"


//-------------------------------------------
#pragma mark - ofxAABaseAlgorithm
//-------------------------------------------
void ofxAABaseAlgorithm::init(){
    
    isActivated = TRUE;
    floatValue = 0.0;
    smoothedFloatValue = 0.0;
    smoothedNormFloatValue = 0.0;
    smoothedNormFloatValueDb = 0.0;

}
//-------------------------------------------
float ofxAABaseAlgorithm::getValue(){
    return floatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getValueDb(){
    //returns floatValue in a logaritmic scale
    //0.000001 to 1 -->  -6 to 0
    return log10(floatValue);
}
//-------------------------------------------
float ofxAABaseAlgorithm::getValueNormalized(){
    return ofMap(floatValue, 0.0, maxEstimatedValue, 0.0, 1.0, TRUE);
}
//-------------------------------------------
float ofxAABaseAlgorithm::getValueNormalized(float min, float max, bool doClamp){
    return ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
}
//-------------------------------------------
float ofxAABaseAlgorithm::getValueDbNormalized(float min, float max, bool doClamp){
    return ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValue(float smthAmnt){
    smoothedFloatValue =  smoothedFloatValue * smthAmnt + (1-smthAmnt) * floatValue;
    return smoothedFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmnt){
    float normVal = ofMap(floatValue, 0.0, maxEstimatedValue, 0.0, 1.0, TRUE);
    smoothedNormFloatValue =  smoothedNormFloatValue * smthAmnt + (1-smthAmnt) * normVal;
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp){
    float normVal = ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValue =  smoothedNormFloatValue * smthAmnt + (1-smthAmnt) * normVal;
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp){
    
    float normVal = ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValueDb = smoothedNormFloatValueDb * smthAmnt + (1-smthAmnt) * normVal;
    return smoothedNormFloatValueDb;
    
}
//-------------------------------------------
bool ofxAABaseAlgorithm::getIsActive(){
    return isActivated;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getMaxEstimatedValue(){
    return maxEstimatedValue;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setActive(bool state){
    isActivated = state;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setValueZero(){
    floatValue = 0.0;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setMaxEstimatedValue(float value){
    maxEstimatedValue = value;
}
//-------------------------------------------
void ofxAABaseAlgorithm::compute(){
    if(isActivated){
        algorithm->compute();
    }
}
//-------------------------------------------
void ofxAABaseAlgorithm::castValueToFloat(){
    if(isActivated)
        floatValue = (float) realValue;
    else
        floatValue = 0.0;
}
//-------------------------------------------
void ofxAABaseAlgorithm::deleteAlgorithm(){
    delete algorithm;
}
//-------------------------------------------
#pragma mark - ofxAAOneVectorOutputAlgorithm
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::init(){
    isActivated = TRUE;
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::initAndAssignSize(int size, int initValues){
    isActivated = TRUE;
    assignFloatValuesSize(size, initValues);
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::assignFloatValuesSize(int size, int val){
    floatValues.assign(size, val);
    smoothedFloatValues.assign(size, val);
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::castValuesToFloat(bool logarithmic){
    
    for (int i=0; i<realValues.size(); i++){
        if(getIsActive()){
            if(logarithmic){
                
                if(realValues[i] == 0.0){
                    floatValues[i] = log10(0.000001);//DB_MIN
                }else{
                    floatValues[i] = log10((float) realValues[i]);
                }
                
            }else{
                floatValues[i] = (float) realValues[i];
            }
        }else{
            if(logarithmic){
                floatValues[i] = log10(0.000001);//DB_MIN
            }else{
                floatValues[i] = 0.0;
            }
        }
        
    }
    
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::updateLogRealValues(){
    logRealValues.resize(realValues.size());
    for (int i=0; i<realValues.size(); ++i)
        logRealValues[i] = amp2db(realValues[i]);
    
}
//-------------------------------------------
int ofxAAOneVectorOutputAlgorithm::getBinsNum(){
    return floatValues.size();
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::getValues(){
    return floatValues;
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::getSmoothedValues(float smthAmnt){
    
    for(int i=0; i<smoothedFloatValues.size(); i++){
        smoothedFloatValues[i] = smoothedFloatValues[i] * smthAmnt + (1-smthAmnt) * floatValues[i];
    }
    
    return smoothedFloatValues;
}
//-------------------------------------------
#pragma mark - ofxAAPitchSalienceFuntionPeaksAlgorithm
//-------------------------------------------
void ofxAAPitchSalienceFunctionPeaksAlgorithm::init(){
    
    limitPeaksNum = TRUE;
    maxPeaksNum = 4;
    
    isActivated = TRUE;

}
//-------------------------------------------
void ofxAAPitchSalienceFunctionPeaksAlgorithm::castValuesToFloat(){
    
    peaks.clear();
    
    peaks.resize(realSalienceBins.size());

    for (int i=0; i<realSalienceBins.size(); i++){
        peaks[i].bin = (float) realSalienceBins[i];
        peaks[i].value = realSalienceValues[i];
    }

}
//-------------------------------------------
vector<SalienceFunctionPeak>& ofxAAPitchSalienceFunctionPeaksAlgorithm::getPeaks(){
    
    if (limitPeaksNum && peaks.size() > maxPeaksNum){
        peaks.resize(maxPeaksNum);
    }
    
    return peaks;
}

//-------------------------------------------
vector<SalienceFunctionPeak>& ofxAAPitchSalienceFunctionPeaksAlgorithm::getSmoothedPeaks(float smthAmnt){
    
    if (limitPeaksNum && peaks.size() > maxPeaksNum){
        peaks.resize(maxPeaksNum);
    }
    
    smoothedPeaks.resize(peaks.size(), SalienceFunctionPeak());
    
    for(int i=0; i<smoothedPeaks.size(); i++){
        smoothedPeaks[i].bin = smoothedPeaks[i].bin * smthAmnt + (1-smthAmnt) * peaks[i].bin;
//        smoothedPeaks[i].bin = peaks[i].bin;
        smoothedPeaks[i].value = smoothedPeaks[i].value * smthAmnt + (1-smthAmnt) * peaks[i].value;
    }
    
    return smoothedPeaks;
}
//-------------------------------------------
#pragma mark - ofxAAPitchDetectAlgorithm
//-------------------------------------------
void ofxAAPitchDetectAlgorithm::init(){
    
    isActivated = TRUE;
    
    pitchFloatVal = 0.0;
    confidenceFloatVal = 0.0;
    
    smoothedPitchFloatValue = 0.0;
    smoothedNormPitchFloatValue = 0.0;
    smoothedConfidenceFloatValue = 0.0;
    
}
//-------------------------------------------
void ofxAAPitchDetectAlgorithm::castValuesToFloat(){
    if(getIsActive()){
        pitchFloatVal = (float) pitchRealVal;
        confidenceFloatVal = (float)confidenceRealVal;
        //avoid negative values...
        if(confidenceFloatVal<0.0) confidenceFloatVal = 0.0;
    }
    else{
        pitchFloatVal = confidenceFloatVal = 0.0;
    }
    
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getPitchValue(){
    return pitchFloatVal;
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getPitchValueNormalized(){
    return ofMap(pitchFloatVal, 0.0, pitchMaxEstimatedValue, 0.0, 1.0, TRUE);
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getConfidenceValue(){
    return confidenceFloatVal;
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getSmoothedPitchValueNormalized(float smthAmnt){
    float normVal = ofMap(pitchFloatVal, 0.0, pitchMaxEstimatedValue, 0.0, 1.0, TRUE);
    smoothedNormPitchFloatValue =  smoothedNormPitchFloatValue * smthAmnt + (1-smthAmnt) * normVal;
    return smoothedNormPitchFloatValue;

}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getSmoothedPitchValue(float smthAmnt){
    smoothedPitchFloatValue =  smoothedPitchFloatValue * smthAmnt + (1-smthAmnt) * pitchFloatVal;
    return smoothedPitchFloatValue;
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getSmoothedConfidenceValue(float smthAmnt){
    smoothedConfidenceFloatValue =  smoothedConfidenceFloatValue * smthAmnt + (1-smthAmnt) * confidenceFloatVal;
    return smoothedConfidenceFloatValue;
}
//-------------------------------------------
void ofxAAPitchDetectAlgorithm::setMaxPitchEstimatedValue(float value){
    pitchMaxEstimatedValue = value;
}


//-------------------------------------------
#pragma mark - ofxAATuningFrequencyAlgorithm
//-------------------------------------------
void ofxAATuningFrequencyAlgorithm::castValuesToFloat(){
    if(getIsActive()){
        freqFloatVal = (float) freqRealVal;
        centsFloatVal = (float) centsRealVal;
    }
    else{
        freqFloatVal = centsFloatVal = 0.0;
    }
    
}
//-------------------------------------------
float ofxAATuningFrequencyAlgorithm::getFreqValue(){
    return freqFloatVal;
}
//-------------------------------------------
float ofxAATuningFrequencyAlgorithm::getCentsValue(){
    return centsFloatVal;
}
//-------------------------------------------

