#pragma once
#include "Singleton.h"
#include <vector>

namespace Rocket
{
	class Tween;
}

namespace Rocket::Core
{
	class TweenSystem : public Singleton<TweenSystem>
	{
		friend Singleton;
	private:
		TweenSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

	//private:
		// ����ϰ� �ִ� ��Ʈ�� ��ü�� ����
		std::vector<Rocket::Tween*> _dotweens;
		// ����� ������ ����� ���� ��Ʈ�� ��ü�� ����
		std::vector<Rocket::Tween*> _deleteObj;

	public:
		Rocket::Tween& CreateTween();
		void Update();
	};
}
