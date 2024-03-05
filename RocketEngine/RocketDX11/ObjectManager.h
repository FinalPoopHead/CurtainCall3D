#pragma once
#include <vector>

#include "Singleton.h"


namespace Rocket::Core
{
	class ResourceManager;

	class Camera;
	class MeshRenderer;
	class TextRenderer;
	class SpriteRenderer;
	class LineRenderer;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<MeshRenderer*>& GetStaticMeshRenderers();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<SpriteRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();

	public:
		Camera* CreateCamera();
		MeshRenderer* CreateMeshRenderer();
		TextRenderer* CreateText();
		SpriteRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<MeshRenderer*> _meshRendererList;
		std::vector<TextRenderer*> _textList;
		std::vector<SpriteRenderer*> _ImageList;
		LineRenderer* _lineRenderer;

	private:
		ResourceManager& _resourceManager;
	};
}
