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

#include "ofxAAOneVectorOutputAlgorithm.h"

ofxAAOneVectorOutputAlgorithm::ofxAAOneVectorOutputAlgorithm(ofxAAAlgorithmType algorithmType, int samplerate, int framesize, int binsSize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize){
    
    assignFloatValuesSize(binsSize, 0.0);
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
