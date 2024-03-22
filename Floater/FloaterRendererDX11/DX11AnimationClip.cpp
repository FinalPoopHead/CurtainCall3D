#include "DX11AnimationClip.h"

void flt::DX11AnimationClip::GetPosition(float time, Vector3f* outPos)
{
	if (keyPosition.empty())
	{
		return;
	}

	auto iter = keyPosition.begin();
	for (; iter != keyPosition.end(); ++iter)
	{
		if (iter->time > time)
		{
			iter--;
			break;
		}
	}

	if (iter == keyPosition.end())
	{
		iter--;
		outPos->x = iter->position.x;
		outPos->y = iter->position.y;
		outPos->z = iter->position.z;

		return;
	}

	Vector3f curr = { iter->position.x, iter->position.y, iter->position.z };
	iter++;
	Vector3f next = { iter->position.x, iter->position.y, iter->position.z };
	iter--;
	*outPos = Vector3f::Lerp(curr, next, (time - iter->time) / (iter[1].time - iter->time));
}

void flt::DX11AnimationClip::GetRotation(float time, Quaternion* outRot)
{
	if (keyRotation.empty())
	{
		return;
	}

	auto iter = keyRotation.begin();
	for (; iter != keyRotation.end(); ++iter)
	{
		if (iter->time > time)
		{
			iter--;
			break;
		}
	}

	if (iter == keyRotation.end())
	{
		iter--;
		*outRot = iter->rotation;
		return;
	}

	Quaternion curr = iter->rotation;
	iter++;
	Quaternion next = iter->rotation;
	iter--;

	*outRot = Quaternion::Slerp(curr, next, (time - iter->time) / (iter[1].time - iter->time));

}

void flt::DX11AnimationClip::GetScale(float time, Vector3f* outScl)
{
	if (keyScale.empty())
	{
		return;
	}

	auto iter = keyScale.begin();
	for (; iter != keyScale.end(); ++iter)
	{
		if (iter->time > time)
		{
			iter--;
			break;
		}
	}

	if (iter == keyScale.end())
	{
		iter--;
		outScl->x = iter->scale.x;
		outScl->y = iter->scale.y;
		outScl->z = iter->scale.z;

		return;
	}

	Vector3f curr = { iter->scale.x, iter->scale.y, iter->scale.z };
	iter++;
	Vector3f next = { iter->scale.x, iter->scale.y, iter->scale.z };
	iter--;

	*outScl = Vector3f::Lerp(curr, next, (time - iter->time) / (iter[1].time - iter->time));
}
