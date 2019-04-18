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

//TODO: Remove this defines?
#define DEFAULT_SILENCE_RATE_SIZE 3
#define DEFAULT_SPECTRUM_CQ_BINS 24
#define DEFAULT_MELBANDS_BANDS_NUM 24
#define DEFAULT_MFCC_BANDS_NUM 40
#define DEFAULT_MFCC_COEFF_NUM 13
#define DEFAULT_GFCC_ERB_BANDS_NUM 40
#define DEFAULT_GFCC_COEFF 13
#define DEFAULT_BARKBANDS_NUM 27
//#define DEFAULT_DCT_COEFF_NUM 10
#define DEFAULT_PITCH_SALIENCE_FUNC_BIN_RES 10
#define DEFAULT_TRISTIMULUS_BANDS_NUM 3
#define DEFAULT_HPCP_SIZE 36

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
    //MARK: - CREATE ALGORITHMS
    void Network::createAlgorithms(){
        dcRemoval = new ofxAAOneVectorOutputAlgorithm(DCRemoval, samplerate, framesize);
        algorithms.push_back(dcRemoval);
        
        windowing = new ofxAAOneVectorOutputAlgorithm(Windowing, samplerate, framesize);
        algorithms.push_back(windowing);
        
        spectrum = new ofxAAOneVectorOutputAlgorithm(Spectrum, samplerate, framesize, (framesize/2)+1);
        algorithms.push_back(spectrum);
        
        //MARK: TEMPORAL
        rms = new ofxAABaseAlgorithm(Rms, samplerate, framesize);
        algorithms.push_back(rms);
        
        power = new ofxAABaseAlgorithm(InstantPower, samplerate, framesize);
        algorithms.push_back(power);
        
        zeroCrossingRate = new ofxAABaseAlgorithm(ZeroCrossingRate, samplerate, framesize);
        algorithms.push_back(zeroCrossingRate);
        
        loudness = new ofxAABaseAlgorithm(Loudness, samplerate, framesize);
        algorithms.push_back(loudness);
        
        loudnessVickers = new ofxAABaseAlgorithm(LoudnessVickers, samplerate, framesize);
        algorithms.push_back(loudnessVickers);
        
        silenceRate = new ofxAAOneVectorOutputAlgorithm(SilenceRate, samplerate, framesize, DEFAULT_SILENCE_RATE_SIZE);
        algorithms.push_back(silenceRate);
        
        dynamicComplexity = new ofxAAOneVectorOutputAlgorithm(DynamicComplexity, samplerate, framesize, 2);
        algorithms.push_back(dynamicComplexity);
        
        //MARK: SFX
        envelope = new ofxAAOneVectorOutputAlgorithm(Envelope, samplerate, framesize);
        algorithms.push_back(envelope);
        
        envelope_acummulated = new ofxAAOneVectorOutputAlgorithm(Envelope, samplerate, framesize);
        algorithms.push_back(envelope_acummulated);
        
        sfx_decrease = new ofxAABaseAlgorithm(Decrease, samplerate, framesize);
        algorithms.push_back(sfx_decrease);
        
        centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        algorithms.push_back(centralMoments);
        
        distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        algorithms.push_back(distributionShape);
        
        logAttackTime = new ofxAAOneVectorOutputAlgorithm(LogAttackTime, samplerate, framesize, 3);
        algorithms.push_back(logAttackTime);
        
        strongDecay = new ofxAABaseAlgorithm(StrongDecay, samplerate, framesize);
        algorithms.push_back(strongDecay);
        
        flatnessSFX = new ofxAABaseAlgorithm(FlatnessSFX, samplerate, framesize);
        algorithms.push_back(flatnessSFX);
        
        maxToTotal = new ofxAABaseAlgorithm(MaxToTotal, samplerate, framesize);
        algorithms.push_back(maxToTotal);
        
        tcToTotal = new ofxAABaseAlgorithm(TCToTotal, samplerate, framesize);
        algorithms.push_back(tcToTotal);
        
        derivativeSFX = new ofxAAOneVectorOutputAlgorithm(DerivativeSFX, samplerate, framesize, 2);
        algorithms.push_back(derivativeSFX);
        
        //MARK: PITCH
        pitchYinFFT = new ofxAAOneVectorOutputAlgorithm(PitchYinFFT, samplerate, framesize, 2);
        algorithms.push_back(pitchYinFFT);
        
        pitchMelodia = new ofxAATwoVectorsOutputAlgorithm(PitchMelodia, samplerate, framesize);
        algorithms.push_back(pitchMelodia);
        
        multiPitchKlapuri = new ofxAAVectorVectorOutputAlgorithm(MultiPitchKlapuri, samplerate, framesize);
        algorithms.push_back(multiPitchKlapuri);
        
        /* Not working: https://github.com/MTG/essentia/issues/835 :
        equalLoudness = new ofxAAOneVectorOutputAlgorithm(EqualLoudness, samplerate, framesize);
        algorithms.push_back(equalLoudness);
        
        multiPitchMelodia = new ofxAAVectorVectorOutputAlgorithm(MultiPitchMelodia, samplerate, framesize);
        algorithms.push_back(multiPitchMelodia);
        */
        
        
        predominantPitchMelodia = new ofxAATwoVectorsOutputAlgorithm(PredominantPitchMelodia, samplerate, framesize);
        algorithms.push_back(predominantPitchMelodia);
        
        
        //MARK: SPECTRAL
        nsgConstantQ = new ofxAANSGConstantQAlgorithm(NSGConstantQ, samplerate, framesize);
        algorithms.push_back(nsgConstantQ);
      
        //MARK: -MelBands
        mfcc = new ofxAATwoVectorsOutputAlgorithm(Mfcc, samplerate, framesize, DEFAULT_MFCC_BANDS_NUM, DEFAULT_MFCC_COEFF_NUM);
        algorithms.push_back(mfcc);
        
        melBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        ofxaa::configureCentralMoments(melBands_centralMoments->algorithm, "pdf", DEFAULT_MFCC_BANDS_NUM-1);
        algorithms.push_back(melBands_centralMoments);
        
        melBands_distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        algorithms.push_back(melBands_distributionShape);
        
        melBands_flatnessDb = new ofxAABaseAlgorithm(FlatnessDB, samplerate, framesize);
        algorithms.push_back(melBands_flatnessDb);
        
        melBands_crest = new ofxAABaseAlgorithm(Crest, samplerate, framesize);
        algorithms.push_back(melBands_crest);
        
        //MARK: -ERB Bands
        gfcc = new ofxAATwoVectorsOutputAlgorithm(Gfcc, samplerate, framesize, DEFAULT_GFCC_ERB_BANDS_NUM, DEFAULT_GFCC_COEFF);
        algorithms.push_back(gfcc);
        
        erbBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        ofxaa::configureCentralMoments(erbBands_centralMoments->algorithm, "pdf", DEFAULT_GFCC_ERB_BANDS_NUM-1);
        algorithms.push_back(erbBands_centralMoments);
        
        erbBands_distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        algorithms.push_back(erbBands_distributionShape);
        
        erbBands_flatnessDb = new ofxAABaseAlgorithm(FlatnessDB, samplerate, framesize);
        algorithms.push_back(erbBands_flatnessDb);
        
        erbBands_crest = new ofxAABaseAlgorithm(Crest, samplerate, framesize);
        algorithms.push_back(erbBands_crest);
        
        //MARK: -BarkBands
        barkBands = new ofxAAOneVectorOutputAlgorithm(BarkBands, samplerate, framesize, DEFAULT_BARKBANDS_NUM);
        algorithms.push_back(barkBands);
        
        barkBands_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        ofxaa::configureCentralMoments(barkBands_centralMoments->algorithm, "pdf", DEFAULT_BARKBANDS_NUM-1);
        algorithms.push_back(barkBands_centralMoments);
        
        barkBands_distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        algorithms.push_back(barkBands_distributionShape);
        
        barkBands_flatnessDb = new ofxAABaseAlgorithm(FlatnessDB, samplerate, framesize);
        algorithms.push_back(barkBands_flatnessDb);
        
        barkBands_crest = new ofxAABaseAlgorithm(Crest, samplerate, framesize);
        algorithms.push_back(barkBands_crest);
        
        unaryOperator_square = new ofxAAOneVectorOutputAlgorithm(UnaryOperator, samplerate, framesize);
        algorithms.push_back(unaryOperator_square);
        
        //MARK: -ERB
        ebr_low = new ofxAABaseAlgorithm(EnergyBand, samplerate, framesize);
        ofxaa::configureEnergyBand(ebr_low->algorithm, 20.0, 150.0);
        algorithms.push_back(ebr_low);
        
        ebr_mid_low = new ofxAABaseAlgorithm(EnergyBand, samplerate, framesize);
        ofxaa::configureEnergyBand(ebr_mid_low->algorithm, 150.0, 800.0);
        algorithms.push_back(ebr_mid_low);
        
        ebr_mid_hi = new ofxAABaseAlgorithm(EnergyBand, samplerate, framesize);
        ofxaa::configureEnergyBand(ebr_mid_hi->algorithm, 800.0, 4000.0);
        algorithms.push_back(ebr_mid_hi);
        
        ebr_hi = new ofxAABaseAlgorithm(EnergyBand, samplerate, framesize);
        ofxaa::configureEnergyBand(ebr_hi->algorithm, 4.000, 20.000);
        algorithms.push_back(ebr_hi);
        
        //MARK: -Spectral Descriptors
        spectral_decrease = new ofxAABaseAlgorithm(Decrease, samplerate, framesize);
        ofxaa::configureDecrease(spectral_decrease->algorithm, samplerate/2);
        algorithms.push_back(spectral_decrease);
        
        spectral_centroid = new ofxAAOneVectorOutputAlgorithm(Centroid, samplerate, framesize);
        ofxaa::configureCentroid(spectral_centroid->algorithm, samplerate/2);
        algorithms.push_back(spectral_centroid);
        
        rollOff = new ofxAABaseAlgorithm(RollOff, samplerate, framesize);
        algorithms.push_back(rollOff);
        
        spectral_entropy = new ofxAABaseAlgorithm(Entropy, samplerate, framesize);
        algorithms.push_back(spectral_entropy);
        
        spectral_energy = new ofxAABaseAlgorithm(Energy, samplerate, framesize);
        algorithms.push_back(spectral_energy);
        
        hfc = new ofxAABaseAlgorithm(Hfc, samplerate, framesize);
        algorithms.push_back(hfc);
        
        spectral_flux = new ofxAABaseAlgorithm(Flux, samplerate, framesize);
        algorithms.push_back(spectral_flux);
        
        strongPeak = new ofxAABaseAlgorithm(StrongPeak, samplerate, framesize);
        algorithms.push_back(strongPeak);
        
        spectralComplexity = new ofxAABaseAlgorithm(SpectralComplexity, samplerate, framesize);
        algorithms.push_back(spectralComplexity);
        
        pitchSalience = new ofxAABaseAlgorithm(PitchSalience, samplerate, framesize);
        algorithms.push_back(pitchSalience);
        
        spectral_centralMoments = new ofxAAOneVectorOutputAlgorithm(CentralMoments, samplerate, framesize);
        ofxaa::configureCentralMoments(spectral_centralMoments->algorithm, "pdf", samplerate/2);
        algorithms.push_back(spectral_centralMoments);
        
        spectral_distributionShape = new ofxAAOneVectorOutputAlgorithm(DistributionShape, samplerate, framesize, 3);
        algorithms.push_back(spectral_distributionShape);
        
        spectralPeaks = new ofxAATwoVectorsOutputAlgorithm(SpectralPeaks, samplerate, framesize);
        algorithms.push_back(spectralPeaks);
        
        dissonance = new ofxAABaseAlgorithm(Dissonance, samplerate, framesize);
        algorithms.push_back(dissonance);
        
        harmonicPeaks = new ofxAATwoVectorsOutputAlgorithm(HarmonicPeaks, samplerate, framesize);
        algorithms.push_back(harmonicPeaks);
        
        inharmonicity = new ofxAABaseAlgorithm(Inharmonicity, samplerate, framesize);
        algorithms.push_back(inharmonicity);
        
        oddToEven = new ofxAABaseAlgorithm(OddToEven, samplerate, framesize);
        algorithms.push_back(oddToEven);
        
        tristimulus = new ofxAAOneVectorOutputAlgorithm(Tristimulus, samplerate, framesize, DEFAULT_TRISTIMULUS_BANDS_NUM);
        algorithms.push_back(tristimulus);
        
   
        //MARK: TONAL
        //src: tonalextractor.cpp
        spectralPeaks_hpcp = new ofxAATwoVectorsOutputAlgorithm(SpectralPeaks, samplerate, framesize);
        ofxaa::configureSpectralPeaks(spectralPeaks_hpcp->algorithm, 0.00001, 5000.0, 10000, 40.0, "magnitude");
        algorithms.push_back(spectralPeaks_hpcp);
        
        hpcp = new ofxAAOneVectorOutputAlgorithm(Hpcp, samplerate, framesize, DEFAULT_HPCP_SIZE);
        ofxaa::configureHPCP(hpcp->algorithm, true, 500.0, 8, 5000.0, false, 40.0, true, "unitMax", 440, 36, "cosine", 0.5);
        algorithms.push_back(hpcp);
        
        hpcp_entropy = new ofxAABaseAlgorithm(Entropy, samplerate, framesize);
        algorithms.push_back(hpcp_entropy);
        
        hpcp_crest = new ofxAABaseAlgorithm(Crest, samplerate, framesize);
        algorithms.push_back(hpcp_crest);
        
        chordsDetection = new ofxAATwoTypesVectorOutputAlgorithm(ChordsDetection, samplerate, framesize);
        algorithms.push_back(chordsDetection);
        
        onsets = new ofxAAOnsetsAlgorithm(windowing, samplerate, framesize);
        algorithms.push_back(onsets);
        
    }
    
    void Network::setDefaultMaxEstimatedValues(){
        return; ///******************************
        //default values set from testing with white noise.
        ///spectral_energy->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ENERGY);
        hfc->setMaxEstimatedValue(DEFAULT_MAX_VALUE_HFC);
        spectralComplexity->setMaxEstimatedValue(DEFAULT_MAX_VALUE_SPECTRAL_COMPLEXITY);
        rollOff->setMaxEstimatedValue(samplerate/2);
        oddToEven->setMaxEstimatedValue(DEFAULT_MAX_VALUE_ODD_TO_EVEN);
        strongPeak->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_PEAK);
        strongDecay->setMaxEstimatedValue(DEFAULT_MAX_VALUE_STRONG_DECAY);
        ///pitchYinFFT->setMaxEstimatedValue(DEFAULT_MAX_VALUE_PITCH_FREQ);//C8
    }
    
    //MARK: - CONNECT ALGORITHMS
    void Network::connectAlgorithms(){
        
        dcRemoval->algorithm->input("signal").set(_audioSignal);
        dcRemoval->algorithm->output("signal").set(dcRemoval->realValues);
        
        windowing->algorithm->input("frame").set(dcRemoval->realValues);
        windowing->algorithm->output("frame").set(windowing->realValues);
        
        spectrum->algorithm->input("frame").set(windowing->realValues);
        spectrum->algorithm->output("spectrum").set(spectrum->realValues);
        
        //MARK: TEMPORAL
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
        
        dynamicComplexity->algorithm->input("signal").set(_accumulatedAudioSignal);
        dynamicComplexity->algorithm->output("dynamicComplexity").set(dynamicComplexity->realValues[0]);
        dynamicComplexity->algorithm->output("loudness").set(dynamicComplexity->realValues[1]);
        
        //MARK: SFX
        //Essentia source: FreesoundSfxDescriptors.cpp
        envelope->algorithm->input("signal").set(dcRemoval->realValues);
        envelope->algorithm->output("signal").set(envelope->realValues);
        
        envelope_acummulated->algorithm->input("signal").set(_accumulatedAudioSignal);
        envelope_acummulated->algorithm->output("signal").set(envelope_acummulated->realValues);
        
        sfx_decrease->algorithm->input("array").set(envelope_acummulated->realValues);
        sfx_decrease->algorithm->output("decrease").set(sfx_decrease->realValue);
        
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
       
     
        //MARK: PITCH
        //source: standard_pitchdemo.cpp
        pitchYinFFT->algorithm->input("spectrum").set(spectrum->realValues);
        pitchYinFFT->algorithm->output("pitch").set(pitchYinFFT->realValues[0]);
        pitchYinFFT->algorithm->output("pitchConfidence").set(pitchYinFFT->realValues[1]);
        
        pitchMelodia->algorithm->input("signal").set(dcRemoval->realValues);
        pitchMelodia->algorithm->output("pitch").set(pitchMelodia->realValues);
        pitchMelodia->algorithm->output("pitchConfidence").set(pitchMelodia->realValues_2);
        
        multiPitchKlapuri->algorithm->input("signal").set(dcRemoval->realValues);
        multiPitchKlapuri->algorithm->output("pitch").set(multiPitchKlapuri->vectorRealValues);
        
        /* Not working:  https://github.com/MTG/essentia/issues/835
        equalLoudness->algorithm->input("signal").set(dcRemoval->realValues);
        equalLoudness->algorithm->output("signal").set(equalLoudness->realValues);
        
        multiPitchMelodia->algorithm->input("signal").set(equalLoudness->realValues);
        multiPitchMelodia->algorithm->output("pitch").set(multiPitchMelodia->vectorRealValues);
        */
        
        predominantPitchMelodia->algorithm->input("signal").set(dcRemoval->realValues);
        predominantPitchMelodia->algorithm->output("pitch").set(predominantPitchMelodia->realValues);
        predominantPitchMelodia->algorithm->output("pitchConfidence").set(predominantPitchMelodia->realValues_2);
        
        //MARK: SPECTRAL
        //Essentia source: FreesoundLowLevelDescriptors.cpp
        nsgConstantQ->algorithm->input("frame").set(windowing->realValues);
        nsgConstantQ->algorithm->output("constantq").set(nsgConstantQ->constantq);
        nsgConstantQ->algorithm->output("constantqdc").set(nsgConstantQ->constantqdc);
        nsgConstantQ->algorithm->output("constantqnf").set(nsgConstantQ->constantqnf);
        //MARK: -MelBands
        mfcc->algorithm->input("spectrum").set(spectrum->realValues);
        mfcc->algorithm->output("bands").set(mfcc->realValues);
        mfcc->algorithm->output("mfcc").set(mfcc->realValues_2);
        
        melBands_centralMoments->algorithm->input("array").set(mfcc->realValues);
        melBands_centralMoments->algorithm->output("centralMoments").set(melBands_centralMoments->realValues);
        
        melBands_distributionShape->algorithm->input("centralMoments").set(melBands_centralMoments->realValues);
        melBands_distributionShape->algorithm->output("kurtosis").set(melBands_distributionShape->realValues[0]);
        melBands_distributionShape->algorithm->output("spread").set(melBands_distributionShape->realValues[1]);
        melBands_distributionShape->algorithm->output("skewness").set(melBands_distributionShape->realValues[2]);
        
        melBands_flatnessDb->algorithm->input("array").set(mfcc->realValues);
        melBands_flatnessDb->algorithm->output("flatnessDB").set(melBands_flatnessDb->realValue);
        
        melBands_crest->algorithm->input("array").set(mfcc->realValues);
        melBands_crest->algorithm->output("crest").set(melBands_crest->realValue);
        
        //MARK: -ERB Bands
        gfcc->algorithm->input("spectrum").set(spectrum->realValues);
        gfcc->algorithm->output("bands").set(gfcc->realValues);
        gfcc->algorithm->output("gfcc").set(gfcc->realValues_2);
        
        erbBands_centralMoments->algorithm->input("array").set(gfcc->realValues);
        erbBands_centralMoments->algorithm->output("centralMoments").set(erbBands_centralMoments->realValues);
        
        erbBands_distributionShape->algorithm->input("centralMoments").set(erbBands_centralMoments->realValues);
        erbBands_distributionShape->algorithm->output("kurtosis").set(erbBands_distributionShape->realValues[0]);
        erbBands_distributionShape->algorithm->output("spread").set(erbBands_distributionShape->realValues[1]);
        erbBands_distributionShape->algorithm->output("skewness").set(erbBands_distributionShape->realValues[2]);
        
        erbBands_flatnessDb->algorithm->input("array").set(gfcc->realValues);
        erbBands_flatnessDb->algorithm->output("flatnessDB").set(erbBands_flatnessDb->realValue);
        
        erbBands_crest->algorithm->input("array").set(gfcc->realValues);
        erbBands_crest->algorithm->output("crest").set(erbBands_crest->realValue);
        
        //MARK: -BarkBands
        barkBands->algorithm->input("spectrum").set(spectrum->realValues);
        barkBands->algorithm->output("bands").set(barkBands->realValues);
        
        barkBands_centralMoments->algorithm->input("array").set(barkBands->realValues);
        barkBands_centralMoments->algorithm->output("centralMoments").set(barkBands_centralMoments->realValues);
        
        barkBands_distributionShape->algorithm->input("centralMoments").set(barkBands_centralMoments->realValues);
        barkBands_distributionShape->algorithm->output("kurtosis").set(barkBands_distributionShape->realValues[0]);
        barkBands_distributionShape->algorithm->output("spread").set(barkBands_distributionShape->realValues[1]);
        barkBands_distributionShape->algorithm->output("skewness").set(barkBands_distributionShape->realValues[2]);
        
        barkBands_flatnessDb->algorithm->input("array").set(barkBands->realValues);
        barkBands_flatnessDb->algorithm->output("flatnessDB").set(barkBands_flatnessDb->realValue);
        
        barkBands_crest->algorithm->input("array").set(barkBands->realValues);
        barkBands_crest->algorithm->output("crest").set(barkBands_crest->realValue);
        
        //MARK: -ERB
        ebr_low->algorithm->input("spectrum").set(spectrum->realValues);
        ebr_low->algorithm->output("energyBand").set(ebr_low->realValue);
        
        ebr_mid_low->algorithm->input("spectrum").set(spectrum->realValues);
        ebr_mid_low->algorithm->output("energyBand").set(ebr_mid_low->realValue);
        
        ebr_mid_hi->algorithm->input("spectrum").set(spectrum->realValues);
        ebr_mid_hi->algorithm->output("energyBand").set(ebr_mid_hi->realValue);
        
        ebr_hi->algorithm->input("spectrum").set(spectrum->realValues);
        ebr_hi->algorithm->output("energyBand").set(ebr_hi->realValue);
        
        //MARK: -Spectral Descriptors
        unaryOperator_square->algorithm->input("array").set(spectrum->realValues);
        unaryOperator_square->algorithm->output("array").set(unaryOperator_square->realValues);
        
        spectral_decrease->algorithm->input("array").set(unaryOperator_square->realValues);
        spectral_decrease->algorithm->output("decrease").set(spectral_decrease->realValue);
        
        spectral_centroid->algorithm->input("array").set(unaryOperator_square->realValues);
        spectral_centroid->algorithm->output("centroid").set(spectral_centroid->realValue);
        
        rollOff->algorithm->input("spectrum").set(spectrum->realValues);
        rollOff->algorithm->output("rollOff").set(rollOff->realValue);
        
        spectral_entropy->algorithm->input("array").set(spectrum->realValues);
        spectral_entropy->algorithm->output("entropy").set(spectral_entropy->realValue);
        
        spectral_energy->algorithm->input("array").set(spectrum->realValues);
        spectral_energy->algorithm->output("energy").set(spectral_energy->realValue);
        
        hfc->algorithm->input("spectrum").set(spectrum->realValues);
        hfc->algorithm->output("hfc").set(hfc->realValue);
        
        spectral_flux->algorithm->input("spectrum").set(spectrum->realValues);
        spectral_flux->algorithm->output("flux").set(spectral_flux->realValue);
        
        strongPeak->algorithm->input("spectrum").set(spectrum->realValues);
        strongPeak->algorithm->output("strongPeak").set(strongPeak->realValue);
        
        spectralComplexity->algorithm->input("spectrum").set(spectrum->realValues);
        spectralComplexity->algorithm->output("spectralComplexity").set(spectralComplexity->realValue);
        
        pitchSalience->algorithm->input("spectrum").set(spectrum->realValues);
        pitchSalience->algorithm->output("pitchSalience").set(pitchSalience->realValue);
        
        spectral_centralMoments->algorithm->input("array").set(spectrum->realValues);
        spectral_centralMoments->algorithm->output("centralMoments").set(spectral_centralMoments->realValues);
        
        spectral_distributionShape->algorithm->input("centralMoments").set(spectral_centralMoments->realValues);
        spectral_distributionShape->algorithm->output("kurtosis").set(spectral_distributionShape->realValues[0]);
        spectral_distributionShape->algorithm->output("spread").set(spectral_distributionShape->realValues[1]);
        spectral_distributionShape->algorithm->output("skewness").set(spectral_distributionShape->realValues[2]);
        
        spectralPeaks->algorithm->input("spectrum").set(spectrum->realValues);
        spectralPeaks->algorithm->output("frequencies").set(spectralPeaks->realValues);
        spectralPeaks->algorithm->output("magnitudes").set(spectralPeaks->realValues_2);
        
        dissonance->algorithm->input("frequencies").set(spectralPeaks->realValues);
        dissonance->algorithm->input("magnitudes").set(spectralPeaks->realValues_2);
        dissonance->algorithm->output("dissonance").set(dissonance->realValue);
        
        harmonicPeaks->algorithm->input("frequencies").set(spectralPeaks->realValues);
        harmonicPeaks->algorithm->input("magnitudes").set(spectralPeaks->realValues_2);
        harmonicPeaks->algorithm->input("pitch").set(pitchYinFFT->realValues[0]);
        harmonicPeaks->algorithm->output("harmonicFrequencies").set(harmonicPeaks->realValues);
        harmonicPeaks->algorithm->output("harmonicMagnitudes").set(harmonicPeaks->realValues_2);
        
        inharmonicity->algorithm->input("frequencies").set(harmonicPeaks->realValues);
        inharmonicity->algorithm->input("magnitudes").set(harmonicPeaks->realValues_2);
        inharmonicity->algorithm->output("inharmonicity").set(inharmonicity->realValue);
        
        oddToEven->algorithm->input("frequencies").set(harmonicPeaks->realValues);
        oddToEven->algorithm->input("magnitudes").set(harmonicPeaks->realValues_2);
        oddToEven->algorithm->output("oddToEvenHarmonicEnergyRatio").set(oddToEven->realValue);
        
        tristimulus->algorithm->input("frequencies").set(harmonicPeaks->realValues);
        tristimulus->algorithm->input("magnitudes").set(harmonicPeaks->realValues_2);
        tristimulus->algorithm->output("tristimulus").set(tristimulus->realValues);
        
        //MARK: TONAL
        spectralPeaks_hpcp->algorithm->input("spectrum").set(spectrum->realValues);
        spectralPeaks_hpcp->algorithm->output("frequencies").set(spectralPeaks_hpcp->realValues);
        spectralPeaks_hpcp->algorithm->output("magnitudes").set(spectralPeaks_hpcp->realValues_2);
        
        hpcp->algorithm->input("frequencies").set(spectralPeaks_hpcp->realValues);
        hpcp->algorithm->input("magnitudes").set(spectralPeaks_hpcp->realValues_2);
        hpcp->algorithm->output("hpcp").set(hpcp->realValues);
        
        hpcp_entropy->algorithm->input("array").set(hpcp->realValues);
        hpcp_entropy->algorithm->output("entropy").set(hpcp_entropy->realValue);
        
        hpcp_crest->algorithm->input("array").set(hpcp->realValues);
        hpcp_crest->algorithm->output("crest").set(hpcp_crest->realValue);
        
        chordsDetection->algorithm->input("pcp").set(hpcp->realValuesAsVec());
        chordsDetection->algorithm->output("chords").set(chordsDetection->stringValues);
        chordsDetection->algorithm->output("strength").set(chordsDetection->realValues);
        
    }

    void Network::computeAlgorithms(vector<Real>& signal, vector<Real>& accumulatedSignal){
        
        _audioSignal = signal;
        _accumulatedAudioSignal = accumulatedSignal;
        
        for (auto algorithm : algorithms){
            if (algorithm->getType() == TCToTotal || algorithm->getType() == StrongDecay){
                if(envelope->realValues[0] != 0.0){
                    //StrongDecay and TcToTotal are not defined for a zero signal
                    algorithm->compute();
                }
            } else {
                algorithm->compute();
            }
        }
        
    }
    
    void Network::deleteAlgorithms(){
        for (auto a : algorithms){
            a->deleteAlgorithm();
            delete a;
        }
    }
    
    float Network::getValue(ofxAAValueType valueType, float smooth, bool normalized){
        return 0.37;
        ///************
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
                return smooth ? sfx_decrease->getSmoothedValue(smooth) : sfx_decrease->getValue();
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
        static vector<float>r (1, 0.37);
        return r;
        ///**************************
    }
    
    bool Network::getOnsetValue(){
        return false;///******************
        ///return onsets->getValue();
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
