/*
 Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzerUnit.h"

enum ofxAASingleAlgorithm{
    RMS,
    ENERGY,
    POWER,
    PITCH_FREQ,
    PITCH_SALIENCE,
    INHARMONICITY,
    HFC,
    CENTROID,
    SPECTRAL_COMPLEXITY,
};

enum ofxAAVectorAlgorithm{
    SPECTRUM,
    MEL_BANDS,
    MFCC,
    HPCP
};


class ofxAudioAnalyzer{
 
 public:
    
    void setup(int sampleRate, int bufferSize, int channels);
    void reset(int sampleRate, int bufferSize, int channels);
    void analyze(const ofSoundBuffer & inBuffer);
    void exit();
    
    ///Getter for single output value Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    float getSingleValue(ofxAASingleAlgorithm algorithm, int channel, float smooth=0.0);
    
    ///Getter for vector output values Algorithms.
    ///Parameters are the same for single values.
    vector<float>& getVectorValues(ofxAAVectorAlgorithm algorithm, int channel, float smooth=0.0);

    ///Pointers for the audio analyzing units.
    ///Use very carefully!
    vector<ofxAudioAnalyzerUnit*>& getChannelAnalyzersPtrs(){return channelAnalyzerUnits;}
    
    
    //ofSoundBuffer soundBuffer;
    

 private:
    
    int _samplerate;
    int _buffersize;
    int _channels;
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzerUnits;
    
    
};

