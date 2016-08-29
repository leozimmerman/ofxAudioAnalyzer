
#include "ofxAudioAnalyzerAlgorithms.h"

//TODO: Document all this funcs and classes. Add description

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
void ofxAABaseAlgorithm::setActive(bool state){
    isActivated = state;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setValueZero(){
    floatValue = 0.0;
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
    
    for(int i=0; i<floatValues.size(); i++){
        smoothedFloatValues[i] = smoothedFloatValues[i] * smthAmnt + (1-smthAmnt) * floatValues[i];
    }
    return smoothedFloatValues;
}
//-------------------------------------------
#pragma mark - ofxAAPitchDetectAlgorithm
//-------------------------------------------
void ofxAAPitchDetectAlgorithm::castValuesToFloat(){
    if(getIsActive()){
        pitchFloatVal = (float) pitchRealVal;
        confidenceFloatVal = (float) confidenceRealVal;
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
float ofxAAPitchDetectAlgorithm::getConfidenceValue(){
    return confidenceFloatVal;
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


