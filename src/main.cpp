// main.cpp
#include "sha256.h"
#include "rabin_karp.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <unordered_set>

using namespace std;

int main() {
    string folderPath = "Data1/";
    vector<string> files; 

    unordered_set<string> fingerprints;
    size_t totalChunks = 0;
    
    // Create a ofstream object to write time data to a file
    ofstream timeData("timeData.txt", ios::out);

    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".iso") {
            files.push_back(entry.path().filename());
        }
    }

    for (size_t i = 0; i < files.size(); i++) {
        ifstream inFile(folderPath + files[i], ios::binary);
        if (inFile.is_open()) {
            // Start timing
            auto startTime = chrono::high_resolution_clock::now();

            auto chunks = rabin_chunking(inFile);

            // End timing
            auto endTime = chrono::high_resolution_clock::now();
            auto timeDuration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

            // Write time data to file
            timeData << "File " << "[" << i << "] took " << timeDuration << " milliseconds to chunk." << endl;

            for (const auto& chunk : chunks) {
                string sha256FP = sha256(chunk);
                fingerprints.insert(sha256FP);
            }
            totalChunks += chunks.size();
            uint64_t totalSize = 0;
            for (const auto& chunk : chunks) {
                totalSize += chunk.size();
            }
            double avgSize = static_cast<double>(totalSize) / chunks.size();
            cout << "File " << "[" << i << "]" << " chunks. Average chunk size: " << avgSize/2 << " KB\n";
        } else {
            cerr << "Failed to open file " << files[i] << "\n";
        }
        inFile.close();
    }

    double deduplicationRatio = 1.0 - (double)fingerprints.size() / totalChunks;
    cout << "Total unique fingerprints: " << fingerprints.size()*2 << "\n";
    cout << "Total chunks: " << totalChunks*2 << "\n";
    cout << "Deduplication ratio: " << deduplicationRatio * 100 << "%\n";

    // Don't forget to close the ofstream
    timeData.close();

    return 0;
}

