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
    
    // Create ofstream objects to write time data to files
    ofstream timeDataChunking("timeDataChunking.txt", ios::out);
    ofstream timeDataFingerprinting("timeDataFingerprinting.txt", ios::out);

    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".iso") {
            files.push_back(entry.path().filename());
        }
    }

    for (size_t i = 0; i < files.size(); i++) {
        ifstream inFile(folderPath + files[i], ios::binary);
        if (inFile.is_open()) {
            // Start timing for chunking
            auto startTimeChunking = chrono::high_resolution_clock::now();

            auto chunks = rabin_chunking(inFile);

            // End timing for chunking
            auto endTimeChunking = chrono::high_resolution_clock::now();
            auto timeDurationChunking = chrono::duration_cast<chrono::milliseconds>(endTimeChunking - startTimeChunking).count();

            // Write chunking time data to file
            timeDataChunking << "File " << "[" << i << "] took " << timeDurationChunking << " milliseconds to chunk." << endl;

            // Start timing for fingerprinting
            auto startTimeFingerprinting = chrono::high_resolution_clock::now();

            for (const auto& chunk : chunks) {
                string sha256FP = sha256(chunk);
                fingerprints.insert(sha256FP);
            }

            // End timing for fingerprinting
            auto endTimeFingerprinting = chrono::high_resolution_clock::now();
            auto timeDurationFingerprinting = chrono::duration_cast<chrono::milliseconds>(endTimeFingerprinting - startTimeFingerprinting).count();

            // Write fingerprinting time data to file
            timeDataFingerprinting << "File " << "[" << i << "] took " << timeDurationFingerprinting << " milliseconds to generate SHA fingerprints." << endl;

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
    timeDataChunking.close();
    timeDataFingerprinting.close();

    return 0;
}
