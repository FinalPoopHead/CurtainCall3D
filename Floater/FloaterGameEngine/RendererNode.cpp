﻿#include "./include/internal/RendererNode.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterRendererCommon/include/IRenderer.h"

flt::RendererNode* flt::RenderableBuilder::build() const
{
	flt::ModelLoader loader;
	flt::RawScene rawScene;
	loader.Load(filePath, &rawScene);
	flt::RawNode* node = rawScene.nodes[0];

	for (int i = 1; i < rawScene.nodes.size(); i++)
	{
		delete rawScene.nodes[i];
	}

	RendererNode* ret = new RendererNode();
	ret->node = node;

	return ret;
}

flt::RendererNode::~RendererNode()
{
	int i = 0;
}

void flt::RendererNode::Release()
{
	delete node;
}
