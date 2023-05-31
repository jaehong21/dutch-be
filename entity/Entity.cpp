#include "Entity.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <vector>

/**
 * Generates a random UUID version 4 string.
 *
 * @return A string representing a UUID version 4.
 */
std::string Entity::generateUuidV4() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    // Generate random 32-bit values for each of the five UUID components
    uint32_t time_low = dis(gen);
    uint16_t time_mid = dis(gen) & 0xFFFF;
    uint16_t time_hi_and_version = dis(gen) & 0x0FFF | 0x4000;
    uint16_t clock_seq = dis(gen) & 0x3FFF | 0x8000;
    uint64_t node = static_cast<uint64_t>(dis(gen)) << 32 | dis(gen);

    // Convert the UUID components to a string
    char buffer[37];
    std::snprintf(buffer, sizeof(buffer), "%08X-%04X-%04X-%04X-%012llX", time_low, time_mid,
                  time_hi_and_version, clock_seq, node);

    std::string uuid(buffer);
    std::transform(uuid.begin(), uuid.end(), uuid.begin(), ::tolower); // Convert to lowercase

    return uuid;
}