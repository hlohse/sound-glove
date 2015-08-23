#include "Midi.h"
#include <algorithm>
using namespace std;

uint8_t Midi::Limit(const uint8_t value,
                    const Midi::ValueType type)
{
    const uint8_t min = static_cast<uint8_t>(Limits::Min);
    const uint8_t max = type == ValueType::Channel ?
        static_cast<uint8_t>(Limits::ChannelMax) : 
        static_cast<uint8_t>(Limits::Max);

    return std::min(std::max(value, min), max);
}

void Midi::SplitStatusChannel(const uint8_t status_channel,
                              Midi::Status& status,
                              uint8_t& channel)
{
    status  = static_cast<Midi::Status>(status_channel & 0xF0); // 1111 0000
    channel = status_channel & 0x0F; // 0000 1111
}

uint8_t Midi::CombineStatusChannel(const Midi::Status status,
                                   const uint8_t channel)
{
    return static_cast<uint8_t>(status) | (channel & 0x0F); // ssss cccc
}

void Midi::SplitPitchBend(const uint16_t pitch_bend,
                          uint8_t& fine,
                          uint8_t& coarse)
{
    const uint8_t fine_7bit   = (uint8_t) (pitch_bend & 0x007F); // 0000 0000 0111 1111
    const uint8_t coarse_7bit = (uint8_t) ((pitch_bend >> 7) & 0x007F);

    fine   = Limit(fine_7bit,   ValueType::Value);
    coarse = Limit(coarse_7bit, ValueType::Value);
}

uint16_t Midi::CombinePitchBend(const uint8_t fine,
                                const uint8_t coarse)
{
    const uint16_t fine_word   = ((uint16_t) fine) & 0x007F;          // 0000 0000 0111 1111
    const uint16_t coarse_word = (((uint16_t) coarse) << 7) & 0x3F80; // 0011 1111 1000 0000
    
    return coarse_word | fine_word; // 00cc cccc cfff ffff
}

