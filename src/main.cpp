#include <iostream>
#include <filesystem>
#include <unordered_set>
#include "sha256.h"
#include "rabin_karp.h"

using namespace std;

int main() {
    std::string folderPath = "Data1/";
    std::vector<std::string> files; 
    
    std::unordered_set<string> fingerprints;
    size_t totalChunks = 0;
    
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".iso") {
            files.push_back(entry.path().filename());
        }
    }

    for (size_t i = 0; i < files.size(); i++) {
        std::ifstream inFile(folderPath + files[i], std::ios::binary);
        if (inFile.is_open()) {
            auto chunks = rabin_chunking(inFile);
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
            std::cout << "File " << "[" << i << "]" << " chunks. Average chunk size: " << avgSize/2 << " KB\n";
        } else {
            std::cerr << "Failed to open file " << files[i] << "\n";
        }
        inFile.close();
    }

    double deduplicationRatio = 1.0 - (double)fingerprints.size() / totalChunks;
    std::cout << "Total unique fingerprints: " << fingerprints.size()*2 << "\n";
    std::cout << "Total chunks: " << totalChunks*2 << "\n";
    std::cout << "Deduplication ratio: " << deduplicationRatio * 100 << "%\n";

    return 0;
}
