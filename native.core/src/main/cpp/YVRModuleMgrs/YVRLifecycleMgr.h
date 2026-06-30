#pragma once

#include <jni.h>

#include "../UnityHeader/IUnityGraphics.h"
#include "../UnityHeader/IUnityInterface.h"
#include "openxr/openxr.h"

class YVRLifecycleMgr
{
public:
    YVRLifecycleMgr();
    ~YVRLifecycleMgr();

    bool isInVrMode();
    bool isFocusing();
    bool isVisible();
    int getFrameCount();
    int getSubmittedFrameCount();

    // Triggered by Unity Engines
    static void onUnityGraphicsDeviceEvent(UnityGfxDeviceEventType event_type);
    jint onJniLoaded(JavaVM* vm, void* reserved);
    void onUnityPluginLoaded(IUnityInterfaces* unityInterfaces);
    void onUnityPluginUnLoaded();
    void onUnityXRGfxStart();
    void onUnityMainThreadStart();
    void onUnityMainThreadStop();
    void onUnityXRGfxStop();

    // Triggered by UnityXR YVRTrackingProvider
    void onUnityMainThreadPause();
    void onUnityMainThreadResume();
    void onUnityDynamicTick();
    void onUnityBeforeRenderTick();

    // Triggered by UnityXR YVRDisplayProvider
    void onUnityGfxThreadPause();
    void onUnityGfxThreadResume();
    void onPreUnityGfxPrepareNextFrame();
    void onPostUnityGfxPrepareNextFrame();
    void onPreUnityGfxSubmitCurrentFrame();
    void onPostUnityGfxSubmitCurrentFrame();

    void enterVrMode();
    void leaveVrMode();
    void setFocus(bool isFocusing);
    void setVisible(bool isVisible);

    XrSessionState sessionState{XR_SESSION_STATE_UNKNOWN};
    static IUnityInterfaces* unityInterfaces;

private:
    bool inVrMode;
    bool focusing{true};
    bool visible{true};
    int frameCount;
    int submittedFrameCount;

    void shutDown();
};
