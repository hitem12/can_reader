//
// Created by tomaszp on 11.09.2026.
//
#include "SpecialId.h"
#include <gtest/gtest.h>

using namespace can_reader;

namespace {
    can::Frame make_frame(std::initializer_list<std::uint8_t> bytes) {
        can::Frame frame{};
        frame.len = static_cast<std::uint8_t>(bytes.size());
        std::copy(bytes.begin(), bytes.end(), frame.data.begin());
        return frame;
    }
}

TEST(SpecialIdDeserialize, DecodesRpmFromLittleEndianBytes0And1) {
    // 1000 == 0x03E8, little-endian => byte0=0xE8, byte1=0x03
    const auto frame = make_frame({0xE8, 0x03, 0x00, 0x00, 0x00});

    const auto result = SpecialId_0x1F1122EE::deserialize(frame);

    EXPECT_FLOAT_EQ(result.RPM, 1000.0f);
}

TEST(SpecialIdDeserialize, DecodesSpeedFromBigEndianBytes1And2ScaledBy0_01) {
    // 0x00FF big-endian at bytes 1,2 -> 255 * 0.01 == 2.55 km/h
    const auto frame = make_frame({0x00, 0x00, 0xFF, 0x00, 0x00});

    const auto result = SpecialId_0x1F1122EE::deserialize(frame);

    EXPECT_FLOAT_EQ(result.speed, 2.55f);
}

TEST(SpecialIdDeserialize, DecodesTemperatureFromByte3WithOffset60) {
    // 0x20 == 32, shifted by an offset of 60 -> -28 deg C
    const auto frame = make_frame({0x00, 0x00, 0x00, 0x20, 0x00});

    const auto result = SpecialId_0x1F1122EE::deserialize(frame);

    EXPECT_FLOAT_EQ(result.temperature, -28.0f);
}

TEST(SpecialIdDeserialize, ThrowsLengthErrorWhenFrameShorterThanExpectedPayload) {
    can::Frame frame{};
    frame.len = SizeOfSpecialId_0x1F1122EE - 1;

    EXPECT_THROW(auto d = SpecialId_0x1F1122EE::deserialize(frame), std::length_error);
}
