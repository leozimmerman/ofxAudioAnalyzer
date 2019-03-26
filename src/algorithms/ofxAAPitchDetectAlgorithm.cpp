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

#include "ofxAAPitchDetectAlgorithm.h"

ofxAAPitchDetectAlgorithm::ofxAAPitchDetectAlgorithm(ofxAAAlgorithmType algorithmType, int samplerate, int framesize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize) {
    
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
void ofxAAPitchDetectAlgorithm::setMaxEstimatedValue(float value){
    pitchMaxEstimatedValue = value;
}
