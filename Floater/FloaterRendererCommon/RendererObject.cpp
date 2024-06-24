#include "./include/RendererObject.h"
#include <filesystem>


flt::RendererObject::~RendererObject()
{
	//delete camera;
	//delete light;
}

void flt::RendererObject::SetRawNode(RawNode* rawNode)
{
	node = rawNode;
	materials.resize(node->meshes.size());

	for (int i = 0; i < node->meshes.size(); i++)
	{
		materials[i] = node->meshes[i].material;
	}
}

void flt::RendererObject::SetMaterial(uint32 meshIndex, const std::wstring& path, RawMaterial::TextureType type)
{
	ASSERT(meshIndex < materials.size(), "Invalid mesh index");

	materials[meshIndex].textures[(int)type]->path = path;

	std::filesystem::path filePath(path);
	materials[meshIndex].textures[(int)type]->name = filePath.filename().wstring();
}
