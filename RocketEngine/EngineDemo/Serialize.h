#pragma once
#include <string>
#include <vector>

#include "RocketAPILoader.h"
#include "../RocketMath/RocketMath.h"

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h" 

using namespace rapidjson;


/// <summary>
/// ���� ���� �ø��������� ��ø������� ���� Ŭ���� 
/// 23.07.31 ������
/// ����� �ø��������� ��ø������� �����ϴ� �Լ��� ������ �ִ�.
/// </summary>


namespace RocketEngine
{
	class Scene;

	class Serialize
	{
	public:
		Serialize();
		~Serialize();

	public:
		// Ʈ������ �����͸� �����ϴ� �Լ�
		std::string GetScene(const Scene& scenedata);
		void TransformSerialization(const GameObject& data, Document& doc, Value& val);
		//RocketEngine::Scene TransformDeserialization(const std::string& jsonData, Scene& scene);
	};
}


