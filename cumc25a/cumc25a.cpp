#include <fmt/printf.h>
#include <Eigen/Dense>
import stdx;

int main()
{
	Eigen::Matrix2d A; A << 1, 2, 3, 4;
	Eigen::Vector2d b(5, 6);
	auto x = A * b;
	fmt::print("x = [{}, {}]\n", x(0), x(1));
	
	return 0;
}
