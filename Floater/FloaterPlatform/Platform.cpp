#include "include/Platform.h"
#include "OsPlatform.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include <chrono>

class flt::Platform::impl : public PLATFORM_OS
{
public:
	impl(bool useCondole) : PLATFORM_OS(useCondole) {}
};

flt::Platform::Platform() : Platform(false)
{

}

flt::Platform::Platform(bool useConsole)
{
	_pOsImpl = new impl(useConsole);
}

flt::Platform::~Platform()
{
	delete _pOsImpl;
}


bool flt::Platform::Initialize(int pixelWidth, int pixelHeight, std::wstring title, std::wstring imgPath)
{
	return _pOsImpl->Initialize(pixelWidth, pixelHeight, title, imgPath);
}

bool flt::Platform::Finalize()
{
	return _pOsImpl->Finalize();
}

bool flt::Platform::Update()
{
	return _pOsImpl->Update();
}

flt::IRenderer* flt::Platform::CreateRenderer(RendererType type)
{
	return _pOsImpl->CreateRenderer(type);
}

void flt::Platform::DestroyRenderer(IRenderer* renderer)
{
	_pOsImpl->DestroyRenderer(renderer);
}

flt::KeyData flt::Platform::GetKey(KeyCode code)
{
	return _pOsImpl->GetKey(code);
}

bool flt::Platform::GetGamePadState(int padIndex, GamePadState* outState)
{
	return _pOsImpl->GetGamePadState(padIndex, outState);
}

bool flt::Platform::SetGamePadVibration(int padIndex, float leftMotor, float rightMotor)
{
	return _pOsImpl->SetGamePadVibration(padIndex, leftMotor, rightMotor);
}

std::vector<int> flt::Platform::GetGamePadIndexList()
{
	return _pOsImpl->GetGamePadIndexList();
}

void flt::Platform::ShowCursor(bool isShow)
{
	_pOsImpl->ShowCursor(isShow);
}

std::wstring flt::Platform::GetExePath()
{
	return _pOsImpl->GetExePath();
}
