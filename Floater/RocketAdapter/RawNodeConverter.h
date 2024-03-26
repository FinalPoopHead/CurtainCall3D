#pragma once

#include "../FloaterRendererCommon/include/RawSkeleton.h"

namespace Rocket::Core
{
	struct RawModel;
	struct RawNode;
	class RocketTransform;

}

namespace flt
{
	struct RocketObject;
	class Transform;
	struct RawNode;

	Rocket::Core::RawModel* ConvertModel(const flt::RawNode& rootNode);

	Rocket::Core::RawNode* ConvertfltRawNodeTorocketRawNodeRecursive(const flt::RawNode& node);
	Rocket::Core::RawNode* ConvertfltBoneTorocketNodeRecursive(
		const flt::RawSkeleton::Bone& bone, 
		const flt::RawSkeleton::Bone* const pIndexOffsetBone, 
		const flt::RawSkeleton* const skeleton);
	void CopyMeshesToModel(Rocket::Core::RawNode* node, Rocket::Core::RawModel* rocketModel);

	void GenerateTransformHierarchyRecursive(Rocket::Core::RocketTransform* rootRkTransform, Transform* rootTransform);

	//Rocket::Core::RawNode* ConvertfltRawNodeTorocketRawNode(const flt::RawNode& node);
}


