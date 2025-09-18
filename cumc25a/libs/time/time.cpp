module;
module Time;

import OriginData;
import stdx;

namespace Time
{
// Duration
Duration::Duration(Number s) : sec(s) {}

Number Duration::seconds() const
{
    return sec;
}

std::chrono::duration<long double> Duration::to_chrono() const
{
    return std::chrono::duration<long double>(static_cast<long double>(sec));
}

Duration& Duration::operator+=(Duration d)
{
    sec += d.sec;
    return *this;
}

Duration& Duration::operator-=(Duration d)
{
    sec -= d.sec;
    return *this;
}

Duration operator+(Duration a, Duration b)
{
    return Duration{ a.sec + b.sec };
}

Duration operator-(Duration a, Duration b)
{
    return Duration{ a.sec - b.sec };
}

Duration operator*(Duration a, Number k)
{
    return Duration{ a.sec * k };
}

Duration operator*(Number k, Duration a)
{
    return Duration{ a.sec * k };
}

Duration operator/(Duration a, Number k)
{
    return Duration{ a.sec / k };
}

Number operator/(Duration a, Duration b)
{
    return a.sec / b.sec;
}

// TimePoint
TimePoint::TimePoint(Number s) : sec(s) {}

TimePoint::TimePoint(Duration d) : sec(d.sec) {}

TimePoint Interval::getLo() const
{
    return lo;
}

TimePoint Interval::getHi() const
{
    return hi;
}

Duration TimePoint::since_epoch() const
{
    return Duration{ sec };
}

Number TimePoint::seconds() const
{
    return sec;
}

TimePoint operator+(TimePoint t, Duration d)
{
    return TimePoint{ t.since_epoch() + d };
}

TimePoint operator-(TimePoint t, Duration d)
{
    return TimePoint{ t.since_epoch() - d };
}

Duration operator-(TimePoint a, TimePoint b)
{
    return Duration{ a.since_epoch() - b.since_epoch() };
}


// Interval

Interval::Interval() : lo(TimePoint{ 0 }), hi(TimePoint{ 0 }) {}

Interval::Interval(TimePoint a, TimePoint b) : lo(a), hi(b)
{
    if (hi < lo)
    {
        std::swap(lo, hi);
    }
}

Interval Interval::from(TimePoint start, Duration len)
{
    if (!(len.sec > Number{}))
    {
        return Interval{}; // 非正长度 → 空
    }
    return Interval{ start, start + len };
}

Duration Interval::length() const
{
    return hi - lo;
}

bool Interval::empty() const
{
    return !(lo < hi);
} // [lo,hi)

bool Interval::contains(TimePoint t) const
{
    return !(t < lo) && (t < hi);
}

Duration union_total_length(const std::vector<Interval>& intervals, bool merge_adjacent)
{
    if (intervals.empty())
    {
        return Duration{ Number{} };
    }

    // 过滤空区间
    std::vector<Interval> v;
    v.reserve(intervals.size());
    for (const auto& iv : intervals)
    {
        if (!iv.empty())
        {
            v.push_back(iv);
        }
    }

    if (v.empty())
    {
        return Duration{ Number{} };
    }

    // 按“起点”排序
    std::sort(v.begin(), v.end(),
              [](const Interval& a, const Interval& b)
              {
                  return a.getLo() < b.getLo();
              });

    Number sum{}; // 使用 Number 的“零”
    TimePoint cur_lo = v[0].getLo();
    TimePoint cur_hi = v[0].getHi();

    for (size_t i = 1; i < v.size(); ++i)
    {
        const auto& iv = v[i];
        const bool overlap = (iv.getLo() < cur_hi);
        const bool adjacent = (!overlap) && (iv.getLo().since_epoch() == cur_hi.since_epoch());

        if (overlap || (merge_adjacent && adjacent))
        {
            if (cur_hi < iv.getHi())
            {
                cur_hi = iv.getHi();
            }
        }
        else
        {
            sum = sum + (cur_hi - cur_lo).sec;
            cur_lo = iv.getLo();
            cur_hi = iv.getHi();
        }
    }
    sum = sum + (cur_hi - cur_lo).sec;
    return Duration{ sum };
}

// ===== 便捷构造 =====
Time::Interval span(Time::TimePoint start, Time::Duration len)
{
    return Time::Interval::from(start, len);
}

Time::Interval bounds(Time::TimePoint a, Time::TimePoint b)
{
    return Time::Interval{ a, b };
}

} // namespace Time