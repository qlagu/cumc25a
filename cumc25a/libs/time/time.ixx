export module Time;

import OriginData;
import stdx;

export namespace Time
{
// ����ʱ�䳤�ȣ���λ���룩
export class Duration
{
  public:
    Number sec{}; // ʼ�ձ�ʾ���롱����ֵ

    // ����
    explicit Duration(Number s);
    template <class Rep, class Period>
    explicit Duration(std::chrono::duration<Rep, Period> d) // ������ chrono::duration
        : sec(static_cast<Number>(std::chrono::duration<long double>(d).count()))
    {
    }

    // ����/ת��
    Number                             seconds() const;
    std::chrono::duration<long double> to_chrono() const;

    // ����
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
Number   operator/(Duration, Duration); // ��ֵ�������٣�

// ʱ��
export class TimePoint
{
  private:
    Number sec{}; // �� 0 �������
  public:
    explicit TimePoint(Number s);
    explicit TimePoint(Duration d);
    template <class Rep, class Period>
    explicit TimePoint(std::chrono::duration<Rep, Period> d) // ������ chrono::duration
        : TimePoint(Duration(d))
    {
    }

    // ����
    Duration since_epoch() const;
    Number   seconds() const;

    // �Ƚ�
    bool operator==(const TimePoint&) const = default;
    auto operator<=>(const TimePoint&) const = default;
};

TimePoint operator+(TimePoint, Duration);
TimePoint operator-(TimePoint, Duration);
Duration  operator-(TimePoint, TimePoint);

// ����ҿ�ʱ������ [lo, hi)
export class Interval
{
  private:
    TimePoint lo;
    TimePoint hi;

  public:
    Interval();
    Interval(TimePoint a, TimePoint b);

    static Interval from(TimePoint start, Duration len); // ��ݣ������ȹ��� [start, start+len)

    Duration  length() const;
    bool      empty() const;               // ���� 0 ��Ϊ��
    bool      contains(TimePoint t) const; // [lo, hi)
    TimePoint getLo() const;
    TimePoint getHi() const;
};

// ===== ���㲢����ʱ�� =====
// intervals������ [lo,hi) ���䣬˳�����⣻�����ڲ��´��������/�ϲ���
// merge_adjacent=true���˵���ȵ���������Ҳ��Ϊ��ͨ��[0,1)+[1,2) �� [0,2)����
export Duration union_total_length(const std::vector<Interval>& intervals, bool merge_adjacent = true);

// ��ݹ���
export Interval span(TimePoint start, Duration len); // [start, start+len)
export Interval bounds(TimePoint a, TimePoint b);    // [min(a,b), max(a,b))
} // namespace Time