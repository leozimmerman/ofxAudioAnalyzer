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

#define DEFAULT_SILENCE_RATE_SIZE 3
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

//for scaling values:
#define MFCC_MAX_ESTIMATED_VALUE 300.0 //???

namespace ofxaa {
    
    Network::Network(int sampleRate, int bufferSize){
        framesize = bufferSize;
        samplerate = sampleRate;
        
        _audioSignal.resize(bufferSize);
        _accumulatedAudioSignal.resize(bufferSize * 87, 0.0); //TODO: Remove duplicated calculation
        
        createAlgorithms();
        setDefaultMaxEstimatedValues();
        connectAlgorithms();
    }
    Network::~Network(){
        deleteAlgorithms();
    }
    
    void Network::createAlgorithms(){
        dcRemoval = new ofxAAOneVectorOutputAlgorithm(DCRemoval, samplerate, framesize);
        vectorAlgorithms.push_back(dcRemoval);
        
        //MARK: Temporal
        rms = new ofxAABaseAlgorithm(Rms, samplerate, framesize);
        algorithms.push_back(rms);
        power = new ofxAABaseAlgorithm(InstantPower, samplerate, framesize);
        algorithms.push_back(power);
        strongDecay = new ofxAABaseAlgorithm(StrongDecay, samplerate, framesize);
        algorithms.push_back(strongDecay);
        zeroCrossingRate = new ofxAABaseAlgorithm(ZeroCrossingRate, samplerate, framesize);
        algorithms.push_back(zeroCrossingRate);
        loudness = new ofxAABaseAlgorithm(Loudness, samplerate, framesize);
        algorithms.push_back(loudness);
        loudnessVickers = new ofxAABaseAlgorithm(LoudnessVickers, samplerate, framesize);
        algorithms.push_back(loudnessVickers);
        silenceRate = new ofxAAOneVectorOutputAlgorithm(SilenceRate, samplerate, framesize, DEFAULT_SILENCE_RATE_SIZE);
        vectorAlgorithms.push_back(silenceRate);
        
        //MARK: SFX
        centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        vectorAlgorithms.push_back(centralMoments);
        decrease = new ofxAABaseAlgorithm(Decrease, samplerate, framesize);
        algorithms.push_back(decrease);
        distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        vectorAlgorithms.push_back(distributionShape);
        derivativeSFX = new ofxAAOneVectorOutputAlgorithm(DerivativeSFX, samplerate, framesize, 2);
        vectorAlgorithms.push_back(derivativeSFX);
        envelope = new ofxAAOneVectorOutputAlgorithm(Envelope, samplerate, framesize);
        vectorAlgorithms.push_back(envelope);
        envelope_acummulated = new ofxAAOneVectorOutputAlgorithm(Envelope, samplerate, framesize);
        vectorAlgorithms.push_back(envelope_acummulated);
        flatnessSFX = new ofxAABaseAlgorithm(FlatnessSFX, samplerate, framesize);
        algorithms.push_back(flatnessSFX);
        logAttackTime = new ofxAAOneVectorOutputAlgorithm(LogAttackTime, samplerate, framesize, 3);
        vectorAlgorithms.push_back(logAttackTime);
        maxToTotal = new ofxAABaseAlgorithm(MaxToTotal, samplerate, framesize);
        algorithms.push_back(maxToTotal);
        tcToTotal = new ofxAABaseAlgorithm(TCToTotal, samplerate, framesize);
        algorithms.push_back(tcToTotal);
        
        
        ///********************************
        windowing = new ofxAAOneVectorOutputAlgorithm(WINDOWING, samplerate, framesize);
        vectorAlgorithms.push_back(windowing);
        
        fft = new ofxAAFftAlgorithm(FFT, samplerate, framesize);
        vectorAlgorithms.push_back(fft);
        
        pitchDetect = new ofxAAPitchDetectAlgorithm(PITCH_YIN_FREQ, samplerate, framesize);
        algorithms.push_back(pitchDetect);
        
        onsets = new ofxAAOnsetsAlgorithm(ONSETS, samplerate, framesize);
        algorithms.push_back(onsets);
        
        cartesian2polar = new ofxAACartToPolAlgorithm(CART_TO_POLAR, samplerate, framesize);
        algorithms.push_back(cartesian2polar);
        spectralPeaks = new ofxAAPeaksAlgorithm(SPECTRAL_PEAKS, samplerate, framesize);
        algorithms.push_back(spectralPeaks);
        harmonicPeaks = new ofxAAPeaksAlgorithm(HARMONIC_PEAKS, samplerate, framesize);
        algorithms.push_back(harmonicPeaks);
        pitchSalienceFunctionPeaks = new ofxAAPitchSalienceFunctionPeaksAlgorithm(PITCH_SALIENCE_FUNC_PEAKS, samplerate, framesize);
        algorithms.push_back(pitchSalienceFunctionPeaks);
        
        
        energy = new ofxAABaseAlgorithm(ENERGY, samplerate, framesize);
        algorithms.push_back(energy);
        
        pitchSalience = new ofxAABaseAlgorithm(PITCH_SALIENCE, samplerate, framesize);
        algorithms.push_back(pitchSalience);
        inharmonicity = new ofxAABaseAlgorithm(INHARMONICITY, samplerate, framesize);
        algorithms.push_back(inharmonicity);
        hfc = new ofxAABaseAlgorithm(HFC, samplerate, framesize);
        algorithms.push_back(hfc);
        centroid = new ofxAABaseAlgorithm(Centroid, samplerate, framesize);
        algorithms.push_back(centroid);
        spectralComplexity = new ofxAABaseAlgorithm(SPECTRAL_COMPLEXITY, samplerate, framesize);
        algorithms.push_back(spectralComplexity);
        dissonance = new ofxAABaseAlgorithm(DISSONANCE, samplerate, framesize);
        algorithms.push_back(dissonance);
        rollOff = new ofxAABaseAlgorithm(ROLL_OFF, samplerate, framesize);
        algorithms.push_back(rollOff);
        oddToEven = new ofxAABaseAlgorithm(ODD_TO_EVEN, samplerate, framesize);
        algorithms.push_back(oddToEven);
        strongPeak = new ofxAABaseAlgorithm(STRONG_PEAK, samplerate, framesize);
        algorithms.push_back(strongPeak);
        
        
        spectrum = new ofxAAOneVectorOutputAlgorithm(SPECTRUM, samplerate, framesize, (framesize/2)+1);
        vectorAlgorithms.push_back(spectrum);
        melBands = new ofxAAOneVectorOutputAlgorithm(MEL_BANDS, samplerate, framesize, DEFAULT_MELBANDS_BANDS_NUM);
        vectorAlgorithms.push_back(melBands);
        dct = new ofxAAOneVectorOutputAlgorithm(DCT, samplerate, framesize, DEFAULT_DCT_COEFF_NUM);
        vectorAlgorithms.push_back(dct);
        hpcp = new ofxAAOneVectorOutputAlgorithm(HPCP, samplerate, framesize, DEFAULT_HPCP_SIZE);
        vectorAlgorithms.push_back(hpcp);
        pitchSalienceFunction = new ofxAAOneVectorOutputAlgorithm(PITCH_SALIENCE_FUNC, samplerate, framesize, DEFAULT_PITCH_SALIENCE_FUNC_BIN_RES);
        vectorAlgorithms.push_back(pitchSalienceFunction);
        tristimulus = new ofxAAOneVectorOutputAlgorithm(TRISTIMULUS, samplerate, framesize, DEFAULT_TRISTIMULUS_BANDS_NUM);
        vectorAlgorithms.push_back(tristimulus);
        
    }
    
    void Network::setDefaultMaxEstimatedValues(){
        //default values set from testing with white noise.
        energy->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ENERGY);
        hfc->setMaxEstimatedValue(DEFAULT_MAX_VALUE_HFC);
        spectralComplexity->setMaxEstimatedValue(DEFAULT_MAX_VALUE_SPECTRAL_COMPLEXITY);
        rollOff->setMaxEstimatedValue(samplerate/2);
        oddToEven->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ODD_TO_EVEN);
        strongPeak->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_PEAK);
        strongDecay->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_DECAY);
        pitchDetect->setMaxEstimatedValue(DEFAULT_MAX_VALUE_PITCH_FREQ);//C8
    }
    
    void Network::connectAlgorithms(){
        
        dcRemoval->algorithm->input("signal").set(_audioSignal);
        dcRemoval->algorithm->output("signal").set(dcRemoval->realValues);
        
        //MARK: Temporal
        rms->algorithm->input("array").set(dcRemoval->realValues);
        rms->algorithm->output("rms").set(rms->realValue);
        
        power->algorithm->input("array").set(dcRemoval->realValues);
        power->algorithm->output("power").set(power->realValue);
        
        zeroCrossingRate->algorithm->input("signal").set(dcRemoval->realValues);
        zeroCrossingRate->algorithm->output("zeroCrossingRate").set(zeroCrossingRate->realValue);
        
        loudness->algorithm->input("signal").set(dcRemoval->realValues);
        loudness->algorithm->output("loudness").set(loudness->realValue);
        
        loudnessVickers->algorithm->input("signal").set(dcRemoval->realValues);
        loudnessVickers->algorithm->output("loudness").set(loudnessVickers->realValue);
        
        silenceRate->algorithm->input("frame").set(dcRemoval->realValues);
        silenceRate->algorithm->output("threshold_0").set(silenceRate->realValues[0]);
        silenceRate->algorithm->output("threshold_1").set(silenceRate->realValues[1]);
        silenceRate->algorithm->output("threshold_2").set(silenceRate->realValues[2]);
        
        //MARK: SFX
        //Essentia source: FreesoundSfxDescriptors.cpp
        envelope->algorithm->input("signal").set(dcRemoval->realValues);
        envelope->algorithm->output("signal").set(envelope->realValues);
        
        envelope_acummulated->algorithm->input("signal").set(_accumulatedAudioSignal);
        envelope_acummulated->algorithm->output("signal").set(envelope_acummulated->realValues);
        
        decrease->algorithm->input("array").set(envelope_acummulated->realValues);
        decrease->algorithm->output("decrease").set(decrease->realValue);
        
        centralMoments->algorithm->input("array").set(envelope_acummulated->realValues);
        centralMoments->algorithm->output("centralMoments").set(centralMoments->realValues);
        
        distributionShape->algorithm->input("centralMoments").set(centralMoments->realValues);
        distributionShape->algorithm->output("kurtosis").set(distributionShape->realValues[0]);
        distributionShape->algorithm->output("spread").set(distributionShape->realValues[1]);
        distributionShape->algorithm->output("skewness").set(distributionShape->realValues[2]);
        
        logAttackTime->algorithm->input("signal").set(envelope_acummulated->realValues);
        logAttackTime->algorithm->output("logAttackTime").set(logAttackTime->realValues[0]);
        logAttackTime->algorithm->output("attackStart").set(logAttackTime->realValues[1]);
        logAttackTime->algorithm->output("attackStop").set(logAttackTime->realValues[2]);
        
        //TODO: Should this also be connected to envelop_accumulated?
        strongDecay->algorithm->input("signal").set(envelope->realValues);
        strongDecay->algorithm->output("strongDecay").set(strongDecay->realValue);
        
        flatnessSFX->algorithm->input("envelope").set(envelope->realValues);
        flatnessSFX->algorithm->output("flatness").set(flatnessSFX->realValue);
        
        maxToTotal->algorithm->input("envelope").set(envelope->realValues);
        maxToTotal->algorithm->output("maxToTotal").set(maxToTotal->realValue);
        
        tcToTotal->algorithm->input("envelope").set(envelope->realValues);
        tcToTotal->algorithm->output("TCToTotal").set(tcToTotal->realValue);
        
        derivativeSFX->algorithm->input("envelope").set(envelope_acummulated->realValues);
        derivativeSFX->algorithm->output("derAvAfterMax").set(derivativeSFX->realValues[0]);
        derivativeSFX->algorithm->output("maxDerBeforeMax").set(derivativeSFX->realValues[1]);
        
        
        
        
        ///*************************************
        ///*************************************
        
        //Energy
        energy->algorithm->input("array").set(dcRemoval->realValues);
        energy->algorithm->output("energy").set(energy->realValue);
       
        
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
    void Network::computeAlgorithms(vector<Real>& signal, vector<Real>& accumulatedSignal){
        _audioSignal = signal;
        _accumulatedAudioSignal = accumulatedSignal;
        
        // MARK: Compute Algorithms
        dcRemoval->compute();
        rms->compute();
        power->compute();
        zeroCrossingRate->compute();
        loudness->compute();
        loudnessVickers->compute();
        silenceRate->compute();
        
        envelope->compute();
        envelope_acummulated->compute();
        decrease->compute();
        centralMoments->compute();
        distributionShape->compute();
        logAttackTime->compute();
        
        flatnessSFX->compute();
        maxToTotal->compute();
        
        derivativeSFX->compute();
        
        if(envelope->realValues[0] != 0.0){
            //the strong decay is not defined for a zero signal
            tcToTotal->compute();
            strongDecay->compute();
        }
        
        ///***
        
        energy->compute();
        
        
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
     
        
        // MARK: Cast Values
        
        for (auto a : algorithms){
            a->castValueToFloat();
        }
        for (auto va: vectorAlgorithms){
            va->castValuesToFloat(false);
        }
        
//        silenceRate->castValuesToFloat(false);
//        spectrum->castValuesToFloat(true);
//        melBands->castValuesToFloat(true);
//        dct->castValuesToFloat(false);
//        hpcp->castValuesToFloat(false);
//        tristimulus->castValuesToFloat(false);

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
 
    float Network::getValue(ofxAAValueType valueType, float smooth, bool normalized){
      
        switch (valueType) {
            case RMS:
                return smooth ? rms->getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) : rms->getValueDbNormalized(DB_MIN, DB_MAX);
            case POWER:
                return smooth ? power->getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) : power->getValueDbNormalized(DB_MIN, DB_MAX);
            case ZERO_CROSSING_RATE:
                return smooth ? zeroCrossingRate->getSmoothedValue(smooth) : zeroCrossingRate->getValue();
            case LOUDNESS:
                return smooth ? loudness->getSmoothedValue(smooth) : loudness->getValue();
            case LOUDNESS_VICKERS:
                return smooth ? loudnessVickers->getSmoothedValueNormalized(smooth, -90, 0) : loudnessVickers->getValueNormalized(-90, 0);
            case SILENCE_RATE_20dB:
                return smooth ? silenceRate->getSmoothedValues(smooth)[0] : silenceRate->getValues()[0];
            case SILENCE_RATE_30dB:
                return smooth ? silenceRate->getSmoothedValues(smooth)[1] : silenceRate->getValues()[1];
            case SILENCE_RATE_60dB:
                return smooth ? silenceRate->getSmoothedValues(smooth)[2] : silenceRate->getValues()[2];
            case DECREASE:
                return smooth ? decrease->getSmoothedValue(smooth) : decrease->getValue();
            case DISTRIBUTION_SHAPE_KURTOSIS:
                return smooth ? distributionShape->getSmoothedValues(smooth)[0] : distributionShape->getValues()[0];
            case DISTRIBUTION_SHAPE_SPREAD:
                return smooth ? distributionShape->getSmoothedValues(smooth)[1] : distributionShape->getValues()[1];
            case DISTRIBUTION_SHAPE_SKEWNESS:
                return smooth ? distributionShape->getSmoothedValues(smooth)[2] : distributionShape->getValues()[2];
            case LOG_ATTACK_TIME:
                return smooth ? logAttackTime->getSmoothedValues(smooth)[0] : logAttackTime->getValues()[0];
            case STRONG_DECAY:
                if (normalized){
                    return smooth ? strongDecay->getSmoothedValueNormalized(smooth) : strongDecay->getValueNormalized();
                }else{
                    return smooth ? strongDecay->getSmoothedValue(smooth) : strongDecay->getValue();
                }
            case FLATNESS_SFX:
                return smooth ? flatnessSFX->getSmoothedValue(smooth) : flatnessSFX->getValue();
            case MAX_TO_TOTAL:
                return smooth ? maxToTotal->getSmoothedValue(smooth) : maxToTotal->getValue();
            case TC_TO_TOTAL:
                return smooth ? tcToTotal->getSmoothedValue(smooth) : tcToTotal->getValue();
            case DERIVATIVE_SFX_AFTER_MAX:
                 return smooth ? derivativeSFX->getSmoothedValues(smooth)[0] : derivativeSFX->getValues()[0];
            case DERIVATIVE_SFX_BEFORE_MAX:
                return smooth ? derivativeSFX->getSmoothedValues(smooth)[1] : derivativeSFX->getValues()[1];
                
                
            default:
                return 3737.3737;
                break;
        }
        
        
    }
    
    vector<float>& Network::getValues(ofxaa::AlgorithmType algorithmType, float smooth){
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
    ofxAABaseAlgorithm* Network::algorithm(ofxaa::AlgorithmType type){
        for (int i=0; i<algorithms.size(); i++){
            if (type == algorithms[i]->getType()){
                return algorithms[i];
            }
        }
        ofLogError()<<"ofxAudioAnalyzerUnit: algorithm type is NOT a Base Algorithm.";
    }
    
    ofxAAOneVectorOutputAlgorithm* Network::vectorAlgorithm(ofxaa::AlgorithmType type){
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


/*
 
 switch (algorithmType) {
 
 case RMS:
 r = smooth ?
 rms.getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX):
 rms.getValueDbNormalized(DB_MIN, DB_MAX);
 break;
 
 case ENERGY:
 r = smooth ?
 energy.getSmoothedValueNormalized(smooth):
 energy.getValueNormalized();
 break;
 
 case POWER:
 r = smooth ?
 power.getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX):
 power.getValueDbNormalized(DB_MIN, DB_MAX);
 break;
 
 case PITCH_FREQ:
 if (normalized){
 r = smooth ?
 pitchDetect.getSmoothedPitchValueNormalized(smooth):
 pitchDetect.getPitchValueNormalized();
 }else{
 r = smooth ?
 pitchDetect.getSmoothedPitchValue(smooth):
 pitchDetect.getPitchValue();
 }
 break;
 
 case PITCH_CONFIDENCE:
 r = smooth ?
 pitchDetect.getSmoothedConfidenceValue(smooth):
 pitchDetect.getConfidenceValue();
 break;
 
 case PITCH_SALIENCE:
 r = smooth ?
 pitchSalience.getSmoothedValue(smooth):
 pitchSalience.getValue();
 break;
 
 case INHARMONICITY:
 r =  smooth ?
 inharmonicity.getSmoothedValue(smooth):
 inharmonicity.getValue();
 break;
 
 case HFC:
 if (normalized){
 r = smooth ?
 hfc.getSmoothedValueNormalized(smooth):
 hfc.getValueNormalized();
 }else{
 r = smooth ?
 hfc.getSmoothedValue(smooth):
 hfc.getValue();
 }
 break;
 
 case SPECTRAL_COMPLEXITY:
 if (normalized){
 r = smooth ?
 spectralComplex.getSmoothedValueNormalized(smooth):
 spectralComplex.getValueNormalized();
 }else{
 r = smooth ?
 spectralComplex.getSmoothedValue(smooth):
 spectralComplex.getValue();
 }
 break;
 
 case CENTROID:
 if (normalized){
 r = smooth ?
 centroid.getSmoothedValueNormalized(smooth):
 centroid.getValueNormalized();
 }else{
 r = smooth ?
 centroid.getSmoothedValue(smooth):
 centroid.getValue();
 }
 break;
 
 case DISSONANCE:
 r = smooth ?
 dissonance.getSmoothedValue(smooth):
 dissonance.getValue();
 break;
 
 case ROLL_OFF:
 if (normalized){
 r = smooth ?
 rollOff.getSmoothedValueNormalized(smooth):
 rollOff.getValueNormalized();
 }else{
 r = smooth ?
 rollOff.getSmoothedValue(smooth):
 rollOff.getValue();
 }
 break;
 case ODD_TO_EVEN:
 if (normalized){
 r = smooth ?
 oddToEven.getSmoothedValueNormalized(smooth):
 oddToEven.getValueNormalized();
 }else{
 r = smooth ?
 oddToEven.getSmoothedValue(smooth):
 oddToEven.getValue();
 //limit value, because this algorithm reaches huge values (eg: 3.40282e+38)
 r = ofClamp(r, 0.0, oddToEven.getMaxEstimatedValue());
 }
 break;
 case STRONG_PEAK:
 if (normalized){
 r = smooth ?
 strongPeak.getSmoothedValueNormalized(smooth):
 strongPeak.getValueNormalized();
 }else{
 r = smooth ?
 strongPeak.getSmoothedValue(smooth):
 strongPeak.getValue();
 }
 break;

 
 */
