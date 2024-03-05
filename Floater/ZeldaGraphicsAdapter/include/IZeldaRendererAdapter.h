#pragma once
#pragma warning(push)
#pragma warning(disable: 4819 26495)
#include "IZeldaRenderer.h"
#pragma warning (pop)


class RendererObjectMgr;
namespace flt
{
	class IRenderer;
}


class IZeldaRendererAdapter : public IZeldaRenderer
{
public:
	IZeldaRendererAdapter();
	virtual ~IZeldaRendererAdapter();

	virtual bool Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen) override;
	
	virtual void Finalize() override;

	// deltaTime의 단위는 second
	virtual void BeginDraw(float deltaTime) override;
	virtual void EndDraw() override;

	virtual void DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a) override;
	virtual void DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame) override;
	virtual void DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame) override;

	virtual void DrawSprite(const Eigen::Vector2f& position, TextureID texture) override;

	virtual TextureID CreateTexture(const std::wstring& texturePath) override;
	virtual void ReleaseTexture(TextureID textureID) override;

	virtual ModelID CreateModel(const std::wstring& modelingFilePath) override;
	virtual void ReleaseModel(ModelID modelID) override;
	virtual std::vector<std::wstring> GetAnimationListByModel(ModelID modelID) override;
	virtual std::vector<float> GetAnimationPlayTime(ModelID model) override;

	virtual CameraID CreateCamera() override;
	virtual void ReleaseCamera(CameraID cameraID) override;

	virtual bool SetMainCamera(CameraID cameraID) override;
	virtual bool UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar) override;

private:
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	bool _vsync;
	HWND _hwnd;
	bool _fullScreen;
	float _deltaTime;
	flt::IRenderer* _renderer;
	RendererObjectMgr* _rendererObjectMgr;
};

