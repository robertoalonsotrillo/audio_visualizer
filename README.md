# audio_visualizer

## Name
Audio Feature Visualiser

## Installation
##### Tested on Mac M1 :
Install prerequisites:
```
brew install pkg-config gcc readline sqlite gdbm freetype libpng
```
Install Essentia's dependencies:
```
brew install eigen libyaml fftw ffmpeg@2.8 libsamplerate libtag tensorflow
```
If you have a newer version of ffmpeg > 2.8:
```
export PKG_CONFIG_PATH="/opt/homebrew/opt/ffmpeg@2.8/lib/pkgconfig"
```
Essentia only works with Eigen 3.3.7 which is not found in BREW and for that reason, we have to build it from source:
```
git clone --single-branch --branch 3.3.7 git@gitlab.com:libeigen/eigen.git
```
Go to the created folder and build:
```
mkdir build
cd build
cmake ..
make install
```
Now we have to build essentia:
```
git clone git@github.com:MTG/essentia.git
```
Configure:
```
cd essentia
python3 waf configure --build-static --lightweight= --fft=KISS
python3 waf build 

```
Install:
```
python3 waf install
```
We now just need to link essentia.a to our project in CMake
