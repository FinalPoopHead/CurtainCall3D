// #pragma once
// #include "Singleton.h"
// #include <string>
// #include <vector>
// #include <unordered_map>
// #include "Data.h"
// 
// #include "../include/rapidjson/document.h"
// #include "../include/rapidjson/writer.h"
// #include "../include/rapidjson/stringbuffer.h"
// #include "../include/rapidjson/prettywriter.h" 
// 
// namespace Rocket
// {
// 	class GameObject;
// 	class Scene;
// }
// 
// namespace Rocket::Core
// {
// 	class IComponent;
// }
// 
// namespace Rocket::Core
// {
// 
// 	class DataSystem : public Singleton<DataSystem>
// 	{
// 		friend Singleton;
// 	private:
// 		DataSystem() = default;
// 		 
// 	public:
// 		~DataSystem() = default;
// 
// 	public:
// 		void Initialize();
// 		void Finalize();
// 
// 	public:
// 		void SetAllDataToScene(std::string scenePath);
// 		void ExportSceneToJson(Rocket::Scene& scenedata);
// 
// 	private:
// 		void GetAllDataFromJson(std::string filename);
// 		void MakeAllObjects();
// 		void CreateObjAndComponent(DataStruct& data);
// 		void SetObjOption(DataStruct& data);
// 
// 		void SerializeComponents(Rocket::GameObject* obj, std::pair< const std::string, std::vector< Rocket::Core::IComponent* >>& comp, rapidjson::Document& doc, rapidjson::Value& objValue);
// 
// 		/// 씬이 가진 컴포넌트들을 시리얼라이즈
// 		// 재귀적으로 수행될 트랜스폼 시리얼라이즈 함수
// 		void TransformSerialization(const Rocket::GameObject& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		// 나머지 컴포넌트에 대한 시리얼라이즈를 수행
// 		void MeshRendererSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void SkinnedMeshRendererSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void SpriteRendererSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		//void AnimationControllerSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, Document& doc, Value& val);
// 		void TextBoxSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 
// 		void CapsuleColliderSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void BoxColliderSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void SphereColliderSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 
// 		void PlaneColliderSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void StaticBoxColliderSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void ButtonSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 
// 		//void AudioClipSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, Document& doc, Value& val);
// 		void CameraSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 
// 		void DirectionalLightSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void PointLightSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 		void SpotLightSerialization(const std::pair<std::string, std::vector<Rocket::Core::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
// 
// 		Rocket::Scene* currentScene;
// 		std::string sceneName;
// 		std::string currentFilePath;
// 		std::vector<DataStruct> serializeData;
// 		std::unordered_map<std::string, Rocket::GameObject*> objs;
// 
// 		DataStruct* _FORDEBUG;
// 	};
// 
// }