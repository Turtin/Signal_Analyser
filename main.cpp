#include <iostream>

#include "file/mp3Reader.h"
#include "math/Complex.h"
#include "math/FourierTransform.cpp"

int main() {
    auto *mp3 = new mp3Reader();
    auto data = mp3->getFile();
    FourierTransformer::run(mp3Reader::castAudioSample(data.soundSamples), data.duration);

    // mp3Reader::convertToDesmos(data);

    return 0;
}
