#include <iostream>

#include "file/mp3Reader.h"
#include "math/FourierTransform.cpp"

int main() {
    auto *mp3 = new mp3Reader();
    auto data = mp3->getFile();


    auto stuff = FourierTransformer::run(mp3Reader::castAudioSample(data.soundSamples), data.duration);

    for (auto& wave : stuff) {
        std::cout << wave.amp << "sin(" << wave.phase << "x+" << wave.shift << ")" << std::endl;
    }
    // mp3Reader::convertToDesmos(data);

    return 0;
}
