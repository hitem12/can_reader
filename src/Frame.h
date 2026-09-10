//
// Created by tomaszp on 10.09.2026.
//

#pragma once
#include <cstdint>
#include <array>
#include <span>
namespace can {
    struct Frame {
        std::uint32_t id;
        std::uint8_t  len;
        bool extended;
        std::array<std::uint8_t, 8> data;

        using value_type     = std::uint8_t;
        using iterator       = std::uint8_t*;
        using const_iterator = const std::uint8_t*;

        [[nodiscard]] iterator       begin()        noexcept { return data.data(); }
        [[nodiscard]] iterator       end()          noexcept { return data.data() + len; }
        [[nodiscard]] const_iterator begin()  const noexcept { return data.data(); }
        [[nodiscard]] const_iterator end()    const noexcept { return data.data() + len; }
        [[nodiscard]] const_iterator cbegin() const noexcept { return begin(); }
        [[nodiscard]] const_iterator cend()   const noexcept { return end(); }

        [[nodiscard]] std::size_t    size()   const noexcept { return len; }
        [[nodiscard]] bool           empty()  const noexcept { return len == 0; }
        std::uint8_t&       operator[](const std::size_t i)       noexcept { return data[i]; }
        const std::uint8_t& operator[](const std::size_t i) const noexcept { return data[i]; }

    };
}