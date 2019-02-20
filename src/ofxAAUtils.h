//
//  ofxAAUtils.hpp
//  Sonoscopio
//
//  Created by Leo on 11/09/2018.
//
#pragma once

#include "ofMain.h"

#include "ofxAudioAnalyzer.h"

#define POWER_STRING "POWER"
#define PITCH_FREQ_STRING "PITCH-FREQ"
#define PITCH_CONFIDENCE_STRING "PITCH-CONF"
#define PITCH_SALIENCE_STRING "PITCH-SALIENCE"
#define HFC_STRING "HFC"
#define CENTROID_STRING "CENTROID"
#define SPEC_COMP_STRING "SPEC-COMP"
#define INHARMONICITY_STRING "INHARM"
#define DISSONANCE_STRING "DISONNANCE"
#define ROLL_OFF_STRING "ROLL-OFF"
#define ODD_TO_EVEN_STRING "ODD-EVEN"
#define ONSETS_STRING "ONSETS"

#define SPECTRUM_STRING "SPECTRUM"
#define MEL_BANDS_STRING "MEL-BANDS"
#define MFCC_STRING "MFCC"
#define HPCP_STRING "HPCP"
#define TRISTIMULUS_STRING "TRISTIMULUS"

namespace ofxaa {
    string algorithmTypeToString(ofxAAAlgorithmType algorithmType);
    ofxAAAlgorithmType stringToAlgorithmType(string stringType);
    
    static const vector<ofxAAAlgorithmType> allAvailableAlgorithmTypes = {POWER, PITCH_FREQ, PITCH_CONFIDENCE, PITCH_SALIENCE, HFC, CENTROID, SPECTRAL_COMPLEXITY, INHARMONICITY, DISSONANCE, ROLL_OFF, ODD_TO_EVEN, ONSETS, SPECTRUM, MEL_BANDS, MFCC, HPCP, TRISTIMULUS};
    
    bool hasTypeVectorOutputValues(ofxAAAlgorithmType type);
}
