#include "XboxPad.h"
#include <string>
#include <cmath>
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/Hash.h"
#include <winioctl.h>

#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")


bool flt::Xbox::Initialize(WinGamePad* outGamePadArr)
{
	HDEVINFO dev = SetupDiGetClassDevsW(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (dev != INVALID_HANDLE_VALUE)
	{
		SP_DEVICE_INTERFACE_DATA idata;
		idata.cbSize = sizeof(idata);

		DWORD index = 0;
		while (SetupDiEnumDeviceInterfaces(dev, NULL, &guid, index, &idata))
		{
			ASSERT(index < 16, "GamePad is full");
			DWORD size;
			SetupDiGetDeviceInterfaceDetailW(dev, &idata, NULL, 0, &size, NULL);

			PSP_DEVICE_INTERFACE_DETAIL_DATA_W detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)LocalAlloc(LMEM_FIXED, size);
			if (detail != NULL)
			{
				detail->cbSize = sizeof(*detail); // yes, sizeof of structure, not allocated memory

				SP_DEVINFO_DATA data;
				data.cbSize = sizeof(data);

				if (SetupDiGetDeviceInterfaceDetailW(dev, &idata, detail, size, &size, &data))
				{
					//outGamePadArr[index] = new WinGamePad();
					outGamePadArr[index].path = detail->DevicePath;
					Connect(&outGamePadArr[index]);
				}
				LocalFree(detail);
			}
			index++;
		}

		SetupDiDestroyDeviceInfoList(dev);
	}

	return true;
}

bool flt::Xbox::Connect(WinGamePad* outGamePad)
{
	ASSERT(outGamePad, "outGamePad is nullptr");

	if (outGamePad->handle != NULL)
	{
		return false;
	}

	outGamePad->handle = CreateFileW(outGamePad->path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (outGamePad->handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	outGamePad->isConnected = true;

	return true;
}

bool flt::Xbox::Disconnect(WinGamePad* outGamePad)
{
	ASSERT(outGamePad, "outGamePad is nullptr");

	BOOL ret = CloseHandle(outGamePad->handle);
	outGamePad->handle = NULL;
	outGamePad->isConnected = false;
	return ret;
}

bool flt::Xbox::Get(WinGamePad* outGamePad)
{
	ASSERT(outGamePad, "outGamePad is nullptr");

	if (outGamePad->handle == NULL)
	{
		return false;
	}

	BYTE in[3] = { 0x01, 0x01, 0x00 };
	BYTE out[29];
	DWORD size;

	if (!DeviceIoControl(outGamePad->handle, 0x8000e00c, in, sizeof(in), out, sizeof(out), &size, NULL) || size != sizeof(out))
	{
		DWORD errorCode = GetLastError();
		if (errorCode == ERROR_DEVICE_NOT_CONNECTED)
		{
			Disconnect(outGamePad);
			return false;
		}
		return false;
	}

	State state;
	state.packet = *(DWORD*)(out + 5);
	state.buttons = *(WORD*)(out + 11);
	state.lTrigger = out[13];
	state.rTrigger = out[14];
	state.lStickX = *(SHORT*)(out + 15);
	state.lStickY = *(SHORT*)(out + 17);
	state.rStickX = *(SHORT*)(out + 19);
	state.rStickY = *(SHORT*)(out + 21);

	//outGamePad->state.buttons
	//	|= ((state.buttons & XBOX_A) ? outGamePad->state.A : 0)
	//	|= ((state.buttons & XBOX_B) ? outGamePad->state.B : 0)
	//	|= ((state.buttons & XBOX_X) ? outGamePad->state.X : 0)
	//	|= ((state.buttons & XBOX_Y) ? outGamePad->state.Y : 0)
	//	|= ((state.buttons & XBOX_BACK) ? outGamePad->state.BACK : 0)
	//	|= ((state.buttons & XBOX_START) ? outGamePad->state.START : 0)
	//	|= ((state.buttons & XBOX_LEFT_SHOULDER) ? outGamePad->state.LB : 0)
	//	|= ((state.buttons & XBOX_RIGHT_SHOULDER) ? outGamePad->state.RB : 0)
	//	|= ((state.buttons & XBOX_LEFT_THUMB) ? outGamePad->state.LSTICK : 0)
	//	|= ((state.buttons & XBOX_RIGHT_THUMB) ? outGamePad->state.RSTICK : 0)
	//	|= ((state.buttons & XBOX_DPAD_UP) ? outGamePad->state.UP : 0)
	//	|= ((state.buttons & XBOX_DPAD_DOWN) ? outGamePad->state.DOWN : 0)
	//	|= ((state.buttons & XBOX_DPAD_LEFT) ? outGamePad->state.LEFT : 0)
	//	|= ((state.buttons & XBOX_DPAD_RIGHT) ? outGamePad->state.RIGHT : 0);
	outGamePad->state.buttonsDown = 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::A) && (state.buttons & XBOX_A) ? outGamePad->state.A : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::B) && (state.buttons & XBOX_B) ? outGamePad->state.B : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::X) && (state.buttons & XBOX_X) ? outGamePad->state.X : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::Y) && (state.buttons & XBOX_Y) ? outGamePad->state.Y : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::BACK) && (state.buttons & XBOX_BACK) ? outGamePad->state.BACK : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::START) && (state.buttons & XBOX_START) ? outGamePad->state.START : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::LB) && (state.buttons & XBOX_LEFT_SHOULDER) ? outGamePad->state.LB : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::RB) && (state.buttons & XBOX_RIGHT_SHOULDER) ? outGamePad->state.RB : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::LSTICK) && (state.buttons & XBOX_LEFT_THUMB) ? outGamePad->state.LSTICK : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::RSTICK) && (state.buttons & XBOX_RIGHT_THUMB) ? outGamePad->state.RSTICK : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::UP) && (state.buttons & XBOX_DPAD_UP) ? outGamePad->state.UP : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::DOWN) && (state.buttons & XBOX_DPAD_DOWN) ? outGamePad->state.DOWN : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::LEFT) && (state.buttons & XBOX_DPAD_LEFT) ? outGamePad->state.LEFT : 0;
	outGamePad->state.buttonsDown |= !(outGamePad->state.buttons & GamePadState::ButtonFlag::RIGHT) && (state.buttons & XBOX_DPAD_RIGHT) ? outGamePad->state.RIGHT : 0;

	outGamePad->state.buttonsUp = 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::A) && !(state.buttons & XBOX_A) ? outGamePad->state.A : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::B) && !(state.buttons & XBOX_B) ? outGamePad->state.B : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::X) && !(state.buttons & XBOX_X) ? outGamePad->state.X : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::Y) && !(state.buttons & XBOX_Y) ? outGamePad->state.Y : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::BACK) && !(state.buttons & XBOX_BACK) ? outGamePad->state.BACK : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::START) && !(state.buttons & XBOX_START) ? outGamePad->state.START : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::LB) && !(state.buttons & XBOX_LEFT_SHOULDER) ? outGamePad->state.LB : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::RB) && !(state.buttons & XBOX_RIGHT_SHOULDER) ? outGamePad->state.RB : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::LSTICK) && !(state.buttons & XBOX_LEFT_THUMB) ? outGamePad->state.LSTICK : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::RSTICK) && !(state.buttons & XBOX_RIGHT_THUMB) ? outGamePad->state.RSTICK : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::UP) && !(state.buttons & XBOX_DPAD_UP) ? outGamePad->state.UP : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::DOWN) && !(state.buttons & XBOX_DPAD_DOWN) ? outGamePad->state.DOWN : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::LEFT) && !(state.buttons & XBOX_DPAD_LEFT) ? outGamePad->state.LEFT : 0;
	outGamePad->state.buttonsUp |= (outGamePad->state.buttons & GamePadState::ButtonFlag::RIGHT) && !(state.buttons & XBOX_DPAD_RIGHT) ? outGamePad->state.RIGHT : 0;

	outGamePad->state.buttons = 0;
	outGamePad->state.buttons |= ((state.buttons & XBOX_A) ? outGamePad->state.A : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_B) ? outGamePad->state.B : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_X) ? outGamePad->state.X : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_Y) ? outGamePad->state.Y : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_BACK) ? outGamePad->state.BACK : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_START) ? outGamePad->state.START : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_LEFT_SHOULDER) ? outGamePad->state.LB : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_RIGHT_SHOULDER) ? outGamePad->state.RB : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_LEFT_THUMB) ? outGamePad->state.LSTICK : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_RIGHT_THUMB) ? outGamePad->state.RSTICK : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_DPAD_UP) ? outGamePad->state.UP : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_DPAD_DOWN) ? outGamePad->state.DOWN : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_DPAD_LEFT) ? outGamePad->state.LEFT : 0);
	outGamePad->state.buttons |= ((state.buttons & XBOX_DPAD_RIGHT) ? outGamePad->state.RIGHT : 0);

	outGamePad->state.lTrigger = state.lTrigger / 255.f;
	outGamePad->state.rTrigger = state.rTrigger / 255.f;
	outGamePad->state.lStickX = state.lStickX / 32768.f;
	outGamePad->state.lStickY = state.lStickY / 32768.f;
	outGamePad->state.rStickX = state.rStickX / 32768.f;
	outGamePad->state.rStickY = state.rStickY / 32768.f;

	// DeadZone 처리

	std::fabs(outGamePad->state.lTrigger) < outGamePad->deadZone.lTrigger ? outGamePad->state.lTrigger = 0.f : 0.f;
	std::fabs(outGamePad->state.rTrigger) < outGamePad->deadZone.rTrigger ? outGamePad->state.rTrigger = 0.f : 0.f;
	if ((std::fabs(outGamePad->state.lStickX) + std::fabs(outGamePad->state.lStickY)) < outGamePad->deadZone.leftStick)
	{
		outGamePad->state.lStickX = 0.f;
		outGamePad->state.lStickY = 0.f;
	}
	if ((std::fabs(outGamePad->state.rStickX) + std::fabs(outGamePad->state.rStickY)) < outGamePad->deadZone.rightStick)
	{
		outGamePad->state.rStickX = 0.f;
		outGamePad->state.rStickY = 0.f;
	}

	return true;
}

bool flt::Xbox::Clear(WinGamePad* outGamePad)
{
	ASSERT(outGamePad, "outGamePad is nullptr");
	outGamePad->state.buttons = 0;

	return true;
}

bool flt::Xbox::Set(WinGamePad* outGamePad, BYTE lMotor, BYTE rMotor)
{
	ASSERT(outGamePad, "outGamePad is nullptr");

	BYTE in[5] = { 0, 0, lMotor, rMotor, 2 };
	if (!DeviceIoControl(outGamePad->handle, 0x8000a010, in, sizeof(in), NULL, 0, NULL, NULL))
	{
		return false;
	}
	return true;
}
