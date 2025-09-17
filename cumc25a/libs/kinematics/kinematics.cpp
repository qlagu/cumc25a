module kinematics;

import stdx;
import OriginData;
import Time;

using namespace Time;

Vector3 uniform_linear_motion(const Vector3& p0, const Vector3& v, TimePoint t)
{
    return p0 + v * t.since_epoch().seconds();
}

Vector3 uniform_acceleration_motion(const Vector3& p0, const Vector3& v0, const Vector3& a, TimePoint t)
{
    Number s = t.since_epoch().seconds();
    return p0 + v0 * s + a * (s * s * 0.5);
}

namespace kinematics
{

// Missile
kinematics::Missile::Missile(const MissilePosition& pos) : position(pos) {}

Vector3 Missile::getMissileVelocity() const
{
    Vector3 v = position;
    v = v.normalized() * OriginData::missile_speed;
    return v;
}

Vector3 kinematics::Missile::positionAtTime(TimePoint t) const
{
    return uniform_linear_motion(position, getMissileVelocity(), t);
}

TimePoint Missile::landingTime() const
{
    //t=|x| / |v|
    Vector3 pos = position;
    Number t = pos.norm() / OriginData::missile_speed;
    return TimePoint{ t };
}


//Uav
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

kinematics::Jamming::Jamming(const Uav& u, const TimePoint& _fuse_time) : 
uav(u), fuse_time(_fuse_time)
{}


} // namespace kinematics