#include "OpenXRProgram.h"

#include "../Utilities/YVRDebug.h"
#include "./Common.h"
#include "../YVRUnityPlugin.h"

OpenXRProgram::OpenXRProgram() = default;

void OpenXRProgram::createInstance()
{
    createInstanceInternal();
}

void OpenXRProgram::destroyInstance()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrDestroyInstance(instance))
}

void OpenXRProgram::createSystem()
{
    AnnounceCallingFunc();
    XrSystemGetInfo systemInfo{XR_TYPE_SYSTEM_GET_INFO};
    systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    OpenXRAPI(xrGetSystem(instance, &systemInfo, &systemId));
}

void OpenXRProgram::InitializeGfxDevice()
{
    // The graphics API can initialize the graphics device now that the systemId and instance
    // handle are available.
    plugin.openxrMgr->graphicsPlugin->InitializeDevice(instance, systemId);
}

void OpenXRProgram::enumerateSupportedExtensions()
{
    uint32_t count = 0;

    OpenXRAPI(xrEnumerateInstanceExtensionProperties(nullptr, 0, &count, nullptr));
    std::vector<XrExtensionProperties> extensions(count);
    for (XrExtensionProperties& extension : extensions)
        extension.type = XR_TYPE_EXTENSION_PROPERTIES;

    OpenXRAPI(xrEnumerateInstanceExtensionProperties(nullptr, static_cast<uint32_t>(extensions.size()), &count, extensions.data()));
    for (const XrExtensionProperties& extension : extensions)
    {
        supportedExtensions.push_back(std::string(extension.extensionName));
        YDebug("Name=%s SpecVersion=%d", extension.extensionName, extension.extensionVersion)
    }
}

void OpenXRProgram::LogLayersAndExtensions()
{
    const auto logExtensions = [](const char* layerName, int indent = 0)
    {
        uint32_t instanceExtensionCount = 0;
        OpenXRAPI(xrEnumerateInstanceExtensionProperties(layerName, 0, &instanceExtensionCount, nullptr));

        std::vector<XrExtensionProperties> extensions(instanceExtensionCount);
        for (XrExtensionProperties& extension : extensions)
            extension.type = XR_TYPE_EXTENSION_PROPERTIES;

        OpenXRAPI(
            xrEnumerateInstanceExtensionProperties(layerName, static_cast<uint32_t>(extensions.size()), &instanceExtensionCount, extensions.data()));

        const std::string indentStr(indent, ' ');
        for (const XrExtensionProperties& extension : extensions)
        {
            YDebug("%s Name=%s SpecVersion=%d", indentStr.c_str(), extension.extensionName, extension.extensionVersion)
        }
    };

    // Log non-layer extensions (layerName==nullptr).
    logExtensions(nullptr);

    // Log layers and any of their extensions.
    {
        uint32_t layerCount;
        OpenXRAPI(xrEnumerateApiLayerProperties(0, &layerCount, nullptr));

        std::vector<XrApiLayerProperties> layers(layerCount);
        for (XrApiLayerProperties& layer : layers)
        {
            layer.type = XR_TYPE_API_LAYER_PROPERTIES;
        }

        OpenXRAPI(xrEnumerateApiLayerProperties((uint32_t)layers.size(), &layerCount, layers.data()));

        YDebug("Available Layers: (%d)", layerCount);

        for (const XrApiLayerProperties& layer : layers)
        {
            YDebug("Name=%s, LayerVersion=%d Description=%d.%d.%d", layer.layerName, layer.layerVersion, layer.description);
            logExtensions(layer.layerName, 4);
        }
    }
    AnnounceCalledFunc();
}

#define XR_MAKE_VERSION_INT32(major, minor, patch) \
((((major) & 0xff) << 24) | (((minor) & 0xff) << 16) | ((patch) & 0xffff))

void OpenXRProgram::createInstanceInternal()
{
    AnnounceCallingFunc();
    // Create union of extensions required by platform and graphics plugins.
    std::vector<const char*> extensions;
    enumerateSupportedExtensions();

    // Transform platform and graphics extension std::strings to C strings.
    const std::vector<std::string> platformExtensions = plugin.openxrMgr->platformPlugin->GetInstanceExtensions();
    const std::vector<std::string> graphicsExtensions = GraphicsPluginBase::GetInstanceExtensions();
    const std::vector<std::string> yvrExtensions = plugin.openxrMgr->extMgr->getInstanceExtensions();
    std::transform(platformExtensions.begin(), platformExtensions.end(), std::back_inserter(extensions),
                   [](const std::string& ext) { return ext.c_str(); });
    std::transform(graphicsExtensions.begin(), graphicsExtensions.end(), std::back_inserter(extensions),
                   [](const std::string& ext) { return ext.c_str(); });
    std::transform(yvrExtensions.begin(), yvrExtensions.end(), std::back_inserter(extensions),
                   [](const std::string& ext) { return ext.c_str(); });

    XrInstanceCreateInfo createInfo{XR_TYPE_INSTANCE_CREATE_INFO};
    createInfo.next = plugin.openxrMgr->platformPlugin->GetInstanceCreateInfo();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.enabledExtensionNames = extensions.data();
    strcpy(createInfo.applicationInfo.engineName, "UnityPFDM");
    strcpy(createInfo.applicationInfo.applicationName, "UnityPFDM");
    createInfo.applicationInfo.applicationVersion = XR_MAKE_VERSION_INT32(1, 0, 0);
    createInfo.applicationInfo.engineVersion = XR_MAKE_VERSION_INT32(1, 25, 1);
    createInfo.applicationInfo.apiVersion = XR_API_VERSION_1_0;

    xrCreateInstance(&createInfo, &instance);
    AnnounceCalledFunc();
}

void OpenXRProgram::LogInstanceInfo()
{
    AnnounceCallingFunc();
    CHECK(instance != XR_NULL_HANDLE);

    XrInstanceProperties instanceProperties{XR_TYPE_INSTANCE_PROPERTIES};
    OpenXRAPI(xrGetInstanceProperties(instance, &instanceProperties));
    YInfo("Instance RuntimeName=%s RuntimeVersion=%s", instanceProperties.runtimeName,
          GetXrVersionString(instanceProperties.runtimeVersion).c_str());
}

void OpenXRProgram::createSession()
{
    AnnounceCallingFunc();
    XrSessionCreateInfo createInfo{XR_TYPE_SESSION_CREATE_INFO};
    createInfo.next = plugin.openxrMgr->graphicsPlugin->GetGraphicsBinding();
    createInfo.systemId = systemId;
    OpenXRAPI(xrCreateSession(instance,&createInfo,&session));
}

void OpenXRProgram::destroySession()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrDestroySession(session));
}

void OpenXRProgram::getStereoViewsConfigs()
{
    uint32_t viewsCount{2};

    constexpr XrViewConfigurationType targetType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    OpenXRAPI(xrEnumerateViewConfigurationViews(instance, systemId, targetType, 0, &viewsCount, nullptr));

    stereoViewsConfigs.resize(viewsCount, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
    OpenXRAPI(xrEnumerateViewConfigurationViews(instance, systemId, targetType, viewsCount, &viewsCount, stereoViewsConfigs.data()));
}

void OpenXRProgram::getQuadViewsConfigs()
{
    const bool extensionEnabled = plugin.openxrMgr->extMgr->isEnableTargetExt(XR_VARJO_QUAD_VIEWS_EXTENSION_NAME)
                                  && plugin.openxrMgr->extMgr->isEnableTargetExt(XR_VARJO_FOVEATED_RENDERING_EXTENSION_NAME);

    if (!extensionEnabled) return;

    XrSystemFoveatedRenderingPropertiesVARJO foveatedRenderProp = {};
    foveatedRenderProp.type = XR_TYPE_SYSTEM_FOVEATED_RENDERING_PROPERTIES_VARJO;
    foveatedRenderProp.next = nullptr;

    XrSystemProperties systemProperties = {XR_TYPE_SYSTEM_PROPERTIES};
    systemProperties.next = &foveatedRenderProp;
    OpenXRAPI(xrGetSystemProperties(plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId, &systemProperties));

    supportQuadViews = foveatedRenderProp.supportsFoveatedRendering;
    if (!supportQuadViews) return;

    constexpr XrViewConfigurationType targetType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_QUAD_VARJO;

    uint32_t viewportConfigTypeCount = 0;
    OpenXRAPI(xrEnumerateViewConfigurations(plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId,
        0, &viewportConfigTypeCount, NULL));

    // Check if the preferred view configuration is supported, if not, fallback to XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO
    std::vector<XrViewConfigurationType> types{};
    types.resize(viewportConfigTypeCount);
    OpenXRAPI(xrEnumerateViewConfigurations( plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId, viewportConfigTypeCount,
        &viewportConfigTypeCount, types.data()));

    if (std::find(types.begin(), types.end(), targetType) == types.end())
        supportQuadViews = false;

    uint32_t viewsCount{4};

    OpenXRAPI(xrEnumerateViewConfigurationViews(instance, systemId, targetType, 0, &viewsCount, nullptr));

    quadViewsConfigs.resize(viewsCount, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
    OpenXRAPI(xrEnumerateViewConfigurationViews(instance, systemId, targetType, viewsCount, &viewsCount, quadViewsConfigs .data()));
}

void OpenXRProgram::getViewConfigurations()
{
    // When creating the open xr program, we dont know which rendering mode will finally be used, so we need to get both stereo and quad views
    getQuadViewsConfigs();
    getStereoViewsConfigs();
}


void OpenXRProgram::BeginSession()
{
    AnnounceCallingFunc();
    XrSessionBeginInfo sessionBeginInfo{XR_TYPE_SESSION_BEGIN_INFO};
    sessionBeginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
    OpenXRAPI(xrBeginSession(session, &sessionBeginInfo));
}

void OpenXRProgram::EndSession()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrEndSession(session));
}
