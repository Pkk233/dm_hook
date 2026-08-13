// sample_utils.cpp
#include "sample_utils.h"

#include <cctype>
#include <cmath>
#include <cstdlib>

namespace sample {

Rgb parse_color(const std::string& hex) {
    // 边界 1：长度必须恰好为 6
    if (hex.size() != 6) {
        throw std::invalid_argument("color string must be exactly 6 hex chars");
    }
    // 边界 2：每个字符必须是十六进制字符
    for (char c : hex) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("color string contains non-hex character");
        }
    }

    const auto to_byte = [](const std::string& s, std::size_t i) -> unsigned char {
        return static_cast<unsigned char>(std::strtol(s.substr(i, 2).c_str(), nullptr, 16));
    };
    return Rgb{to_byte(hex, 0), to_byte(hex, 2), to_byte(hex, 4)};
}

int clamp(int value, int lo, int hi) {
    // 异常：区间非法
    if (lo > hi) {
        throw std::invalid_argument("lo must not be greater than hi");
    }
    if (value < lo) return lo;   // 边界：低于下限
    if (value > hi) return hi;   // 边界：高于上限
    return value;                // 正常：区间内
}

double safe_divide(double a, double b) {
    // 异常：除以 0
    if (b == 0.0) {
        throw std::domain_error("division by zero");
    }
    return a / b;
}

double distance(double x1, double y1, double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

} // namespace sample
