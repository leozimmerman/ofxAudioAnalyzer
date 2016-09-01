#include "ofxAudioAnalyzerUnit.h"

//TODO: Check centroid, specComp, hfc max estimated values for normalizing


#pragma mark - Main funcs
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::setup(int sampleRate, int bufferSize){
    
    #pragma mark -Init variables:
    
    framesize = bufferSize;
    hopsize = framesize/2;
    sr = sampleRate;
    zeropadding = 0;
    framerate = (Real) sr / hopsize;
    
    audioBuffer.resize(bufferSize);
    
    //-Max estimated values:
    maxEnergyEstimatedValue = 100.0;
    maxHfcEstimatedValue = 500.0; //antes 1000
    maxSpecCompEstimatedValue = 15.0; //antes 30
    maxCentroidEstimatedValue = 5000.0;//antes 7000
    
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
    
    spectrum.initAndAssignSize((bufferSize/2)+1, 0.0);
    melBands.initAndAssignSize(MELBANDS_BANDS_NUM,0);
    dct.initAndAssignSize(DCT_COEFF_NUM, 0);
    hpcp.initAndAssignSize(HPCP_SIZE, 0);
    pitchSalienceFunction.initAndAssignSize(10, 0.0);//TODO: make it a define and variable?

    dcremoval.init();
    window.init();
    fft.init();
    cartesian2polar.init();
    spectralPeaks.init();
    harmonicPeaks.init();
    
    pitchSalienceFunctionPeaks.init();
    multiPitchKlapuri.init();

    essentia::init();

    #pragma mark -Create algorithms
    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    rms.algorithm = factory.create("RMS");
    energy.algorithm = factory.create("Energy");
    power.algorithm = factory.create("InstantPower");

    dcremoval.algorithm = factory.create("DCRemoval", "sampleRate", sr);

    window.algorithm = factory.create("Windowing",
                                 "type", "hann",
                                 "zeroPadding", zeropadding);

    fft.algorithm = factory.create("FFT", "size", framesize);
    cartesian2polar.algorithm = factory.create("CartesianToPolar");

    onsets.onsetHfc.algorithm     = factory.create("OnsetDetection", "method", "hfc", "sampleRate", sr);
    onsets.onsetComplex.algorithm = factory.create("OnsetDetection", "method", "complex", "sampleRate", sr);
    onsets.onsetFlux.algorithm    = factory.create("OnsetDetection", "method", "flux", "sampleRate", sr);

    spectrum.algorithm = factory.create("Spectrum",
                                   "size", framesize);

    hfc.algorithm = factory.create("HFC", "sampleRate", sr);

    pitchSalience.algorithm = factory.create("PitchSalience", "sampleRate",sr);

    centroid.algorithm = factory.create("Centroid", "range", sr/2);

    spectralComplex.algorithm = factory.create("SpectralComplexity", "sampleRate", sr);
    
    dissonance.algorithm = factory.create("Dissonance");
    
    spectralPeaks.algorithm = factory.create("SpectralPeaks",
                                "maxPeaks", PEAKS_MAXPEAKS_NUM,
                                "magnitudeThreshold", 0.00001,
                                "minFrequency", PEAKS_MIN_FREQ,
                                "maxFrequency", PEAKS_MAX_FREQ,
                                "orderBy", "frequency");


    melBands.algorithm = factory.create("MelBands", "inputSize", (framesize/2)+1,
                              "sampleRate", sr);
    dct.algorithm = factory.create("DCT", "inputSize", MELBANDS_BANDS_NUM, "outputSize", DCT_COEFF_NUM);


    inharmonicity.algorithm = factory.create("Inharmonicity");

    pitchDetect.algorithm = factory.create("PitchYinFFT",
                                      "frameSize", framesize,
                                      "sampleRate", sr);

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
    
    //MultiPitch Kalpuri:
    multiPitchKlapuri.setup(&pitchSalienceFunctionPeaks, &spectrum, sr);

    //-Shutdown factory:
    factory.shutdown();
    
    
}


//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::analyze(const vector<float> & inBuffer){
    
    if(inBuffer.size() != framesize){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: buffer requested to analyze size(" <<inBuffer.size()<<")doesnt match the buffer size already set: "<<framesize;
    }
    
    //Cast of incoming audio buffer to Real ------------
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
    pitchSalienceFunctionPeaks.castValuesToFloat();
    
    onsets.castValuesToFloat();
    onsets.evaluate();
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::exit(){

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
            pitchSalienceFunction.setActive(state);//FIXME: Esto esta bien?
            pitchSalienceFunctionPeaks.setActive(state);
            break;
        case ONSETS:
            onsets.setActive(state);
            break;
            
        default:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm to set active.";
            break;
    }
}



//----------------------------------------------
#pragma mark - Get values
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
                energy.getSmoothedValueNormalized(smooth, 0.0, maxEnergyEstimatedValue):
                energy.getValueNormalized(0.0, maxEnergyEstimatedValue);
            break;
            
        case POWER:
            r = smooth ?
                power.getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX):
                power.getValueDbNormalized(DB_MIN, DB_MAX);
            break;
            
        case PITCH_FREQ:
            r = smooth ?
                pitchDetect.getSmoothedPitchValue(smooth):
                pitchDetect.getPitchValue();
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
                hfc.getSmoothedValueNormalized(smooth, 0.0, maxHfcEstimatedValue):
                hfc.getValueNormalized(0.0, maxHfcEstimatedValue);
            }else{
                r = smooth ?
                hfc.getSmoothedValue(smooth):
                hfc.getValue();
            }
            break;
            
        case SPECTRAL_COMPLEXITY:
            if (normalized){
                r = smooth ?
                spectralComplex.getSmoothedValueNormalized(smooth, 0.0, maxSpecCompEstimatedValue):
                spectralComplex.getValueNormalized(0.0, maxSpecCompEstimatedValue);
            }else{
                r = smooth ?
                spectralComplex.getSmoothedValue(smooth):
                spectralComplex.getValue();
            }
            break;
            
        case CENTROID:
            if (normalized){
                r = smooth ?
                centroid.getSmoothedValueNormalized(smooth, 0.0, maxCentroidEstimatedValue):
                centroid.getValueNormalized(0.0, maxCentroidEstimatedValue);
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
            
        default:
            ofLogError()<<"ofxAudioAnalyzerUnit: wrong algorithm for getting values.";
            break;
    }
}
//----------------------------------------------
vector<SalienceFunctionPeak>& ofxAudioAnalyzerUnit::getPitchSaliencePeaksRef(){
    return pitchSalienceFunctionPeaks.getPeaks();
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
void ofxAudioAnalyzerUnit::resetOnsets(){
    onsets.reset();
}
void ofxAudioAnalyzerUnit::setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
    
    onsets.setOnsetAlpha(alpha);
    onsets.setOnsetSilenceTreshold(silenceTresh);
    onsets.setOnsetTimeTreshold(timeTresh);
    onsets.setUseTimeTreshold(useTimeTresh);

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


