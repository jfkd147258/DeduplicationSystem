#include "rabin_karp.h"

using namespace std;

const int BUFFER_SIZE = 8192;  // define buffer size
const int WINDOW_SIZE = 32;              
const uint64_t CHUNK_AVG_SIZE = 8192;
const uint64_t CHUNK_MASK = CHUNK_AVG_SIZE - 1;
const int BLOCK_MIN_SIZE = 2048;
const int BLOCK_MAX_SIZE = 16384;  // Increase max chunk size

uint64_t rabin_karp_rolling_hash(const char* data, int len) {
    static const uint64_t prime_base = 257;
    static const uint64_t prime_mod = 1e9 + 7;

    uint64_t hash = 0;
    for (int i = 0; i < len; ++i) {
        hash = (hash * prime_base + data[i]) % prime_mod;
    }

    return hash;
}

std::vector<string> rabin_chunking(std::ifstream& file) {
    std::vector<string> chunks;
    char buffer[BUFFER_SIZE];

    string chunk;
    int chunkSize = 0;

    while (file.read(buffer, sizeof(buffer))) {
        for (int i = 0; i < file.gcount(); i += WINDOW_SIZE) {
            chunkSize += min(WINDOW_SIZE, (int)file.gcount() - i);
            uint64_t fp = rabin_karp_rolling_hash(buffer + i, min(WINDOW_SIZE, (int)file.gcount() - i));

            if ((fp & CHUNK_MASK) == CHUNK_MASK && chunkSize >= BLOCK_MIN_SIZE) {
                chunks.push_back(chunk);
                chunk.clear();
                chunkSize = 0;
            } else if (chunkSize >= BLOCK_MAX_SIZE) {
                chunks.push_back(chunk);
                chunk.clear();
                chunkSize = 0;
            } else {
                chunk.append(buffer + i, min(WINDOW_SIZE, (int)file.gcount() - i));
            }
        }
    }

    if (!chunk.empty()) {
        chunks.push_back(chunk);
    }

    return chunks;
}
