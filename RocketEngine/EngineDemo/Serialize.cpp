#include "Serialize.h"

namespace RocketEngine
{

	Serialize::Serialize()
	{

	}

	Serialize::~Serialize()
	{

	}

	std::string Serialize::GetScene(const Scene& scenedata)
	{
		Document doc;
		doc.SetArray();


		for (auto i : scenedata._originalList)
		{
			Value objValue(kObjectType);

			Value nameValue;
			nameValue.SetString(i->objName, strlen(i->objName), doc.GetAllocator());
			objValue.AddMember(StringRef("__object_name__"), nameValue, doc.GetAllocator());

			TransformSerialization(*i, doc, objValue);
		}

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}

	void Serialize::TransformSerialization(const GameObject& data, Document& doc, Value& val)
	{

		// position
		Value positionValue(kObjectType);
		positionValue.AddMember("x", data.transform.GetPosition().x, doc.GetAllocator());
		positionValue.AddMember("y", data.transform.GetPosition().y, doc.GetAllocator());
		positionValue.AddMember("z", data.transform.GetPosition().z, doc.GetAllocator());
		val.AddMember("position", positionValue, doc.GetAllocator());

		// rotation
		Value rotationValue(kObjectType);
		rotationValue.AddMember("x", data.transform.GetRotation().x, doc.GetAllocator());
		rotationValue.AddMember("y", data.transform.GetRotation().y, doc.GetAllocator());
		rotationValue.AddMember("z", data.transform.GetRotation().z, doc.GetAllocator());
		rotationValue.AddMember("w", data.transform.GetRotation().w, doc.GetAllocator());
		val.AddMember("rotation", rotationValue, doc.GetAllocator());

		//scale
		Value scaleValue(kObjectType);
		scaleValue.AddMember("x", data.transform.GetScale().x, doc.GetAllocator());
		scaleValue.AddMember("y", data.transform.GetScale().y, doc.GetAllocator());
		scaleValue.AddMember("z", data.transform.GetScale().z, doc.GetAllocator());
		val.AddMember("scale", scaleValue, doc.GetAllocator());

		if (!data.transform.GetChildrenVec().empty())
		{
			for (auto i : data.transform.GetChildrenVec())
			{
				TransformSerialization(*i->gameObject, doc, val);
			}
		}

		doc.PushBack(val, doc.GetAllocator());
	}

	//RocketEngine::Scene Serialize::TransformDeserialization(const std::string& jsonData, Scene& scene)
	//{

	//}

}

