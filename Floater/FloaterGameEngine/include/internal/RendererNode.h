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
		void Release();
		RawNode* node;
	};

	template struct Resource<RendererNode>;

	struct RenderableBuilder : public IBuilder<RendererNode>
	{
		RenderableBuilder() : RenderableBuilder(L"") {}
		RenderableBuilder(const std::wstring filePath) : filePath(filePath) {}
		RenderableBuilder(const RenderableBuilder& other) = delete;

		virtual RendererNode* build() const override;

		std::wstring filePath;
	};
}
