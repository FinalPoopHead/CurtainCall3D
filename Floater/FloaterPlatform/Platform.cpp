#include "include/Platform.h"
#include "OsPlatform.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include <chrono>
#include <filesystem>

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

bool flt::Platform::Update(float deltaSeconds)
{
	return _pOsImpl->Update(deltaSeconds);
}

flt::IRenderer* flt::Platform::CreateRenderer(RendererType type)
{
	return _pOsImpl->CreateRenderer(type);
}

void flt::Platform::DestroyRenderer(IRenderer* renderer)
{
	_pOsImpl->DestroyRenderer(renderer);
}

flt::Vector2f flt::Platform::GetWindowSize()
{
	return _pOsImpl->GetWindowSize();
}

flt::KeyData flt::Platform::GetKey(KeyCode code)
{
	return _pOsImpl->GetKey(code);
}

flt::KeyData flt::Platform::GetKeyDown(KeyCode code)
{
	return _pOsImpl->GetKeyDown(code);
}

flt::KeyData flt::Platform::GetKeyUp(KeyCode code)
{
	return _pOsImpl->GetKeyUp(code);
}

bool flt::Platform::GamePadConnected(int padIndex)
{
	return _pOsImpl->GamePadConnected(padIndex);
}

bool flt::Platform::GetGamePadState(int padIndex, GamePadState* outState)
{
	return _pOsImpl->GetGamePadState(padIndex, outState);
}

bool flt::Platform::SetGamePadVibration(int padIndex, bool isRightMoter, float moterPower, float time)
{
	return _pOsImpl->SetGamePadVibration(padIndex, isRightMoter, moterPower, time);
}

bool flt::Platform::SetGamePadAnalogDeadZone(int padIndex, GamePadDeadZone* deadZone)
{
	return _pOsImpl->SetGamePadAnalogDeadZone(padIndex, deadZone);
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

std::wstring flt::Platform::GetAbsPath(std::wstring relativePath)
{
	std::filesystem::path p(relativePath);
	ASSERT(p.is_absolute() == false, "Already absolute path");

	std::filesystem::path b(_pOsImpl->GetExePath());
	std::filesystem::path a = std::filesystem::absolute(b / p);
	return a;
}

void flt::Platform::Exit()
{
	_pOsImpl->Exit();
}
