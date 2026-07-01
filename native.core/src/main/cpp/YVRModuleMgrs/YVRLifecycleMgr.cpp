#include "YVRLifecycleMgr.h"
#include <unistd.h>
#include "../UnityHeader/IUnityGraphics.h"
#include "../YVRUnityPlugin.h"
#include "Events/YVREventType.h"

IUnityInterfaces* YVRLifecycleMgr::unityInterfaces = nullptr;

YVRLifecycleMgr::YVRLifecycleMgr()
    : frameCount(0),
      submittedFrameCount(0),
      inVrMode(false) {}

YVRLifecycleMgr::~YVRLifecycleMgr() = default;

jint YVRLifecycleMgr::onJniLoaded(JavaVM* vm, void* reserved)
{
    AnnounceCallingFunc();
    JNIEnv* javaEnv;
    if (vm->GetEnv(reinterpret_cast<void**>(&javaEnv), JNI_VERSION_1_2) != JNI_OK)
    {
        YError("Can not get jni environment");
        return -1;
    }

    const jclass unityPlayerClass = (*javaEnv).FindClass("com/unity3d/player/UnityPlayer");
    const jfieldID fieldId = javaEnv->GetStaticFieldID(unityPlayerClass, "currentActivity", "Landroid/app/Activity;");
    const jobject unityPlayerActivity = javaEnv->GetStaticObjectField(unityPlayerClass, fieldId);
    plugin.javaVm = vm;
    plugin.activity = javaEnv->NewGlobalRef(unityPlayerActivity);

    return JNI_VERSION_1_2;
}

bool YVRLifecycleMgr::isInVrMode() { return this->inVrMode; }
bool YVRLifecycleMgr::isFocusing() { return this->focusing; }
bool YVRLifecycleMgr::isVisible() { return this->visible; }

void YVRLifecycleMgr::enterVrMode()
{
    AnnounceCallingFunc();

    plugin.openxrMgr->program->BeginSession();

    plugin.renderMgr->configsMgr->setExtraLatency(true);
    plugin.renderMgr->configsMgr->setDisplayRate(plugin.renderMgr->configsMgr->getDisplayRate());
    plugin.renderMgr->configsMgr->refreshSystemProperty(true);
    this->inVrMode = true;

    plugin.renderMgr->frameWaited = false;
    plugin.renderMgr->frameBegan = false;

    AnnounceCalledFunc();
}

void YVRLifecycleMgr::leaveVrMode()
{
    AnnounceCallingFunc();
    plugin.openxrMgr->program->EndSession();

    this->inVrMode = false;
    plugin.renderMgr->frameWaited = false;
    plugin.renderMgr->frameBegan = false;

    AnnounceCalledFunc();
}

void YVRLifecycleMgr::setFocus(bool isFocusing)
{
    if (this->focusing == isFocusing) return;
    this->focusing = isFocusing;
    plugin.renderMgr->configsMgr->refreshSystemProperty(true);
    plugin.eventsMgr->triggerBaseEvents(this->focusing ? kFocusGained : kFocusLost);
}

void YVRLifecycleMgr::setVisible(bool isVisible)
{
    if (this->visible == isVisible) return;
    this->visible = isVisible;
    plugin.renderMgr->configsMgr->refreshSystemProperty(true);
    plugin.eventsMgr->triggerBaseEvents(this->visible ? kVisibilityGained : kVisibilityLost);
}

void YVRLifecycleMgr::shutDown()
{
    AnnounceCallingFunc();
    unityXRProviderInitialized = false;
}

void YVRLifecycleMgr::onUnityGraphicsDeviceEvent(UnityGfxDeviceEventType event_type)
{
    if (event_type == kUnityGfxDeviceEventShutdown)
    {
        plugin.lifecycleMgr->unityXRProviderInitialized = false;
        return;
    }

    IUnityGraphics* unityGraphics = unityInterfaces != nullptr ? unityInterfaces->Get<IUnityGraphics>() : nullptr;
    if (unityGraphics == nullptr)
    {
        YError("Unity graphics interface is null for device event=%d", event_type);
        return;
    }

    const auto renderer = unityGraphics->GetRenderer();
    YDebug("Graphics event is %d, and renderer is %d ", event_type, renderer);
    if (event_type != kUnityGfxDeviceEventInitialize) return;
    if (renderer == kUnityGfxRendererNull) return;
    if (plugin.lifecycleMgr->unityXRProviderInitialized) return;

    plugin.unityXRProviderMgr->initialize(unityInterfaces);
    plugin.lifecycleMgr->unityXRProviderInitialized = true;
}

void YVRLifecycleMgr::onUnityPluginLoaded(IUnityInterfaces* unityInterfaces)
{
    AnnounceCallingFunc();
    plugin.unityProfiler = unityInterfaces->Get<IUnityProfiler>();
    if (plugin.unityProfiler != nullptr)
    {
        plugin.developmentBuildMode = plugin.unityProfiler->IsAvailable() != 0;
    }
    YVRLifecycleMgr::unityInterfaces = unityInterfaces;
    IUnityGraphics* unityGraphics = YVRLifecycleMgr::unityInterfaces->Get<IUnityGraphics>();
    if (unityGraphics == nullptr)
    {
        YError("Unity graphics interface is null during plugin load");
        return;
    }
    unityGraphics->RegisterDeviceEventCallback(onUnityGraphicsDeviceEvent);

    GraphicsPluginBase::setUnityInterface(unityInterfaces);
    if (unityGraphics->GetRenderer() != kUnityGfxRendererNull)
    {
        onUnityGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
    }

    plugin.openxrMgr->initialize();
}

void YVRLifecycleMgr::onUnityPluginUnLoaded() { this->shutDown(); }

#pragma region "Unity Main Thread Event"

void YVRLifecycleMgr::onUnityMainThreadPause()
{
    AnnounceCallingFunc();

    plugin.eventsMgr->pollEvents();
}

void YVRLifecycleMgr::onUnityMainThreadResume()
{
    AnnounceCallingFunc();
    constexpr XrAndroidThreadTypeKHR type = XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR;
    const auto threadID = static_cast<uint32_t>(gettid());
    OpenXRAPI(xrSetAndroidApplicationThreadKHR(plugin.openxrMgr->program->session, type, threadID));

    // The tracking space is a system value, which means that it may be overwritten by other applications when this application is paused
    // Thus, reset the saved tracking space every time app resumed.
    plugin.inputMgr->setTrackingSpace(plugin.inputMgr->getTrackingSpace());
    plugin.eventsMgr->pollEvents();
}

void YVRLifecycleMgr::onUnityDynamicTick()
{
    frameCount++;
    plugin.eventsMgr->pollEvents();
}

void YVRLifecycleMgr::onUnityBeforeRenderTick() {}

#pragma endregion "Unity Main Thread Event"

bool YVRLifecycleMgr::onUnityXRGfxStart()
{
    AnnounceCallingFunc();
    plugin.openxrMgr->selectGraphicPlugin(unityInterfaces);
    plugin.openxrMgr->program->InitializeGfxDevice();

    if (!plugin.openxrMgr->program->createSession())
    {
        YError("Failed to create OpenXR session");
        return false;
    }
    plugin.inputMgr->createAppSpace();
    plugin.inputMgr->createViewSpace();
    plugin.inputMgr->createLocalSpace();
    plugin.inputMgr->createStageSpace();
    return true;
}

void YVRLifecycleMgr::onUnityXRGfxStop()
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->disableAllLayers(true);
    plugin.inputMgr->destroyAppSpace();
    plugin.inputMgr->destroyViewSpace();
    plugin.inputMgr->destroyLocalSpace();
    plugin.inputMgr->destroyStageSpace();
    plugin.inputMgr->destroyActions();
    plugin.openxrMgr->program->destroySession();
    plugin.openxrMgr->program->destroyInstance();
}

void YVRLifecycleMgr::onUnityMainThreadStart()
{
    AnnounceCallingFunc();

    plugin.inputMgr->createActions();
}

void YVRLifecycleMgr::onUnityMainThreadStop()
{
    AnnounceCallingFunc();
}

void YVRLifecycleMgr::onUnityGfxThreadResume()
{
    AnnounceCallingFunc();
    enterVrMode();
    plugin.renderMgr->renderLayersMgr->resumeAllActiveLayers();
    plugin.eventsMgr->pollEvents();
}

void YVRLifecycleMgr::onUnityGfxThreadPause()
{
    AnnounceCallingFunc();
    plugin.renderMgr->EndFrame();
    plugin.renderMgr->renderLayersMgr->pauseAllActiveLayers();
    plugin.eventsMgr->pollEvents();
    leaveVrMode();
    submittedFrameCount = 0; // Restore submitted frame count as the first frame after pause may have wrong display time
}

void YVRLifecycleMgr::onPreUnityGfxPrepareNextFrame()
{
    static bool renderThreadReported = false;
    if (!renderThreadReported)
    {
        constexpr XrAndroidThreadTypeKHR type = XR_ANDROID_THREAD_TYPE_RENDERER_MAIN_KHR;
        const auto threadID = static_cast<uint32_t>(gettid());
        OpenXRAPI(xrSetAndroidApplicationThreadKHR(plugin.openxrMgr->program->session, type, threadID));
        renderThreadReported = true;
    }
    // Only handle task while eye buffer has been created
    if (plugin.renderMgr->renderLayersMgr->getEyeBufferLayer() != nullptr)
    {
        int handledTaskCount = 0;
        while (plugin.gfxTasksMgr->HandleAnOnPrepareTask() && handledTaskCount <= 100)
        {
            handledTaskCount++;
        }
    }
}

void YVRLifecycleMgr::onPostUnityGfxPrepareNextFrame()
{}

void YVRLifecycleMgr::onPreUnityGfxSubmitCurrentFrame()
{
    // Only handle task while eye buffer has been created
    if (plugin.renderMgr->renderLayersMgr->getEyeBufferLayer() != nullptr)
    {
        int handledTaskCount = 0;
        while (plugin.gfxTasksMgr->HandleAnOnPreSubmitTask() && handledTaskCount <= 100)
        {
            handledTaskCount++;
        }
    };
}

void YVRLifecycleMgr::onPostUnityGfxSubmitCurrentFrame()
{
    // Only handle task while eye buffer has been created
    if (plugin.renderMgr->renderLayersMgr->getEyeBufferLayer() != nullptr)
    {
        int handledTaskCount = 0;
        while (plugin.gfxTasksMgr->HandleAnOnPostSubmitTask() && handledTaskCount <= 100)
        {
            handledTaskCount++;
        }
    };

    submittedFrameCount++;
}

int YVRLifecycleMgr::getFrameCount() { return frameCount; }

int YVRLifecycleMgr::getSubmittedFrameCount() { return submittedFrameCount; }
