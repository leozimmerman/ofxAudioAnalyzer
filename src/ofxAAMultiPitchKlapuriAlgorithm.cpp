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

#include "ofxAAMultiPitchKlapuriAlgorithm.h"

//-------------------------------------------
void ofxAAMultiPitchKlapuriAlgorithm::setup(ofxAAPitchSalienceFunctionPeaksAlgorithm* saliencePeaksPtr,
                                            ofxAAOneVectorOutputAlgorithm* spectrumPtr,
                                            int sampleRate){
    
    _saliencePeaksAlgthm = saliencePeaksPtr;
    _spectrumAlgthm = spectrumPtr;
    _sampleRate = sampleRate;
    
    _binResolution  = 10;//Default?
    _referenceFrequency = 55;//Default?
    _numberHarmonicsMax = 10;//defult?
    
    _binsInSemitone = floor(100.0 / _binResolution);
    _centToHertzBase     = pow(2, _binResolution / 1200.0);
    _binsInOctave        = 1200.0 / _binResolution;
    _referenceTerm       = 0.5 - _binsInOctave * log2(_referenceFrequency);
    
    _numberBins = frequencyToCentBin(_sampleRate/2);
    _centSpectrum.resize(_numberBins);
    
    
    nearestBinWeights.resize(_binsInSemitone + 1);
    for (int b=0; b <= _binsInSemitone; b++) {
        nearestBinWeights[b] = pow(cos((Real(b)/_binsInSemitone)* M_PI/2), 2);
    }
    
    
    
}
//-------------------------------------------
void ofxAAMultiPitchKlapuriAlgorithm::compute(){
    
    //If theres no peaks, return
    if (_saliencePeaksAlgthm->realSalienceBins.size() == 0 || isActivated == false){
        return;
    }
    
    //----------
    
    // Joint F0 estimation (pitch salience function peaks as candidates)
    
    // compute the cent-scaled spectrum
    fill(_centSpectrum.begin(), _centSpectrum.end(), (Real) 0.0);
    for (int i=0; i<(int)_spectrumAlgthm->realValues.size(); i++) {
        Real f = (Real(i) / Real(_spectrumAlgthm->realValues.size())) * (_sampleRate/2);
        int k = frequencyToCentBin(f);
        if (k>0 && k<_numberBins) {
            _centSpectrum[k] += _spectrumAlgthm->realValues[i];
        }
    }
    
    // get indices corresponding to harmonics of each found peak
    vector<vector<int> > kPeaks;
    for (int i=0; i<(int)_saliencePeaksAlgthm->realSalienceBins.size(); i++) {
        vector<int> k;
        Real f = _referenceFrequency * pow(_centToHertzBase, _saliencePeaksAlgthm->realSalienceBins[i]);
        for (int m=0; m<_numberHarmonicsMax; m++) {
            // find the exact peak for each harmonic
            int kBin = frequencyToCentBin(f*(m+1));
            int kBinMin = max(0, int(kBin-_binsInSemitone));
            int kBinMax = min(_numberBins-1, int(kBin+_binsInSemitone));
            vector<Real> specSegment;
            for (int ii=kBinMin; ii<=kBinMax; ii++) {
                specSegment.push_back(_centSpectrum[ii]);
            }
            kBin = kBinMin + argmax(specSegment)-1;
            k.push_back(kBin);
        }
        kPeaks.push_back(k);
    }
    
    // candidate Spectra
    vector<vector<Real> > Z;
    for (int i=0; i<(int)_saliencePeaksAlgthm->realSalienceBins.size(); i++) {
        vector<Real> z(_numberBins, 0.);
        for (int h=0; h<_numberHarmonicsMax; h++) {
            int hBin = kPeaks[i][h];
            for(int b = max(0, hBin-_binsInSemitone); b <= min(_numberBins-1, hBin+_binsInSemitone); b++) {
                z[b] += nearestBinWeights[abs(b-hBin)] * getWeight(hBin, h) * 0.25; // 0.25 is cancellation parameter
            }
        }
        Z.push_back(z);
    }
    
    // TODO: segfault somewhere here
    // inhibition function
    int numCandidates = _saliencePeaksAlgthm->realSalienceBins.size();
    vector<vector<Real> > inhibition;
    
    for (int i=0; i<numCandidates; i++) {
        vector<Real> inh(numCandidates, 0.);
        for (int j=0; j<numCandidates; j++) {
            for (int m=0; m<_numberHarmonicsMax; m++) {
                inh[j] += getWeight(kPeaks[i][m], m) * _centSpectrum[kPeaks[i][m]] * Z[j][kPeaks[i][m]];
            }
        }
        inhibition.push_back(inh);
    }
    
    // polyphony estimation initialization
    vector<int> finalSelection;
    int p = 1;
    Real gamma = 0.73;
    Real S = _saliencePeaksAlgthm->realSalienceValues[argmax(_saliencePeaksAlgthm->realSalienceValues)] / pow(p,gamma);
    finalSelection.push_back(argmax(_saliencePeaksAlgthm->realSalienceValues));
    
    // goodness function
    vector<vector<Real> > G;
    for (int i=0; i<numCandidates; i++) {
        vector<Real> g;
        for (int j=0; j<numCandidates; j++) {
            if(i==j) {
                g.push_back(0.0);
            } else {
                Real g_val = _saliencePeaksAlgthm->realSalienceValues[i] + _saliencePeaksAlgthm->realSalienceValues[j] - (inhibition[i][j] + inhibition[j][i]);
                g.push_back(g_val);
            }
        }
        G.push_back(g);
    }
    
    vector<vector<int> > selCandInd;
    vector<Real> selCandVal;
    vector<Real> localF0;
    
    while (true) {
        // find numCandidates largest values
        Real maxVal = -1;
        int maxInd_i = 0;
        int maxInd_j = 0;
        
        for (int I=0; I < numCandidates; I++) {
            vector<int> localInd;
            for (int i=0; i < numCandidates; i++) {
                for (int j=0; j < numCandidates; j++) {
                    if (G[i][j] > maxVal) {
                        maxVal = G[i][j];
                        maxInd_i = i;
                        maxInd_j = j;
                    }
                }
            }
            
            localInd.push_back(maxInd_i);
            localInd.push_back(maxInd_j);
            selCandInd.push_back(localInd);
            selCandVal.push_back(G[maxInd_i][maxInd_j]);
            G[maxInd_i][maxInd_j] = -1;
            maxVal = -1;
            maxInd_i = 0;
            maxInd_j = 0;
        }
        
        // re-estimate polyphony
        p++;
        Real Snew = selCandVal[argmax(selCandVal)] / pow(p,gamma);
        if (Snew > S) {
            finalSelection.clear();
            for (int i=0; i<(int)selCandInd[0].size(); i++) {
                finalSelection.push_back(selCandInd[0][i]);
            }
            // re-calculate goddess function
            for (int i=0; i<numCandidates; i++) {
                for (int j=0; j<numCandidates; j++) {
                    G[i][j] += _saliencePeaksAlgthm->realSalienceValues[j];
                    for (int ii=0; ii<(int)selCandInd[i].size(); ii++) {
                        G[i][j] -= (inhibition[selCandInd[i][ii]][j] + inhibition[j][selCandInd[i][ii]]);
                    }
                }
            }
            S = Snew;
        }
        else {
            // add estimated f0 to frame
            for (int i=0; i<(int)finalSelection.size(); i++) {
                Real freq = _referenceFrequency * pow(_centToHertzBase, _saliencePeaksAlgthm->realSalienceBins[finalSelection[i]]);
                localF0.push_back(freq);
            }
            break;
        }
    }
    
    //cast to floats:
    pitches.clear();
    for (int i=0; i<localF0.size(); i++){
        float value = (float) localF0[i];
        pitches.push_back(value);
    }
    
}
//-------------------------------------------
vector<float>& ofxAAMultiPitchKlapuriAlgorithm::getPitches(){
    return pitches;
}
//-------------------------------------------
int ofxAAMultiPitchKlapuriAlgorithm::frequencyToCentBin(Real frequency){
    // +0.5 term is used instead of +1 (as in [1]) to center 0th bin to 55Hz
    // formula: floor(1200 * log2(frequency / _referenceFrequency) / _binResolution + 0.5)
    //  --> 1200 * (log2(frequency) - log2(_referenceFrequency)) / _binResolution + 0.5
    //  --> 1200 * log2(frequency) / _binResolution + (0.5 - 1200 * log2(_referenceFrequency) / _binResolution)
    return floor(_binsInOctave * log2(frequency) + _referenceTerm);
    
}
//-------------------------------------------
Real ofxAAMultiPitchKlapuriAlgorithm::getWeight(int centBin, int harmonicNumber) {
    Real f = _referenceFrequency * pow(_centToHertzBase, centBin);
    Real alpha = 27.0;
    Real beta = 320.0;
    Real w = (f+alpha) / (harmonicNumber*f+beta);
    return w;
}
