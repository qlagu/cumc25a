#include<fmt/core.h>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<iostream>
import basic;
import occlusion;

using namespace Kinematics;
using namespace Time;
int main()
{
    Missile missile(OriginData::missile1);
    Vector3 uav1_p = OriginData::uav1;
    Vector3 false_target_p = OriginData::falseTargetPositon;
    Vector3 uav_vec = uav1_p - false_target_p;
    //限制到等高面
    uav_vec.z() = 0;
    // 设定120m/s
    uav_vec.normalize();
    uav_vec *= -120;

    Uav uav(OriginData::uav1, uav_vec, TimePoint{1.5});
    Jamming jamming(uav, TimePoint{1.5+3.6});
    Cloud cloud(jamming);

    auto opt = cloud.centerAtTime(TimePoint{ 1.5 + 3.6 });
    auto vec = opt.value();
    fmt::print("uav veloctity [{},{},{}] \n", uav_vec.x(), uav_vec.y(), uav_vec.z());
    fmt::print("cloud center at t=5.1s: [{},{},{}]\n", vec.x(), vec.y(),
               vec.z());

    OcclusionModel<OcclusionModelType::Simple> model(missile, cloud);
    auto interval = model.occlusionInterval(Duration{0.01});
    fmt::println("[{},{}]", interval.getLo().seconds(), interval.getHi().seconds());
    fmt::println("occlusion duration: {}", interval.length().seconds());
    return 0;
}