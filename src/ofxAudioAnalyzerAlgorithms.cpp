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

//----------------------------------------------
float ofxAABaseAlgorithm::smoothAttackRelease(float currentValue, float newValue, float smoothAttck, float smoothRlse) {
    float smthAmnt = (newValue > currentValue) ? smoothAttck : smoothRlse;
    return currentValue * smthAmnt + (1-smthAmnt) * newValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValue(float smthAmntAttck, float smthAmntRlse){

    smoothedFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedFloatValue, floatValue, smthAmntAttck, smthAmntRlse);
    return smoothedFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmntAttck, float smthAmntRlse){
    float normVal = ofMap(floatValue, 0.0, maxEstimatedValue, 0.0, 1.0, TRUE);
    smoothedNormFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedNormFloatValue, normVal, smthAmntAttck, smthAmntRlse);
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmntAttck, float smthAmntRlse, float min, float max, bool doClamp){
    float normVal = ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedNormFloatValue, normVal, smthAmntAttck, smthAmntRlse);
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueDbNormalized(float smthAmntAttck, float smthAmntRlse, float min, float max, bool doClamp){
    
    float normVal = ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValueDb = ofxAABaseAlgorithm::smoothAttackRelease(smoothedNormFloatValueDb, normVal, smthAmntAttck, smthAmntRlse);
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
vector<float>& ofxAAOneVectorOutputAlgorithm::getSmoothedValues(float smthAmntAttck, float smthAmntRlse){
    
    for(int i=0; i<smoothedFloatValues.size(); i++){
        smoothedFloatValues[i] = ofxAABaseAlgorithm::smoothAttackRelease(smoothedFloatValues[i], floatValues[i], smthAmntAttck, smthAmntRlse);
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
vector<SalienceFunctionPeak>& ofxAAPitchSalienceFunctionPeaksAlgorithm::getSmoothedPeaks(float smthAmntAttck, float smthAmntRlse){
    
    if (limitPeaksNum && peaks.size() > maxPeaksNum){
        peaks.resize(maxPeaksNum);
    }
    
    smoothedPeaks.resize(peaks.size(), SalienceFunctionPeak());
    
    for(int i=0; i<smoothedPeaks.size(); i++){

        smoothedPeaks[i].bin = ofxAABaseAlgorithm::smoothAttackRelease(smoothedPeaks[i].bin, peaks[i].bin, smthAmntAttck, smthAmntRlse);
        smoothedPeaks[i].value = ofxAABaseAlgorithm::smoothAttackRelease(smoothedPeaks[i].bin, peaks[i].value, smthAmntAttck, smthAmntRlse);
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
float ofxAAPitchDetectAlgorithm::getSmoothedPitchValueNormalized(float smthAmntAttck, float smthAmntRlse){
    float normVal = ofMap(pitchFloatVal, 0.0, pitchMaxEstimatedValue, 0.0, 1.0, TRUE);
    smoothedNormPitchFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedNormPitchFloatValue, normVal, smthAmntAttck, smthAmntRlse);
    return smoothedNormPitchFloatValue;

}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getSmoothedPitchValue(float smthAmntAttck, float smthAmntRlse){
    smoothedPitchFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedPitchFloatValue, pitchFloatVal, smthAmntAttck, smthAmntRlse);
    return smoothedPitchFloatValue;
}
//-------------------------------------------
float ofxAAPitchDetectAlgorithm::getSmoothedConfidenceValue(float smthAmntAttck, float smthAmntRlse){
    smoothedConfidenceFloatValue = ofxAABaseAlgorithm::smoothAttackRelease(smoothedConfidenceFloatValue, confidenceFloatVal, smthAmntAttck, smthAmntRlse);
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

