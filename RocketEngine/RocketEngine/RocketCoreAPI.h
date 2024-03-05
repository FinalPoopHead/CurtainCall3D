#pragma once
#include "DLLExporter.h"

#include <string>
#include <unordered_map>
#include "RocketAudio.h"

/// �����Ϳ��� ������ ����� �����ϱ� ���� �ѹ� �����Ͽ� export �ϴ� �ڵ�.
/// �����Ϳ��� Open�ϰ� ���� �κи� ������ �� �ֵ��� �ߴ�.
///
/// 23.08.02 ������&������ �����.
namespace Rocket::Core
{
	ROCKET_API std::unordered_map<std::string, RocketAudio>& GetSoundMapFromSoundSystem();
}