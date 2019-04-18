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

#include "ofxAudioAnalyzerUtils.h"

namespace ofxaa {
    
    void initializeEssentia(){
        if (!essentia::isInitialized()){
            essentia::init();
        }
    }
    //----------------------------------------------
    void shutEssentiaFactoryDown(){
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        factory.shutdown();
    }
    //----------------------------------------------
    bool algorithmHasVectorOutput(ofxAABaseAlgorithm* algorithm){
        ofxAAOneVectorOutputAlgorithm* vectorAlgorithm = dynamic_cast<ofxAAOneVectorOutputAlgorithm*>(algorithm);
        return vectorAlgorithm != NULL;
    }
    //----------------------------------------------
    bool algorithmHasOutputInDbs(ofxAABaseAlgorithm* algorithm){
        for (auto a : algorithmsWithOutputInDbs){
            if (algorithm->getType() == a){
                return true;
            }
        }
        return false;
    }
    
    //----------------------------------------------
    bool algorithmHasNormalizedSingleOutputByDefault(ofxAABaseAlgorithm* algorithm){
        if (algorithmHasVectorOutput(algorithm)){
            return false;
        }
        for (auto a : algorithmsWithNoNormalizedSingleOutput){
            if (algorithm->getType() == a){
                return false;
            }
        }
        return true;
    }
}
