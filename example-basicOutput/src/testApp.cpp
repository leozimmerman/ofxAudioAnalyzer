#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::update(){
    spectrum1 = audioAnalyzer1.getSpectrum();
}

//--------------------------------------------------------------
void testApp::draw(){
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
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
        //ANALYZE ESSENTIA ***********************************
        for (int i = 0; i < bufferSize; i++){
            buffer_1[i]	= output[i*nChannels] = ofRandomf() * 0.5;
            buffer_2[i]	= output[i*nChannels+1] = ofRandomf() * 0.5;
        }
        audioAnalyzer1.analyze(buffer_1, bufferSize);
        //audioAnalyzer2.analyze(buffer_2, bufferSize);
        
    
    
}
//--------------------------------------------------------------
void testApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
