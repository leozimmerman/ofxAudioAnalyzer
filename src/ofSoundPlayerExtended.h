//************************
//Class taken and modified from
//https://github.com/YCAMInterlab/ofxTimeline/tree/master/libs/ofOpenALSoundPlayer_TimelineAdditions/src
//****************************

#pragma once

#include "ofConstants.h"

#include "ofMain.h"
#include "ofSoundBaseTypes.h"
#include "ofEvents.h"
#include "ofThread.h"

#if defined (TARGET_OF_IPHONE) || defined (TARGET_OSX)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <sndfile.h>

#ifdef OF_USING_MPG123
#include <mpg123.h>
#endif

class ofSoundPlayerExtended : public ofBaseSoundPlayer, public ofThread {

public:
    
    ofSoundPlayerExtended();
    virtual ~ofSoundPlayerExtended();
    
    bool load(const std::filesystem::path& fileName, bool isStream = false) override;
    void unload() override;
    void play() override;
    void stop() override;
    
    void setVolume(float vol) override;
    void setPan(float vol) override;
    void setSpeed(float spd) override;
    void setPaused(bool bP) override;
    void setLoop(bool bLp) override;
    void setMultiPlay(bool bMp) override;
    void setPosition(float pct) override; // 0 = start, 1 = end;
    void setPositionMS(int ms) override;
    
    float getPosition() const override;
    int getPositionMS() const override;
    bool isPlaying() const override;
    float getSpeed() const override;
    float getPan() const override;
    bool isLoaded() const override;
    float getVolume() const override;
    
    bool isPaused() const;
    float getDuration() const;
    int getNumChannels() const;
    int getSampleRate() const;
    
    static void initialize();
    static void close();
    

   
    vector<short> & getBuffer();
    vector<float>& getCurrentBuffer(int _size);
    vector<float>& getBufferForFrame(int _frame, float _fps, int _size);
    vector<float>& getCurrentBufferForChannel(int _size, int channel);//ofxAA
    vector<float>& getBufferForChannelForFrame(int _frame, float _fps, int _size, int channel);//ofxAA
    vector<float> currentBuffer;
    
    //ofxAA:
    ofSoundBuffer& getCurrentSoundBuffer(int _size);
    ofSoundBuffer& getCurrentSoundBufferMono(int _size);
    
    ofSoundBuffer& getSoundBufferForFrame(int _frame, float _fps, int _size);
    ofSoundBuffer& getSoundBufferMonoForFrame(int _frame, float _fps, int _size);
    
    ofSoundBuffer currentSoundBuffer;
    ofSoundBuffer channelSoundBuffer;
    
    //---
    
    static ALCcontext * alContext;
protected:
    
    void threadedFunction();
    
    void ofOpenALSoundUpdate();
    void update(ofEventArgs & args);
    
    
    bool sfReadFile(const std::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
    bool sfStream(const std::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
#ifdef OF_USING_MPG123
    bool mpg123ReadFile(const std::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
    bool mpg123Stream(const std::filesystem::path& path,std::vector<short> & buffer,std::vector<float> & fftAuxBuffer);
#endif
    bool decoderReadFile(const std::filesystem::path& fileName,vector<short> & buffer,vector<float> & fftAuxBuffer);
    
    bool readFile(const std::filesystem::path& fileName,std::vector<short> & buffer);
    bool stream(const std::filesystem::path& fileName, std::vector<short> & buffer);
    
    bool isStreaming;
    bool bMultiPlay;
    bool bLoop;
    bool bLoadedOk;
    bool bPaused;
    float pan; // 0 - 1
    float volume; // 0 - 1
    float internalFreq; // 44100 ?
    float speed; // -n to n, 1 = normal, -1 backwards
    unsigned int length; // in samples;
    
    static ALCdevice * alDevice;
    
    
    vector<float> window;
    float windowSum;
    float bandWidth;
    
    int channels;
    float duration; //in secs
    int samplerate;
    vector<ALuint> buffers;
    vector<ALuint> sources;
    
    
    
    SNDFILE* streamf;
    size_t stream_samples_read;
#ifdef OF_USING_MPG123
    mpg123_handle * mp3streamf;
#endif
    int stream_encoding;
    int mp3_buffer_size;
    int stream_subformat;
    double stream_scale;
    vector<short> buffer;
    vector<float> fftAuxBuffer;
    float curMaxAverage;
    
    bool stream_end;
    
    bool timeSet;
    float justSetTime;

    
};

