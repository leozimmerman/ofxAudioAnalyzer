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


#include "ofxAANetwork.h"

#define DEFAULT_MELBANDS_BANDS_NUM 24
#define DEFAULT_DCT_COEFF_NUM 10
#define DEFAULT_PITCH_SALIENCE_FUNC_BIN_RES 10
#define DEFAULT_TRISTIMULUS_BANDS_NUM 3
#define DEFAULT_HPCP_SIZE 12

#define DEFAULT_MAX_VALUE_ENERGY 100.0
#define DEFAULT_MAX_VALUE_HFC 2000.0
#define DEFAULT_MAX_VALUE_SPECTRAL_COMPLEXITY 20.0
#define DEFAULT_MAX_VALUE_ODD_TO_EVEN 10.0
#define DEFAULT_MAX_VALUE_STRONG_PEAK 20.0
#define DEFAULT_MAX_VALUE_STRONG_DECAY 100.0
#define DEFAULT_MAX_VALUE_PITCH_FREQ 4186.0 //C8

namespace ofxaa {
    
    Network::Network(int sampleRate, int bufferSize){
        _framesize = bufferSize;
        _samplerate = sampleRate;
        
        _audioSignal.resize(bufferSize);
        
        createAlgorithms();
        setDefaultMaxEstimatedValues();
        connectAlgorithms();
    }
    Network::~Network(){
        deleteAlgorithms();
    }
    
    void Network::createAlgorithms(){
        fft = new ofxAAFftAlgorithm(FFT, _samplerate, _framesize);
        vectorAlgorithms.push_back(fft);
        pitchDetect = new ofxAAPitchDetectAlgorithm(PITCH_YIN_FREQ, _samplerate, _framesize);
        algorithms.push_back(pitchDetect);
        onsets = new ofxAAOnsetsAlgorithm(ONSETS, _samplerate, _framesize);
        algorithms.push_back(onsets);
        cartesian2polar = new ofxAACartToPolAlgorithm(CART_TO_POLAR, _samplerate, _framesize);
        algorithms.push_back(cartesian2polar);
        spectralPeaks = new ofxAAPeaksAlgorithm(SPECTRAL_PEAKS, _samplerate, _framesize);
        algorithms.push_back(spectralPeaks);
        harmonicPeaks = new ofxAAPeaksAlgorithm(HARMONIC_PEAKS, _samplerate, _framesize);
        algorithms.push_back(harmonicPeaks);
        pitchSalienceFunctionPeaks = new ofxAAPitchSalienceFunctionPeaksAlgorithm(PITCH_SALIENCE_FUNC_PEAKS, _samplerate, _framesize);
        algorithms.push_back(pitchSalienceFunctionPeaks);
        
        rms = new ofxAABaseAlgorithm(RMS, _samplerate, _framesize);
        algorithms.push_back(rms);
        energy = new ofxAABaseAlgorithm(ENERGY, _samplerate, _framesize);
        algorithms.push_back(energy);
        power = new ofxAABaseAlgorithm(POWER, _samplerate, _framesize);
        algorithms.push_back(power);
        pitchSalience = new ofxAABaseAlgorithm(PITCH_SALIENCE, _samplerate, _framesize);
        algorithms.push_back(pitchSalience);
        inharmonicity = new ofxAABaseAlgorithm(INHARMONICITY, _samplerate, _framesize);
        algorithms.push_back(inharmonicity);
        hfc = new ofxAABaseAlgorithm(HFC, _samplerate, _framesize);
        algorithms.push_back(hfc);
        centroid = new ofxAABaseAlgorithm(CENTROID, _samplerate, _framesize);
        algorithms.push_back(centroid);
        spectralComplexity = new ofxAABaseAlgorithm(SPECTRAL_COMPLEXITY, _samplerate, _framesize);
        algorithms.push_back(spectralComplexity);
        dissonance = new ofxAABaseAlgorithm(DISSONANCE, _samplerate, _framesize);
        algorithms.push_back(dissonance);
        rollOff = new ofxAABaseAlgorithm(ROLL_OFF, _samplerate, _framesize);
        algorithms.push_back(rollOff);
        oddToEven = new ofxAABaseAlgorithm(ODD_TO_EVEN, _samplerate, _framesize);
        algorithms.push_back(oddToEven);
        strongPeak = new ofxAABaseAlgorithm(STRONG_PEAK, _samplerate, _framesize);
        algorithms.push_back(strongPeak);
        strongDecay = new ofxAABaseAlgorithm(STRONG_DECAY, _samplerate, _framesize);
        algorithms.push_back(strongDecay);
        
        spectrum = new ofxAAOneVectorOutputAlgorithm(SPECTRUM, _samplerate, _framesize, (_framesize/2)+1);
        vectorAlgorithms.push_back(spectrum);
        melBands = new ofxAAOneVectorOutputAlgorithm(MEL_BANDS, _samplerate, _framesize, DEFAULT_MELBANDS_BANDS_NUM);
        vectorAlgorithms.push_back(melBands);
        dct = new ofxAAOneVectorOutputAlgorithm(DCT, _samplerate, _framesize, DEFAULT_DCT_COEFF_NUM);
        vectorAlgorithms.push_back(dct);
        hpcp = new ofxAAOneVectorOutputAlgorithm(HPCP, _samplerate, _framesize, DEFAULT_HPCP_SIZE);
        vectorAlgorithms.push_back(hpcp);
        pitchSalienceFunction = new ofxAAOneVectorOutputAlgorithm(PITCH_SALIENCE_FUNC, _samplerate, _framesize, DEFAULT_PITCH_SALIENCE_FUNC_BIN_RES);
        vectorAlgorithms.push_back(pitchSalienceFunction);
        tristimulus = new ofxAAOneVectorOutputAlgorithm(TRISTIMULUS, _samplerate, _framesize, DEFAULT_TRISTIMULUS_BANDS_NUM);
        vectorAlgorithms.push_back(tristimulus);
        dcRemoval = new ofxAAOneVectorOutputAlgorithm(DC_REMOVAL, _samplerate, _framesize);
        vectorAlgorithms.push_back(dcRemoval);
        windowing = new ofxAAOneVectorOutputAlgorithm(WINDOWING, _samplerate, _framesize);
        vectorAlgorithms.push_back(windowing);
    }
    
    void Network::setDefaultMaxEstimatedValues(){
        //default values set from testing with white noise.
        energy->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ENERGY);
        hfc->setMaxEstimatedValue(DEFAULT_MAX_VALUE_HFC);
        spectralComplexity->setMaxEstimatedValue(DEFAULT_MAX_VALUE_SPECTRAL_COMPLEXITY);
        rollOff->setMaxEstimatedValue(_samplerate/2);
        oddToEven->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ODD_TO_EVEN);
        strongPeak->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_PEAK);
        strongDecay->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_DECAY);
        pitchDetect->setMaxEstimatedValue(DEFAULT_MAX_VALUE_PITCH_FREQ);//C8
    }
    
    void Network::connectAlgorithms(){
        //DCRemoval
        dcRemoval->algorithm->input("signal").set(_audioSignal);
        dcRemoval->algorithm->output("signal").set(dcRemoval->realValues);
        //RMS
        rms->algorithm->input("array").set(dcRemoval->realValues);
        rms->algorithm->output("rms").set(rms->realValue);
        //Energy
        energy->algorithm->input("array").set(dcRemoval->realValues);
        energy->algorithm->output("energy").set(energy->realValue);
        //Power
        power->algorithm->input("array").set(dcRemoval->realValues);
        power->algorithm->output("power").set(dcRemoval->realValue);
        //StrongDecay
        strongDecay->algorithm->input("signal").set(dcRemoval->realValues);
        strongDecay->algorithm->output("strongDecay").set(strongDecay->realValue);
        //Window
        windowing->algorithm->input("frame").set(dcRemoval->realValues);
        windowing->algorithm->output("frame").set(windowing->realValues);
        //Onsets
        fft->algorithm->input("frame").set(windowing->realValues);
        fft->algorithm->output("fft").set(fft->fftRealValues);
        cartesian2polar->algorithm->input("complex").set(fft->fftRealValues);
        cartesian2polar->algorithm->output("magnitude").set(cartesian2polar->magnitudes);
        cartesian2polar->algorithm->output("phase").set(cartesian2polar->phases);
        //-
        onsets->onsetHfc.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
        onsets->onsetHfc.algorithm->input("phase").set(cartesian2polar->phases);
        onsets->onsetHfc.algorithm->output("onsetDetection").set(onsets->onsetHfc.realValue);
        //-
        onsets->onsetComplex.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
        onsets->onsetComplex.algorithm->input("phase").set(cartesian2polar->phases);
        onsets->onsetComplex.algorithm->output("onsetDetection").set(onsets->onsetComplex.realValue);
        //-
        onsets->onsetFlux.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
        onsets->onsetFlux.algorithm->input("phase").set(cartesian2polar->phases);
        onsets->onsetFlux.algorithm->output("onsetDetection").set(onsets->onsetFlux.realValue);
        //Spectrum
        spectrum->algorithm->input("frame").set(windowing->realValues);
        spectrum->algorithm->output("spectrum").set(spectrum->realValues);
        //HFC
        hfc->algorithm->input("spectrum").set(spectrum->realValues);
        hfc->algorithm->output("hfc").set(hfc->realValue);
        //Pitch Salience
        pitchSalience->algorithm->input("spectrum").set(spectrum->realValues);
        pitchSalience->algorithm->output("pitchSalience").set(pitchSalience->realValue);
        //Centroid
        centroid->algorithm->input("array").set(spectrum->realValues);
        centroid->algorithm->output("centroid").set(centroid->realValue);
        //Spectral Complexity
        spectralComplexity->algorithm->input("spectrum").set(spectrum->realValues);
        spectralComplexity->algorithm->output("spectralComplexity").set(spectralComplexity->realValue);
        //Peak detection
        spectralPeaks->algorithm->input("spectrum").set(spectrum->realValues);
        spectralPeaks->algorithm->output("frequencies").set(spectralPeaks->frequencies);
        spectralPeaks->algorithm->output("magnitudes").set(spectralPeaks->magnitudes);
        //HPCP
        hpcp->algorithm->input("frequencies").set(spectralPeaks->frequencies);
        hpcp->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
        hpcp->algorithm->output("hpcp").set(hpcp->realValues);
        //MelBands
        melBands->algorithm->input("spectrum").set(spectrum->realValues);
        melBands->algorithm->output("bands").set(melBands->realValues);
        //DCT
        dct->algorithm->input("array").set(melBands->logRealValues);
        dct->algorithm->output("dct").set(dct->realValues);
        //PitchDetection
        pitchDetect->algorithm->input("spectrum").set(spectrum->realValues);
        pitchDetect->algorithm->output("pitch").set(pitchDetect->pitchRealVal);
        pitchDetect->algorithm->output("pitchConfidence").set(pitchDetect->confidenceRealVal);
        //HarmonicPeaks
        harmonicPeaks->algorithm->input("frequencies").set(spectralPeaks->frequencies);
        harmonicPeaks->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
        harmonicPeaks->algorithm->input("pitch").set(pitchDetect->pitchRealVal);
        harmonicPeaks->algorithm->output("harmonicFrequencies").set(harmonicPeaks->frequencies);
        harmonicPeaks->algorithm->output("harmonicMagnitudes").set(harmonicPeaks->magnitudes);
        //Inharmonicity
        inharmonicity->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
        inharmonicity->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
        inharmonicity->algorithm->output("inharmonicity").set(inharmonicity->realValue);
        //Dissonance
        dissonance->algorithm->input("frequencies").set(spectralPeaks->frequencies);
        dissonance->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
        dissonance->algorithm->output("dissonance").set(dissonance->realValue);
        //Pitch Salience Function
        pitchSalienceFunction->algorithm->input("frequencies").set(spectralPeaks->frequencies);
        pitchSalienceFunction->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
        pitchSalienceFunction->algorithm->output("salienceFunction").set(pitchSalienceFunction->realValues);
        //Pitch Salience Function Peaks
        pitchSalienceFunctionPeaks->algorithm->input("salienceFunction").set(pitchSalienceFunction->realValues);
        pitchSalienceFunctionPeaks->algorithm->output("salienceBins").set(pitchSalienceFunctionPeaks->realSalienceBins);
        pitchSalienceFunctionPeaks->algorithm->output("salienceValues").set(pitchSalienceFunctionPeaks->realSalienceValues);
        
        //RollOff
        rollOff->algorithm->input("spectrum").set(spectrum->realValues);
        rollOff->algorithm->output("rollOff").set(rollOff->realValue);
        //StrongPeak
        strongPeak->algorithm->input("spectrum").set(spectrum->realValues);
        strongPeak->algorithm->output("strongPeak").set(strongPeak->realValue);
        
        //OddToEven
        oddToEven->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
        oddToEven->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
        oddToEven->algorithm->output("oddToEvenHarmonicEnergyRatio").set(oddToEven->realValue);
        //Tristimulus
        tristimulus->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
        tristimulus->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
        tristimulus->algorithm->output("tristimulus").set(tristimulus->realValues);
        
        //MultiPitch Kalpuri:
        ///multiPitchKlapuri.setup(&pitchSalienceFunctionPeaks, &spectrum, _samplerate);
    }
    void Network::computeAlgorithms(vector<Real>& signal){
        _audioSignal = signal;
        
        // MARK: Compute Algorithms
        dcRemoval->compute();
        rms->compute();
        energy->compute();
        power->compute();
        
        windowing->compute();
        
        if(onsets->getIsActive()){
            fft->compute();
            cartesian2polar->compute();
            onsets->compute();
        }
        
        //spectrum must always be computed as it is neede for other algorithms
        spectrum->algorithm->compute();
        
        hfc->compute();
        pitchSalience->compute();
        pitchDetect->compute();
        centroid->compute();
        spectralComplexity->compute();
        
        if(melBands->getIsActive()){
            melBands->algorithm->compute();
            if(dct->getIsActive()){
                melBands->updateLogRealValues();
                dct->compute();
            }
        }else{
            dct->setActive(false);//dct needs melBands to be active
        }
        spectralPeaks->compute();
        hpcp->compute();
        
        if (inharmonicity->getIsActive()){
            harmonicPeaks->compute();
            inharmonicity->algorithm->compute();
        }
        
        dissonance->compute();
        pitchSalienceFunction->compute();
        pitchSalienceFunctionPeaks->compute();
        
        ///multiPitchKlapuri.compute();
        
        rollOff->compute();
        oddToEven->compute();
        strongPeak->compute();
        
        tristimulus->compute();
        if(dcRemoval->realValues[0] != 0.0){
            //the strong decay is not defined for a zero signal
            strongDecay->compute();
        }
        
        // MARK: Cast Values
        rms->castValueToFloat();
        power->castValueToFloat();

        spectrum->castValuesToFloat(true);
        melBands->castValuesToFloat(true);
        dct->castValuesToFloat(false);
        hpcp->castValuesToFloat(false);
        tristimulus->castValuesToFloat(false);

        for (auto a : algorithms){
            a->castValueToFloat();
        }
        pitchSalienceFunctionPeaks->castValuesToFloat();
        pitchDetect->castValuesToFloat();
        onsets->castValuesToFloat();

        onsets->evaluate();
    }
    
    void Network::deleteAlgorithms(){
        for (auto a : algorithms){
            a->deleteAlgorithm();
            delete a;
        }
        for (auto a :vectorAlgorithms){
            a->deleteAlgorithm();
            delete a;
        }
    }
 
    float Network::getValue(ofxAAAlgorithmType algorithmType, float smooth, bool normalized){
        ///TEST
        float val = smooth ? hfc->getSmoothedValueNormalized(smooth) : hfc->getValueNormalized();
       // cout << val << endl;
        return val;
        ///*
    }
    
    vector<float>& Network::getValues(ofxAAAlgorithmType algorithmType, float smooth){
         return smooth ? hpcp->getSmoothedValues(smooth) : hpcp->getValues();
    }
    
    bool Network::getOnsetValue(){
        return onsets->getValue();
    }
    //----------------------------------------------
    void Network::resetOnsets(){
        onsets->reset();
    }
    //----------------------------------------------
    void Network::setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
        onsets->setOnsetAlpha(alpha);
        onsets->setOnsetSilenceThreshold(silenceTresh);
        onsets->setOnsetTimeThreshold(timeTresh);
        onsets->setUseTimeThreshold(useTimeTresh);
    }
    
    /*
    ofxAABaseAlgorithm* Network::algorithm(ofxAAAlgorithmType type){
        for (int i=0; i<algorithms.size(); i++){
            if (type == algorithms[i]->getType()){
                return algorithms[i];
            }
        }
        ofLogError()<<"ofxAudioAnalyzerUnit: algorithm type is NOT a Base Algorithm.";
    }
    
    ofxAAOneVectorOutputAlgorithm* Network::vectorAlgorithm(ofxAAAlgorithmType type){
        for (int i=0; i<vectorAlgorithms.size(); i++){
            if (type == vectorAlgorithms[i]->getType()){
                return vectorAlgorithms[i];
            }
        }
    }
    */
    //----------------------------------------------
    
//    void Network::setSalienceFunctionPeaksParameters(int maxPeaks){
//        pitchSalienceFunctionPeaks->setMaxPeaksNum(maxPeaks);
//    }
//
//    vector<SalienceFunctionPeak>& Network::getPitchSaliencePeaksRef(float smooth){
//        return smooth ? pitchSalienceFunctionPeaks->getSmoothedPeaks(smooth) : pitchSalienceFunctionPeaks->getPeaks();
//    }
    

    
}
