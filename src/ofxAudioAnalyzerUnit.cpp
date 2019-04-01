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

#include "ofxAudioAnalyzerUnit.h"
#include "ofxAudioAnalyzerUtils.h"
#include "ofxAAConfigurations.h"



#pragma mark - Main funcs

ofxAudioAnalyzerUnit::ofxAudioAnalyzerUnit(int sampleRate, int bufferSize) {
    samplerate = sampleRate;
    framesize = bufferSize;
    
    audioBuffer.resize(bufferSize);
    accumulatedAudioBuffer.resize(bufferSize * 87, 0.0); //almost a second
    
    network = new ofxaa::Network(samplerate, framesize);
}
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::analyze(const vector<float> & inBuffer){
    
    if(inBuffer.size() != framesize){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: buffer requested to analyze size(" <<inBuffer.size()<<")doesnt match the buffer size already set: "<<framesize;
    }
    
    //Cast of incoming audio buffer to Real
    for (int i=0; i<inBuffer.size();i++){
        audioBuffer[i] = (Real) inBuffer[i];
    }
    
    accumulatedAudioBuffer.erase(accumulatedAudioBuffer.begin(), accumulatedAudioBuffer.begin()+framesize);
    accumulatedAudioBuffer.insert(accumulatedAudioBuffer.end(), audioBuffer.begin(), audioBuffer.end());
    
    network->computeAlgorithms(audioBuffer, accumulatedAudioBuffer);
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::exit(){
    delete network;
}

//--------------------------------------------------------------
#pragma mark - Activates
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActive(ofxaa::AlgorithmType algorithmType, bool state){
    //TODO: !
    ///network->setActive(algorithmType, state);
}
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getIsActive(ofxaa::AlgorithmType algorithmType){
    //TODO: !
    ///return  network->getIsActive(algorithmType)
    return true;
}
//----------------------------------------------
#pragma mark - Get values
//----------------------------------------------
float ofxAudioAnalyzerUnit::getValue(ofxAAValueType valueType, float smooth, bool normalized){
    auto value = network->getValue(valueType , smooth, normalized);
   // cout<<value<<endl;
    return value;
    
    //TODO: !
    /*
     
    switch (algorithmType) {
        case PITCH_YIN_FREQ:
            if (normalized){
                return smooth ? pitchDetect->getSmoothedPitchValueNormalized(smooth): pitchDetect->getPitchValueNormalized();
            }else{
                return smooth ? pitchDetect->getSmoothedPitchValue(smooth) : pitchDetect->getPitchValue();
            }
            break;
            
        case PITCH_YIN_CONFIDENCE:
            return smooth ? pitchDetect->getSmoothedConfidenceValue(smooth) : pitchDetect->getConfidenceValue();
            break;
            
        default:
            ofxAABaseAlgorithm* baseAlgorithm =  algorithm(algorithmType);
            
            if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
                ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a vector of values" << baseAlgorithm->algorithm->name();
                return 0.0;
            }
            if (normalized && !ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
                return smooth ? baseAlgorithm->getSmoothedValueNormalized(smooth): baseAlgorithm->getValueNormalized();
            } else if (ofxaa::algorithmHasOutputInDbs(baseAlgorithm)) {
                return smooth ? baseAlgorithm->getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) : baseAlgorithm->getValueDbNormalized(DB_MIN, DB_MAX);
            } else {
                return smooth ? baseAlgorithm->getSmoothedValue(smooth): baseAlgorithm->getValue();
            }
            break;
    }
    */
}

//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getValues(ofxaa::AlgorithmType algorithmType, float smooth){
    auto values = network->getValues(algorithmType, algorithmType);
    return values;
    //TODO: !
    /*
    ofxAAOneVectorOutputAlgorithm* algorithm = vectorAlgorithm(algorithmType);
    
    if (!ofxaa::algorithmHasVectorOutput(algorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a single of values" << algorithm->algorithm->name();
        static vector<float>r (1, 0.0);
        return r;
    }
    
    return smooth ? algorithm->getSmoothedValues(smooth) : algorithm->getValues();
    */
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getBinsNum(ofxaa::AlgorithmType algorithmType){
    auto size = network->getValues(algorithmType, 0.0).size();
    return size;
    //TODO: !
    /*
    ofxAAOneVectorOutputAlgorithm* algorithm = vectorAlgorithm(algorithmType);
    
    if (!ofxaa::algorithmHasVectorOutput(algorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a single of values" << algorithm->algorithm->name();
        return 0;
    }
    
    return algorithm->getBinsNum();
    */
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getMaxEstimatedValue(ofxaa::AlgorithmType algorithmType){
    return 1.0;
    //TODO: !
    /*
     ofxAABaseAlgorithm* baseAlgorithm = algorithm(algorithmType);
    
    if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting max estimated value. This is algorithm outputs a vector" << baseAlgorithm->algorithm->name();
    }
    
    if (ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting max estimated value. This is algorithm outputs a normalized value by default." << baseAlgorithm->algorithm->name();
    }
    
    return baseAlgorithm->getMaxEstimatedValue();
    */
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setMaxEstimatedValue(ofxaa::AlgorithmType algorithmType, float value){
    //TODO: !
    /*
     ofxAABaseAlgorithm* baseAlgorithm = algorithm(algorithmType);
    
    if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
         ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Setting max estimated value. This is algorithm outputs a vector" << baseAlgorithm->algorithm->name();
    }
    
    if (ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Setting max estimated value. This is algorithm outputs a normalized value by default." << baseAlgorithm->algorithm->name();
    }
    
    baseAlgorithm->setMaxEstimatedValue(value);
    */
}
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getOnsetValue(){
     return network->getOnsetValue();
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::resetOnsets(){
    network->resetOnsets();
}

