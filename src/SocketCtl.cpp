//
// Created by tomaszp on 10.09.2026.
//

#include "SocketCtl.h"


std::error_code can_reader::SocketCtl::open_socket(const std::string_view interface) noexcept{
    fd_ = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (!fd_)
    {
        return {std::error_code{errno, std::generic_category()}};
    }
    constexpr can_filter filter {
            .can_id   = 0x00112200 | CAN_EFF_FLAG,
            .can_mask = 0x00FFFF00 | CAN_EFF_FLAG
        };


    setsockopt(fd_, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
    struct ifreq ifr {};
    std::strcpy(ifr.ifr_name, interface.data());
    if (::ioctl(fd_, SIOCGIFINDEX, &ifr) < 0) {
        close_socket();
        return std::error_code{errno, std::system_category()};
    }
    struct sockaddr_can addr {};
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
        return {std::error_code{errno, std::generic_category()}};
    }
    struct can_frame frame {};
    ::read(fd_, &frame, sizeof(frame));
    return {};
}

void can_reader::SocketCtl::close_socket() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

