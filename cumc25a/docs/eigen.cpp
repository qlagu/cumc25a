// g++ -O2 -march=native -std=c++17 main.cpp && ./a.out
#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace std;
using namespace Eigen;

int main()
{
    cout.setf(std::ios::fixed);
    cout.precision(6);

    // ===== 1) 基础类型与创建 =====
    Matrix3d A; // 3x3, double
    A << 2, -1, 0, -1, 2, -1, 0, -1, 2;

    Vector3d b(1, 0, 1);                           // 3x1
    MatrixXd M = MatrixXd::Zero(2, 3);             // 动态矩阵
    VectorXd v = VectorXd::LinSpaced(5, 1.0, 5.0); // [1..5]

    cout << "[A]\n" << A << "\n\n";
    cout << "[b]\n" << b.transpose() << "\n\n";
    cout << "[M Zero]\n" << M << "\n\n";
    cout << "[v 1..5]\n" << v.transpose() << "\n\n";

    // ===== 2) 基本运算 =====
    cout << "A * b = " << (A * b).transpose() << "\n";
    cout << "A^T:\n" << A.transpose() << "\n";
    cout << "det(A) = " << A.determinant() << "\n\n";

    // 切块 / 取子矩阵
    MatrixXd R = MatrixXd::Random(4, 4);
    cout << "[R]\n" << R << "\n\n";
    cout << "topLeftCorner(2,2):\n" << R.topLeftCorner(2, 2) << "\n\n";
    cout << "block(1,1,2,3):\n" << R.block(1, 1, 2, 3) << "\n\n";

    // ===== 3) Array 逐元素运算 / 广播 =====
    MatrixXd X = MatrixXd::Constant(2, 3, 2.0);
    MatrixXd Y = MatrixXd::Random(2, 3);
    cout << "Hadamard X .* Y:\n" << (X.array() * Y.array()) << "\n\n";
    cout << "exp(Y):\n" << Y.array().exp() << "\n\n";
    // 广播（把列向量加到每列）
    VectorXd shift = VectorXd::LinSpaced(Y.rows(), 0.1, 0.1 * Y.rows());
    cout << "Y + shift (broadcast rows):\n" << (Y.colwise() + shift) << "\n\n";

    // ===== 4) 归约 / 范数 / 正交化 =====
    cout << "Y.sum() = " << Y.sum() << ", Y.mean() = " << Y.mean() << ", ||Y||_F = " << Y.norm() << "\n\n";
    // 每列中心化
    MatrixXd Yc = Y.colwise() - Y.rowwise().mean().transpose();
    cout << "column-centered Y:\n" << Yc << "\n\n";

    // ===== 5) 线性方程求解（不要直接 inverse）=====
    // 对称正定 → LLT / LDLT；一般方阵 → PartialPivLU / FullPivLU；最小二乘 → QR/SVD
    Vector3d x1 = A.ldlt().solve(b);
    cout << "Solve (SPD) A x = b  via LDLT: x = " << x1.transpose() << "\n";
    Vector3d x2 = A.colPivHouseholderQr().solve(b);
    cout << "Solve (general) via colPivHouseholderQr: x = " << x2.transpose() << "\n\n";

    // ===== 6) 特征值 / SVD =====
    SelfAdjointEigenSolver<Matrix3d> es(A);
    cout << "eigvals(A): " << es.eigenvalues().transpose() << "\n";
    cout << "eigvecs(A):\n" << es.eigenvectors() << "\n\n";

    MatrixXd B = MatrixXd::Random(4, 3);
    JacobiSVD<MatrixXd> svd(B, ComputeThinU | ComputeThinV);
    cout << "SVD(B): singular values = " << svd.singularValues().transpose() << "\n";
    cout << "rank(B) ~ " << svd.rank() << "\n\n";

    // ===== 7) 最小二乘 =====
    // min ||C*x - d||_2
    MatrixXd C = MatrixXd::Random(6, 3);
    VectorXd d = VectorXd::Random(6);
    VectorXd x_ls = C.colPivHouseholderQr().solve(d);
    cout << "Least Squares x = " << x_ls.transpose() << ", residual = " << (C * x_ls - d).norm() << "\n\n";

    // ===== 8) 几何：旋转/变换（3D）=====
    Vector3d p(1, 0, 0);
    AngleAxisd Rz(M_PI / 4, Vector3d::UnitZ()); // 绕Z轴 45°
    Vector3d p_rot = Rz * p;
    cout << "Rotate p by 45deg about Z: " << p_rot.transpose() << "\n";
    // 仿射变换：先旋转再平移
    Affine3d T = Translation3d(1, 2, 3) * Rz;
    Vector3d p_tf = T * p;
    cout << "Affine T * p: " << p_tf.transpose() << "\n\n";
    // 四元数
    Quaterniond q(Rz);
    cout << "Quaternion (w,x,y,z): " << q.w() << " " << q.x() << " " << q.y() << " " << q.z() << "\n\n";

    // ===== 9) Map：零拷贝地包裹外部数据 =====
    double raw[6] = { 1, 2, 3, 4, 5, 6 };
    Map<Matrix<double, 2, 3, RowMajor>> Mraw(raw); // 原始数据按行主序
    cout << "Map Mraw(2x3):\n" << Mraw << "\n\n";
    // 修改将回写到 raw
    Mraw(0, 0) = 9;
    cout << "raw[0] = " << raw[0] << " (should be 9)\n\n";

    // ===== 10) 组合赋值与 noalias()（避免临时与自赋值错用）=====
    MatrixXd P = MatrixXd::Random(100, 100);
    MatrixXd Q = MatrixXd::Random(100, 100);
    MatrixXd Z(100, 100);
    Z.noalias() = P * Q; // 告诉 Eigen: 目标与操作数无别名，可优化

    // ===== 11) 稀疏（简要）=====
    {
        typedef SparseMatrix<double> SpMat;
        typedef Triplet<double> Tpl;
        vector<Tpl> trips;
        const int n = 5;
        // 三对角
        for (int i = 0; i < n; i++)
        {
            if (i > 0)
                trips.emplace_back(i, i - 1, -1);
            trips.emplace_back(i, i, 2);
            if (i + 1 < n)
                trips.emplace_back(i, i + 1, -1);
        }
        SpMat S(n, n);
        S.setFromTriplets(trips.begin(), trips.end());

        VectorXd rhs = VectorXd::Ones(n);
        SimplicialLDLT<SpMat> sldlt;
        sldlt.compute(S);
        VectorXd xs = sldlt.solve(rhs);
        cout << "Sparse solve (tri-diag) x^T = " << xs.transpose() << "\n\n";
    }

    // ===== 12) 布尔/比较与选择 =====
    MatrixXd Zm = MatrixXd::Random(3, 3);
    ArrayXXd Za = Zm.array();
    ArrayXXd mask = (Za > 0.0);
    ArrayXXd picked = mask.select(Za, 0.0); // 正数保留，负数置0
    cout << "pick positives:\n" << picked.matrix() << "\n\n";

    // ===== 13) 行列操作（归一化 / 排序索引）=====
    MatrixXd D = MatrixXd::Random(3, 4);
    // 每列 L2 归一化
    D = D.colwise().normalized();
    cout << "colwise normalized:\n" << D << "\n\n";
    // argmax（逐列）
    for (int j = 0; j < D.cols(); ++j)
    {
        Eigen::Index idx;
        D.col(j).array().abs().maxCoeff(&idx);
        cout << "col " << j << " argmax |.| = " << idx << "\n";
    }
    cout << "\n";

    // ===== 14) 类型/存储布局（RowMajor 示例）=====
    Matrix<double, 2, 3, RowMajor> RM; // 行主序
    RM << 1, 2, 3, 4, 5, 6;
    cout << "RowMajor RM:\n" << RM << "\n\n";

    // ===== 15) 数值稳定小技巧：log-sum-exp =====
    VectorXd z = VectorXd::LinSpaced(4, -10, 5);
    double mmax = z.maxCoeff();
    double lse = mmax + (z.array() - mmax).exp().sum();
    lse = std::log(lse);
    cout << "log-sum-exp(z) = " << lse << "\n";

    return 0;
}
