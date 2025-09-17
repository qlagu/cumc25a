module;
export module kinematics;

import OriginData;
import Time;

using namespace Time;

Vector3 uniform_linear_motion(const Vector3& p0, const Vector3& v, TimePoint t);

Vector3 uniform_acceleration_motion(const Vector3& p0, const Vector3& v0, const Vector3& a, TimePoint t);

export namespace kinematics
{

export class Missile
{
  private:
    const MissilePosition& position;
    Vector3 getMissileVelocity() const; // 获取导弹速度矢量

  public:
    Missile(const MissilePosition& pos);
    Vector3 positionAtTime(TimePoint t) const;
    TimePoint landingTime() const; // 落地时间
};

export class Uav
{
  private:
    const UavPosition& position;
    const Vector3 velocity;       // 速度
    const TimePoint release_time; // 投弹时刻（释放时刻）
  public:
    Uav(const UavPosition& pos, const Vector3& v, const TimePoint& time_rel);

    Vector3 positionAtTime(TimePoint t) const;
    bool isReleased(TimePoint t) const; // 在时间t时是否已经投弹
    TimePoint getReleaseTime() const;   // 距离投弹还剩多少时间
};

export class Jamming
{
  private:
    const Uav uav;
    const TimePoint fuse_time; // 引信时刻
  public:
    Jamming(const Uav& u, const TimePoint& _fuse_time);

    Vector3 positionAtTime(TimePoint t) const;
    bool isBomb(TimePoint t) const; // 在时间t时是否已经起爆
    TimePoint timeToBomb() const;   // 距离起爆还剩多少时间
    Vector3 positionAtBomb() const; // 起爆时的位置
};

class Cloud
{
  private:
    const Jamming jamming;

  public:
    Cloud(const Jamming& j);
    bool isExist(TimePoint t) const;         // 在时间t时云雾是否有效
    Vector3 centerAtTime(TimePoint t) const; // 云雾中心在时间t时的位置
};

} // namespace kinematics
