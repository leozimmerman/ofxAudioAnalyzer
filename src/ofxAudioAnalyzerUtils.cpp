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

#define MELBANDS_BANDS_NUM 24
#define DCT_COEFF_NUM 13
#define PITCH_SALIENCE_FUNC_NUM 10
#define TRISTIMULUS_BANDS_NUM 3

#define HPCP_SIZE 12
#define HPCP_MIN_FREQ 40.0//hz
#define HPCP_MAX_FREQ 5000.0//hz

#define PEAKS_MAXPEAKS_NUM 10000
#define PEAKS_MIN_FREQ 40.0//hz
#define PEAKS_MAX_FREQ 5000.0//hz

#include "ofxAudioAnalyzerUtils.h"

namespace ofxaa {
    
    Algorithm* createAlgorithmWithType(ofxAAAlgorithmType algorithmType, int samplerate, int framesize){
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        
        
        //factory.init();
        
        switch (algorithmType) {
            case RMS:
                return factory.create("RMS");
            case ENERGY:
                return factory.create("Energy");;
            case POWER:
                return factory.create("InstantPower");
            case DC_REMOVAL:
                return factory.create("DCRemoval", "sampleRate", samplerate);
            case WINDOW:
                return factory.create("Windowing", "type", "hann", "zeroPadding", 0);///**
            case FFT:
                return factory.create("FFT", "size", framesize);
            case CART_TO_POLAR:
                return factory.create("CartesianToPolar");
            case ONSETS_DETECTION_HFC:
                return factory.create("OnsetDetection", "method", "hfc", "sampleRate", samplerate);
            case ONSETS_DETECTION_COMPLEX:
                return factory.create("OnsetDetection", "method", "complex", "sampleRate", samplerate);
            case ONSETS_DETECTION_FLUX:
                return factory.create("OnsetDetection", "method", "flux", "sampleRate", samplerate);
            case SPECTRUM:
                return factory.create("Spectrum", "size", framesize);
            case HFC:
                return factory.create("HFC", "sampleRate", samplerate);
            case PITCH_SALIENCE:
                return factory.create("PitchSalience", "sampleRate", samplerate);
            case CENTROID:
                return factory.create("Centroid", "range", samplerate/2);
            case SPECTRAL_COMPLEXITY:
                return factory.create("SpectralComplexity", "sampleRate", samplerate);
            case DISSONANCE:
                return factory.create("Dissonance");
            case ROLL_OFF:
                return factory.create("RollOff", "sampleRate", samplerate);
            case ODD_TO_EVEN:
                return factory.create("OddToEvenHarmonicEnergyRatio");
            case STRONG_PEAK:
                return factory.create("StrongPeak");
            case STRONG_DECAY:
                return factory.create("StrongDecay", "sampleRate", samplerate);
            case SPECTRAL_PEAKS:
                return factory.create("SpectralPeaks",
                              "maxPeaks", PEAKS_MAXPEAKS_NUM,
                              "magnitudeThreshold", 0.00001,
                              "minFrequency", PEAKS_MIN_FREQ,
                              "maxFrequency", PEAKS_MAX_FREQ,
                              "orderBy", "frequency");
            case MEL_BANDS:
                return factory.create("MelBands",
                                      "inputSize", (framesize/2)+1,
                                      "sampleRate", samplerate,
                                      "highFrequencyBound", samplerate/2,
                                      "numberBands", MELBANDS_BANDS_NUM);
            case INHARMONICITY:
                return factory.create("Inharmonicity");
            
            case DCT:
                return factory.create("DCT",
                                      "inputSize", MELBANDS_BANDS_NUM,
                                      "outputSize", DCT_COEFF_NUM);
                
            case HPCP:
                return factory.create("HPCP",
                                      "size", HPCP_SIZE,
                                      "referenceFrequency", 440,
                                      "bandPreset", false,
                                      "minFrequency", HPCP_MIN_FREQ,
                                      "maxFrequency", HPCP_MAX_FREQ,
                                      "weightType", "squaredCosine",
                                      "nonLinear", false,
                                      "windowSize", 4.0/3.0);
                
            case PITCH_SALIENCE_FUNC:
                return factory.create("PitchSalienceFunction");
                
            case PITCH_SALIENCE_FUNC_PEAKS:
                return factory.create("PitchSalienceFunctionPeaks");
                
            case TRISTIMULUS:
                return factory.create("Tristimulus");
                
            case HARMONIC_PEAKS:
                return factory.create("HarmonicPeaks");
            
            case PITCH_YIN_FREQ://Duplicate
                return factory.create("PitchYinFFT", "frameSize", framesize, "sampleRate", samplerate);///*
                
            case PITCH_YIN_CONFIDENCE://Duplicate
                return factory.create("PitchYinFFT", "frameSize", framesize, "sampleRate", samplerate);///*
                
            case ONSETS:
                ///???
                break;
            
            case MULTI_PITCHES:
                ///???
                break;
            case MULTI_PITCH_KLAPURI:
                ///???
                break;
            
                
            case NONE:
                return NULL;
                break;
        }
        // factory.shutdown();
    }
    
    bool algorithmHasVectorOutput(ofxAABaseAlgorithm& algorithm){
        ofxAAOneVectorOutputAlgorithm* vectorAlgorithm = dynamic_cast<ofxAAOneVectorOutputAlgorithm*>(&algorithm);
        return vectorAlgorithm != NULL;
    }
    
    bool algorithmHasOutputInDbs(ofxAABaseAlgorithm& algorithm){
        for (auto a : algorithmsWithOutputInDbs){
            if (algorithm.getType() == a){
                return true;
            }
        }
        return false;
    }
    
    bool algorithmHasNormalizedSingleOutputByDefault(ofxAABaseAlgorithm& algorithm){
        if (algorithmHasVectorOutput(algorithm)){
            return false;
        }
        for (auto a : algorithmsWithNoNormalizedSingleOutput){
            if (algorithm.getType() == a){
                return false;
            }
        }
        return true;
    }
    
    string midiToNoteName(int midiNote){
        string noteName;
        int mod = midiNote%12;
        switch (mod){
            case 0: noteName = "C";
                break;
            case 1: noteName = "C#";
                break;
            case 2: noteName = "D";
                break;
            case 3: noteName = "D#";
                break;
            case 4: noteName = "E";
                break;
            case 5: noteName = "F";
                break;
            case 6: noteName = "F#";
                break;
            case 7: noteName = "G";
                break;
            case 8: noteName = "G#";
                break;
            case 9: noteName = "A";
                break;
            case 10: noteName = "Bb";
                break;
            case 11: noteName = "B";
                break;
            default:
                break;
                
        }
        return (noteName);
    }
    
    int pitchToMidi(float pitch){
        return round (12*log2(pitch/440) + 69);
    }
}
