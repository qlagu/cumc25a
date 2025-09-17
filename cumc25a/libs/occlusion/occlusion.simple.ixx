export module occlusion:simple;
export import :base;
import basic;

using namespace Kinematics;
using namespace Time;
using namespace std;

export template <>
class OcclusionModel<OcclusionModelType::Simple>
{
  private:
    Missile missile;
    Cloud cloud;
    const Duration initialFindDt{0.5}; // 初始遍历步长

    // 粗略的两次"状态改变"时间区间,用于为二分提供初始范围
    pair<Interval, Interval> initialChangeIntervals() const;
  public:
    OcclusionModel(const Missile& _missile, const Cloud& _cloud);
    bool isOccluded(TimePoint t) const;
    // @dt:时间二分精度
    // 当前条件下的遮挡区间
    Interval occlusionInterval(Duration dt) const;

};