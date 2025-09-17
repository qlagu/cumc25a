export module occlusion:base;

enum class OcclusionModelType
{
	Simple,
    Advanced
};

export template <OcclusionModelType T>
class OcclusionModel;