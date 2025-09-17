// main.cpp
import stdx;
using namespace std::chrono_literals;
import Time;

using namespace Time;
using namespace std;

int main()
{
    // 若你的模块里把 Number 设为 double，则下面直接传 double 即可
    TimePoint t0{0.0};            // 0 s
    TimePoint t1{Duration(2s)};   // 2 s
    TimePoint t2{Duration(3.5s)}; // 3.5 s

    // 构造区间（半开区间 [lo, hi)）
    Interval a = bounds(t0, t1);                            // [0.0, 2.0)
    Interval b = Time::span(TimePoint(1.5), Duration(2s));  // [1.5, 3.5)
    Interval c = Time::span(TimePoint(4.0), Duration(0.5)); // [4.0, 4.5)

    std::vector<Interval> intervals{a, b, c};

    // 计算并集累计时长
    Duration total = union_total_length(intervals); // 默认 merge_adjacent=true
    std::cout << "Union total length = " << total.seconds() << " s\n";

    // 期望输出：4.0 s
    // 解释：a ∪ b = [0.0, 3.5)（长度 3.5），再并上 c = [4.0, 4.5)（长度 0.5），总计 4.0。
}
