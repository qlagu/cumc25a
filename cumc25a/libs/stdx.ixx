export module stdx;

#if defined(__INTELLISENSE__) || (!defined(__cpp_modules) && !defined(_MSVC_MODULES)) ||                               \
    defined(__clang_analyzer__) || defined(__CUDACC_RTC__)

// ---- 仅供 IntelliSense 使用的头文件 ----
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <cmath>
#include <compare>
#include <concepts>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <cstdint> // 添加此行以定义 uint64_t
#include <map>
#include <memory>
#include <mutex>
#include <numbers> // C++20 数学常数
#include <numeric>
#include <optional>
#include <print>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <shared_mutex>
#include <span>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <cassert>
#include <cstddef>

#else

// ---- 真正编译时使用 C++23 模块化标准库 ----
export import std;

#endif
