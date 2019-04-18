/*
 * Copyright (C) 2019 Leo Zimmerman [http://www.leozimmerman.com.ar]
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

#include "ofxAABaseAlgorithm.h"
#include "ofxAAFactory.h"

ofxAABaseAlgorithm::ofxAABaseAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize){
    type = algorithmType;
    
    algorithm = ofxaa::createAlgorithmWithType(type, samplerate, framesize);
    
    isActive = TRUE;
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
    smoothedFloatValue = smooth(floatValue, smoothedFloatValue, smthAmnt);
    return smoothedFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmnt){
    float normVal = ofMap(floatValue, 0.0, maxEstimatedValue, 0.0, 1.0, TRUE);
    smoothedNormFloatValue =  smooth(normVal, smoothedNormFloatValue, smthAmnt);
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp){
    float normVal = ofMap(floatValue, min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValue =  smooth(normVal, smoothedNormFloatValue, smthAmnt);
    return smoothedNormFloatValue;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp){
    
    float normVal = ofMap(getValueDb(), min, max, 0.0, 1.0, doClamp);
    smoothedNormFloatValueDb = smooth(normVal, smoothedNormFloatValueDb, smthAmnt);
    return smoothedNormFloatValueDb;
    
}
//-------------------------------------------
bool ofxAABaseAlgorithm::getIsActive(){
    return isActive;
}
//-------------------------------------------
ofxaa::AlgorithmType ofxAABaseAlgorithm::getType(){
    return type;
}
//-------------------------------------------
float ofxAABaseAlgorithm::getMaxEstimatedValue(){
    return maxEstimatedValue;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setActive(bool state){
    isActive = state;
}
//-------------------------------------------
void ofxAABaseAlgorithm::setMaxEstimatedValue(float value){
    maxEstimatedValue = value;
}
//-------------------------------------------
void ofxAABaseAlgorithm::compute(){
    if(isActive){
        algorithm->compute();
        castToFloat();
    }
}
//-------------------------------------------
void ofxAABaseAlgorithm::castToFloat(){
    if(isActive){
        floatValue = (float) realValue;
    } else {
        floatValue = 0.0;
    }
}
//-------------------------------------------
void ofxAABaseAlgorithm::deleteAlgorithm(){
    delete algorithm;
}
//-------------------------------------------
float ofxAABaseAlgorithm::smooth(float newValue, float previousValue, float amount){
    return previousValue * amount + (1-amount) * newValue;
}
