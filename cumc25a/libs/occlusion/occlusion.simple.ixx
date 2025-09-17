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
    const Duration initialFindDt{0.5}; // ��ʼ��������

    // ���Ե�����"״̬�ı�"ʱ������,����Ϊ�����ṩ��ʼ��Χ
    pair<Interval, Interval> initialChangeIntervals() const;
  public:
    OcclusionModel(const Missile& _missile, const Cloud& _cloud);
    bool isOccluded(TimePoint t) const;
    // @dt:ʱ����־���
    // ��ǰ�����µ��ڵ�����
    Interval occlusionInterval(Duration dt) const;

};