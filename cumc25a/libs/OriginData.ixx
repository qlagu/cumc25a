module;
#include <Eigen/Core>
#include <Eigen/Dense>

export module OriginData;
import Time;

// 预留，可能要改为可记录计算时精度损失的数据类型
export using Vector3 = Eigen::Matrix<Number, 3, 1>;

// 假目标位置
export class FalseTarget
{
  private:
    const Vector3 position;
    // 私有构造函数，防止外部实例化
    FalseTarget(const Vector3& pos) : position(pos) {};
    FalseTarget(Number x, Number y, Number z) : position(x, y, z) {};

    friend struct FalseTargetFactory;

  public:
    // 隐式转换转换为Eigen::Vector3d的操作符
    operator const Vector3&() const
    {
        return position;
    };
};

struct FalseTargetFactory
{
    static inline const FalseTarget false_target{0, 0, 0};
};

// 真目标位置及尺寸
export class TrueTarget
{
  private:
    const Vector3 center;
    const Number  radius;
    const Number  height;
    // 私有构造函数，防止外部实例化
    TrueTarget(const Vector3& pos, Number r, Number h) : center(pos), radius(r), height(h) {};
    TrueTarget(Number x, Number y, Number z, Number r, Number h) : center(x, y, z), radius(r), height(h) {};

  public:
    friend struct TrueTargetFactory;
    auto getCenter() const
    {
        return center;
    }
    auto getRadius() const
    {
        return radius;
    }
    auto getHeight() const
    {
        return height;
    }
};

struct TrueTargetFactory
{
    static inline const TrueTarget true_target{0, 200, 0, 7, 10};
};

export class MissilePosition
{
  private:
    const Vector3 position;
    // 私有构造函数，防止外部实例化
    MissilePosition(const Vector3& pos) : position(pos) {};
    MissilePosition(Number x, Number y, Number z) : position(x, y, z) {};

  public:
    friend struct MissilePositionFactory;
    // 隐式转换转换为Eigen::Vector3d的操作符
    operator const Vector3&() const
    {
        return position;
    };
};

struct MissilePositionFactory
{
    static inline const MissilePosition M1{20000, 0, 2000};
    static inline const MissilePosition M2{19000, 600, 2100};
    static inline const MissilePosition M3{18000, -600, 1900};
    static inline const Number          speed = 300;
};

export class UavPosition
{
  private:
    const Vector3 position;
    // 私有构造函数，防止外部实例化
    UavPosition(const Vector3& pos) : position(pos) {};
    UavPosition(Number x, Number y, Number z) : position(x, y, z) {};

  public:
    friend struct UavPositionFactory;
    // 隐式转换转换为Eigen::Vector3d的操作符
    operator const Vector3&() const
    {
        return position;
    };
};

struct UavPositionFactory
{
    static inline const UavPosition fy1{17800, 0, 1800};
    static inline const UavPosition fy2{12000, 1400, 1400};
    static inline const UavPosition fy3{6000, -3000, 700};
    static inline const UavPosition fy4{11000, 2000, 1800};
    static inline const UavPosition fy5{13000, -2000, 1300};
    static inline const Number      min_speed = 70;
    static inline const Number      max_speed = 120;
};

export namespace OriginData
{
export inline const auto& falseTargetPositon = FalseTargetFactory::false_target;
export inline const auto& trueTarget         = TrueTargetFactory::true_target;

export inline const auto& missile1      = MissilePositionFactory::M1;
export inline const auto& missile2      = MissilePositionFactory::M2;
export inline const auto& missile3      = MissilePositionFactory::M3;
export inline const auto& missile_speed = MissilePositionFactory::speed;

export inline const auto& uav1          = UavPositionFactory::fy1;
export inline const auto& uav2          = UavPositionFactory::fy2;
export inline const auto& uav3          = UavPositionFactory::fy3;
export inline const auto& uav4          = UavPositionFactory::fy4;
export inline const auto& uav5          = UavPositionFactory::fy5;
export inline const auto& uav_min_speed = UavPositionFactory::min_speed;
export inline const auto& uav_max_speed = UavPositionFactory::max_speed;

export inline constexpr const Number jamming_interval = 1; // 每架无人机投弹间隔
export inline const Vector3          gravity{0, 0, -9.81}; // 重力加速度

export inline constexpr const Number cloud_radius = 10;        // 云雾半径
export inline const Time::Duration cloud_life{20.0};   // 云雾持续时间
export inline const Vector3          cloud_velocity{0, 0, -3}; // 云雾速度
}; // namespace OriginData