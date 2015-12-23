#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    
    int bufferSize = 512;
    int sampleRate = 44100;
    int outChannels = 2;
    int inChannels = 0;
    int ticksPerBuffer = bufferSize/64;
    
    buffer_1 =  new float[bufferSize];
    buffer_2 =  new float[bufferSize];
    for(int i = 0; i < bufferSize; i++)
        buffer_1[i]=buffer_2[i]= 0;
    audioAnalyzer1.setup(512, 44100);
    audioAnalyzer2.setup(512, 44100);
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, ticksPerBuffer);
    spectrumSize = bufferSize/2 + 1;
    spectrum1 = new float[spectrumSize];
    for(int i = 0; i < spectrumSize; i++)
        spectrum1[i]= 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    spectrum1 = audioAnalyzer1.getSpectrum();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::seaGreen);
    for(int i = 0; i < spectrumSize; i++){
        float h = ofMap(spectrum1[i], log10(0.001), log10(1.0), 0.0, 1.0);
        float x = ofGetWidth()/float(spectrumSize);
        x *= i;
        float y = ofGetHeight()*.75;
        ofLine(x,y,x,y-500*h);
        
    }
}

//------------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    
    //ANALYZE ESSENTIA ***********************************
    for (int i = 0; i < bufferSize; i++){
        buffer_1[i]	= output[i*nChannels] = ofRandomf() * 0.5;
        buffer_2[i]	= output[i*nChannels+1] = ofRandomf() * 0.5;
    }
    audioAnalyzer1.analyze(buffer_1, bufferSize);
    //audioAnalyzer2.analyze(buffer_2, bufferSize);
    
    
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

