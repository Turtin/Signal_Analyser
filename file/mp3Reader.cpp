#include "mp3Reader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <list>
#include <vector>

#define MINIMP3_IMPLEMENTATION
#include "../include/minimp3_ex.h"

namespace fs = std::filesystem;

void mp3Reader::getFile() {
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

    if (files.size() == 0) {
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
        return;
    }

    // loading data into memory
    file.clear();
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cout << "Unable to read file" << std::endl;
        return;
    }

    // initialising decoder
    mp3dec_ex_t dec;
    if (mp3dec_ex_open_buf(&dec, buffer.data(), buffer.size(), 0) != 0) {
        std::cout << "Unable to open mp3 file" << std::endl;
        return;
    }

    std::vector<mp3d_sample_t> pcmSamples(dec.samples);
    size_t samplesCount = mp3dec_ex_read(&dec, pcmSamples.data(), pcmSamples.size());

    double duration = (static_cast<double>(pcmSamples.size()) / dec.info.channels) / dec.info.hz;

    mp3dec_ex_close(&dec);

    std::stringstream ss;
    ss << "[";
    double resolution = duration / static_cast<double>(pcmSamples.size());
    for (int i = 0; i < samplesCount; i+=100) {
        ss << "(" << resolution * i << "," << pcmSamples[i] << "),";
    }

    ss << "]";

    std::cout << ss.str() << std::endl;
}
