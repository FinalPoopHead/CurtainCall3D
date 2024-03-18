#include "DX11AnimationClip.h"

void flt::DX11AnimationClip::GetPosition(float time, Vector3f* outPos)
{
	if (keyPosition.empty())
	{
		return;
	}

	if (keyPosition[positionIndex].time > time)
	{
		positionIndex = 0;
	}

	int size = (int)keyPosition.size();
	for (int i = positionIndex; i < size; ++i)
	{
		if (keyPosition[i].time > time)
		{
			break;
		}
		positionIndex = i;
	}

	if (positionIndex == size - 1)
	{
		*outPos = (Vector3f)keyPosition[positionIndex].position;
		return;
	}

	Vector3f curr = (Vector3f)keyPosition[positionIndex].position;
	Vector3f next = (Vector3f)keyPosition[positionIndex + 1].position;

	float currTIme = keyPosition[positionIndex].time;
	float nextTime = keyPosition[positionIndex + 1].time;

	*outPos = Vector3f::Lerp(curr, next, (time - currTIme) / (nextTime - currTIme));
}

void flt::DX11AnimationClip::GetRotation(float time, Quaternion* outRot)
{
	if (keyRotation.empty())
	{
		return;
	}

	if (keyRotation[rotationIndex].time > time)
	{
		rotationIndex = 0;
	}

	int size = (int)keyRotation.size();
	for (int i = rotationIndex; i < size; ++i)
	{
		if (keyRotation[i].time > time)
		{
			break;
		}
		rotationIndex = i;
	}

	if (rotationIndex == size - 1)
	{
		*outRot = keyRotation[rotationIndex].rotation;
		return;
	}

	Quaternion curr = keyRotation[rotationIndex].rotation;
	Quaternion next = keyRotation[rotationIndex + 1].rotation;

	float currTIme = keyRotation[rotationIndex].time;
	float nextTime = keyRotation[rotationIndex + 1].time;

	*outRot = Quaternion::Slerp(curr, next, (time - currTIme) / (nextTime - currTIme));
}

void flt::DX11AnimationClip::GetScale(float time, Vector3f* outScl)
{
	if (keyScale.empty())
	{
		return;
	}

	if (keyScale[scaleIndex].time > time)
	{
		scaleIndex = 0;
	}

	int size = (int)keyScale.size();
	for (int i = scaleIndex; i < size; ++i)
	{
		if (keyScale[i].time > time)
		{
			break;
		}
		scaleIndex = i;
	}

	if (scaleIndex == size - 1)
	{
		*outScl = (Vector3f)keyScale[scaleIndex].scale;
		return;
	}

	Vector3f curr = (Vector3f)keyScale[scaleIndex].scale;
	Vector3f next = (Vector3f)keyScale[scaleIndex + 1].scale;

	float currTIme = keyScale[scaleIndex].time;
	float nextTime = keyScale[scaleIndex + 1].time;

	*outScl = Vector3f::Lerp(curr, next, (time - currTIme) / (nextTime - currTIme));
}
