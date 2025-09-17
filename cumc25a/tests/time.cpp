// main.cpp
import stdx;
using namespace std::chrono_literals;
import Time;

using namespace Time;
using namespace std;

int main()
{
    // �����ģ����� Number ��Ϊ double��������ֱ�Ӵ� double ����
    TimePoint t0{0.0};            // 0 s
    TimePoint t1{Duration(2s)};   // 2 s
    TimePoint t2{Duration(3.5s)}; // 3.5 s

    // �������䣨�뿪���� [lo, hi)��
    Interval a = bounds(t0, t1);                            // [0.0, 2.0)
    Interval b = Time::span(TimePoint(1.5), Duration(2s));  // [1.5, 3.5)
    Interval c = Time::span(TimePoint(4.0), Duration(0.5)); // [4.0, 4.5)

    std::vector<Interval> intervals{a, b, c};

    // ���㲢���ۼ�ʱ��
    Duration total = union_total_length(intervals); // Ĭ�� merge_adjacent=true
    std::cout << "Union total length = " << total.seconds() << " s\n";

    // ���������4.0 s
    // ���ͣ�a �� b = [0.0, 3.5)������ 3.5�����ٲ��� c = [4.0, 4.5)������ 0.5�����ܼ� 4.0��
}
