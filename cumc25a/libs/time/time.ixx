export module Time;

import OriginData;
import stdx;

export namespace Time
{
// 连续时间长度（单位：秒）
export class Duration
{
  public:
    Number sec{}; // 始终表示“秒”的数值

    // 构造
    explicit Duration(Number s);
    template <class Rep, class Period>
    explicit Duration(std::chrono::duration<Rep, Period> d) // 从任意 chrono::duration
        : sec(static_cast<Number>(std::chrono::duration<long double>(d).count()))
    {
    }

    // 访问/转换
    Number                             seconds() const;
    std::chrono::duration<long double> to_chrono() const;

    // 运算
    bool operator==(const Duration&) const = default;
    auto operator<=>(const Duration&)const =default;
    Duration& operator+=(Duration);
    Duration& operator-=(Duration);
};

Duration operator+(Duration, Duration);
Duration operator-(Duration, Duration);
Duration operator*(Duration, Number);
Duration operator*(Number, Duration);
Duration operator/(Duration, Number);
Number   operator/(Duration, Duration); // 比值（无量纲）

// 时刻
export class TimePoint
{
  private:
    Number sec{}; // 自 0 起的秒数
  public:
    explicit TimePoint(Number s);
    explicit TimePoint(Duration d);
    template <class Rep, class Period>
    explicit TimePoint(std::chrono::duration<Rep, Period> d) // 从任意 chrono::duration
        : TimePoint(Duration(d))
    {
    }

    // 访问
    Duration since_epoch() const;
    Number   seconds() const;

    // 比较
    bool operator==(const TimePoint&) const = default;
    auto operator<=>(const TimePoint&) const = default;
};

TimePoint operator+(TimePoint, Duration);
TimePoint operator-(TimePoint, Duration);
Duration  operator-(TimePoint, TimePoint);

// 左闭右开时间区间 [lo, hi)
export class Interval
{
  private:
    TimePoint lo;
    TimePoint hi;

  public:
    Interval();
    Interval(TimePoint a, TimePoint b);

    static Interval from(TimePoint start, Duration len); // 便捷：按长度构造 [start, start+len)

    Duration  length() const;
    bool      empty() const;               // 长度 0 视为空
    bool      contains(TimePoint t) const; // [lo, hi)
    TimePoint getLo() const;
    TimePoint getHi() const;
};

// ===== 计算并集总时长 =====
// intervals：若干 [lo,hi) 区间，顺序任意；函数内部会拷贝并排序/合并。
// merge_adjacent=true：端点相等的相邻区间也视为连通（[0,1)+[1,2) → [0,2)）。
export Duration union_total_length(const std::vector<Interval>& intervals, bool merge_adjacent = true);

// 便捷构造
export Interval span(TimePoint start, Duration len); // [start, start+len)
export Interval bounds(TimePoint a, TimePoint b);    // [min(a,b), max(a,b))
} // namespace Time