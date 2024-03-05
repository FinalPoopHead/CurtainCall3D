#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererDX11/include/CreateRenderer.h"
#include "./include/IZeldaRendererAdapter.h"
#include "RendererObjectMgr.h"

IZeldaRendererAdapter::IZeldaRendererAdapter() :
	_screenWidth(0),
	_screenHeight(0),
	_vsync(false),
	_hwnd(nullptr),
	_fullScreen(false),
	_deltaTime(0.0f),
	_renderer(nullptr),
	_rendererObjectMgr(nullptr)
{

}

IZeldaRendererAdapter::~IZeldaRendererAdapter()
{
	if (_renderer)
	{
		flt::DestroyRendererDX11(_renderer);
		_renderer = nullptr;
	}

	if (_rendererObjectMgr)
	{
		delete _rendererObjectMgr;
		_rendererObjectMgr = nullptr;
	}
}

bool IZeldaRendererAdapter::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_vsync = vsync;
	_hwnd = hwnd;
	_fullScreen = fullScreen;
	_deltaTime = 0.0f;

	_renderer = flt::CreateRendererDX11(hwnd);

	if (_renderer == nullptr)
	{
		return false;
	}

	_rendererObjectMgr = new RendererObjectMgr();

	return true;
}

void IZeldaRendererAdapter::Finalize()
{
	flt::DestroyRendererDX11(_renderer);
}

void IZeldaRendererAdapter::BeginDraw(float deltaTime)
{
	_deltaTime = deltaTime;
}

void IZeldaRendererAdapter::EndDraw()
{
	_renderer->Render(_deltaTime);
	return;
}

void IZeldaRendererAdapter::DrawCube(const Eigen::Matrix4f& worldMatrix, TextureID texture, bool wireFrame, float r, float g, float b, float a)
{

}

void IZeldaRendererAdapter::DrawModel(const Eigen::Matrix4f& worldMatrix, ModelID model, bool wireFrame)
{

}

void IZeldaRendererAdapter::DrawAnimation(const Eigen::Matrix4f& worldMatrix, ModelID model, std::wstring animationName, float animationTime, bool wireFrame)
{

}

void IZeldaRendererAdapter::DrawSprite(const Eigen::Vector2f& position, TextureID texture)
{

}

TextureID IZeldaRendererAdapter::CreateTexture(const std::wstring& texturePath)
{
	return TextureID{};
}

void IZeldaRendererAdapter::ReleaseTexture(TextureID textureID)
{
	return;
}

ModelID IZeldaRendererAdapter::CreateModel(const std::wstring& modelingFilePath)
{
	return ModelID{};
}

void IZeldaRendererAdapter::ReleaseModel(ModelID modelID)
{
	return;
}

std::vector<std::wstring> IZeldaRendererAdapter::GetAnimationListByModel(ModelID modelID)
{
	return std::vector<std::wstring>();
}

std::vector<float> IZeldaRendererAdapter::GetAnimationPlayTime(ModelID model)
{
	return std::vector<float>();
}

CameraID IZeldaRendererAdapter::CreateCamera()
{
	return CameraID{};
}

void IZeldaRendererAdapter::ReleaseCamera(CameraID cameraID)
{
	return;
}

bool IZeldaRendererAdapter::SetMainCamera(CameraID cameraID)
{
	return false;
}

bool IZeldaRendererAdapter::UpdateCamera(CameraID cameraID, const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	return false;
}
