#ifndef SIGNAL_ANALYSER_MP3READER_H
#define SIGNAL_ANALYSER_MP3READER_H
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "minimp3.h"


class mp3Reader {
private:
    // Stores the data that is outputted by the getFile function
    struct SoundData {
        double duration;
        double resolution;
        size_t samplesCount;
        std::vector<mp3d_sample_t> soundSamples;
    };

    // Custom error to throw when the attempt to get the sound data fails
    class FileGetError : public std::exception {
    private:
        std::string msg;

    public:
        explicit FileGetError(std::string  msg) : msg(std::move(msg)) {}

        [[nodiscard]] const char* what() const noexcept override {
            return msg.c_str();
        }
    };

public:
    // Finds a sound file to read and attempts to read it
    // Out puts the SoundData struct with all the information about the file and its data
    static SoundData getFile();
    // Temporary function to help visualise the wave in the sound file
    [[deprecated]] static std::string convertToDesmos(const SoundData& sound_data);
};


#endif //SIGNAL_ANALYSER_MP3READER_H