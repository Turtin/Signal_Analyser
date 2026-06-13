#include "mp3Reader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#define MINIMP3_IMPLEMENTATION
#include "../include/minimp3_ex.h"

namespace fs = std::filesystem;

// Finds a sound file to read and attempts to read it
// Out puts the SoundData struct with all the information about the file and its data
mp3Reader::SoundData mp3Reader::getFile() {
    SoundData data; // Holds the data to output

    std::cout << "Searching for mp3 files..." << std::endl;

    fs::path runningDir = fs::current_path();
    std::list<fs::path> files;
    fs::path signalFile;

    // finding and selecting the file the process
    try {
        for (const auto& entry: fs::directory_iterator(runningDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
                files.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << e.what() << std::endl;
    }

    if (files.empty()) {
        std::cout << "No mp3 files found in " << runningDir.parent_path() << std::endl;
    } else if (files.size() == 1) {
        std::cout << "Found File at: " << files.front().string() << std::endl
        << "Defaulting to this file!"  << std::endl;

        signalFile = files.front();
    } else {
        std::cout << "Found " << files.size() << " mp3 files" << std::endl;
    }

    // Opening the file
    std::ifstream file(signalFile, std::ifstream::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        throw FileGetError("Unable to open file");
    }

    // loading data into memory
    file.clear();
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cout << "Unable to read file" << std::endl;
        throw FileGetError("Unable to read file");
    }

    // initialising decoder
    mp3dec_ex_t dec;
    if (mp3dec_ex_open_buf(&dec, buffer.data(), buffer.size(), 0) != 0) {
        std::cout << "Unable to open mp3 file" << std::endl;
        throw FileGetError("Unable to open mp3 file");
    }

    std::vector<mp3d_sample_t> pcmSamples(dec.samples);

    // storing the data to output
    data.samplesCount = mp3dec_ex_read(&dec, pcmSamples.data(), pcmSamples.size());
    data.duration = (static_cast<double>(pcmSamples.size()) / dec.info.channels) / dec.info.hz;
    data.resolution = data.duration / static_cast<double>(pcmSamples.size());
    data.soundSamples = std::move(pcmSamples);

    mp3dec_ex_close(&dec);

    return data;
}

// Temporary function to help visualise the wave in the sound file
std::string mp3Reader::convertToDesmos(const SoundData& sound_data) {
    std::vector<mp3d_sample_t> audioSamples = sound_data.soundSamples;

    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < sound_data.samplesCount; i+=100) {
        ss << "(" << sound_data.resolution * i << "," << audioSamples[i] << "),";
    }
    ss << "]";

    std::cout << ss.str() << std::endl;
    return ss.str();
}
