// sample_utils.h
// 演示用的示例函数集合（与 dm_hook 图色/坐标场景相关，但逻辑保持简单独立）。
// 目的：为「函数调用的测试代码」提供被测对象，覆盖：
//   - 正常输入 / 返回值
//   - 边界值（最小/最大、相同点、极小商）
//   - 异常（非法参数抛异常）
#pragma once

#include <string>
#include <stdexcept>

namespace sample {

// 把 "RRGGBB" 十六进制颜色串解析为 RGB（0~255）。
// 非法输入抛 std::invalid_argument：
//   - 长度不为 6
//   - 含非十六进制字符
//   - 带前缀（如 "#FF0000"）
struct Rgb {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

Rgb parse_color(const std::string& hex);

// 把 value 限制在 [lo, hi] 区间内。
// 若 lo > hi 视为非法参数，抛 std::invalid_argument。
int clamp(int value, int lo, int hi);

// 安全除法：除数为 0 抛 std::domain_error，否则返回 a / b。
double safe_divide(double a, double b);

// 两点欧氏距离（用于演示浮点近似断言 EXPECT_NEAR）。
double distance(double x1, double y1, double x2, double y2);

} // namespace sample
