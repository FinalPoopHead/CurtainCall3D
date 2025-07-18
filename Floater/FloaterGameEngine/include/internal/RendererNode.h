#pragma once
#include "../../../FloaterRendererCommon/include/IBuilder.h"
#include "../../../FloaterRendererCommon/include/Resource.h"
#include <string>

namespace flt
{
	class ResourceMgr;
	struct RawNode;

	class RendererNode
	{
	public:
		RendererNode() : node(nullptr) {}
		~RendererNode();

	public:
		void Release();
		RawNode* node;
	};

	template struct Resource<RendererNode>;

	struct RendererNodeBuilder : public IBuilder<RendererNode>
	{
		RendererNodeBuilder(const std::wstring& filePath) : IBuilder<RendererNode>(filePath), filePath(filePath) {}
		RendererNodeBuilder(const RendererNodeBuilder& other) = delete;

		virtual RendererNode* build() const override;

		std::wstring filePath;
	};
}
