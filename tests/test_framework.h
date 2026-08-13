// test_framework.h
// 一个零依赖的轻量 C++ 单元测试框架，便于直接编译运行，也方便替换为 GoogleTest。
//
// 用法：
//   1) 在测试 .cpp 中 #include "test_framework.h" 和被测头文件
//   2) 用 TEST(name) { ... } 定义用例
//   3) 在文件末尾写 int main() { return run_all_tests(); }
//
// 断言宏：
//   EXPECT_TRUE(expr)       表达式为真
//   EXPECT_FALSE(expr)      表达式为假
//   EXPECT_EQ(a, b)         a == b
//   EXPECT_NEAR(a, b, eps)  |a - b| <= eps（浮点）
//   EXPECT_THROW(expr, T)  expr 抛出类型为 T 的异常
//   EXPECT_NO_THROW(expr)   expr 不抛异常
//
// 失败仅计入计数、不终止用例，便于一次看到所有失败点（与 assert() 不同）。
#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& test_registry() {
    static std::vector<TestCase> r;
    return r;
}

struct TestRegistrar {
    TestRegistrar(const std::string& name, std::function<void()> fn) {
        test_registry().push_back({name, std::move(fn)});
    }
};

inline int& tf_passed() { static int p = 0; return p; }
inline int& tf_failed() { static int f = 0; return f; }

// 定义一条测试用例（静态函数 + 自动注册），供测试 .cpp 使用
#define TEST(name)                                           \
    static void name();                                     \
    static ::TestRegistrar _reg_##name(#name, name);        \
    static void name()

#define EXPECT_TRUE(expr)                                                  \
    do {                                                                   \
        if (!(expr)) {                                                     \
            std::cerr << "  [FAIL] " #expr "  @ " << __FILE__ << ":"       \
                      << __LINE__ << "\n";                                 \
            ::tf_failed()++;                                               \
        } else {                                                           \
            ::tf_passed()++;                                               \
        }                                                                  \
    } while (0)

#define EXPECT_FALSE(expr) EXPECT_TRUE(!(expr))

#define EXPECT_EQ(a, b) EXPECT_TRUE((a) == (b))

#define EXPECT_NEAR(a, b, eps) EXPECT_TRUE(std::abs((a) - (b)) <= (eps))

#define EXPECT_THROW(expr, ex)                                            \
    do {                                                                   \
        bool _thrown = false;                                             \
        try { (void)(expr); }                                             \
        catch (const ex&) { _thrown = true; }                             \
        catch (...) {}                                                     \
        if (!_thrown) {                                                   \
            std::cerr << "  [FAIL] " #expr " did not throw " #ex          \
                      "  @ " << __FILE__ << ":" << __LINE__ << "\n";      \
            ::tf_failed()++;                                              \
        } else {                                                           \
            ::tf_passed()++;                                              \
        }                                                                  \
    } while (0)

#define EXPECT_NO_THROW(expr)                                             \
    do {                                                                   \
        try { (void)(expr); ::tf_passed()++; }                           \
        catch (...) {                                                     \
            std::cerr << "  [FAIL] " #expr " threw unexpectedly"          \
                      "  @ " << __FILE__ << ":" << __LINE__ << "\n";      \
            ::tf_failed()++;                                              \
        }                                                                  \
    } while (0)

// 运行所有已注册用例并打印汇总，返回 0 表示全部通过
inline int run_all_tests() {
    for (auto& t : test_registry()) {
        std::cout << "[RUN] " << t.name << "\n";
        t.fn();
    }
    std::cout << "\n========================================\n";
    std::cout << "  PASSED: " << ::tf_passed() << "   FAILED: " << ::tf_failed() << "\n";
    std::cout << "========================================\n";
    return ::tf_failed() == 0 ? 0 : 1;
}
