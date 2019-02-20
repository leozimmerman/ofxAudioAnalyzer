//
//  ofxAAUtils.cpp
//  Sonoscopio
//
//  Created by Leo on 11/09/2018.
//

#include "ofxAAUtils.h"

namespace ofxaa{
    string algorithmTypeToString(ofxAAAlgorithmType algorithmType) {
        switch (algorithmType) {
            case POWER: return POWER_STRING;
            case PITCH_FREQ: return PITCH_FREQ_STRING;
            case PITCH_CONFIDENCE: return PITCH_CONFIDENCE_STRING;
            case PITCH_SALIENCE: return PITCH_SALIENCE_STRING;
            case HFC: return HFC_STRING;
            case CENTROID: return CENTROID_STRING;
            case SPECTRAL_COMPLEXITY: return SPEC_COMP_STRING;
            case INHARMONICITY: return INHARMONICITY_STRING;
            case DISSONANCE: return DISSONANCE_STRING;
            case ROLL_OFF: return ROLL_OFF_STRING;
            case ODD_TO_EVEN: return ODD_TO_EVEN_STRING;
            case ONSETS: return ONSETS_STRING;
            case SPECTRUM: return SPECTRUM_STRING;
            case MEL_BANDS: return MEL_BANDS_STRING;
            case MFCC: return MFCC_STRING;
            case HPCP: return HPCP_STRING;
            case TRISTIMULUS: return TRISTIMULUS_STRING;
            default: return "";
        }
    }
    
    ofxAAAlgorithmType stringToAlgorithmType(string stringType){
        if (stringType == algorithmTypeToString(RMS)){
            return RMS;
        } else if (stringType == algorithmTypeToString(POWER)){
            return POWER;
        } else if (stringType == algorithmTypeToString(PITCH_FREQ)){
            return PITCH_FREQ;
        } else if (stringType == algorithmTypeToString(PITCH_CONFIDENCE)){
            return PITCH_CONFIDENCE;
        } else if (stringType == algorithmTypeToString(PITCH_SALIENCE)){
            return PITCH_SALIENCE;
        } else if (stringType == algorithmTypeToString(HFC)){
            return HFC;
        } else if (stringType == algorithmTypeToString(CENTROID)){
            return CENTROID;
        } else if (stringType == algorithmTypeToString(SPECTRAL_COMPLEXITY)){
            return SPECTRAL_COMPLEXITY;
        } else if (stringType == algorithmTypeToString(INHARMONICITY)){
            return INHARMONICITY;
        } else if (stringType == algorithmTypeToString(DISSONANCE)){
            return DISSONANCE;
        } else if (stringType == algorithmTypeToString(ROLL_OFF)){
            return ROLL_OFF;
        } else if (stringType == algorithmTypeToString(ODD_TO_EVEN)){
            return ODD_TO_EVEN;
        } else if (stringType == algorithmTypeToString(ONSETS)){
            return ONSETS;
        } else if (stringType == algorithmTypeToString(SPECTRUM)){
            return SPECTRUM;
        }else if (stringType == algorithmTypeToString(MEL_BANDS)){
            return MEL_BANDS;
        }else if (stringType == algorithmTypeToString(MFCC)){
            return MFCC;
        }else if (stringType == algorithmTypeToString(HPCP)){
            return HPCP;
        }else if (stringType == algorithmTypeToString(TRISTIMULUS)){
            return TRISTIMULUS;
        }
    }
    
    bool hasTypeVectorOutputValues(ofxAAAlgorithmType type){
        if (type == SPECTRUM || type == MEL_BANDS || type == MFCC || type == HPCP || type == TRISTIMULUS){
            return true;
        }
        return false;
    }
}
