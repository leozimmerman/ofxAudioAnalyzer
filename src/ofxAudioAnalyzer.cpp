#include "ofxAudioAnalyzer.h"


void ofxAudioAnalyzer::setup(int bufferSize, int sampleRate){

        framesize = bufferSize;
        hopsize = framesize/2;
        sr = sampleRate;
        zeropadding = 0;
        framerate = (Real) sr / hopsize;

        spectrum_f.assign((bufferSize/2)+1, 0.0);
        melBands_f.assign(24, 0);
        dct_f.assign(10,0 );
        hpcp_f.assign(12,0);

    
        ///onsets...
        detecBufferSize = bufferSize; ///revisar
        detection_sum.assign(detecBufferSize,0);
        detections.assign(3, vector<Real> (detecBufferSize));
        silenceTreshold = 0.02;
        alpha = 0.1;
        addHfc = addComplex = addFlux = true;

        doRms=doEnergy=doPower=doPitch=doTuning=doOnsets=doMelbands=doMfcc=
        doHpcp=doHfc=doCentroid=doSpcCmplx=doInharmon= true;

        audioBuffer.resize(bufferSize);

        essentia::init();

    /// instanciate factory and create algorithms---------------------------------------------------

        AlgorithmFactory& factory = AlgorithmFactory::instance();

        rms = factory.create("RMS");
        energy = factory.create("Energy");
        power = factory.create("InstantPower");

        dcremoval = factory.create("DCRemoval", "sampleRate", sr);

        window = factory.create("Windowing",
                                     "type", "hann",
                                     "zeroPadding", zeropadding);

        fft = factory.create("FFT", "size", framesize);
        cartesian2polar = factory.create("CartesianToPolar");

        onsetHfc     = factory.create("OnsetDetection", "method", "hfc", "sampleRate", sr);
        onsetComplex = factory.create("OnsetDetection", "method", "complex", "sampleRate", sr);
        onsetFlux    = factory.create("OnsetDetection", "method", "flux", "sampleRate", sr);

        spectrum = factory.create("Spectrum",
                                       "size", framesize);

        hfc = factory.create("HFC", "sampleRate", sr);

        pitchSalience = factory.create("PitchSalience", "sampleRate",sr);

        centroid = factory.create("Centroid", "range", sr/2);

        spectralComplex = factory.create("SpectralComplexity", "sampleRate", sr);

        peaks = factory.create("SpectralPeaks",
                                    "maxPeaks", 10000,
                                    "magnitudeThreshold", 0.00001,
                                    "minFrequency", 40,
                                    "maxFrequency", 5000,
                                    "orderBy", "frequency");


        melBands = factory.create("MelBands", "inputSize", (framesize/2)+1,
                                  "sampleRate", sr);
        dct = factory.create("DCT", "inputSize", 24, "outputSize", 10);

        tuningFreq = factory.create("TuningFrequency");

        inharmonicity = factory.create("Inharmonicity");

        pitchDetect = factory.create("PitchYinFFT",
                                          "frameSize", framesize,
                                          "sampleRate", sr);

        harmonicPeaks = factory.create("HarmonicPeaks");

        hpcp = factory.create("HPCP",
                                       "size", 12,
                                       "referenceFrequency", 440,
                                       "bandPreset", false,
                                       "minFrequency", 40.0,
                                       "maxFrequency", 5000.0,
                                       "weightType", "squaredCosine",
                                       "nonLinear", false,
                                       "windowSize", 4.0/3.0);

  

    ///Algorithm diagram--------------------------------------------------------


        //DCRemoval
        dcremoval->input("signal").set(audioBuffer);
        dcremoval->output("signal").set(audioBuffer_dc);
        //RMS
        rms->input("array").set(audioBuffer_dc);
        rms->output("rms").set(rmsValue);
        //Energy
        energy->input("array").set(audioBuffer_dc);
        energy->output("energy").set(energyValue);
        //Power
        power->input("array").set(audioBuffer_dc);
        power->output("power").set(powerValue);
        //Window
        window->input("frame").set(audioBuffer_dc);
        window->output("frame").set(windowedframe);
        //Onsets
        fft->input("frame").set(windowedframe);
        fft->output("fft").set(fftValues);
        cartesian2polar->input("complex").set(fftValues);
        cartesian2polar->output("magnitude").set(c2pMagValues);
        cartesian2polar->output("phase").set(c2pPhaseValues);
        //-
        onsetHfc->input("spectrum").set(c2pMagValues);
        onsetHfc->input("phase").set(c2pPhaseValues);
        onsetHfc->output("onsetDetection").set(onsetHfcValue);
        //-
        onsetComplex->input("spectrum").set(c2pMagValues);
        onsetComplex->input("phase").set(c2pPhaseValues);
        onsetComplex->output("onsetDetection").set(onsetComplexValue);
        //-
        onsetFlux->input("spectrum").set(c2pMagValues);
        onsetFlux->input("phase").set(c2pPhaseValues);
        onsetFlux->output("onsetDetection").set(onsetFluxValue);
        //Spectrum
        spectrum->input("frame").set(windowedframe);
        spectrum->output("spectrum").set(spec);
        //HFC
        hfc->input("spectrum").set(spec);
        hfc->output("hfc").set(hfcValue);
        //Pitch Salience
        pitchSalience->input("spectrum").set(spec);
        pitchSalience->output("pitchSalience").set(salienceValue);
        //Centroid
        centroid->input("array").set(spec);
        centroid->output("centroid").set(centroidValue);
        //Spectral Complexity
        spectralComplex->input("spectrum").set(spec);
        spectralComplex->output("spectralComplexity").set(spectralComplexValue);
        //Peak detection
        peaks->input("spectrum").set(spec);
        peaks->output("frequencies").set(peakFreqValues);
        peaks->output("magnitudes").set(peakMagValues);
        //HPCP
        hpcp->input("frequencies").set(peakFreqValues);
        hpcp->input("magnitudes").set(peakMagValues);
        hpcp->output("hpcp").set(hpcpValues);
        //MelBands
        melBands->input("spectrum").set(spec);
        melBands->output("bands").set(melValues);
        //DCT
        dct->input("array").set(logMelBands);
        dct->output("dct").set(dctValues);
        //PitchDetection
        pitchDetect->input("spectrum").set(spec);
        pitchDetect->output("pitch").set(thisPitch);
        pitchDetect->output("pitchConfidence").set(thisConf);
        //Tuning frequency
        tuningFreq->input("frequencies").set(peakFreqValues);
        tuningFreq->input("magnitudes").set(peakMagValues);
        tuningFreq->output("tuningFrequency").set(tunFreqValue);
        tuningFreq->output("tuningCents").set(tunCentsValue);
        //HarmonicPeaks
        harmonicPeaks->input("frequencies").set(peakFreqValues);
        harmonicPeaks->input("magnitudes").set(peakMagValues);
        harmonicPeaks->input("pitch").set(thisPitch);
        harmonicPeaks->output("harmonicFrequencies").set(harmFreqValues);
        harmonicPeaks->output("harmonicMagnitudes").set(harmMagValues);
        //Inharmonicity
        inharmonicity->input("frequencies").set(harmFreqValues);
        inharmonicity->input("magnitudes").set(harmMagValues);
        inharmonicity->output("inharmonicity").set(inharmValue);

}

//--------------------------------------------------------------
void ofxAudioAnalyzer::exit(){

    delete dcremoval;
    delete rms;
    delete energy;
    delete power;
    delete window;
    delete spectrum;
    delete peaks;
    delete pitchDetect;
    delete tuningFreq;
    delete inharmonicity;
    delete melBands;
    delete dct;
    delete harmonicPeaks;
    delete hpcp;
    delete hfc;
    delete pitchSalience;
    delete centroid;
    delete spectralComplex;
    delete fft;
    delete cartesian2polar;
    delete onsetComplex;
    delete onsetHfc;
    delete onsetFlux;

    essentia::shutdown();

}

//--------------------------------------------------------------
void ofxAudioAnalyzer::analyze(float * iBuffer, int bufferSize){

    ///Cast of incoming audio buffer to Real ------------

       vector <float> fBuffer;
       fBuffer.resize(bufferSize);

        memcpy(&fBuffer[0], iBuffer, sizeof(float) * bufferSize);
        for (int i=0; i<bufferSize;i++){
             audioBuffer[i] = (Real) fBuffer[i];
        }

    /// Compute Algorithms--------------------------

        dcremoval->compute();

        if(doRms) rms->compute();
        if(doEnergy) energy->compute();
        if(doPower)power->compute();

        window->compute();
        if(doOnsets){
            fft->compute();
            cartesian2polar->compute();
            onsetHfc->compute();
            onsetComplex->compute();
            onsetFlux->compute();
        }
        spectrum->compute();
        if(doHfc) hfc->compute();
        if (doPitch){
            pitchSalience->compute();
            pitchDetect->compute();
        }
        if(doCentroid) centroid->compute();
        if(doSpcCmplx) spectralComplex->compute();
        if(doMelbands){
            melBands->compute();
            if(doMfcc){
                logMelBands.resize(melValues.size());
                for (int i=0; i<melValues.size(); ++i)
                    logMelBands[i] = amp2db(melValues[i]);
                dct->compute();
            }
        }
        peaks->compute();
        if(doHpcp) hpcp->compute();
        if (doTuning) tuningFreq->compute();
        if (doInharmon){
            harmonicPeaks->compute();
            inharmonicity->compute();
        }


    ///Cast results to FLOATS--------------------------------------
    //Spectrum
    for (int i=0; i<spec.size(); i++)
        spectrum_f[i] = log10((float) spec[i]);

    //RMS
    if(doRms) rms_f = (float) rmsValue;
    else rms_f = 0.0;
    //Energy
    if(doEnergy) energy_f = (float) energyValue;
    else energy_f = 0.0;
    //Instant power
    if(doPower) power_f = (float) powerValue;
    else power_f = 0.0;


    if(doPitch){
        //PitchDetection
        YinFrequency_f = (float) thisPitch;
        YinConfidence_f = (float) thisConf;
        //PitchSalience
        salience_f = (float) salienceValue;
    }else{
        YinFrequency_f = YinConfidence_f = salience_f = 0.0;
    }
    if(doTuning){
        //Tuning freq
        tFreq_f = (float) tunFreqValue;
        tCents_f = (float) tunCentsValue;
    }else{
        tFreq_f = tCents_f = 0.0;
    }
    if(doOnsets){
        //onsetHFC
        onsetHfc_f = (float) onsetHfcValue;
        //onsetComplex
        onsetComplex_f = (float) onsetComplexValue;
        //onsetFlux
        onsetFlux_f = (float) onsetFluxValue;
        //onsetEvaluation
        isOnset = onsetEvaluation(onsetHfcValue, onsetComplexValue, onsetFluxValue);
    }else{
        onsetFlux_f = onsetHfc_f = onsetComplex_f = 0.0;
        isOnset = false;
    }

        //MelBands
        for (int i=0; i<melValues.size(); i++){
            if(doMelbands){
                melBands_f[i] = log10 ((float) melValues[i]);
                if(doMfcc){
                    for (int i=0; i<dctValues.size(); i++)
                        dct_f[i] = (float) dctValues[i];
                }else{
                    for (int i=0; i<dctValues.size(); i++)
                        dct_f[i] = 0.0;
                }
            }else{
                melBands_f[i] = log10(0.001);
                for (int i=0; i<dctValues.size(); i++)
                    dct_f[i] = 0.0;
            }
        }
        //HPCP
        if (doHpcp){
            for (int i=0; i<hpcpValues.size(); i++)
                hpcp_f[i] = (float) hpcpValues[i];
        }else{
            for (int i=0; i<hpcpValues.size(); i++)
                hpcp_f[i] = 0.0;
        }
        //HFC
        if(doHfc) hfc_f = (float) hfcValue;
        else hfc_f = 0.0;
        //Centroid
        if(doCentroid) centroid_f = (float) centroidValue;
        else centroid_f = 0.0;
        //SpectralComplexity
        if (doSpcCmplx) spectralComplex_f = (float) spectralComplexValue;
        else spectralComplex_f = 0.0;
        //Inharmonicity
        if (doInharmon) inharm_f = (float) inharmValue;
        else inharm_f = 0.0;


}
//--------------------------------------------------------------
 bool ofxAudioAnalyzer::onsetEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux){

    Real prop_hfc, prop_complex, prop_flux;

    if (iDetectHfc > hfc_max) {
        prop_hfc = iDetectHfc/hfc_max;
        hfc_max = iDetectHfc;
        for (int i=0; i<detections[0].size(); i++)
            detections[0][i] /= prop_hfc;
    }
    if (iDetectComplex > complex_max){
        prop_complex = iDetectComplex/complex_max;
        complex_max = iDetectComplex;
         for (int i=0; i<detections[1].size(); i++)
            detections[1][i] /= prop_complex;
    }
    if (iDetectFlux > flux_max){
        prop_flux = iDetectFlux/flux_max;
        flux_max = iDetectFlux;
         for (int i=0; i<detections[2].size(); i++)
            detections[2][i] /= prop_flux;
    }

    Real hfc_norm = iDetectHfc / hfc_max;
    Real complex_norm = iDetectComplex / complex_max;
    Real flux_norm = iDetectFlux / flux_max;

    detections[0].push_back(hfc_norm);
    detections[0].erase(detections[0].begin());

    detections[1].push_back(complex_norm);
    detections[1].erase(detections[1].begin());

    detections[2].push_back(flux_norm);
    detections[2].erase(detections[2].begin());

    for (int i=0; i<detection_sum.size(); i++){
        int n=0;
        detection_sum[i]=0;
        if(addHfc){
            detection_sum[i] += detections[0][i];
            n++;
        }
        if(addComplex){
            detection_sum[i] += detections[1][i];
            n++;
        }
        if(addFlux){
            detection_sum[i] += detections[2][i];
            n++;
        }
        if(n>0) detection_sum[i] /= n;
        if(detection_sum[i] < silenceTreshold) detection_sum[i] = 0.0;
    }

    Real buffer_median = median (detection_sum);
    Real buffer_mean = mean (detection_sum);
    Real onset_threshold = buffer_median + alpha * buffer_mean;

    bool onsetDetection = detection_sum[detection_sum.size()-1] > onset_threshold;

    return onsetDetection;

 }

void ofxAudioAnalyzer::resetOnsets(){
    
    hfc_max = complex_max = flux_max = 0.0;
    for (int i=0; i<detection_sum.size(); i++)
        detection_sum[i] = 0.0;

}

