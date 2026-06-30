#include "YVR_Lib_Types.h"
#include "YVRTrackingMgr.h"
#include "Headers/XR/IUnityInterface.h"
#include "Headers/XR/IUnityXRTrace.h"
#include "Headers/XR/UnitySubsystemTypes.h"
#include "ProviderContext.h"
#include "YVRUnityPlugin.h"

YVRUnityPlugin plugin;

extern "C" __declspec(dllexport)
void YVRGetControllerState(yvrController mask, yvrControllerState & state)
{
	switch (mask)
	{
	case yvrController::kControllerLeft:
		state = yvrTrackingMgr.hmdState.m_LControllerState;
		break;
	case yvrController::kControllerRight:
		state = yvrTrackingMgr.hmdState.m_RControllerState;
	default:
		break;
	}
}

extern "C" __declspec(dllexport)
uint64_t YVRGetPoseIndex()
{
	uint64_t index = yvrTrackingMgr.hmdState.targetTimestampNs;
	return index;
}

int cachedFrameIndex = -1;
yvrControllerConnectedInfo cachedInfo;

extern "C" __declspec(dllexport)
bool YVRGetControllerConnected(yvrController mask, int frameCount)
{
	if (frameCount != cachedFrameIndex)
	{
		cachedInfo = yvrTrackingMgr.hmdState.m_YVRControllerConnectedInfo;
		cachedFrameIndex = frameCount;
	}

	if (mask == kControllerLeft) return cachedInfo.left_connected;
	if (mask == kControllerRight) return cachedInfo.right_connected;

	return false;
}

static ProviderContext* s_Context{};

UnitySubsystemErrorCode LoadDisplay(ProviderContext&);
UnitySubsystemErrorCode LoadInput(ProviderContext&);

extern "C"
void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces * unityInterfaces)
{
	auto* ctx = s_Context = new ProviderContext;

	ctx->interfaces = unityInterfaces;
	ctx->trace = unityInterfaces->Get<IUnityXRTrace>();
	LoadDisplay(*ctx);
	LoadInput(*ctx);
}

extern "C"
void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
	delete s_Context;
}

extern "C"  __declspec(dllexport)
uint32_t DebugEnable(bool enable)
{
	EnableLog = enable;
	return 0;
}

extern "C"  __declspec(dllexport)
void YVRSetXRUserDefinedSettings(YVRXRUserDefinedSettings * userDefinedSettings)
{
	plugin.renderConfigsMgr->setColorSpace(userDefinedSettings->useLinearColorSpace ? yvrColorSpace::kColorSpaceSRGB
		: yvrColorSpace::kColorSpaceLinear);
	plugin.renderConfigsMgr->setMonoscopic(userDefinedSettings->useMonoscopic);
	plugin.renderConfigsMgr->setStereoRenderMode((YVRStereoRenderMode)userDefinedSettings->stereoRenderingMode);
	plugin.renderConfigsMgr->setResolutionScale(userDefinedSettings->resolutionScale);
	plugin.renderConfigsMgr->setDepthBufferSize(userDefinedSettings->use16BitDepthBuffer ? YVRDepthBufferSize::Bit_16
		: YVRDepthBufferSize::Bit_24);
}

extern "C"  __declspec(dllexport)
void YVRSetDevelopmentBuildMode(bool isDevelopmentBuild)
{
	plugin.developmentBuildMode = isDevelopmentBuild;
}

extern "C"  __declspec(dllexport)
const char* GetPreviewToolService() {
	return plugin.previewToolService->GetPreviewToolService();
}

extern "C"  __declspec(dllexport)
void YVRGetHandHandJointLocations(const Handedness handedness, YVRHandJointLocations * handJointLocations, void* locations,
	void* velocities)
{
	plugin.yvrHandMgr->GetHandHandJointLocations(handedness, handJointLocations, (XrHandJointLocationEXT*)locations, (XrHandJointVelocityEXT*)velocities);
}

extern "C"  __declspec(dllexport)
XrInputDeviceFlagsYVR YVRGetCurrentInputDevice()
{
	if (plugin.yvrHandMgr)
	{
		plugin.yvrHandMgr->GetCurrentInputDevice();
	}
	else
	{
		return XR_INPUT_DEVICES_YVR_CONTROLLER_BIT_YVR;
	}
}