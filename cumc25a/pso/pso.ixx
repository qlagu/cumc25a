module;
#include <Eigen/Core>
export module PSO;

import basic;

template <class T>
concept FieldLike =                           // ����ṹ����ֵ����
    std::numeric_limits<T>::is_specialized && // �� numeric_limits
    requires(T a, T b, double d) {
        // �ɴ� double ���죨�����ʽת����
        { T(d) } -> std::same_as<T>;

        // ������
        { a + b } -> std::same_as<T>;
        { a - b } -> std::same_as<T>;
        { a * b } -> std::same_as<T>;
        { a / b } -> std::same_as<T>;
        { -a } -> std::same_as<T>;

        // �Ƚϣ�PSO �������ضϡ����űȽϵȣ�
        { a < b } -> std::convertible_to<bool>;
        { a <= b } -> std::convertible_to<bool>;
        { a == b } -> std::convertible_to<bool>;
    };

struct PSOOptions
{
    int swarm_size = 40;
    int max_iters = 1000;
    Number w_start = 0.9;
    Number w_end = 0.4;
    Number c1 = 2.05;
    Number c2 = 2.05;
    bool use_constriction = true; // Clerc-Kennedy ��
    Number vmax_fac = 0.2;        // vmax = vmax_fac * (ub-lb)
    int stall_iters = 150;        // <=0 �ر���ͣ
    uint64_t seed = std::random_device{}();
};

template <unsigned int Dim, FieldLike RT>
class Particle
{
  public:
    using VectorD = Eigen::Matrix<Number, Dim, 1>;
    VectorD position;
    VectorD velocity;
    VectorD best_position;
    RT best_value = std::numeric_limits<RT>::infinity();

    Particle()
        : position(VectorD::Zero()), velocity(VectorD::Zero()),
          best_position(VectorD::Zero())
    {
    }
};

template <unsigned int Dim>
struct PSOResult
{
    Eigen::VectorXd best_position;
    Number best_value;
    int iterations;
    bool converged;
};

template <unsigned int Dim>
struct Bounds
{
    Eigen::Matrix<Number, Dim, 1> lb;
    Eigen::Matrix<Number, Dim, 1> ub;
};

template <unsigned int Dim, FieldLike RT>
class PSO
{
  public:
    using VectorD = Eigen::Matrix<Number, Dim, 1>;
    using ParticleD = Particle<Dim>;
    using ObjectiveFunc = std::function<RT(const VectorD&)>;
    using BoundsD = Bounds<Dim>;
  private:
    PSOOptions options;
    ObjectiveFunc objective;
    BoundsD bounds;

    Number omega;                // ʵʱ����Ȩ��
    Vecotr<ParticleD> particles; // ����Ⱥ
    VectorD global_best_position;
    RT global_best_value = std::numeric_limits<RT>::infinity();

  public:
    PSO(const PSOOptions& opts, ObjectiveFunc obj,BoundsD bounds);
    void initialParticle(); // �����ʼ������Ⱥ
    void initialParticle(
        const std::vector<VectorD>& init_pos); // ��ָ��λ�ó�ʼ������Ⱥ
    PSOResult<Dim> optimize(const VectorD& lb, const VectorD& ub); 
};
