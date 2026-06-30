#pragma once
#include "YVR_Lib_Types.h"
#include "WriteLog.h"

enum YVRStereoRenderMode
{
	MultiPasses,
	SinglePassMultiviewed = 2
};

enum YVRDepthBufferSize
{
	Bit_16,
	Bit_24
};

class YVRRenderConfigsMgr
{
	public:
	
	YVRRenderConfigsMgr();
	int resolutionWidth;
	int resolutionHeight;

	const YVRStereoRenderMode& getStereoRenderMode() const { return renderMode; }
	const bool isUsingSinglePass() const { return false; }
	void setStereoRenderMode(const YVRStereoRenderMode& renderMode) { this->renderMode = renderMode; }

	const YVRDepthBufferSize& getDepthBufferSize() const { return depthBufferSize; }
	void setDepthBufferSize(const YVRDepthBufferSize& depthBufferSize) { this->depthBufferSize = depthBufferSize; }

	const yvrColorSpace& getColorSpace() const { return colorSpace; }
	void setColorSpace(const yvrColorSpace& colorSpace) { this->colorSpace = colorSpace; }

	const float& getResolutionScale() const { return resolutionScale; }
	void setResolutionScale(const float& scale) { this->resolutionScale = scale; }

	const int& getDisplayRate();
	void setDisplayRate(const int& displayRate);

	const bool& isMonoscopic();
	void setMonoscopic(const bool& isMonoscopic);
	void setResolution(int width,int height);
	const bool& isReallocateTexture();
	void setReallocateTexture(const bool& isReset);
	void setEyeBias(float* lfrustumPose,float* rfrustumPose);
	float* getEyeBias(int pass);

	~YVRRenderConfigsMgr();
private:
	const int defaultDisplayRate = 90.0f;
	int displayRate;
	bool monoscopic;
	YVRStereoRenderMode renderMode;
	YVRDepthBufferSize depthBufferSize;
	yvrColorSpace colorSpace;
	float resolutionScale;
	bool resetResolution;
	float lfrustumPose[7]{ 0,0,0,1,0,0,0 };
	float rfrustumPose[7]{ 0,0,0,1,0,0,0 };
};