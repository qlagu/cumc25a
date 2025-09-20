#include <fmt/printf.h>
#include <Eigen/Dense>
import stdx;
import OriginData;

int main()
{
    Vector3 v1{ 1, 2, 3 };
    auto p = OriginData::cloud_life;
    fmt::print("cloud life: {}\n", p.seconds());
    fmt::print("v1: [{}, {}, {}]\n", v1[0], v1[1], v1[2]);
	return 0;
}
