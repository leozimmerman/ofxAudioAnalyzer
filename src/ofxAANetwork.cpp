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
#include "ofxAAConfigurations.h"
#include "ofxAAFactory.h"

#define LOUDNESS_MAX_VALUE 100.0
#define DYN_COMP_MAX_VALUE 50.0
#define STRONG_DECAY_MAX_VALUE 120.0
#define FLATNESS_SFX_MAX_VALUE 60.0

#define CREST_MAX_VALUE 50.0
#define ENERGY_MAX_VALUE 1.50
#define ENTROPY_MAX_VALUE 10.0

#define KURTOSIS_MIN_VALUE -100
#define KURTOSIS_MAX_VALUE 1000

#define SPREAD_MIN_VALUE 0.0
#define SPREAD_MAX_VALUE 0.2

#define SKEWNESS_MIN_VALUE -2.0
#define SKEWNESS_MAX_VALUE 25.0

#define SPECTRAL_COMPLEXITY_MAX_VALUE 75.0

#define HFC_MAX_VALUE 8000.0
#define ODD_TO_EVEN_MAX_VALUE 10.0
#define STRONG_PEAK_MAX_VALUE 150
#define PITCH_YIN_FREQ_MAX_VALUE 4186.0 //C8

#define GFCC_MAX_VALUE 36000

#define HPCP_SIZE 12

//TODO: Remove deprecated mfcc ?

namespace ofxaa {
    
    Network::Network(int sr, int bufferSize){
        _framesize = bufferSize;
        _samplerate = sr;
        
        _audioSignal.resize(bufferSize);
        _accumulatedAudioSignal.resize(bufferSize * ACCUMULATED_SIGNAL_MULTIPLIER, 0.0);
        
        createAlgorithms();
        connectAlgorithms();
    }
    
    Network::~Network(){
        deleteAlgorithms();
    }
    
    void Network::deleteAlgorithms(){
        for (auto a : algorithms){
            a->deleteAlgorithm();
            delete a;
        }
    }
    
    //MARK: - CREATE ALGORITHMS
    void Network::createAlgorithms(){
        int sr = _samplerate;
        int fs = _framesize;
        
        vector<float> distributionShapeMinValues { KURTOSIS_MIN_VALUE, SPREAD_MIN_VALUE, SKEWNESS_MIN_VALUE };
        vector<float> distributionShapeMaxValues { KURTOSIS_MAX_VALUE, SPREAD_MAX_VALUE, SKEWNESS_MAX_VALUE };
        
        dcRemoval = new ofxAAOneVectorOutputAlgorithm(DCRemoval, sr, fs);
        algorithms.push_back(dcRemoval);
        
        windowing = new ofxAAOneVectorOutputAlgorithm(Windowing, sr, fs);
        algorithms.push_back(windowing);
        
        spectrum = new ofxAAOneVectorOutputAlgorithm(Spectrum, sr, fs, (fs/2)+1);
        spectrum->hasLogarithmicValues = true;
        algorithms.push_back(spectrum);
        
        //MARK: TEMPORAL
        rms = new ofxAASingleOutputAlgorithm(Rms, sr, fs);
        rms->hasLogarithmicValues = true;
        algorithms.push_back(rms);
        
        power = new ofxAASingleOutputAlgorithm(InstantPower, sr, fs);
        power->hasLogarithmicValues = true;
        algorithms.push_back(power);
        
        zeroCrossingRate = new ofxAASingleOutputAlgorithm(ZeroCrossingRate, sr, fs);
        zeroCrossingRate->isNormalizedByDefault = true;
        algorithms.push_back(zeroCrossingRate);
        
        loudness = new ofxAASingleOutputAlgorithm(Loudness, sr, fs);
        loudness->maxEstimatedValue = LOUDNESS_MAX_VALUE;
        algorithms.push_back(loudness);
        
        /*
         Only works at 44100 samplerate
        loudnessVickers = new ofxAASingleOutputAlgorithm(LoudnessVickers, sr, fs);
        loudnessVickers->hasDbValues = true;
        algorithms.push_back(loudnessVickers);
        */
        
        silenceRate = new ofxAAOneVectorOutputAlgorithm(SilenceRate, sr, fs, 3);
        silenceRate->isNormalizedByDefault = true;
        algorithms.push_back(silenceRate);
        
        dynamicComplexity = new ofxAAOneVectorOutputAlgorithm(DynamicComplexity, sr, fs, 2);
        dynamicComplexity->maxEstimatedValue = DYN_COMP_MAX_VALUE;
//        algorithms.push_back(dynamicComplexity);
        
        //MARK: SFX
        envelope = new ofxAAOneVectorOutputAlgorithm(Envelope, sr, fs);
        algorithms.push_back(envelope);
        
        envelope_acummulated = new ofxAAOneVectorOutputAlgorithm(Envelope, sr, fs);
        algorithms.push_back(envelope_acummulated);
        
        sfx_decrease = new ofxAASingleOutputAlgorithm(Decrease, sr, fs);
        sfx_decrease->hasLogarithmicValues = true;
//        algorithms.push_back(sfx_decrease);
        
        centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, sr, fs);
//        algorithms.push_back(centralMoments);
        
        distributionShape = new ofxAADistributionShapeAlgorithm(sr, fs);
        distributionShape->setMinEstimatedValues(distributionShapeMinValues);
        distributionShape->setMaxEstimatedValues(distributionShapeMaxValues);
//        algorithms.push_back(distributionShape);
        
        logAttackTime = new ofxAAOneVectorOutputAlgorithm(LogAttackTime, sr, fs, 3);
//        algorithms.push_back(logAttackTime);
        
        strongDecay = new ofxAASingleOutputAlgorithm(StrongDecay, sr, fs);
        strongDecay->maxEstimatedValue = STRONG_DECAY_MAX_VALUE;
        algorithms.push_back(strongDecay);
        
        flatnessSFX = new ofxAASingleOutputAlgorithm(FlatnessSFX, sr, fs);
        flatnessSFX->maxEstimatedValue = FLATNESS_SFX_MAX_VALUE;
        algorithms.push_back(flatnessSFX);
        
        maxToTotal = new ofxAASingleOutputAlgorithm(MaxToTotal, sr, fs);
        maxToTotal->isNormalizedByDefault = true;
        algorithms.push_back(maxToTotal);
        
        tcToTotal = new ofxAASingleOutputAlgorithm(TCToTotal, sr, fs);
        tcToTotal->isNormalizedByDefault = true;
        algorithms.push_back(tcToTotal);
        
        derivativeSFX = new ofxAAOneVectorOutputAlgorithm(DerivativeSFX, sr, fs, 2);
//        algorithms.push_back(derivativeSFX);
        
        //MARK: PITCH
        pitchYinFFT = new ofxAAOneVectorOutputAlgorithm(PitchYinFFT, sr, fs, 2);
        pitchYinFFT->setMaxEstimatedValues({PITCH_YIN_FREQ_MAX_VALUE, 1.0});
        algorithms.push_back(pitchYinFFT);
        
        pitchMelodia = new ofxAATwoVectorsOutputAlgorithm(PitchMelodia, sr, fs);
//        algorithms.push_back(pitchMelodia); // NOT WORKING
        
        multiPitchKlapuri = new ofxAAVectorVectorOutputAlgorithm(MultiPitchKlapuri, sr, fs);
        algorithms.push_back(multiPitchKlapuri);
        
        /* Not working: https://github.com/MTG/essentia/issues/835 :
        equalLoudness = new ofxAAOneVectorOutputAlgorithm(EqualLoudness, sr, fs);
        algorithms.push_back(equalLoudness);
        
        multiPitchMelodia = new ofxAAVectorVectorOutputAlgorithm(MultiPitchMelodia, sr, fs);
        algorithms.push_back(multiPitchMelodia);
        */
        
        
        predominantPitchMelodia = new ofxAATwoVectorsOutputAlgorithm(PredominantPitchMelodia, sr, fs);
//        algorithms.push_back(predominantPitchMelodia); // NOT WORKING
        
        
        //MARK: SPECTRAL
        nsgConstantQ = new ofxAANSGConstantQAlgorithm(NSGConstantQ, sr, fs);
        
//        algorithms.push_back(nsgConstantQ); // HIGH CPU CONSUMING ALGORITHM
      
        //MARK: -MelBands
        mfcc = new ofxAATwoVectorsOutputAlgorithm(Mfcc, sr, fs, MELBANDS_NUMBER_BANDS, 13);
        mfcc->hasLogarithmicValues = true;
        algorithms.push_back(mfcc);
        
        melBands = new ofxAAOneVectorOutputAlgorithm(MelBands, sr, fs, MELBANDS_NUMBER_BANDS);
        melBands->hasLogarithmicValues = true;
        algorithms.push_back(melBands);
        
        melBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, sr, fs);
        ofxaa::configureCentralMoments(melBands_centralMoments->algorithm, "pdf", MELBANDS_NUMBER_BANDS-1);
        algorithms.push_back(melBands_centralMoments);
        
        melBands_distributionShape = new ofxAADistributionShapeAlgorithm(sr, fs);
        melBands_distributionShape->setMinEstimatedValues(distributionShapeMinValues);
        melBands_distributionShape->setMaxEstimatedValues(distributionShapeMaxValues);
        algorithms.push_back(melBands_distributionShape);
        
        melBands_flatnessDb = new ofxAASingleOutputAlgorithm(FlatnessDB, sr, fs);
        melBands_flatnessDb->isNormalizedByDefault = true;
        algorithms.push_back(melBands_flatnessDb);
        
        melBands_crest = new ofxAASingleOutputAlgorithm(Crest, sr, fs);
        melBands_crest->maxEstimatedValue = CREST_MAX_VALUE;
        algorithms.push_back(melBands_crest);
        
        //MARK: -ERB Bands
        gfcc = new ofxAATwoVectorsOutputAlgorithm(Gfcc, sr, fs, GFCC_NUMBER_BANDS, 13);
        gfcc->maxEstimatedValue = GFCC_MAX_VALUE ;
        gfcc->hasLogarithmicValues = true;
        algorithms.push_back(gfcc);
        
        erbBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, sr, fs);
        ofxaa::configureCentralMoments(erbBands_centralMoments->algorithm, "pdf", GFCC_NUMBER_BANDS-1);
        algorithms.push_back(erbBands_centralMoments);
        
        erbBands_distributionShape = new ofxAADistributionShapeAlgorithm(sr, fs);
        erbBands_distributionShape->setMinEstimatedValues(distributionShapeMinValues);
        erbBands_distributionShape->setMaxEstimatedValues(distributionShapeMaxValues);
        algorithms.push_back(erbBands_distributionShape);
        
        erbBands_flatnessDb = new ofxAASingleOutputAlgorithm(FlatnessDB, sr, fs);
        erbBands_flatnessDb->isNormalizedByDefault = true;
        algorithms.push_back(erbBands_flatnessDb);
        
        erbBands_crest = new ofxAASingleOutputAlgorithm(Crest, sr, fs);
        erbBands_crest->maxEstimatedValue = CREST_MAX_VALUE;
        algorithms.push_back(erbBands_crest);
        
        //MARK: -BarkBands
        barkBands = new ofxAAOneVectorOutputAlgorithm(BarkBands, sr, fs, BARKBANDS_NUMBER_BANDS);
        barkBands->hasLogarithmicValues = true;
        algorithms.push_back(barkBands);
        
        barkBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, sr, fs);
        ofxaa::configureCentralMoments(barkBands_centralMoments->algorithm, "pdf", BARKBANDS_NUMBER_BANDS-1);
        algorithms.push_back(barkBands_centralMoments);
        
        barkBands_distributionShape = new ofxAADistributionShapeAlgorithm(sr, fs);
        barkBands_distributionShape->setMinEstimatedValues(distributionShapeMinValues);
        barkBands_distributionShape->setMaxEstimatedValues(distributionShapeMaxValues);
        algorithms.push_back(barkBands_distributionShape);
        
        barkBands_flatnessDb = new ofxAASingleOutputAlgorithm(FlatnessDB, sr, fs);
        barkBands_flatnessDb->isNormalizedByDefault = true;
        algorithms.push_back(barkBands_flatnessDb);
        
        barkBands_crest = new ofxAASingleOutputAlgorithm(Crest, sr, fs);
        barkBands_crest->maxEstimatedValue = CREST_MAX_VALUE;
        algorithms.push_back(barkBands_crest);
        
        unaryOperator_square = new ofxAAOneVectorOutputAlgorithm(UnaryOperator, sr, fs);
        algorithms.push_back(unaryOperator_square);
        
        //MARK: -ERB
        ebr_low = new ofxAASingleOutputAlgorithm(EnergyBand, sr, fs);
        ofxaa::configureEnergyBand(ebr_low->algorithm, 20.0, 150.0);
        ebr_low->maxEstimatedValue = ENERGY_MAX_VALUE;
        ebr_low->hasLogarithmicValues = true;
        algorithms.push_back(ebr_low);
        
        ebr_mid_low = new ofxAASingleOutputAlgorithm(EnergyBand, sr, fs);
        ofxaa::configureEnergyBand(ebr_mid_low->algorithm, 150.0, 800.0);
        ebr_mid_low->maxEstimatedValue = ENERGY_MAX_VALUE;
        ebr_mid_low->hasLogarithmicValues = true;
        algorithms.push_back(ebr_mid_low);
        
        ebr_mid_hi = new ofxAASingleOutputAlgorithm(EnergyBand, sr, fs);
        ofxaa::configureEnergyBand(ebr_mid_hi->algorithm, 800.0, 4000.0);
        ebr_mid_hi->maxEstimatedValue = ENERGY_MAX_VALUE;
        ebr_mid_hi->hasLogarithmicValues = true;
        algorithms.push_back(ebr_mid_hi);
        
        ebr_hi = new ofxAASingleOutputAlgorithm(EnergyBand, sr, fs);
        ofxaa::configureEnergyBand(ebr_hi->algorithm, 4.000, 20.000);
        ebr_hi->maxEstimatedValue = ENERGY_MAX_VALUE;
        ebr_hi->hasLogarithmicValues = true;
        algorithms.push_back(ebr_hi);
        
        //MARK: -Spectral Descriptors
        spectral_decrease = new ofxAASingleOutputAlgorithm(Decrease, sr, fs);
        ofxaa::configureDecrease(spectral_decrease->algorithm, sr/2);
        spectral_decrease->hasLogarithmicValues = true;
        algorithms.push_back(spectral_decrease);
        
        spectral_centroid = new ofxAASingleOutputAlgorithm(Centroid, sr, fs);
        ofxaa::configureCentroid(spectral_centroid->algorithm, sr/2);
        spectral_centroid->maxEstimatedValue = sr/4;
        algorithms.push_back(spectral_centroid);
        
        rollOff = new ofxAASingleOutputAlgorithm(RollOff, sr, fs);
        rollOff->maxEstimatedValue = sr/2;
        algorithms.push_back(rollOff);
        
        spectral_entropy = new ofxAASingleOutputAlgorithm(Entropy, sr, fs);
        spectral_entropy->maxEstimatedValue = ENTROPY_MAX_VALUE;
        algorithms.push_back(spectral_entropy);
        
        spectral_energy = new ofxAASingleOutputAlgorithm(Energy, sr, fs);
        spectral_energy->maxEstimatedValue = ENERGY_MAX_VALUE;
        algorithms.push_back(spectral_energy);
        
        hfc = new ofxAASingleOutputAlgorithm(Hfc, sr, fs);
        hfc->maxEstimatedValue = HFC_MAX_VALUE;
        algorithms.push_back(hfc);
        
        spectral_flux = new ofxAASingleOutputAlgorithm(Flux, sr, fs);
        spectral_flux->isNormalizedByDefault = true;
        algorithms.push_back(spectral_flux);
        
        strongPeak = new ofxAASingleOutputAlgorithm(StrongPeak, sr, fs);
        strongPeak->maxEstimatedValue = STRONG_PEAK_MAX_VALUE;
        algorithms.push_back(strongPeak);
        
        spectralComplexity = new ofxAASingleOutputAlgorithm(SpectralComplexity, sr, fs);
        spectralComplexity->maxEstimatedValue = SPECTRAL_COMPLEXITY_MAX_VALUE;
        algorithms.push_back(spectralComplexity);
        
        pitchSalience = new ofxAASingleOutputAlgorithm(PitchSalience, sr, fs);
        pitchSalience->isNormalizedByDefault = true;
        algorithms.push_back(pitchSalience);
        
        spectral_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, sr, fs);
        ofxaa::configureCentralMoments(spectral_centralMoments->algorithm, "pdf", sr/2);
        algorithms.push_back(spectral_centralMoments);
        
        spectral_distributionShape = new ofxAADistributionShapeAlgorithm(sr, fs);
        spectral_distributionShape->setMinEstimatedValues(distributionShapeMinValues);
        spectral_distributionShape->setMaxEstimatedValues(distributionShapeMaxValues);
        algorithms.push_back(spectral_distributionShape);
        
        spectralPeaks = new ofxAATwoVectorsOutputAlgorithm(SpectralPeaks, sr, fs);
        algorithms.push_back(spectralPeaks);
        
        dissonance = new ofxAASingleOutputAlgorithm(Dissonance, sr, fs);
        dissonance->isNormalizedByDefault = true;
        algorithms.push_back(dissonance);
        
        harmonicPeaks = new ofxAATwoVectorsOutputAlgorithm(HarmonicPeaks, sr, fs);
        algorithms.push_back(harmonicPeaks);
        
        inharmonicity = new ofxAASingleOutputAlgorithm(Inharmonicity, sr, fs);
        inharmonicity->isNormalizedByDefault = true;
        algorithms.push_back(inharmonicity);
        
        oddToEven = new ofxAASingleOutputAlgorithm(OddToEven, sr, fs);
        oddToEven->maxEstimatedValue = ODD_TO_EVEN_MAX_VALUE;
        algorithms.push_back(oddToEven);
        
        tristimulus = new ofxAAOneVectorOutputAlgorithm(Tristimulus, sr, fs, 3);
        tristimulus->isNormalizedByDefault = true;
        algorithms.push_back(tristimulus);
        
   
        //MARK: TONAL
        //src: tonalextractor.cpp
        spectralPeaks_hpcp = new ofxAATwoVectorsOutputAlgorithm(SpectralPeaks, sr, fs);
        ofxaa::configureSpectralPeaks(spectralPeaks_hpcp->algorithm, 0.00001, 5000.0, 10000, 40.0, "magnitude");
        algorithms.push_back(spectralPeaks_hpcp);
        
        hpcp = new ofxAAOneVectorOutputAlgorithm(Hpcp, sr, fs, HPCP_SIZE);
        ofxaa::configureHPCP(hpcp->algorithm, true, 500.0, 0, 5000.0, false, 40.0, false, "unitMax", 440, HPCP_SIZE, "squaredCosine", 1.0);
        hpcp->isNormalizedByDefault = true;
        algorithms.push_back(hpcp);
        
        hpcp_entropy = new ofxAASingleOutputAlgorithm(Entropy, sr, fs);
        hpcp_entropy->maxEstimatedValue = ENTROPY_MAX_VALUE;
        algorithms.push_back(hpcp_entropy);
        
        hpcp_crest = new ofxAASingleOutputAlgorithm(Crest, sr, fs);
        hpcp_crest->maxEstimatedValue = CREST_MAX_VALUE;
        algorithms.push_back(hpcp_crest);
        
        chordsDetection = new ofxAATwoTypesVectorOutputAlgorithm(ChordsDetection, sr, fs);
        algorithms.push_back(chordsDetection);
        
        onsets = new ofxAAOnsetsAlgorithm(windowing, sr, fs);
        algorithms.push_back(onsets);
    }
    
    //MARK: - CONNECT ALGORITHMS
    void Network::connectAlgorithms(){
        
        dcRemoval->algorithm->input("signal").set(_audioSignal);
        dcRemoval->algorithm->output("signal").set(dcRemoval->outputValues);
        
        windowing->algorithm->input("frame").set(dcRemoval->outputValues);
        windowing->algorithm->output("frame").set(windowing->outputValues);
        
        spectrum->algorithm->input("frame").set(windowing->outputValues);
        spectrum->algorithm->output("spectrum").set(spectrum->outputValues);
        
        //MARK: TEMPORAL
        rms->algorithm->input("array").set(dcRemoval->outputValues);
        rms->algorithm->output("rms").set(rms->outputValue);
        
        power->algorithm->input("array").set(dcRemoval->outputValues);
        power->algorithm->output("power").set(power->outputValue);
        
        zeroCrossingRate->algorithm->input("signal").set(dcRemoval->outputValues);
        zeroCrossingRate->algorithm->output("zeroCrossingRate").set(zeroCrossingRate->outputValue);
        
        loudness->algorithm->input("signal").set(dcRemoval->outputValues);
        loudness->algorithm->output("loudness").set(loudness->outputValue);
        
        /* Only at 44100 sr
        loudnessVickers->algorithm->input("signal").set(dcRemoval->outputValues);
        loudnessVickers->algorithm->output("loudness").set(loudnessVickers->outputValue);
        */
        silenceRate->algorithm->input("frame").set(dcRemoval->outputValues);
        silenceRate->algorithm->output("threshold_0").set(silenceRate->outputValues[0]);
        silenceRate->algorithm->output("threshold_1").set(silenceRate->outputValues[1]);
        silenceRate->algorithm->output("threshold_2").set(silenceRate->outputValues[2]);
        
        dynamicComplexity->algorithm->input("signal").set(_accumulatedAudioSignal);
        dynamicComplexity->algorithm->output("dynamicComplexity").set(dynamicComplexity->outputValues[0]);
        dynamicComplexity->algorithm->output("loudness").set(dynamicComplexity->outputValues[1]);
        
        //MARK: SFX
        //Essentia source: FreesoundSfxDescriptors.cpp
        envelope->algorithm->input("signal").set(dcRemoval->outputValues);
        envelope->algorithm->output("signal").set(envelope->outputValues);
        
        envelope_acummulated->algorithm->input("signal").set(_accumulatedAudioSignal);
        envelope_acummulated->algorithm->output("signal").set(envelope_acummulated->outputValues);
        
        sfx_decrease->algorithm->input("array").set(envelope_acummulated->outputValues);
        sfx_decrease->algorithm->output("decrease").set(sfx_decrease->outputValue);
        
        centralMoments->algorithm->input("array").set(envelope_acummulated->outputValues);
        centralMoments->algorithm->output("centralMoments").set(centralMoments->outputValues);
        
        distributionShape->algorithm->input("centralMoments").set(centralMoments->outputValues);
        distributionShape->algorithm->output("kurtosis").set(distributionShape->outputValues[0]);
        distributionShape->algorithm->output("spread").set(distributionShape->outputValues[1]);
        distributionShape->algorithm->output("skewness").set(distributionShape->outputValues[2]);
        
        logAttackTime->algorithm->input("signal").set(envelope_acummulated->outputValues);
        logAttackTime->algorithm->output("logAttackTime").set(logAttackTime->outputValues[0]);
        logAttackTime->algorithm->output("attackStart").set(logAttackTime->outputValues[1]);
        logAttackTime->algorithm->output("attackStop").set(logAttackTime->outputValues[2]);
        
        //TODO: Should this also be connected to envelop_accumulated?
        strongDecay->algorithm->input("signal").set(envelope_acummulated->outputValues);
        strongDecay->algorithm->output("strongDecay").set(strongDecay->outputValue);
        
        flatnessSFX->algorithm->input("envelope").set(envelope_acummulated->outputValues);
        flatnessSFX->algorithm->output("flatness").set(flatnessSFX->outputValue);
        
        maxToTotal->algorithm->input("envelope").set(envelope_acummulated->outputValues);
        maxToTotal->algorithm->output("maxToTotal").set(maxToTotal->outputValue);
        
        tcToTotal->algorithm->input("envelope").set(envelope_acummulated->outputValues);
        tcToTotal->algorithm->output("TCToTotal").set(tcToTotal->outputValue);
        
        derivativeSFX->algorithm->input("envelope").set(envelope_acummulated->outputValues);
        derivativeSFX->algorithm->output("derAvAfterMax").set(derivativeSFX->outputValues[0]);
        derivativeSFX->algorithm->output("maxDerBeforeMax").set(derivativeSFX->outputValues[1]);
       
     
        //MARK: PITCH
        //source: standard_pitchdemo.cpp
        pitchYinFFT->algorithm->input("spectrum").set(spectrum->outputValues);
        pitchYinFFT->algorithm->output("pitch").set(pitchYinFFT->outputValues[0]);
        pitchYinFFT->algorithm->output("pitchConfidence").set(pitchYinFFT->outputValues[1]);
        
        pitchMelodia->algorithm->input("signal").set(dcRemoval->outputValues);
        pitchMelodia->algorithm->output("pitch").set(pitchMelodia->outputValues);
        pitchMelodia->algorithm->output("pitchConfidence").set(pitchMelodia->outputValues_2);
        
        multiPitchKlapuri->algorithm->input("signal").set(dcRemoval->outputValues);
        multiPitchKlapuri->algorithm->output("pitch").set(multiPitchKlapuri->vectorRealValues);
        
        /* Not working:  https://github.com/MTG/essentia/issues/835
        equalLoudness->algorithm->input("signal").set(dcRemoval->realValues);
        equalLoudness->algorithm->output("signal").set(equalLoudness->realValues);
        
        multiPitchMelodia->algorithm->input("signal").set(equalLoudness->realValues);
        multiPitchMelodia->algorithm->output("pitch").set(multiPitchMelodia->vectorRealValues);
        */
        
        predominantPitchMelodia->algorithm->input("signal").set(dcRemoval->outputValues);
        predominantPitchMelodia->algorithm->output("pitch").set(predominantPitchMelodia->outputValues);
        predominantPitchMelodia->algorithm->output("pitchConfidence").set(predominantPitchMelodia->outputValues_2);
        
        //MARK: SPECTRAL
        //Essentia source: FreesoundLowLevelDescriptors.cpp
        nsgConstantQ->algorithm->input("frame").set(windowing->outputValues);
        nsgConstantQ->algorithm->output("constantq").set(nsgConstantQ->constantq);
        nsgConstantQ->algorithm->output("constantqdc").set(nsgConstantQ->constantqdc);
        nsgConstantQ->algorithm->output("constantqnf").set(nsgConstantQ->constantqnf);
        //MARK: -MelBands
        mfcc->algorithm->input("spectrum").set(spectrum->outputValues);
        mfcc->algorithm->output("bands").set(mfcc->outputValues);
        mfcc->algorithm->output("mfcc").set(mfcc->outputValues_2);
        
        melBands->algorithm->input("spectrum").set(spectrum->outputValues);
        melBands->algorithm->output("bands").set(melBands->outputValues);
        
        melBands_centralMoments->algorithm->input("array").set(melBands->outputValues);
        melBands_centralMoments->algorithm->output("centralMoments").set(melBands_centralMoments->outputValues);
        
        melBands_distributionShape->algorithm->input("centralMoments").set(melBands_centralMoments->outputValues);
        melBands_distributionShape->algorithm->output("kurtosis").set(melBands_distributionShape->outputValues[0]);
        melBands_distributionShape->algorithm->output("spread").set(melBands_distributionShape->outputValues[1]);
        melBands_distributionShape->algorithm->output("skewness").set(melBands_distributionShape->outputValues[2]);
        
        melBands_flatnessDb->algorithm->input("array").set(melBands->outputValues);
        melBands_flatnessDb->algorithm->output("flatnessDB").set(melBands_flatnessDb->outputValue);
        
        melBands_crest->algorithm->input("array").set(melBands->outputValues);
        melBands_crest->algorithm->output("crest").set(melBands_crest->outputValue);
        
        //MARK: -ERB Bands
        gfcc->algorithm->input("spectrum").set(spectrum->outputValues);
        gfcc->algorithm->output("bands").set(gfcc->outputValues);
        gfcc->algorithm->output("gfcc").set(gfcc->outputValues_2);
        
        erbBands_centralMoments->algorithm->input("array").set(gfcc->outputValues);
        erbBands_centralMoments->algorithm->output("centralMoments").set(erbBands_centralMoments->outputValues);
        
        erbBands_distributionShape->algorithm->input("centralMoments").set(erbBands_centralMoments->outputValues);
        erbBands_distributionShape->algorithm->output("kurtosis").set(erbBands_distributionShape->outputValues[0]);
        erbBands_distributionShape->algorithm->output("spread").set(erbBands_distributionShape->outputValues[1]);
        erbBands_distributionShape->algorithm->output("skewness").set(erbBands_distributionShape->outputValues[2]);
        
        erbBands_flatnessDb->algorithm->input("array").set(gfcc->outputValues);
        erbBands_flatnessDb->algorithm->output("flatnessDB").set(erbBands_flatnessDb->outputValue);
        
        erbBands_crest->algorithm->input("array").set(gfcc->outputValues);
        erbBands_crest->algorithm->output("crest").set(erbBands_crest->outputValue);
        
        //MARK: -BarkBands
        barkBands->algorithm->input("spectrum").set(spectrum->outputValues);
        barkBands->algorithm->output("bands").set(barkBands->outputValues);
        
        barkBands_centralMoments->algorithm->input("array").set(barkBands->outputValues);
        barkBands_centralMoments->algorithm->output("centralMoments").set(barkBands_centralMoments->outputValues);
        
        barkBands_distributionShape->algorithm->input("centralMoments").set(barkBands_centralMoments->outputValues);
        barkBands_distributionShape->algorithm->output("kurtosis").set(barkBands_distributionShape->outputValues[0]);
        barkBands_distributionShape->algorithm->output("spread").set(barkBands_distributionShape->outputValues[1]);
        barkBands_distributionShape->algorithm->output("skewness").set(barkBands_distributionShape->outputValues[2]);
        
        barkBands_flatnessDb->algorithm->input("array").set(barkBands->outputValues);
        barkBands_flatnessDb->algorithm->output("flatnessDB").set(barkBands_flatnessDb->outputValue);
        
        barkBands_crest->algorithm->input("array").set(barkBands->outputValues);
        barkBands_crest->algorithm->output("crest").set(barkBands_crest->outputValue);
        
        //MARK: -ERB
        ebr_low->algorithm->input("spectrum").set(spectrum->outputValues);
        ebr_low->algorithm->output("energyBand").set(ebr_low->outputValue);
        
        ebr_mid_low->algorithm->input("spectrum").set(spectrum->outputValues);
        ebr_mid_low->algorithm->output("energyBand").set(ebr_mid_low->outputValue);
        
        ebr_mid_hi->algorithm->input("spectrum").set(spectrum->outputValues);
        ebr_mid_hi->algorithm->output("energyBand").set(ebr_mid_hi->outputValue);
        
        ebr_hi->algorithm->input("spectrum").set(spectrum->outputValues);
        ebr_hi->algorithm->output("energyBand").set(ebr_hi->outputValue);
        
        //MARK: -Spectral Descriptors
        unaryOperator_square->algorithm->input("array").set(spectrum->outputValues);
        unaryOperator_square->algorithm->output("array").set(unaryOperator_square->outputValues);
        
        spectral_decrease->algorithm->input("array").set(unaryOperator_square->outputValues);
        spectral_decrease->algorithm->output("decrease").set(spectral_decrease->outputValue);
        
        spectral_centroid->algorithm->input("array").set(unaryOperator_square->outputValues);
        spectral_centroid->algorithm->output("centroid").set(spectral_centroid->outputValue);
        
        rollOff->algorithm->input("spectrum").set(spectrum->outputValues);
        rollOff->algorithm->output("rollOff").set(rollOff->outputValue);
        
        spectral_entropy->algorithm->input("array").set(spectrum->outputValues);
        spectral_entropy->algorithm->output("entropy").set(spectral_entropy->outputValue);
        
        spectral_energy->algorithm->input("array").set(spectrum->outputValues);
        spectral_energy->algorithm->output("energy").set(spectral_energy->outputValue);
        
        hfc->algorithm->input("spectrum").set(spectrum->outputValues);
        hfc->algorithm->output("hfc").set(hfc->outputValue);
        
        spectral_flux->algorithm->input("spectrum").set(spectrum->outputValues);
        spectral_flux->algorithm->output("flux").set(spectral_flux->outputValue);
        
        strongPeak->algorithm->input("spectrum").set(spectrum->outputValues);
        strongPeak->algorithm->output("strongPeak").set(strongPeak->outputValue);
        
        spectralComplexity->algorithm->input("spectrum").set(spectrum->outputValues);
        spectralComplexity->algorithm->output("spectralComplexity").set(spectralComplexity->outputValue);
        
        pitchSalience->algorithm->input("spectrum").set(spectrum->outputValues);
        pitchSalience->algorithm->output("pitchSalience").set(pitchSalience->outputValue);
        
        spectral_centralMoments->algorithm->input("array").set(spectrum->outputValues);
        spectral_centralMoments->algorithm->output("centralMoments").set(spectral_centralMoments->outputValues);
        
        spectral_distributionShape->algorithm->input("centralMoments").set(spectral_centralMoments->outputValues);
        spectral_distributionShape->algorithm->output("kurtosis").set(spectral_distributionShape->outputValues[0]);
        spectral_distributionShape->algorithm->output("spread").set(spectral_distributionShape->outputValues[1]);
        spectral_distributionShape->algorithm->output("skewness").set(spectral_distributionShape->outputValues[2]);
        
        spectralPeaks->algorithm->input("spectrum").set(spectrum->outputValues);
        spectralPeaks->algorithm->output("frequencies").set(spectralPeaks->outputValues);
        spectralPeaks->algorithm->output("magnitudes").set(spectralPeaks->outputValues_2);
        
        dissonance->algorithm->input("frequencies").set(spectralPeaks->outputValues);
        dissonance->algorithm->input("magnitudes").set(spectralPeaks->outputValues_2);
        dissonance->algorithm->output("dissonance").set(dissonance->outputValue);
        
        harmonicPeaks->algorithm->input("frequencies").set(spectralPeaks->outputValues);
        harmonicPeaks->algorithm->input("magnitudes").set(spectralPeaks->outputValues_2);
        harmonicPeaks->algorithm->input("pitch").set(pitchYinFFT->outputValues[0]);
        harmonicPeaks->algorithm->output("harmonicFrequencies").set(harmonicPeaks->outputValues);
        harmonicPeaks->algorithm->output("harmonicMagnitudes").set(harmonicPeaks->outputValues_2);
        
        inharmonicity->algorithm->input("frequencies").set(harmonicPeaks->outputValues);
        inharmonicity->algorithm->input("magnitudes").set(harmonicPeaks->outputValues_2);
        inharmonicity->algorithm->output("inharmonicity").set(inharmonicity->outputValue);
        
        oddToEven->algorithm->input("frequencies").set(harmonicPeaks->outputValues);
        oddToEven->algorithm->input("magnitudes").set(harmonicPeaks->outputValues_2);
        oddToEven->algorithm->output("oddToEvenHarmonicEnergyRatio").set(oddToEven->outputValue);
        
        tristimulus->algorithm->input("frequencies").set(harmonicPeaks->outputValues);
        tristimulus->algorithm->input("magnitudes").set(harmonicPeaks->outputValues_2);
        tristimulus->algorithm->output("tristimulus").set(tristimulus->outputValues);
        
        //MARK: TONAL
        spectralPeaks_hpcp->algorithm->input("spectrum").set(spectrum->outputValues);
        spectralPeaks_hpcp->algorithm->output("frequencies").set(spectralPeaks_hpcp->outputValues);
        spectralPeaks_hpcp->algorithm->output("magnitudes").set(spectralPeaks_hpcp->outputValues_2);
        
        hpcp->algorithm->input("frequencies").set(spectralPeaks_hpcp->outputValues);
        hpcp->algorithm->input("magnitudes").set(spectralPeaks_hpcp->outputValues_2);
        hpcp->algorithm->output("hpcp").set(hpcp->outputValues);
        
        hpcp_entropy->algorithm->input("array").set(hpcp->outputValues);
        hpcp_entropy->algorithm->output("entropy").set(hpcp_entropy->outputValue);
        
        hpcp_crest->algorithm->input("array").set(hpcp->outputValues);
        hpcp_crest->algorithm->output("crest").set(hpcp_crest->outputValue);
        
        chordsDetection->algorithm->input("pcp").set(hpcp->realValuesAsVec());
        chordsDetection->algorithm->output("chords").set(chordsDetection->stringValues);
        chordsDetection->algorithm->output("strength").set(chordsDetection->outputValues);
        
    }
    //MARK: - COMPUTE
    

    void Network::computeAlgorithms(vector<Real>& signal, vector<Real>& accumulatedSignal){
        
        _audioSignal = signal;
        _accumulatedAudioSignal = accumulatedSignal;
        
//        for (auto algorithm : algorithms){
        int size = algorithms.size();//70
        for (int i=0; i<algorithms.size(); i++){
            if (algorithms[i]->getType() == TCToTotal || algorithms[i]->getType() == StrongDecay){
                if(envelope->outputValues[0] != 0.0){
                    //StrongDecay and TcToTotal are not defined for a zero signal
                    algorithms[i]->compute();
                }
            } else {
                
                algorithms[i]->compute();
                
            }
        }
        
    }
    
    //MARK: - GET VALUES
    float Network::getValue(ofxAAValue value, float smooth, bool normalized){
        switch (value) {
            
            case SILENCE_RATE_20dB:
                return silenceRate->getValueAtIndex(0, smooth, normalized);
            case SILENCE_RATE_30dB:
                return silenceRate->getValueAtIndex(1, smooth, normalized);
            case SILENCE_RATE_60dB:
                return silenceRate->getValueAtIndex(2, smooth, normalized);
           
            case DYNAMIC_COMPLEXITY:
                return dynamicComplexity->getValueAtIndex(0, smooth, normalized);
                
          
          
            case LOG_ATTACK_TIME:
                return logAttackTime->getValueAtIndex(0, smooth, normalized);

            case DERIVATIVE_SFX_AFTER_MAX:
                return derivativeSFX->getValueAtIndex(0, smooth, normalized);
            case DERIVATIVE_SFX_BEFORE_MAX:
                return derivativeSFX->getValueAtIndex(1, smooth, normalized);
            
            case PITCH_YIN_FREQUENCY:
                return pitchYinFFT->getValueAtIndex(0, smooth, normalized);
            case PITCH_YIN_CONFIDENCE:
                return pitchYinFFT->getValueAtIndex(1, smooth, normalized);
           
            case DISTRIBUTION_SHAPE_KURTOSIS:
                return distributionShape->getKurtosisValue(smooth, normalized);
            case DISTRIBUTION_SHAPE_SPREAD:
                return distributionShape->getSpreadValue(smooth, normalized);
            case DISTRIBUTION_SHAPE_SKEWNESS:
                return distributionShape->getSkewnessValue(smooth, normalized);
                
            case MEL_BANDS_KURTOSIS:
                return melBands_distributionShape->getKurtosisValue(smooth, normalized);
            case MEL_BANDS_SPREAD:
                return melBands_distributionShape->getSpreadValue(smooth, normalized);
            case MEL_BANDS_SKEWNESS:
                return melBands_distributionShape->getSkewnessValue(smooth, normalized);
                
            case ERB_BANDS_KURTOSIS:
                return erbBands_distributionShape->getKurtosisValue(smooth, normalized);
            case ERB_BANDS_SPREAD:
                return erbBands_distributionShape->getSpreadValue(smooth, normalized);
            case ERB_BANDS_SKEWNESS:
                 return erbBands_distributionShape->getSkewnessValue(smooth, normalized);
           
            case BARK_BANDS_KURTOSIS:
                return barkBands_distributionShape->getKurtosisValue(smooth, normalized);
            case BARK_BANDS_SPREAD:
                return barkBands_distributionShape->getSpreadValue(smooth, normalized);
            case BARK_BANDS_SKEWNESS:
                return barkBands_distributionShape->getSkewnessValue(smooth, normalized);
           
            case SPECTRAL_KURTOSIS:
                return spectral_distributionShape->getKurtosisValue(smooth, normalized);
            case SPECTRAL_SPREAD:
                return spectral_distributionShape->getSpreadValue(smooth, normalized);
            case SPECTRAL_SKEWNESS:
                return spectral_distributionShape->getSkewnessValue(smooth, normalized);
                
            case ONSETS:
                return onsets->getValue();
                
            case NONE:
                ofLogError()<< "ofxAANetwork: getValue() for NONE value type";
                return 0.0;
                
            default:
                auto singleAlgorithm = dynamic_cast<ofxAASingleOutputAlgorithm*>(getAlgorithmWithType(value));
                return singleAlgorithm->getValue(smooth, normalized);
        }
    }
    
    vector<float>& Network::getValues(ofxAABinsValue value, float smooth, bool normalized){
        static vector<float> r(1, 0.0);
        switch (value){
            case PITCH_MELODIA_FREQUENCIES:
                return pitchMelodia->getValues(smooth, normalized);
            case PITCH_MELODIA_CONFIDENCES:
                return pitchMelodia->getValues2(smooth, normalized);
            case PREDOMINANT_PITCH_MELODIA_FREQUENCIES:
                return predominantPitchMelodia->getValues(smooth, normalized);
            case PREDOMINANT_PITCH_MELODIA_CONFIDENCES:
                return predominantPitchMelodia->getValues2(smooth, normalized);
                
            case NONE_BINS:
                ofLogError()<< "ofxAANetwork: getValues() for NONE_BINS type";
                return r;
                
            default:
                return getAlgorithmWithType(value)->getValues(smooth, normalized);
        }
    }
    //MARK: - 
    ofxAABaseAlgorithm* Network::getAlgorithmWithType(ofxAAValue valueType){
        switch (valueType) {
                //MARK: TEMPORAL
            case RMS:
                return rms;
            case POWER:
                return power;
            case ZERO_CROSSING_RATE:
                return zeroCrossingRate;
            case LOUDNESS:
                return loudness;
            case SILENCE_RATE_20dB:
                return silenceRate;
            case SILENCE_RATE_30dB:
                return silenceRate;
            case SILENCE_RATE_60dB:
                return silenceRate;
                //MARK: SFX
            case DECREASE:
                return sfx_decrease;
            case DYNAMIC_COMPLEXITY:
                return dynamicComplexity;
            case DISTRIBUTION_SHAPE_KURTOSIS:
                return distributionShape;
            case DISTRIBUTION_SHAPE_SPREAD:
                return distributionShape;
            case DISTRIBUTION_SHAPE_SKEWNESS:
                return distributionShape;
            case LOG_ATTACK_TIME:
                return logAttackTime;
            case STRONG_DECAY:
                return strongDecay;
            case FLATNESS_SFX:
                return flatnessSFX;
            case MAX_TO_TOTAL:
                return maxToTotal;
            case TC_TO_TOTAL:
                return tcToTotal;
            case DERIVATIVE_SFX_AFTER_MAX:
                return derivativeSFX;
            case DERIVATIVE_SFX_BEFORE_MAX:
                return derivativeSFX;
                //MARK: PITCH
            case PITCH_YIN_FREQUENCY:
                return pitchYinFFT;
            case PITCH_YIN_CONFIDENCE:
                return pitchYinFFT;
                //MARK: SPECTRAL
            case MEL_BANDS_KURTOSIS:
                return melBands_distributionShape;
            case MEL_BANDS_SPREAD:
                return melBands_distributionShape;
            case MEL_BANDS_SKEWNESS:
                return melBands_distributionShape;
            case MEL_BANDS_FLATNESS_DB:
                return melBands_flatnessDb;
            case MEL_BANDS_CREST:
                return melBands_crest;
                
            case ERB_BANDS_KURTOSIS:
                return erbBands_distributionShape;
            case ERB_BANDS_SPREAD:
                return erbBands_distributionShape;
            case ERB_BANDS_SKEWNESS:
                return erbBands_distributionShape;
            case ERB_BANDS_FLATNESS_DB:
                return erbBands_flatnessDb;
            case ERB_BANDS_CREST:
                return erbBands_crest;
                
            case BARK_BANDS_KURTOSIS:
                return barkBands_distributionShape;
            case BARK_BANDS_SPREAD:
                return barkBands_distributionShape;
            case BARK_BANDS_SKEWNESS:
                return barkBands_distributionShape;
            case BARK_BANDS_FLATNESS_DB:
                return barkBands_flatnessDb;
            case BARK_BANDS_CREST:
                return barkBands_crest;
                
            case ENERGY_BAND_LOW:
                return ebr_low;
            case ENERGY_BAND_MID_LOW:
                return ebr_mid_low;
            case ENERGY_BAND_MID_HI:
                return ebr_mid_hi;
            case ENERGY_BAND_HI:
                return ebr_hi;
                
            case SPECTRAL_KURTOSIS:
                return spectral_distributionShape;
            case SPECTRAL_SPREAD:
                return spectral_distributionShape;
            case SPECTRAL_SKEWNESS:
                return spectral_distributionShape;
            case SPECTRAL_DECREASE:
                return spectral_decrease;
            case SPECTRAL_ROLLOFF:
                return rollOff;
            case SPECTRAL_ENERGY:
                return spectral_energy;
            case SPECTRAL_ENTROPY:
                return spectral_entropy;
            case SPECTRAL_CENTROID:
                return spectral_centroid;
            case SPECTRAL_COMPLEXITY:
                return spectralComplexity;
            case SPECTRAL_FLUX:
                return spectral_flux;
            case DISSONANCE:
                return dissonance;
            case HFC:
                return hfc;
            case PITCH_SALIENCE:
                return pitchSalience;
                
            case INHARMONICITY:
                return inharmonicity;
            case ODD_TO_EVEN:
                return oddToEven;
            case STRONG_PEAK:
                return strongPeak;
                
            case HPCP_CREST:
                return hpcp_crest;
            case HPCP_ENTROPY:
                return hpcp_entropy;
                
            case ONSETS:
                return onsets;
                
            case NONE:
                ofLogError()<< "ofxAANetwork: getValue() for NONE value type";
                return NULL;
        }
    }
    
    ofxAAOneVectorOutputAlgorithm* Network::getAlgorithmWithType(ofxAABinsValue valueType){
        switch (valueType){
            case SPECTRUM:
                return spectrum;
            case MFCC_MEL_BANDS:
                return melBands;
            case GFCC_ERB_BANDS:
                return gfcc;
            case BARK_BANDS:
                return barkBands;
            case TRISTIMULUS:
                return tristimulus;
            case HPCP:
                return hpcp;
                
            case PITCH_MELODIA_FREQUENCIES:
                return pitchMelodia;
            case PITCH_MELODIA_CONFIDENCES:
                return pitchMelodia;
            case PREDOMINANT_PITCH_MELODIA_FREQUENCIES:
                return predominantPitchMelodia;
            case PREDOMINANT_PITCH_MELODIA_CONFIDENCES:
                return predominantPitchMelodia;
                
            case NONE_BINS:
                ofLogError()<< "ofxAANetwork: getValues() for NONE_BINS type.";
                return NULL;
        }
    }
    //----------------------------------------------
    float Network::getMinEstimatedValue(ofxAAValue valueType){
        //TODO: Copy max
        return getAlgorithmWithType(valueType)->minEstimatedValue;
    }
    //----------------------------------------------
    float Network::getMaxEstimatedValue(ofxAAValue valueType){
        switch (valueType) {
            case DISTRIBUTION_SHAPE_KURTOSIS:
                return distributionShape->getMaxKurtosisValue();
            case DISTRIBUTION_SHAPE_SPREAD:
                return distributionShape->getMaxSpreadValue();
            case DISTRIBUTION_SHAPE_SKEWNESS:
                return distributionShape->getMaxSkewnessValue();
                
            case MEL_BANDS_KURTOSIS:
                return melBands_distributionShape->getMaxKurtosisValue();
            case MEL_BANDS_SPREAD:
                return melBands_distributionShape->getMaxSpreadValue();
            case MEL_BANDS_SKEWNESS:
                return melBands_distributionShape->getMaxSkewnessValue();
                
            case ERB_BANDS_KURTOSIS:
                return erbBands_distributionShape->getMaxKurtosisValue();
            case ERB_BANDS_SPREAD:
                return erbBands_distributionShape->getMaxSpreadValue();
            case ERB_BANDS_SKEWNESS:
                return erbBands_distributionShape->getMaxSkewnessValue();
                
            case BARK_BANDS_KURTOSIS:
                return barkBands_distributionShape->getMaxKurtosisValue();
            case BARK_BANDS_SPREAD:
                return barkBands_distributionShape->getMaxSpreadValue();
            case BARK_BANDS_SKEWNESS:
                return barkBands_distributionShape->getMaxSkewnessValue();
                
            case SPECTRAL_KURTOSIS:
                return spectral_distributionShape->getMaxKurtosisValue();
            case SPECTRAL_SPREAD:
                return spectral_distributionShape->getMaxSpreadValue();
            case SPECTRAL_SKEWNESS:
                return spectral_distributionShape->getMaxSkewnessValue();
            case PITCH_YIN_FREQUENCY:
                return pitchYinFFT->getMaxEstimatedValues()[0];
                
            default:
                return getAlgorithmWithType(valueType)->maxEstimatedValue;
    
        }
    }
    //----------------------------------------------
    float Network::getMaxEstimatedValue(ofxAABinsValue valueType){
        return getAlgorithmWithType(valueType)->maxEstimatedValue;
    }
    
    //----------------------------------------------
    float Network::getMinEstimatedValue(ofxAABinsValue valueType){
        return getAlgorithmWithType(valueType)->minEstimatedValue;
    }
    //----------------------------------------------
    void Network::setMaxEstimatedValue(ofxAAValue valueType, float value){
        switch (valueType) {
            case DISTRIBUTION_SHAPE_KURTOSIS:
                distributionShape->setMaxKurtosisValue(value);
            case DISTRIBUTION_SHAPE_SPREAD:
                distributionShape->setMaxSpreadValue(value);
            case DISTRIBUTION_SHAPE_SKEWNESS:
                distributionShape->setMaxSkewnessValue(value);
                
            case MEL_BANDS_KURTOSIS:
                return melBands_distributionShape->setMaxKurtosisValue(value);
            case MEL_BANDS_SPREAD:
                return melBands_distributionShape->setMaxSpreadValue(value);
            case MEL_BANDS_SKEWNESS:
                return melBands_distributionShape->setMaxSkewnessValue(value);
                
            case ERB_BANDS_KURTOSIS:
                return erbBands_distributionShape->setMaxKurtosisValue(value);
            case ERB_BANDS_SPREAD:
                return erbBands_distributionShape->setMaxSpreadValue(value);
            case ERB_BANDS_SKEWNESS:
                return erbBands_distributionShape->setMaxSkewnessValue(value);
                
            case BARK_BANDS_KURTOSIS:
                return barkBands_distributionShape->setMaxKurtosisValue(value);
            case BARK_BANDS_SPREAD:
                return barkBands_distributionShape->setMaxSpreadValue(value);
            case BARK_BANDS_SKEWNESS:
                return barkBands_distributionShape->setMaxSkewnessValue(value);
                
            case SPECTRAL_KURTOSIS:
                return spectral_distributionShape->setMaxKurtosisValue(value);
            case SPECTRAL_SPREAD:
                return spectral_distributionShape->setMaxSpreadValue(value);
            case SPECTRAL_SKEWNESS:
                return spectral_distributionShape->setMaxSkewnessValue(value);
            case PITCH_YIN_FREQUENCY:
                return pitchYinFFT->setMaxEstimatedValues({value, 1.0});
                
            default:
                getAlgorithmWithType(valueType)->maxEstimatedValue = value;
                break;
        }
    }
    //----------------------------------------------
    void Network::setMaxEstimatedValue(ofxAABinsValue valueType, float value){
        getAlgorithmWithType(valueType)->maxEstimatedValue = value;
    }
}
