#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <string>
#include <vector>
#include <fstream>

uint64_t rabin_karp_rolling_hash(const char* data, int len);
std::vector<std::string> rabin_chunking(std::ifstream& file);

#endif //RABIN_KARP_H
