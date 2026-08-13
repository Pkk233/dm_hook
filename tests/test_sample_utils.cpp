// test_sample_utils.cpp
// 针对 sample_utils 的测试用例，按「正常输入 / 边界值 / 异常情况」三类组织。
// 结构清晰：每个 TEST 只验证一个行为点，新增用例只需再写一个 TEST(...)。
#include "test_framework.h"
#include "sample_utils.h"

#include <cmath>

using namespace sample;

// ===================== parse_color：正常输入 =====================
TEST(parse_color_normal_uppercase) {
    auto c = parse_color("FF0000");
    EXPECT_EQ(c.r, 255);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST(parse_color_normal_lowercase) {
    auto c = parse_color("12ab34");
    EXPECT_EQ(c.r, 0x12);
    EXPECT_EQ(c.g, 0xab);
    EXPECT_EQ(c.b, 0x34);
}

TEST(parse_color_normal_mixed) {
    auto c = parse_color("A1B2C3");
    EXPECT_EQ(c.r, 0xA1);
    EXPECT_EQ(c.g, 0xB2);
    EXPECT_EQ(c.b, 0xC3);
}

// ===================== parse_color：边界值 =====================
TEST(parse_color_boundary_min) {
    auto c = parse_color("000000");
    EXPECT_EQ(c.r, 0);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST(parse_color_boundary_max) {
    auto c = parse_color("FFFFFF");
    EXPECT_EQ(c.r, 255);
    EXPECT_EQ(c.g, 255);
    EXPECT_EQ(c.b, 255);
}

// ===================== parse_color：异常情况 =====================
TEST(parse_color_invalid_empty) {
    EXPECT_THROW(parse_color(""), std::invalid_argument);
}

TEST(parse_color_invalid_length5) {
    EXPECT_THROW(parse_color("FF000"), std::invalid_argument);
}

TEST(parse_color_invalid_length7) {
    EXPECT_THROW(parse_color("FF0000F"), std::invalid_argument);
}

TEST(parse_color_invalid_nonhex) {
    EXPECT_THROW(parse_color("GGGGGG"), std::invalid_argument);
    EXPECT_THROW(parse_color("zzzzzz"), std::invalid_argument);
}

TEST(parse_color_invalid_with_prefix) {
    EXPECT_THROW(parse_color("#FF0000"), std::invalid_argument);
}

// ===================== clamp：正常输入 / 边界值 =====================
TEST(clamp_normal_middle) {
    EXPECT_EQ(clamp(5, 0, 10), 5);
}

TEST(clamp_boundary_equal_lo) {
    EXPECT_EQ(clamp(0, 0, 10), 0);
}

TEST(clamp_boundary_equal_hi) {
    EXPECT_EQ(clamp(10, 0, 10), 10);
}

TEST(clamp_boundary_below_lo) {
    EXPECT_EQ(clamp(-5, 0, 10), 0);
}

TEST(clamp_boundary_above_hi) {
    EXPECT_EQ(clamp(15, 0, 10), 10);
}

TEST(clamp_exception_lo_gt_hi) {
    EXPECT_THROW(clamp(5, 10, 0), std::invalid_argument);
}

// ===================== safe_divide：正常 / 边界 / 异常 =====================
TEST(safe_divide_normal) {
    EXPECT_EQ(safe_divide(10.0, 2.0), 5.0);
}

TEST(safe_divide_boundary_dividend_zero) {
    EXPECT_EQ(safe_divide(0.0, 5.0), 0.0);
}

TEST(safe_divide_boundary_fraction) {
    EXPECT_NEAR(safe_divide(1.0, 3.0), 0.333333, 1e-5);
}

TEST(safe_divide_exception_divzero) {
    EXPECT_THROW(safe_divide(5.0, 0.0), std::domain_error);
    EXPECT_THROW(safe_divide(0.0, 0.0), std::domain_error);
}

// ===================== distance：正常 / 边界 / 近似 =====================
TEST(distance_normal_pythagorean) {
    EXPECT_EQ(distance(0.0, 0.0, 3.0, 4.0), 5.0);
}

TEST(distance_boundary_same_point) {
    EXPECT_EQ(distance(2.0, 2.0, 2.0, 2.0), 0.0);
}

TEST(distance_boundary_diagonal) {
    EXPECT_NEAR(distance(0.0, 0.0, 1.0, 1.0), std::sqrt(2.0), 1e-9);
}

int main() {
    return run_all_tests();
}
