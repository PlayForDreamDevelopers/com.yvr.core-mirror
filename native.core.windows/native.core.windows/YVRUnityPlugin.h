#pragma once
#include "YVRRenderConfigsMgr.h"
#include "PreviewToolService.h"
#include "Module/Hand/YVRHandMgr.h"

class YVRUnityPlugin
{
public:
	bool developmentBuildMode;
	YVRRenderConfigsMgr* renderConfigsMgr;
	PreviewToolService* previewToolService;
	YVRHandMgr* yvrHandMgr;

	YVRUnityPlugin()
	{
		renderConfigsMgr = new YVRRenderConfigsMgr();
		yvrHandMgr = new YVRHandMgr();
	}

	void StartPreviewToolService()
	{
		previewToolService = new PreviewToolService();
	}

	void StopPreviewToolService()
	{
		previewToolService->StopService();
		delete previewToolService;
	}
};

extern YVRUnityPlugin plugin;