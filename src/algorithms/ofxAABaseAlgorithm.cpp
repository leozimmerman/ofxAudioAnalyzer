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



//#define DB_MIN -6

//LoudnessVickers uses -90

ofxAABaseAlgorithm::ofxAABaseAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize){
    _algorithmType = algorithmType;
    
    algorithm = ofxaa::createAlgorithmWithType(_algorithmType, samplerate, framesize);
    
    isActive = TRUE;
    
    hasLogaritmicValues = false;
    hasDbValues = false;
    isNormalizedByDefault = false;
    minEstimatedValue = 0.0;
    maxEstimatedValue = 1.0;
}

//-------------------------------------------
void ofxAABaseAlgorithm::compute(){
    if(isActive){
        algorithm->compute();
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

