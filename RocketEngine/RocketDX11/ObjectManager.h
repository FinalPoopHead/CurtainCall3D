﻿#pragma once
#include <vector>
#include <memory>
#include <d3d11_2.h>
#include <dxgi1_3.h>

#include "Singleton.h"
#include "TextRenderer.h"
#include "Axis.h"
#include "Grid.h"
#include "CubeMap.h"
#include "LineRenderer.h"

namespace Rocket::Core
{
	class ResourceManager;

	class Camera;
	class MeshRenderer;
	class DynamicModelRenderer;
	class StaticModelRenderer;
	class SpriteRenderer;
	class DirectionalLight;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		void Initialize(ID3D11Device* device, float screenWidth, float screenHeight);
		void Finalize();

	public:
		std::vector<MeshRenderer*>& GetMeshRenderers();
		std::vector<DynamicModelRenderer*>& GetDynamicModelRenderers();
		std::vector<StaticModelRenderer*>& GetStaticModelRenderers();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<SpriteRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();
		std::vector<DirectionalLight*>& GetDirectionalLightList();
		CubeMap* GetCubeMap();
		std::vector<Camera*>& GetCameraList();

		void SetScreenSize(float width, float height) { _screenWidth = width; _screenHeight = height; }

	public:
		Camera* CreateCamera();
		MeshRenderer* CreateMeshRenderer();
		DynamicModelRenderer* CreateDynamicModelRenderer();
		StaticModelRenderer* CreateStaticModelRenderer();
		TextRenderer* CreateText();
		SpriteRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();
		DirectionalLight* CreateDirectionalLight();

	public:
		void DestroyCamera(Camera* camera);
		void DestroyMeshRenderer(MeshRenderer* meshRenderer);
		void DestroyDynamicModelRenderer(DynamicModelRenderer* dynamicModelRenderer);
		void DestroyStaticModelRenderer(StaticModelRenderer* staticModelRenderer);
		void DestroyTextRenderer(TextRenderer* textRenderer);
		void DestroySpriteRenderer(SpriteRenderer* spriteRenderer);
		void DestroyDirectionalLight(DirectionalLight* directionalLight);

		/// Debug 객체들.
	public:
		std::unique_ptr<TextRenderer> _debugText;
		std::unique_ptr<Axis> _axis;
		std::unique_ptr<Grid> _grid;

		// TODO : 얘네도 다 unique_ptr로 바꿀까? 아니면 해제를 똑바로 잘 해줄까? 지금은 해제를 수동으로 해주는 방향으로 가자.
	private:
		float _screenWidth;
		float _screenHeight;

		std::unique_ptr<LineRenderer> _lineRenderer;

		std::vector<Camera*> _cameraList;
		std::vector<MeshRenderer*> _meshRendererList;
		std::vector<DynamicModelRenderer*> _dynamicModelRendererList;
		std::vector<StaticModelRenderer*> _staticModelRendererList;
		std::vector<TextRenderer*> _textList;
		std::vector<SpriteRenderer*> _spriteList;
		std::vector<DirectionalLight*> _directionalLightList;
		std::unique_ptr<CubeMap> _cubeMap;

	private:
		ResourceManager& _resourceManager;
	};
}
