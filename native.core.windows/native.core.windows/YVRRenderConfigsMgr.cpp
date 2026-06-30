#include "YVRRenderConfigsMgr.h"
YVRRenderConfigsMgr::YVRRenderConfigsMgr()
{
	displayRate = 90;
	monoscopic = false;
}

YVRRenderConfigsMgr::~YVRRenderConfigsMgr() {}

const int& YVRRenderConfigsMgr::getDisplayRate()
{
	return displayRate;
}

void YVRRenderConfigsMgr::setDisplayRate(const int& displayRate)
{
	if (displayRate == -1) return;
	this->displayRate = displayRate;
}

const bool& YVRRenderConfigsMgr::isMonoscopic() { return monoscopic; }
void YVRRenderConfigsMgr::setMonoscopic(const bool& isMonoscopic) { monoscopic = isMonoscopic; }

void YVRRenderConfigsMgr::setResolution(int width, int height)
{
	resolutionWidth = width;
	resolutionHeight = height;
	setReallocateTexture(true);
}

void YVRRenderConfigsMgr::setReallocateTexture(const bool& isReset) { this->resetResolution = isReset; }

const bool& YVRRenderConfigsMgr::isReallocateTexture() { return resetResolution; }

void YVRRenderConfigsMgr::setEyeBias(float* leftEyeBias, float* rightEyeBias)
{
	memcpy(&lfrustumPose, leftEyeBias, sizeof(lfrustumPose));
	memcpy(&rfrustumPose, rightEyeBias, sizeof(rfrustumPose));
}

float* YVRRenderConfigsMgr::getEyeBias(int pass)
{
	return pass != 0 ? lfrustumPose : rfrustumPose;
}