module;
#include <cassert>
module Kinematics;

import stdx;
import OriginData;
import Time;

using namespace Time;
using namespace std;

Vector3 uniform_linear_motion(const Vector3& p0, const Vector3& v, TimePoint t)
{
    return p0 + v * t.since_epoch().seconds();
}

Vector3 uniform_acceleration_motion(const Vector3& p0, const Vector3& v0,
                                    const Vector3& a, TimePoint t)
{
    Number s = t.since_epoch().seconds();
    return p0 + v0 * s + a * (s * s * 0.5);
}

namespace Kinematics
{

// Missile
Kinematics::Missile::Missile(const MissilePosition& pos) : position(pos) {}

Vector3 Missile::getMissileVelocity() const
{
    Vector3 v = position;
    v = -v.normalized() * OriginData::missile_speed;
    return v;
}

Vector3 Kinematics::Missile::positionAtTime(TimePoint t) const
{
    return uniform_linear_motion(position, getMissileVelocity(), t);
}

TimePoint Missile::landingTime() const
{
    // t=|x| / |v|
    Vector3 pos = position;
    Number t = pos.norm() / OriginData::missile_speed;
    return TimePoint{ t };
}

// Uav
Uav::Uav(const UavPosition& pos, const Vector3& v, const TimePoint& time_rel)
    : position(pos), velocity(v), release_time(time_rel)
{
}

Vector3 Uav::positionAtTime(TimePoint t) const
{
    return uniform_linear_motion(position, velocity, t);
}

bool Uav::isReleased(TimePoint t) const
{
    return t >= release_time;
}

TimePoint Uav::getReleaseTime() const
{
    return release_time;
}

Vector3 Uav::releasePosition() const
{
    return positionAtTime(release_time);
}

Vector3 Uav::getVelocity() const
{
    return velocity;
}

// Jamming
Kinematics::Jamming::Jamming(const Uav& u, const TimePoint& _fuse_time)
    : uav(u), fuse_time(_fuse_time)
{
    assert(u.getReleaseTime() <= fuse_time);
}

optional<Vector3> Jamming::positionAtTime(TimePoint t) const
{
    if (!uav.isReleased(t))
    {
        return nullopt;
    }

    auto delta_t = TimePoint{ t - uav.getReleaseTime() };
    return uniform_acceleration_motion(uav.releasePosition(), uav.getVelocity(),
                                       OriginData::gravity, delta_t);
}

bool Jamming::isBomb(TimePoint t) const
{
    return t >= fuse_time && uav.isReleased(t);
}

TimePoint Jamming::getBombTime() const
{
    return fuse_time;
}

Vector3 Jamming::positionAtBomb() const
{
    auto pos = positionAtTime(fuse_time);
    assert(pos != nullopt);
    return pos.value();
}

// Cloud
Kinematics::Cloud::Cloud(const Jamming& j) : jamming(j) {}

bool Cloud::isExist(TimePoint t) const
{
    return (t >= jamming.getBombTime()) &&
           (t <= (jamming.getBombTime() + Duration{ OriginData::cloud_life }));
}

optional<Vector3> Cloud::centerAtTime(TimePoint t) const
{
    if (!isExist(t))
    {
        return nullopt;
    }
    auto ini_pos = jamming.positionAtBomb();
    auto delta_t = TimePoint{ t - jamming.getBombTime() };
    return uniform_linear_motion(ini_pos, OriginData::cloud_velocity,
                                 delta_t);
}

} // namespace Kinematics