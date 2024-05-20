#pragma once
#include "../FloaterGameEngine/include/GameObject.h"


class GlideObject : public flt::GameObject 
{
public:
	GlideObject();
	~GlideObject();

	void Update(float deltaTime) override;
};
