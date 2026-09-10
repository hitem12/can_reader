//
// Created by tomaszp on 10.09.2026.
//
#pragma once
#include<system_error>

#include <string>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <cstring>
#include <unistd.h>
#include <cstdint>
#include "Frame.h"
enum class can_frame_error {
    ok = 0,
    error_frame,
    remote_request,
    unexpected_id,
    invalid_length,
    payload_too_short,
};

namespace detail {
    class can_frame_category final : public std::error_category {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "can_frame"; }
        [[nodiscard]] std::string message(int ev) const override {
            switch (static_cast<can_frame_error>(ev)) {
                case can_frame_error::ok:                return "Success";
                case can_frame_error::error_frame:       return "CAN error frame received";
                case can_frame_error::remote_request:    return "remote transmission request (no payload)";
                case can_frame_error::unexpected_id:     return "unexpected CAN identifier";
                case can_frame_error::invalid_length:    return "invalid data length code";
                case can_frame_error::payload_too_short: return "payload shorter than expected";
            }
            return "unknown CAN frame error";
        }
    };
}

inline const std::error_category& can_frame_category() noexcept {
    static detail::can_frame_category cat;
    return cat;
}

inline std::error_code make_error_code(can_frame_error e) noexcept {
    return {static_cast<int>(e), can_frame_category()};
}

template <> struct std::is_error_code_enum<can_frame_error> : std::true_type {};
namespace can_reader {




    class SocketCtl
    {
        int fd_ {};
    public:
        SocketCtl() = default;
        SocketCtl(const SocketCtl&) = delete;
        SocketCtl(SocketCtl&&) = delete;
        SocketCtl& operator=(const SocketCtl&) = delete;
        SocketCtl& operator=(SocketCtl&&) = delete;
        ~SocketCtl() {if (fd_ > 0) {close_socket();}}
        [[nodiscard]] int get() const {return fd_;}
        [[nodiscard]] std::error_code open_socket(std::string_view interface) noexcept;
        void close_socket();
        [[nodiscard]]std::error_code read(can::Frame& out) const {
            if (fd_ < 0) return std::make_error_code(std::errc::bad_file_descriptor);

            struct can_frame frame {};
            if (::read(fd_, &frame, sizeof(frame)) != sizeof(frame)) {
                return {std::error_code{errno, std::generic_category()}};
            }
            if (const auto er = validate(frame)) return er;
            from_linux(frame, out);
            return {};
        }
    private:
        static std::error_code validate(const can_frame& f) noexcept {
            if (f.can_id & CAN_ERR_FLAG)   return can_frame_error::error_frame;
            if (f.can_id & CAN_RTR_FLAG)   return can_frame_error::remote_request;
            if (f.len > CAN_MAX_DLEN)      return can_frame_error::invalid_length;
            return {};
        }
        static void from_linux(const can_frame& raw, can::Frame& out) noexcept {

            out.extended = (raw.can_id & CAN_EFF_FLAG) != 0;
            out.id       = raw.can_id & (out.extended ? CAN_EFF_MASK : CAN_SFF_MASK);
            out.len      = std::min<std::uint8_t>(raw.len, CAN_MAX_DLEN);
            std::memcpy(out.data.data(), raw.data, out.len);
        }

    };
} // can_reader
