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

#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzerUnit.h"

class ofxAudioAnalyzer{
 
 public:
    
    void setup(int sampleRate, int bufferSize, int channels);
    void reset(int sampleRate, int bufferSize, int channels);
    void analyze(const ofSoundBuffer & inBuffer);
    void exit();
    
    int getSampleRate() {return _samplerate;}
    int getBufferSize() {return _buffersize;}
    int getChannelsNum(){return _channels;}
    
    ///Gets value of single output  Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    float getValue(ofxAAValueType valueType, int channel, float smooth=0.0, bool normalized=false);
    
    ///Gets values of vector output Algorithms.
    ///\param algorithm
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param smooth: smoothing amount. 0.0=non smoothing, 1.0=fixed value
    vector<float>& getValues(ofxaa::AlgorithmType algorithm, int channel, float smooth=0.0);
    
    ///Returns if there is an onset in the speciefied channel.
    bool getOnsetValue(int channel);
    
    ///Gets the state of an algorithm
    bool getIsActive(int channel, ofxaa::AlgorithmType algorithm);

    ///Pointers for the audio analyzing units.
    ///Use very carefully!
    vector<ofxAudioAnalyzerUnit*>& getChannelAnalyzersPtrs(){return channelAnalyzerUnits;}
    
    ///Resets onsetsr detections buffer
    void resetOnsets(int channel);
    
    ///Activates and deactives algorithms.
    void setActive(int channel, ofxaa::AlgorithmType algorithm, bool state);
    
    ///Set max estimated values for algorithms that are not normalized
    void setMaxEstimatedValue(int channel, ofxaa::AlgorithmType algorithm, float value);
    
    ///Sets onsets detection parameters
    ///\param channel: starting from 0 (for stereo setup, 0 and 1)
    ///\param alpha: the proportion of the mean included to reject smaller peaks--filters very short onsets
    ///\param silenceThreshold: the thhreshold for silence
    ///\param timeThreshold: time threshold in ms.
    ///\param useTimeThreshold: use or note the time threshold.
    void setOnsetsParameters(int channel, float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
    

 private:
    
    int _samplerate;
    int _buffersize;
    int _channels;
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzerUnits;
    
    
};

