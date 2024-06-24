#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "RendererNode.h"
#include "../../../FloaterRendererCommon/include/RawNode.h"
#include <string>

namespace flt
{
	class IRenderer;
	struct RendererObject;

	class DirectionalLight : public Component<DirectionalLight>
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();

		void SetDirection(flt::Vector3f dir);
		void SetDiffuseColor(flt::Vector3f color);
		void SetSpecularColor(flt::Vector3f color);
		void SetAmbientColor(flt::Vector3f color);
		void SetIntensity(float intensity);

	protected:
		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

	private:
		flt::Vector3f _direction;
		flt::Vector3f _diffuseColor;
		flt::Vector3f _specularColor;
		flt::Vector3f _ambientColor;
		float _intensity;
		IRenderer& _renderer;
		RendererObject* _rendererObject;
		uint64 _hObject;
		bool _isDraw;
		bool _isRegisted;
	};
}
