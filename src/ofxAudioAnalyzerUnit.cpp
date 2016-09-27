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

#include "ofxAudioAnalyzerUnit.h"

#pragma mark - Main funcs
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::setup(int sampleRate, int bufferSize){
    
    #pragma mark -Init variables:
    
    _framesize = bufferSize;
    hopsize = _framesize/2;
    _samplerate = sampleRate;
    zeropadding = 0;
    framerate = (Real) _samplerate / hopsize;
    
    audioBuffer.resize(bufferSize);

    
    #pragma mark -Init algorithms:
    
    onsets.setup(bufferSize);
    
    rms.init();
    energy.init();
    power.init();
    pitchDetect.init();
    pitchSalience.init();
    inharmonicity.init();
    hfc.init();
    centroid.init();
    spectralComplex.init();
    dissonance.init();
    rollOff.init();
    oddToEven.init();
    strongPeak.init();
    strongDecay.init();
    
    spectrum.initAndAssignSize((bufferSize/2)+1, 0.0);
    melBands.initAndAssignSize(MELBANDS_BANDS_NUM,0);
    dct.initAndAssignSize(DCT_COEFF_NUM, 0);
    hpcp.initAndAssignSize(HPCP_SIZE, 0);
    pitchSalienceFunction.initAndAssignSize(PITCH_SALIENCE_FUNC_NUM, 0.0);
    tristimulus.initAndAssignSize(TRISTIMULUS_BANDS_NUM, 0.0);
    
    dcremoval.init();
    window.init();
    fft.init();
    cartesian2polar.init();
    spectralPeaks.init();
    harmonicPeaks.init();
    
    //-:Set Max Estimated Values for Non Normalized Algorithms
    //default values set from testing with white noise.
    energy.setMaxEstimatedValue(100.0);
    pitchDetect.setMaxPitchEstimatedValue(4186.0);//C8
    hfc.setMaxEstimatedValue(2000.0);
    spectralComplex.setMaxEstimatedValue(20.0);
    centroid.setMaxEstimatedValue(11000.0);
    rollOff.setMaxEstimatedValue(sampleRate/2);
    oddToEven.setMaxEstimatedValue(10.0);
    strongPeak.setMaxEstimatedValue(20.0);
    strongDecay.setMaxEstimatedValue(100.0);
    
    

    //------Not very useful...
    pitchSalienceFunctionPeaks.init();
    setActive(PITCH_SALIENCE_FUNC_PEAKS, false);
    multiPitchKlapuri.init();
    setActive(MULTI_PITCHES, false);
    //------------------
    

    essentia::init();

    #pragma mark -Create algorithms
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    rms.algorithm = factory.create("RMS");
    energy.algorithm = factory.create("Energy");
    power.algorithm = factory.create("InstantPower");

    dcremoval.algorithm = factory.create("DCRemoval", "sampleRate", _samplerate);

    window.algorithm = factory.create("Windowing",
                                 "type", "hann",
                                 "zeroPadding", zeropadding);

    fft.algorithm = factory.create("FFT", "size", _framesize);
    cartesian2polar.algorithm = factory.create("CartesianToPolar");

    onsets.onsetHfc.algorithm     = factory.create("OnsetDetection", "method", "hfc", "sampleRate", _samplerate);
    onsets.onsetComplex.algorithm = factory.create("OnsetDetection", "method", "complex", "sampleRate", _samplerate);
    onsets.onsetFlux.algorithm    = factory.create("OnsetDetection", "method", "flux", "sampleRate", _samplerate);

    spectrum.algorithm = factory.create("Spectrum",
                                   "size", _framesize);

    hfc.algorithm = factory.create("HFC", "sampleRate", _samplerate);

    pitchSalience.algorithm = factory.create("PitchSalience", "sampleRate",_samplerate);

    centroid.algorithm = factory.create("Centroid", "range", _samplerate/2);

    spectralComplex.algorithm = factory.create("SpectralComplexity", "sampleRate", _samplerate);
    
    dissonance.algorithm = factory.create("Dissonance");
    
    rollOff.algorithm = factory.create("RollOff",
                                       "sampleRate", _samplerate);
    oddToEven.algorithm = factory.create("OddToEvenHarmonicEnergyRatio");
    strongPeak.algorithm = factory.create("StrongPeak");
    strongDecay.algorithm = factory.create("StrongDecay",
                                           "sampleRate", _samplerate);
    
    spectralPeaks.algorithm = factory.create("SpectralPeaks",
                                "maxPeaks", PEAKS_MAXPEAKS_NUM,
                                "magnitudeThreshold", 0.00001,
                                "minFrequency", PEAKS_MIN_FREQ,
                                "maxFrequency", PEAKS_MAX_FREQ,
                                "orderBy", "frequency");


    melBands.algorithm = factory.create("MelBands",
                                        "inputSize", (_framesize/2)+1,
                                        "sampleRate", _samplerate,
                                        "highFrequencyBound", _samplerate/2,
                                        "numberBands", MELBANDS_BANDS_NUM);
    
    dct.algorithm = factory.create("DCT",
                                   "inputSize", MELBANDS_BANDS_NUM,
                                   "outputSize", DCT_COEFF_NUM);

    inharmonicity.algorithm = factory.create("Inharmonicity");

    pitchDetect.algorithm = factory.create("PitchYinFFT",
                                      "frameSize", _framesize,
                                      "sampleRate", _samplerate);

    harmonicPeaks.algorithm = factory.create("HarmonicPeaks");

    hpcp.algorithm = factory.create("HPCP",
                                   "size", HPCP_SIZE,
                                   "referenceFrequency", 440,
                                   "bandPreset", false,
                                   "minFrequency", HPCP_MIN_FREQ,
                                   "maxFrequency", HPCP_MAX_FREQ,
                                   "weightType", "squaredCosine",
                                   "nonLinear", false,
                                   "windowSize", 4.0/3.0);

    
    pitchSalienceFunction.algorithm = factory.create("PitchSalienceFunction");
    pitchSalienceFunctionPeaks.algorithm = factory.create("PitchSalienceFunctionPeaks");
    
    tristimulus.algorithm = factory.create("Tristimulus");
    
    
    #pragma mark -Connect algorithms
    
    //DCRemoval
    dcremoval.algorithm->input("signal").set(audioBuffer);
    dcremoval.algorithm->output("signal").set(dcremoval.realValues);
    //RMS
    rms.algorithm->input("array").set(dcremoval.realValues);
    rms.algorithm->output("rms").set(rms.realValue);
    //Energy
    energy.algorithm->input("array").set(dcremoval.realValues);
    energy.algorithm->output("energy").set(energy.realValue);
    //Power
    power.algorithm->input("array").set(dcremoval.realValues);
    power.algorithm->output("power").set(power.realValue);
    //Window
    window.algorithm->input("frame").set(dcremoval.realValues);
    window.algorithm->output("frame").set(window.realValues);
    //Onsets
    fft.algorithm->input("frame").set(window.realValues);
    fft.algorithm->output("fft").set(fft.fftRealValues);
    cartesian2polar.algorithm->input("complex").set(fft.fftRealValues);
    cartesian2polar.algorithm->output("magnitude").set(cartesian2polar.magnitudes);
    cartesian2polar.algorithm->output("phase").set(cartesian2polar.phases);
    //-
    onsets.onsetHfc.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsets.onsetHfc.algorithm->input("phase").set(cartesian2polar.phases);
    onsets.onsetHfc.algorithm->output("onsetDetection").set(onsets.onsetHfc.realValue);
    //-
    onsets.onsetComplex.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsets.onsetComplex.algorithm->input("phase").set(cartesian2polar.phases);
    onsets.onsetComplex.algorithm->output("onsetDetection").set(onsets.onsetComplex.realValue);
    //-
    onsets.onsetFlux.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsets.onsetFlux.algorithm->input("phase").set(cartesian2polar.phases);
    onsets.onsetFlux.algorithm->output("onsetDetection").set(onsets.onsetFlux.realValue);
    //Spectrum
    spectrum.algorithm->input("frame").set(window.realValues);
    spectrum.algorithm->output("spectrum").set(spectrum.realValues);
    //HFC
    hfc.algorithm->input("spectrum").set(spectrum.realValues);
    hfc.algorithm->output("hfc").set(hfc.realValue);
    //Pitch Salience
    pitchSalience.algorithm->input("spectrum").set(spectrum.realValues);
    pitchSalience.algorithm->output("pitchSalience").set(pitchSalience.realValue);
    //Centroid
    centroid.algorithm->input("array").set(spectrum.realValues);
    centroid.algorithm->output("centroid").set(centroid.realValue);
    //Spectral Complexity
    spectralComplex.algorithm->input("spectrum").set(spectrum.realValues);
    spectralComplex.algorithm->output("spectralComplexity").set(spectralComplex.realValue);
    //Peak detection
    spectralPeaks.algorithm->input("spectrum").set(spectrum.realValues);
    spectralPeaks.algorithm->output("frequencies").set(spectralPeaks.frequencies);
    spectralPeaks.algorithm->output("magnitudes").set(spectralPeaks.magnitudes);
    //HPCP
    hpcp.algorithm->input("frequencies").set(spectralPeaks.frequencies);
    hpcp.algorithm->input("magnitudes").set(spectralPeaks.magnitudes);
    hpcp.algorithm->output("hpcp").set(hpcp.realValues);
    //MelBands
    melBands.algorithm->input("spectrum").set(spectrum.realValues);
    melBands.algorithm->output("bands").set(melBands.realValues);
    //DCT
    dct.algorithm->input("array").set(melBands.logRealValues);
    dct.algorithm->output("dct").set(dct.realValues);
    //PitchDetection
    pitchDetect.algorithm->input("spectrum").set(spectrum.realValues);
    pitchDetect.algorithm->output("pitch").set(pitchDetect.pitchRealVal);
    pitchDetect.algorithm->output("pitchConfidence").set(pitchDetect.confidenceRealVal);
    //HarmonicPeaks
    harmonicPeaks.algorithm->input("frequencies").set(spectralPeaks.frequencies);
    harmonicPeaks.algorithm->input("magnitudes").set(spectralPeaks.magnitudes);
    harmonicPeaks.algorithm->input("pitch").set(pitchDetect.pitchRealVal);
    harmonicPeaks.algorithm->output("harmonicFrequencies").set(harmonicPeaks.frequencies);
    harmonicPeaks.algorithm->output("harmonicMagnitudes").set(harmonicPeaks.magnitudes);
    //Inharmonicity
    inharmonicity.algorithm->input("frequencies").set(harmonicPeaks.frequencies);
    inharmonicity.algorithm->input("magnitudes").set(harmonicPeaks.magnitudes);
    inharmonicity.algorithm->output("inharmonicity").set(inharmonicity.realValue);
    //Dissonance
    dissonance.algorithm->input("frequencies").set(spectralPeaks.frequencies);
    dissonance.algorithm->input("magnitudes").set(spectralPeaks.magnitudes);
    dissonance.algorithm->output("dissonance").set(dissonance.realValue);
    //Pitch Salience Function
    pitchSalienceFunction.algorithm->input("frequencies").set(spectralPeaks.frequencies);
    pitchSalienceFunction.algorithm->input("magnitudes").set(spectralPeaks.magnitudes);
    pitchSalienceFunction.algorithm->output("salienceFunction").set(pitchSalienceFunction.realValues);
    //Pitch Salience Function Peaks
    pitchSalienceFunctionPeaks.algorithm->input("salienceFunction").set(pitchSalienceFunction.realValues);
    pitchSalienceFunctionPeaks.algorithm->output("salienceBins").set(pitchSalienceFunctionPeaks.realSalienceBins);
    pitchSalienceFunctionPeaks.algorithm->output("salienceValues").set(pitchSalienceFunctionPeaks.realSalienceValues);

    //RollOff
    rollOff.algorithm->input("spectrum").set(spectrum.realValues);
    rollOff.algorithm->output("rollOff").set(rollOff.realValue);
    //StrongPeak
    strongPeak.algorithm->input("spectrum").set(spectrum.realValues);
    strongPeak.algorithm->output("strongPeak").set(strongPeak.realValue);
    //StrongDecay
    strongDecay.algorithm->input("signal").set(dcremoval.realValues);
    strongDecay.algorithm->output("strongDecay").set(strongDecay.realValue);
    //OddToEven
    oddToEven.algorithm->input("frequencies").set(harmonicPeaks.frequencies);
    oddToEven.algorithm->input("magnitudes").set(harmonicPeaks.magnitudes);
    oddToEven.algorithm->output("oddToEvenHarmonicEnergyRatio").set(oddToEven.realValue);
    //Tristimulus
    tristimulus.algorithm->input("frequencies").set(harmonicPeaks.frequencies);
    tristimulus.algorithm->input("magnitudes").set(harmonicPeaks.magnitudes);
    tristimulus.algorithm->output("tristimulus").set(tristimulus.realValues);
    
    //MultiPitch Kalpuri:
    multiPitchKlapuri.setup(&pitchSalienceFunctionPeaks, &spectrum, _samplerate);
    
    //-Shutdown factory:
    factory.shutdown();
    
    
}


//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::analyze(const vector<float> & inBuffer){
    
    if(inBuffer.size() != _framesize){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: buffer requested to analyze size(" <<inBuffer.size()<<")doesnt match the buffer size already set: "<<_framesize;
    }
    
    //Cast of incoming audio buffer to Real
    for (int i=0; i<inBuffer.size();i++){
        audioBuffer[i] = (Real) inBuffer[i];
    }
    
    #pragma mark -Compute Algorithms
    
    dcremoval.compute();
    rms.compute();
    energy.compute();
    power.compute();
    window.compute();
    
    if(onsets.getIsActive()){
        fft.compute();
        cartesian2polar.compute();
        onsets.compute();
    }
    
    //spectrum must always be computed as it is neede for other algorithms
    spectrum.algorithm->compute();
    
    hfc.compute();
    pitchSalience.compute();
    pitchDetect.compute();
    centroid.compute();
    spectralComplex.compute();
    if(melBands.getIsActive()){
        melBands.algorithm->compute();
        if(dct.getIsActive()){
            melBands.updateLogRealValues();
            dct.compute();
        }
    }else{
        dct.setActive(false);//dct needs melBands to be active
    }
    spectralPeaks.compute();
    hpcp.compute();
    
    if (inharmonicity.getIsActive()){
        harmonicPeaks.compute();
        inharmonicity.algorithm->compute();
    }

    dissonance.compute();
    pitchSalienceFunction.compute();
    pitchSalienceFunctionPeaks.compute();
    
    multiPitchKlapuri.compute();
    
    rollOff.compute();
    oddToEven.compute();
    strongPeak.compute();
    
    tristimulus.compute();
    if(dcremoval.realValues[0] != 0.0){
        //the strong decay is not defined for a zero signal
        strongDecay.compute();
    }
    
    
    #pragma mark -Cast results to float
    
    spectrum.castValuesToFloat(true);
    
    rms.castValueToFloat();
    energy.castValueToFloat();
    power.castValueToFloat();
    pitchDetect.castValuesToFloat();
    pitchSalience.castValueToFloat();
    
    melBands.castValuesToFloat(true);
    dct.castValuesToFloat(false);
    hpcp.castValuesToFloat(false);
    hfc.castValueToFloat();
    centroid.castValueToFloat();
    spectralComplex.castValueToFloat();
    inharmonicity.castValueToFloat();
    dissonance.castValueToFloat();
    rollOff.castValueToFloat();
    oddToEven.castValueToFloat();
    strongPeak.castValueToFloat();
    strongDecay.castValueToFloat();
    
    pitchSalienceFunctionPeaks.castValuesToFloat();
    
    tristimulus.castValuesToFloat(false);
    
    onsets.castValuesToFloat();
    onsets.evaluate();
    
 
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::exit(){

    rollOff.deleteAlgorithm();
    oddToEven.deleteAlgorithm();
    strongPeak.deleteAlgorithm();
    strongDecay.deleteAlgorithm();
    tristimulus.deleteAlgorithm();
    
    dcremoval.deleteAlgorithm();
    rms.deleteAlgorithm();
    energy.deleteAlgorithm();
    power.deleteAlgorithm();
    window.deleteAlgorithm();;
    spectrum.deleteAlgorithm();
    spectralPeaks.deleteAlgorithm();;
    pitchDetect.deleteAlgorithm();
    pitchSalience.deleteAlgorithm();
    dissonance.deleteAlgorithm();
    
    melBands.deleteAlgorithm();
    dct.deleteAlgorithm();
    harmonicPeaks.deleteAlgorithm();;
    hpcp.deleteAlgorithm();
    hfc.deleteAlgorithm();
    inharmonicity.deleteAlgorithm();
    centroid.deleteAlgorithm();
    spectralComplex.deleteAlgorithm();
    fft.deleteAlgorithm();;
    cartesian2polar.deleteAlgorithm();;
    onsets.onsetComplex.deleteAlgorithm();;
    onsets.onsetHfc.deleteAlgorithm();;
    onsets.onsetFlux.deleteAlgorithm();;
    pitchSalienceFunction.deleteAlgorithm();
    pitchSalienceFunctionPeaks.deleteAlgorithm();
    
    essentia::shutdown();
    
    ofLogVerbose()<<"AudioAnalyzer exit";

}

//--------------------------------------------------------------
#pragma mark - Activates
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActive(ofxAAAlgorithm algorithm, bool state){
    
    switch (algorithm) {
        case RMS:
            rms.setActive(state);
            break;
        case ENERGY:
            energy.setActive(state);
            break;
        case POWER:
            power.setActive(state);
            break;
        case PITCH_FREQ:
            pitchDetect.setActive(state);
            break;
        case PITCH_CONFIDENCE:
            pitchDetect.setActive(state);
            break;
        case PITCH_SALIENCE:
            pitchSalience.setActive(state);
            break;
        case INHARMONICITY:
            inharmonicity.setActive(state);
            break;
        case HFC:
             hfc.setActive(state);
            break;
        case CENTROID:
            centroid.setActive(state);
            break;
        case SPECTRAL_COMPLEXITY:
            spectralComplex.setActive(state);
            break;
        case DISSONANCE:
            dissonance.setActive(state);
            break;
        case SPECTRUM:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: Spectrum Algorithm cant be turned off.";
            break;
        case MEL_BANDS:
            melBands.setActive(state);
            if(state==false)dct.setActive(state);//dct needs melBands to be active.
            break;
        case MFCC:
            //dct needs melBands to be active.
            melBands.setActive(state);
            dct.setActive(state);
            break;
        case HPCP:
            hpcp.setActive(state);
            break;
        case MULTI_PITCHES:
            multiPitchKlapuri.setActive(state);
            break;
        case PITCH_SALIENCE_FUNC_PEAKS:
            pitchSalienceFunction.setActive(state);
            pitchSalienceFunctionPeaks.setActive(state);
            break;
        case ONSETS:
            onsets.setActive(state);
            break;
        case ROLL_OFF:
            rollOff.setActive(state);
            break;
        case ODD_TO_EVEN:
            oddToEven.setActive(state);
            break;
        case STRONG_PEAK:
            strongPeak.setActive(state);
            break;
        case STRONG_DECAY:
            strongDecay.setActive(state);
            break;
        case TRISTIMULUS:
            tristimulus.setActive(state);
            break;
            
        default:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm to set active.";
            break;
    }
}



//----------------------------------------------
#pragma mark - Get values
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getIsActive(ofxAAAlgorithm algorithm){
    
    switch (algorithm) {
        case RMS:
            return rms.getIsActive();
            break;
        case ENERGY:
            return energy.getIsActive();
            break;
        case POWER:
            return power.getIsActive();
            break;
        case PITCH_FREQ:
            return pitchDetect.getIsActive();
            break;
        case PITCH_CONFIDENCE:
            return pitchDetect.getIsActive();
            break;
        case PITCH_SALIENCE:
            return pitchSalience.getIsActive();
            break;
        case INHARMONICITY:
            return inharmonicity.getIsActive();
            break;
        case HFC:
            return hfc.getIsActive();
            break;
        case CENTROID:
            return centroid.getIsActive();
            break;
        case SPECTRAL_COMPLEXITY:
            return spectralComplex.getIsActive();
            break;
        case DISSONANCE:
            return dissonance.getIsActive();
            break;
        case SPECTRUM:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: Spectrum Algorithm cant be turned off.";
            break;
        case MEL_BANDS:
            return melBands.getIsActive();
            break;
        case MFCC:
            return dct.getIsActive();
            break;
        case HPCP:
            return hpcp.getIsActive();
            break;
        case MULTI_PITCHES:
            return multiPitchKlapuri.getIsActive();
            break;
        case PITCH_SALIENCE_FUNC_PEAKS:
            return pitchSalienceFunctionPeaks.getIsActive();
            break;
        case ONSETS:
            return onsets.getIsActive();
            break;
        case ROLL_OFF:
            return rollOff.getIsActive();
            break;
        case ODD_TO_EVEN:
            return oddToEven.getIsActive();
            break;
        case STRONG_PEAK:
            return strongPeak.getIsActive();
            break;
        case STRONG_DECAY:
            return strongDecay.getIsActive();
            break;
        case TRISTIMULUS:
            return tristimulus.getIsActive();
            break;
            
        default:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm to get if is active.";
            break;
    }

}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getValue(ofxAAAlgorithm algorithm, float smooth, bool normalized){
    
    float r = 0.0;
    
    switch (algorithm) {
        
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
                hfc.getSmoothedValueNormalized(smooth):
                hfc.getValueNormalized();
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
        case STRONG_DECAY:
            if (normalized){
                r = smooth ?
                strongDecay.getSmoothedValueNormalized(smooth):
                strongDecay.getValueNormalized();
            }else{
                r = smooth ?
                strongDecay.getSmoothedValue(smooth):
                strongDecay.getValue();
            }
            break;
            
            
        default:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for getting value.";
            break;
    }
    
    return r;
}
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getOnsetValue(){
    return onsets.getValue();
}
//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getValues(ofxAAAlgorithm algorithm, float smooth){
    
    switch (algorithm) {
        
        case SPECTRUM:
            return smooth ? spectrum.getSmoothedValues(smooth) : spectrum.getValues();
            break;
            
        case MEL_BANDS:
            return smooth ? melBands.getSmoothedValues(smooth) : melBands.getValues();
            break;
            
        case MFCC:
            return smooth ? dct.getSmoothedValues(smooth) : dct.getValues();
            break;
            
        case HPCP:
            return smooth ? hpcp.getSmoothedValues(smooth) : hpcp.getValues();
            break;
            
        case MULTI_PITCHES:
            return multiPitchKlapuri.getPitches();
            break;
            
        case TRISTIMULUS:
            return smooth ? tristimulus.getSmoothedValues(smooth) : tristimulus.getValues();
            break;
            
        default:
            ofLogError()<<"ofxAudioAnalyzerUnit: wrong algorithm for getting values.";
            break;
    }
}
//----------------------------------------------
vector<SalienceFunctionPeak>& ofxAudioAnalyzerUnit::getPitchSaliencePeaksRef(float smooth){
    
    return smooth ? pitchSalienceFunctionPeaks.getSmoothedPeaks(smooth) : pitchSalienceFunctionPeaks.getPeaks();
    
//    return pitchSalienceFunctionPeaks.getPeaks();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getBinsNum(ofxAAAlgorithm algorithm){
    
    switch (algorithm) {
        
        case SPECTRUM:
            return spectrum.getBinsNum();
            break;
        case MEL_BANDS:
            return melBands.getBinsNum();
            break;
        case MFCC:
            return dct.getBinsNum();
            break;
        case HPCP:
            return hpcp.getBinsNum();
            break;
            
        default:
            ofLogError()<<"ofxAudioAnalyzerUnit: wrong algorithm for getting bins number.";
            break;
    }
    
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getMaxEstimatedValue(ofxAAAlgorithm algorithm){
    
    float r = 0.0;
    
    switch (algorithm) {
            
        case ENERGY:
            r = energy.getMaxEstimatedValue();
            break;
        case PITCH_FREQ:
            r = pitchDetect.getMaxPitchEstimatedValue();
            break;
        case HFC:
            r = hfc.getMaxEstimatedValue();
            break;
        case SPECTRAL_COMPLEXITY:
            r = spectralComplex.getMaxEstimatedValue();
            break;
        case CENTROID:
            r = centroid.getMaxEstimatedValue();
            break;
        case ROLL_OFF:
            r = rollOff.getMaxEstimatedValue();
            break;
        case ODD_TO_EVEN:
            r = oddToEven.getMaxEstimatedValue();
            break;
        case STRONG_PEAK:
            r = strongPeak.getMaxEstimatedValue();
            break;
        case STRONG_DECAY:
            r = strongDecay.getMaxEstimatedValue();
            break;
            
        default:
            ofLogError()<<"ofxAudioAnalyzerUnit: wrong algorithm for setting max estimated value.";
            break;
    }
    
    return r;
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setMaxEstimatedValue(ofxAAAlgorithm algorithm, float value){
    
    switch (algorithm) {
            
        case ENERGY:
            energy.setMaxEstimatedValue(value);
            break;
        case PITCH_FREQ:
            pitchDetect.setMaxPitchEstimatedValue(value);
            break;
        case HFC:
            hfc.setMaxEstimatedValue(value);
            break;
        case SPECTRAL_COMPLEXITY:
            spectralComplex.setMaxEstimatedValue(value);
            break;
        case CENTROID:
            centroid.setMaxEstimatedValue(value);
            break;
        case ROLL_OFF:
            rollOff.setMaxEstimatedValue(value);
            break;
        case ODD_TO_EVEN:
            oddToEven.setMaxEstimatedValue(value);
            break;
        case STRONG_PEAK:
            strongPeak.setMaxEstimatedValue(value);
            break;
        case STRONG_DECAY:
            strongDecay.setMaxEstimatedValue(value);
            break;
            
        default:
             ofLogError()<<"ofxAudioAnalyzerUnit: wrong algorithm for setting max estimated value.";
            break;
    }
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::resetOnsets(){
    onsets.reset();
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
    
    onsets.setOnsetAlpha(alpha);
    onsets.setOnsetSilenceTreshold(silenceTresh);
    onsets.setOnsetTimeTreshold(timeTresh);
    onsets.setUseTimeTreshold(useTimeTresh);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setSalienceFunctionPeaksParameters(int maxPeaks){
    pitchSalienceFunctionPeaks.setMaxPeaksNum(maxPeaks);
}
//----------------------------------------------
#pragma mark - Utils
//----------------------------------------------
int ofxAudioAnalyzerUnit::getPitchFreqAsMidiNote(float smooth){
    return pitchToMidi(getValue(PITCH_FREQ, smooth));
}
//----------------------------------------------
string ofxAudioAnalyzerUnit::getPitchFreqAsNoteName(float smooth){
    return midiToNoteName(getValue(PITCH_FREQ, smooth));
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::pitchToMidi(float pitch){
    return round (12*log2(pitch/440) + 69);
}
//--------------------------------------------------------------
string ofxAudioAnalyzerUnit::midiToNoteName(int midiNote){
    
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


