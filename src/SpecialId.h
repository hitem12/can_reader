//
// Created by tomaszp on 10.09.2026.
//

#pragma once
#include <cstdint>
#include <stdexcept>
#include <fmt/core.h>

#include "Frame.h"
inline uint16_t read_be16(const uint8_t* p) { return static_cast<uint16_t>((p[0] << 8) | p[1]); }
inline uint16_t read_le16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | (p[1] << 8)); }

namespace can_reader {
    constexpr  std::size_t SizeOfSpecialId_0x1F1122EE {5};
    struct  SpecialId_0x1F1122EE {
            float RPM;          // big-endian na drucie
            float speed;        // little-endian na drucie
            float  temperature;
            [[nodiscard]] static SpecialId_0x1F1122EE deserialize(const can::Frame frame);
    };
} // can_reader

template<>
struct fmt::formatter<can_reader::SpecialId_0x1F1122EE> : fmt::formatter<std::string_view>
{
    auto format(const can_reader::SpecialId_0x1F1122EE& ff, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "rpm:{:.2f} speed:{:.2f}, tmp:{:.2f}", ff.RPM, ff.speed, ff.temperature);
    }
};