#pragma once


namespace dae
{
	struct TransformTrashTheCache
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		TransformTrashTheCache transform;
		int ID{};
	};

	class GameObject3DAlt
	{
	public:
		TransformTrashTheCache* transform{};
		int ID{};
	};
}