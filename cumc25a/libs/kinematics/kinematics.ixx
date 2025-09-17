module;
export module Kinematics;

import OriginData;
import Time;

using namespace Time;
using namespace std;

Vector3 uniform_linear_motion(const Vector3& p0, const Vector3& v, TimePoint t);

Vector3 uniform_acceleration_motion(const Vector3& p0, const Vector3& v0,
                                    const Vector3& a, TimePoint t);

export namespace Kinematics
{

export class Missile
{
  private:
    const MissilePosition& position;

  public:
    Missile(const MissilePosition& pos);
    Vector3 positionAtTime(TimePoint t) const;
    TimePoint landingTime() const; // ���ʱ��
    Vector3 getMissileVelocity() const; // ��ȡ�����ٶ�ʸ��
};

export class Uav
{
  private:
    const UavPosition& position;
    const Vector3 velocity;       // �ٶ�
    const TimePoint release_time; // Ͷ��ʱ�̣��ͷ�ʱ�̣�
  public:
    Uav(const UavPosition& pos, const Vector3& v, const TimePoint& time_rel);

    Vector3 positionAtTime(TimePoint t) const;
    bool isReleased(TimePoint t) const; // ��ʱ��tʱ�Ƿ��Ѿ�Ͷ��
    TimePoint getReleaseTime() const;   // ����Ͷ����ʣ����ʱ��
    Vector3 releasePosition() const;    // Ͷ��λ��
    Vector3 getVelocity() const;
};

export class Jamming
{
  private:
    const Uav uav;
    const TimePoint fuse_time; // ����ʱ��
  public:
    Jamming(const Uav& u, const TimePoint& _fuse_time);

    optional<Vector3> positionAtTime(TimePoint t) const;
    bool isBomb(TimePoint t) const; // ��ʱ��tʱ�Ƿ��Ѿ���
    TimePoint getBombTime() const;  // ��ʱ��
    Vector3 positionAtBomb() const; // ��ʱ��λ��
};

class Cloud
{
  private:
    const Jamming jamming;

  public:
    Cloud(const Jamming& j);
    // ��ʱ��tʱ�����Ƿ���Ч
    bool isExist(TimePoint t) const;
    // ����������ʱ��tʱ��λ��
    optional<Vector3> centerAtTime(TimePoint t) const;
};

} // namespace kinematics
