#include "mp3Reader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#define MINIMP3_IMPLEMENTATION
#include "../cmd/SelectionGrid.h"
#include "../include/minimp3_ex.h"

namespace fs = std::filesystem;

// Gets the user input for which file to use and outputs it
fs::path mp3Reader::fileSelect(const std::vector<fs::path>& files) {
    std::vector<std::string> paths;

    for (const fs::path& path : files) { // convert type
        paths.push_back(path.filename().string());
    }

    SelectionGrid grid(paths); // Get user input
    grid.create();

    return fs::path(grid.result);
}


// Finds a sound file to read and attempts to read it
// Out puts the SoundData struct with all the information about the file and its data
mp3Reader::SoundData mp3Reader::getFile() {
    SoundData data; // Holds the data to output

    std::cout << "Searching for mp3 files..." << std::endl;

    fs::path runningDir = fs::current_path();
    std::vector<fs::path> files;
    fs::path signalFile;

    // finding and selecting the file the process
    try { // Gets a list of compatible files
        for (const auto& entry: fs::directory_iterator(runningDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
                files.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << e.what() << std::endl;
    }

    // selecting and handling the files
    if (files.empty()) {
        std::cout << "No mp3 files found in " << runningDir.parent_path() << std::endl;
    } else if (files.size() == 1) {
        std::cout << "Found File at: " << files.front().string() << std::endl
        << "Defaulting to this file!"  << std::endl;

        signalFile = files.front();
    } else {
        std::cout << "Found " << files.size() << " mp3 files" << std::endl;
        signalFile = fileSelect(files);
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

std::vector<double> mp3Reader::castAudioSample(const std::vector<mp3d_sample_t> &original) {
    std::vector<double> data;

    for (const mp3d_sample_t point : original) {
        data.emplace_back(point);
    }

    return data;
}
