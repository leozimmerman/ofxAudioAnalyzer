//
//  ofxAATwoTypesVectorOutputAlgorithm.cpp
//  example-allAlgorithms
//
//  Created by Leonardo Zimmerman on 17/04/2019.
//

#include "ofxAATwoTypesVectorOutputAlgorithm.h"

ofxAATwoTypesVectorOutputAlgorithm::ofxAATwoTypesVectorOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize, int outputSize_1, int outputSize_2) :  ofxAAOneVectorOutputAlgorithm(algorithmType, samplerate, framesize, outputSize_1) {
    assignSecondOutpuValuesSize(outputSize_2, "");
}

void ofxAATwoTypesVectorOutputAlgorithm::assignSecondOutpuValuesSize(int size, string val){
    stringValues.assign(size, "");
}

