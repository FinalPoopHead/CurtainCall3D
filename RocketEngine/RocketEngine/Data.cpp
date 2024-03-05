#include "Data.h"

namespace Rocket::Core
{
	Vector3 DataStruct::GetPosition()
	{
		return { transform->position_x, transform->position_y, transform->position_z };
	}

	Quaternion DataStruct::GetRotation()
	{
		return { transform->rotation_w, transform->rotation_x, transform->rotation_y, transform->rotation_z };
	}

	Vector3 DataStruct::GetScale()
	{
		return { transform->scale_x, transform->scale_y, transform->scale_z };
	}

	Vector3 boxColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	Quaternion boxColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	Vector3 boxColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	Vector3 capsuleColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	Quaternion capsuleColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	Vector3 capsuleColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	Vector3 sphereColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	Quaternion sphereColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	Vector3 sphereColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	Vector3 planeColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	Quaternion planeColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	Vector3 planeColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}

	Vector3 staticBoxColliderData::GetPositionOffset()
	{
		return { positionOffset[0], positionOffset[1] , positionOffset[2] };
	}

	Quaternion staticBoxColliderData::GetRotationOffset()
	{
		return { rotationOffset[0], rotationOffset[1], rotationOffset[2], rotationOffset[3] };
	}

	Vector3 staticBoxColliderData::GetScaleOffset()
	{
		return { scaleOffset[0], scaleOffset[1] , scaleOffset[2] };
	}
}