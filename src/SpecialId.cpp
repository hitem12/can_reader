//
// Created by tomaszp on 10.09.2026.
//

#include "SpecialId.h"

constexpr float speed_scale_factor = 0.01;
constexpr int16_t tmp_offset = -60;

can_reader::SpecialId_0x1F1122EE can_reader::SpecialId_0x1F1122EE::deserialize(const can::Frame frame) {
    const auto p = frame.begin();
    if (p + SizeOfSpecialId_0x1F1122EE > frame.end()) {
        throw std::length_error("frame 0x1F1122EE: expected 5 bytes, got " + std::to_string(frame.len));
    }
    SpecialId_0x1F1122EE result {};
    result.RPM = static_cast<float>(read_le16(p));
    result.speed = static_cast<float>(read_be16(p+1)) * speed_scale_factor;
    result.temperature = static_cast<float>(p[3]) + tmp_offset;
    return result;
}
