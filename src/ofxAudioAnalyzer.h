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


class ofxAudioAnalyzer{
 
 public:
    
    void setup(int sampleRate, int bufferSize, int channels);
    void reset(int sampleRate, int bufferSize, int channels);
    void analyze(const ofSoundBuffer & inBuffer);
    void exit();
    
    ///Gets value of single output  Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    float getValue(ofxAAAlgorithm algorithm, int channel, float smooth=0.0, bool normalized=false);
    
    ///Gets values of vector output Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    vector<float>& getValues(ofxAAAlgorithm algorithm, int channel, float smooth=0.0);
    
    ///Gets the array of pitch salience function peaks: bin/cents & value
    vector<SalienceFunctionPeak>& getSalienceFunctionPeaks(int channel);
    
    ///Returns if there is an onset in the speciefied channel.
    bool getOnsetValue(int channel);

    ///Pointers for the audio analyzing units.
    ///Use very carefully!
    vector<ofxAudioAnalyzerUnit*>& getChannelAnalyzersPtrs(){return channelAnalyzerUnits;}
    
    ///Resets onsetsr detections buffer
    void resetOnsets(int channel);
    
    ///Sets onsets detection parameters
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param alpha: the proportion of the mean included to reject smaller peaks--filters very short onsets
    ///\param silenceTreshold: the threshold for silence
    ///\param timeTreshold: time treshold in ms.
    ///\param useTimeTreshold: use or note the time treshold.
    void setOnsetsParameters(int channel, float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
    
   
    

 private:
    
    int _samplerate;
    int _buffersize;
    int _channels;
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzerUnits;
    
    
};

