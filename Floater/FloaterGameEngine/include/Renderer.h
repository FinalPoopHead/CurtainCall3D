#pragma once
#include "Component.h"



namespace flt
{
	struct RawNode;

	class Renderer : public Component
	{
	public:
		Renderer(GameObject& gameObject);
		~Renderer();

		void virtual Start() override;
		void virtual OnEnable() override;
		void virtual OnDisable() override;
		void virtual OnDestroy();

		void SetRawNode(const RawNode& rawNode);

	private:
		GameObject& _gameObject;
	};
}
