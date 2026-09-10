//
// Created by tomaszp on 10.09.2026.
//

#include "SpecialId.h"

constexpr float scale_factor = 0.01;


can_reader::SpecialId_0x1F1122EE can_reader::SpecialId_0x1F1122EE::deserialize(const can::Frame frame) {
    const auto p = frame.begin();
    if (p + SizeOfSpecialId_0x1F1122EE > frame.end()) {
        throw std::length_error("frame 0x1F1122EE: expected 5 bytes, got " + std::to_string(frame.len));
    }
    SpecialId_0x1F1122EE result {};
    result.RPM = static_cast<float>(read_be16(p)) * scale_factor;
    result.speed = static_cast<float>(read_be16(p+2)) * scale_factor;
    result.temperature = static_cast<float>(p[4]) * scale_factor;
    return result;
}
