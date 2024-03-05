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
/// 게임 씬의 시리얼라이즈와 디시리얼라이즈를 위한 클래스 
/// 23.07.31 오수안
/// 멤버로 시리얼라이즈와 디시리얼라이즈를 수행하는 함수를 가지고 있다.
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
		// 트랜스폼 데이터만 수행하는 함수
		std::string GetScene(const Scene& scenedata);
		void TransformSerialization(const GameObject& data, Document& doc, Value& val);
		//RocketEngine::Scene TransformDeserialization(const std::string& jsonData, Scene& scene);
	};
}


