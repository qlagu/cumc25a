export module occlusion:base;

export enum class OcclusionModelType
{
	Simple,
    Advanced
};

export template <OcclusionModelType T>
class OcclusionModel;