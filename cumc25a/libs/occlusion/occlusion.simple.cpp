module occlusion:simple;

import :base;
import basic;

using namespace Kinematics;

bool OcclusionModel<OcclusionModelType::Simple>::isOccluded(TimePoint t) const
{
    //line from missile to false_target
    const Vector3 direct = missile.getMissileVelocity().normalized();

    //center of cloud
    auto center_opt = cloud.centerAtTime(t);
    if (!cloud.isExist(t)||center_opt==nullopt)
    {
        return false;
    }
    auto center = center_opt.value();
    // calc distance of point to line
    Vector3 false_target = OriginData::falseTargetPositon;
    //from target to center
    auto center_pos_vec = center - false_target;
    Number projection_length = center_pos_vec.dot(center_pos_vec);

    Number a = projection_length, c = center_pos_vec.norm();
    Number distance = sqrt(c*c - a*a);

    return distance > OriginData::cloud_radius;
}