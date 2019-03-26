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

#include "ofxAAPitchSalienceFunctionPeaksAlgorithm.h"

void ofxAAPitchSalienceFunctionPeaksAlgorithm::castValuesToFloat(){
    
    peaks.clear();
    peaks.resize(realSalienceBins.size());
    
    for (int i=0; i<realSalienceBins.size(); i++){
        peaks[i].bin = (float) realSalienceBins[i];
        peaks[i].value = realSalienceValues[i];
    }
}
//-------------------------------------------
vector<SalienceFunctionPeak>& ofxAAPitchSalienceFunctionPeaksAlgorithm::getPeaks(){
    
    if (limitPeaksNum && peaks.size() > maxPeaksNum){
        peaks.resize(maxPeaksNum);
    }
    return peaks;
}
//-------------------------------------------
vector<SalienceFunctionPeak>& ofxAAPitchSalienceFunctionPeaksAlgorithm::getSmoothedPeaks(float smthAmnt){
    
    if (limitPeaksNum && peaks.size() > maxPeaksNum){
        peaks.resize(maxPeaksNum);
    }
    
    smoothedPeaks.resize(peaks.size(), SalienceFunctionPeak());
    
    for(int i=0; i<smoothedPeaks.size(); i++){
        smoothedPeaks[i].bin = smoothedPeaks[i].bin * smthAmnt + (1-smthAmnt) * peaks[i].bin;
        //        smoothedPeaks[i].bin = peaks[i].bin;
        smoothedPeaks[i].value = smoothedPeaks[i].value * smthAmnt + (1-smthAmnt) * peaks[i].value;
    }
    return smoothedPeaks;
}
