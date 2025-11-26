#ifndef CUSTOMGENERATOR_H
#define CUSTOMGENERATOR_H

#include <string>
#include <cstdint>

class HashGenerator
{
private:
    static const int prekesKodai[440];
    static const int LOOKUP_TABLE_SIZE = 440;
    static const int MIXING_ROUNDS = 4;

    static const uint64_t MIX_CONSTANT_1 = 0x517CC1B727220A95ULL;
    static const uint64_t MIX_CONSTANT_2 = 0xBF58476D1CE4E5B9ULL;
    static const uint64_t MIX_CONSTANT_3 = 0x94D049BB133111EBULL;

    inline uint64_t rotl64(uint64_t x, int r) const
    {
        return (x << r) | (x >> (64 - r));
    }

    inline uint64_t rotr64(uint64_t x, int r) const
    {
        return (x >> r) | (x << (64 - r));
    }

public:
    std::string generateHash(std::string input);
    bool validateHashDifficulty(const std::string& hash, int difficulty);
    uint64_t varikliukas(uint64_t seed, uint64_t offset);
    uint64_t finalMix(uint64_t hash);
};

#endif