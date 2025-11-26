#include "customGenerator.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

string HashGenerator::generateHash(string input){
    uint64_t hash = MIX_CONSTANT_1;
    hash = varikliukas(hash, static_cast<uint64_t>(input.length()) * MIX_CONSTANT_2);

    for (size_t i = 0; i < input.length(); i++)
    {
        uint64_t charValue = static_cast<uint64_t>(static_cast<unsigned char>(input[i]));
        uint64_t charMix = charValue * 0x9E3779B97F4A7C15ULL;  // Golden ratio 
        charMix ^= (i * MIX_CONSTANT_1);
        charMix = rotl64(charMix, 13);

        hash ^= charMix;
        hash = varikliukas(hash, charValue + (i << 8));
        hash = rotl64(hash, 7);
    }

    hash = varikliukas(hash, (input.length() << 32) ^ MIX_CONSTANT_3);

    uint64_t tableIndex1 = hash % LOOKUP_TABLE_SIZE;
    uint64_t tableIndex2 = (hash >> 21) % LOOKUP_TABLE_SIZE;
    uint64_t tableIndex3 = (hash >> 42) % LOOKUP_TABLE_SIZE;

    uint64_t tableSeed = static_cast<uint64_t>(prekesKodai[tableIndex1]);
    tableSeed ^= static_cast<uint64_t>(prekesKodai[tableIndex2]) << 16;
    tableSeed ^= static_cast<uint64_t>(prekesKodai[tableIndex3]) << 32;

    hash = varikliukas(hash, tableSeed);

    for (int round = 0; round < MIXING_ROUNDS * 3; round++)
    {
        hash = varikliukas(hash, rotl64(hash, 29) ^ rotr64(hash, 19));
        hash ^= MIX_CONSTANT_2;
    }

    hash = finalMix(hash);

    uint64_t hash1 = hash;
    uint64_t hash2 = finalMix(hash ^ MIX_CONSTANT_1);
    uint64_t hash3 = finalMix(hash ^ MIX_CONSTANT_2);
    uint64_t hash4 = finalMix(hash ^ MIX_CONSTANT_3);

    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    ss << std::setw(16) << hash1;
    ss << std::setw(16) << hash2;
    ss << std::setw(16) << hash3;
    ss << std::setw(16) << hash4;

    string resultString = ss.str();

    return resultString;
}

uint64_t HashGenerator::varikliukas(uint64_t seed, uint64_t offset)
{
    uint64_t mixed = seed + offset + MIX_CONSTANT_1;

    mixed ^= rotl64(mixed, 33);
    mixed *= MIX_CONSTANT_2;
    mixed ^= rotr64(mixed, 29);
    mixed *= MIX_CONSTANT_3;
    mixed ^= rotl64(mixed, 32);

    mixed += rotr64(mixed, 17);
    mixed ^= MIX_CONSTANT_1;

    return mixed;
}

uint64_t HashGenerator::finalMix(uint64_t hash)
{
    hash ^= hash >> 33;
    hash *= 0xFF51AFD7ED558CCDULL;
    hash ^= hash >> 33;
    hash *= 0xC4CEB9FE1A85EC53ULL;
    hash ^= hash >> 33;
    return hash;
}

bool HashGenerator::validateHashDifficulty(const std::string& hash, int difficulty) {
    if (difficulty <= 0 || difficulty > hash.length()) {
        return true; 
    }

    for (int i = 0; i < difficulty; i++) {
        if (hash[i] != '0') {
            return false;
        }
    }
    return true;
}

const int HashGenerator::prekesKodai[440] = {
    375489, 396069, 376381, 347557, 376206, 405686, 410243, 344899, 375468, 381335,
    338017, 396083, 389180, 410870, 347591, 384593, 365675, 366992, 383811, 394402,
    377336, 369578, 366129, 404723, 371776, 371740, 380613, 378200, 371796, 408460,
    378147, 401010, 352019, 373971, 372045, 391073, 388960, 364909, 371109, 378344,
    370899, 373438, 355508, 371140, 371170, 339145, 410970, 370311, 378961, 380616,
    355031, 429221, 410965, 371729, 378751, 377518, 379387, 408063, 383689, 391477,
    360861, 371404, 371641, 419956, 406059, 370309, 371049, 371123, 389670, 371120,
    367703, 378412, 378848, 377507, 392115, 404414, 416142, 373781, 421128, 401966,
    384291, 420480, 499110, 400122, 388684, 357055, 383359, 385161, 378408, 379032,
    386733, 382133, 371882, 358402, 499103, 381967, 327582, 410618, 499109, 356370,
    377455, 384271, 352813, 411415, 423607, 360070, 430779, 498671, 362190, 399688,
    408777, 385206, 415116, 390701, 375441, 388973, 377537, 370898, 395323, 408416,
    497104, 373838, 380862, 393467, 391248, 370400, 394711, 371009, 366392, 372773,
    381780, 358185, 402248, 391066, 371532, 371047, 499084, 381795, 372753, 390353,
    392100, 403782, 384266, 372347, 373383, 378356, 351359, 497109, 379715, 391264,
    357563, 341912, 397708, 364718, 397725, 498014, 390205, 387299, 371932, 378753,
    408465, 429556, 414897, 388629, 389616, 389089, 357233, 497110, 497473, 358234,
    499096, 415313, 391262, 339003, 370672, 382012, 381756, 345555, 420111, 371402,
    381452, 392385, 499268, 371744, 498661, 394509, 498666, 406909, 370639, 379230,
    385722, 406912, 398315, 373101, 370646, 398887, 374032, 400458, 390229, 405977,
    323004, 370642, 349146, 348738, 499104, 378383, 368521, 382142, 309013, 370255,
    391147, 370645, 404277, 357553, 394714, 378756, 370268, 395683, 413069, 382150,
    370169, 397013, 382166, 411238, 413628, 372458, 384882, 373826, 421908, 412198,
    499734, 400429, 384049, 358416, 391572, 498649, 386432, 379077, 364923, 408402,
    387251, 389207, 341968, 363714, 497490, 424225, 371027, 390199, 372068, 371166,
    371139, 399507, 389677, 389800, 390217, 381450, 498659, 372420, 364943, 371128,
    393511, 371860, 313502, 392364, 411203, 499407, 406907, 405565, 393472, 384886,
    411250, 498660, 377533, 395808, 349238, 369817, 371408, 498442, 392404, 381524,
    329495, 364710, 390222, 389357, 414889, 403666, 414629, 372415, 497990, 383952,
    370182, 382136, 393337, 406209, 370637, 370628, 370414, 407787, 374188, 497154,
    390331, 360847, 499897, 408707, 400068, 372424, 371438, 360784, 498747, 420687,
    390999, 371877, 420762, 397728, 497482, 411530, 326779, 385210, 371832, 370267,
    370181, 498050, 306349, 342594, 393552, 359628, 373349, 403689, 390458, 390437,
    410692, 421572, 413110, 397730, 382137, 372796, 372421, 381789, 379465, 364725,
    498664, 363821, 354687, 371753, 381784, 371595, 372407, 412923, 408401, 373656,
    356094, 370704, 421867, 377338, 392875, 363348, 387232, 360314, 385648, 370264,
    358048, 372216, 392381, 385324, 358183, 371538, 371876, 375543, 403082, 395694,
    371546, 372024, 379704, 415156, 372805, 370721, 370626, 392390, 371334, 499204,
    373217, 371157, 419967, 364659, 401220, 376610, 379160, 399727, 373644, 371621,
    381736, 498514, 357450, 384007, 370161, 345603, 432209, 400199, 391043, 414072,
    379416, 381504, 498443, 371625, 371861, 371446, 390226, 372408, 358018, 371605,
    392405, 372358, 388369, 498515, 400409, 397731, 413086, 366558, 321894, 497105,
    408463, 388614, 371747, 415101, 421909, 387279, 370187, 410633, 372375, 419987,
    371262, 385125, 371167, 370531, 358334, 327840, 371288, 371794, 383417, 354371};