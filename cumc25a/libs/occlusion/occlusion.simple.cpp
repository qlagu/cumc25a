module;
#include <fmt/core.h>
#include <fmt/ranges.h>
module occlusion:simple_impl;

import :simple;
import :base;
import basic;

using namespace Kinematics;

// �ж������Ƿ��㹻С�����߶����ӣ����⾫�����⣩
template <class T>
inline bool close_interval(T lo, T hi, T eps)
{
    const T scale = std::max<T>(T(1), std::max(std::abs(lo), std::abs(hi)));
    return std::abs(hi - lo) <= eps * scale;
}

// ���������ֲ��ң�Ѱ�ҵ�һ������ pred ��λ��
template <class T>
T fbin_search_first_true(T lo, T hi, auto pred, T eps = T(1e-12),
                         int max_iter = 2000)
{
    if (hi < lo)
    {
        std::swap(lo, hi);
    }

    // ��� hi �����㣬���޽⣬���� hi ���ڱ�
    if (!pred(hi))
    {
        return hi;
    }

    for (int it = 0; it < max_iter && !close_interval(lo, hi, eps); ++it)
    {
        T mid = std::midpoint(lo, hi);
        if (pred(mid))
        {
            hi = mid; // �� -> �����ұ�
        }
        else
        {
            lo = mid; // �� -> ̧�����
        }
    }
    return hi;
}

OcclusionModel<OcclusionModelType::Simple>::OcclusionModel(
    const Missile& _missile, const Cloud& _cloud)
    : missile(_missile), cloud(_cloud)
{
}

pair<Interval, Interval> OcclusionModel<
    OcclusionModelType::Simple>::initialChangeIntervals() const
{
    vector<Interval> occlusion_intervals;
    Number up_side =
        min(missile.landingTime().seconds(), OriginData::cloud_life);
    bool last_time_point_occluded = isOccluded(TimePoint{ 0 });
    for (Number t = initialFindDt.seconds(); t <= up_side;
         t += initialFindDt.seconds()) // TODO
    {
        TimePoint tp{ t };
        bool occluded = isOccluded(tp);
        /*fmt::println("[t:{}][occluded:{}]", t, occluded);*/
        // ״̬�ı�
        if (occluded != last_time_point_occluded)
        {
            occlusion_intervals.push_back(
                Interval{ tp - initialFindDt, tp + initialFindDt });
        }
        last_time_point_occluded = occluded;
    }

    while (occlusion_intervals.size() < 2)
    {
        // ������ռλ
        occlusion_intervals.push_back(Interval());
    }

    return { occlusion_intervals[0], occlusion_intervals[1] };
}

bool OcclusionModel<OcclusionModelType::Simple>::isOccluded(TimePoint t) const
{
    if (!cloud.isExist(t))
    {
        return false;
    }
    auto center_opt = cloud.centerAtTime(t);
    if (!center_opt)
    {
        return false;
    }

    const Vector3 target_pos = OriginData::trueTarget.getCenter();
    const Vector3 missile_pos = missile.positionAtTime(t);
    const Vector3 cloud_center = *center_opt;

    const Number R = OriginData::cloud_radius;

    // �߶� T->M
    Vector3 sight_line_vec = missile_pos - target_pos;
    Number sight_line_len = sight_line_vec.norm();
    if (sight_line_len == Number(0))
    {
        // �˻���������Ŀ���غϣ���������Ŀ����Ƿ��������ڡ���
        return (cloud_center - target_pos).squaredNorm() <= R * R;
    }

    Vector3 v_hat = sight_line_vec / sight_line_len;

    // ��Ŀ��ָ�������ĵ�����
    Vector3 u = cloud_center - target_pos;

    // �з���ͶӰ��������Ŀ��Ϊ0�����򵼵�Ϊ����
    Number tparam = u.dot(v_hat);

    // �е��߶� [0, sight_line_len]
    Number tclamp = std::clamp<Number>(tparam, Number(0), sight_line_len);

    // �߶��ϵ������
    Vector3 nearest = target_pos + v_hat * tclamp;

    // �������ľ��루��ƽ������ sqrt��
    Number d2 = (cloud_center - nearest).squaredNorm();

    //fmt::println("[Func: isOccluded]");
    //fmt::println("[t:{}]", t.seconds());
    //fmt::println("[cloud_exist:{}]", cloud.isExist(t));
    //fmt::println("[center_opt:{}]", center_opt.has_value());
    //fmt::println("[target_pos:{}]", fmt::join(target_pos, ", "));
    //fmt::println("[missile_pos:{}]", fmt::join(missile_pos, ", "));
    //fmt::println("[cloud_center:{}]", fmt::join(cloud_center, ", "));
    //fmt::println("[cloud_radius:{}]", R);
    //fmt::println("[sight_line_vec:{}]", fmt::join(sight_line_vec, ", "));
    //fmt::println("[sight_line_len:{}]", sight_line_len);
    //fmt::println("[v_hat:{}]", fmt::join(v_hat, ", "));
    //fmt::println("[u:{}]", fmt::join(u, ", "));
    //fmt::println("[tparam:{}]", tparam);
    //fmt::println("[tclamp:{}]", tclamp);
    //fmt::println("[nearest:{}]", fmt::join(nearest, ", "));
    //fmt::println("[d2:{}]", d2);

    return d2 <= R * R;
}


Interval OcclusionModel<OcclusionModelType::Simple>::occlusionInterval(
    Duration dt) const
{
    auto toSearch = initialChangeIntervals();
    if (toSearch.first.empty() && toSearch.second.empty())
    {
        return Interval{};
    }

    auto start = fbin_search_first_true(toSearch.first.getLo().seconds(),
                                        toSearch.first.getHi().seconds(),
                                        [this](Number t)
                                        {
                                            // ��һ�Σ����ڵ�->�ڵ������ҵ�һ���ڵ�ʱ��
                                            return isOccluded(TimePoint{ t });
                                        });

    auto end = fbin_search_first_true(toSearch.second.getLo().seconds(),
                                      toSearch.second.getHi().seconds(),
                                      [this](Number t)
                                      {
                                          // �ڶ��Σ��ڵ�->���ڵ������ҵ�һ�����ڵ�ʱ��
                                          return !isOccluded(TimePoint{ t });
                                      });

    return Interval{ TimePoint{ start }, TimePoint{ end } };
}
