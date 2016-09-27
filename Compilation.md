# Compilation

## Makefile

- Makefiles by Terminal (OSX & Linux): 
    * For Essentia: config.make -> PROJECT_LDFLAGS= -lessentia -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lsamplerate -ltag -lfftw3f -lavresample

    * For Sound Player (Open AL): config.make -> PROJECT_LDFLAGS=  -framework OpenAL

Easy way for makefile compilation: copy config.make and addons.make and Makefile