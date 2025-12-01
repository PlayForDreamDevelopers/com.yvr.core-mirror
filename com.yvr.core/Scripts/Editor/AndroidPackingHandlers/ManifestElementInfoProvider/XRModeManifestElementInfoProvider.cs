using UnityEditor;
using YVR.Utilities.Editor.PackingProcessor;

namespace YVR.Core.Editor.Packing
{
    public class XRModeManifestElementInfoProvider : ManifestElementInfoProvider
    {
        private static XRModeManifestElementInfoProvider s_Instance;

        [InitializeOnLoadMethod]
        private static void Init()
        {
            s_Instance ??= new XRModeManifestElementInfoProvider();
            AndroidManifestHandler.manifestElementInfoProviders.Add(s_Instance);
        }

        public override string manifestElementName => "org.khronos.openxr.intent.category.IMMERSIVE_HMD";

        private  string applicationModeName => "com.yvr.application.mode";

        public override void HandleManifestElementInfo()
        {
            var info = new ManifestTagInfo("/manifest/application/activity/intent-filter",
                                           "category", "name",
                                           manifestElementName);

            AndroidManifestHandler.UpdateManifestElement(manifestElementName, info);
            RequireVROnly();
        }

        private void RequireVROnly()
        {
            var info = new ManifestTagInfo("/manifest/application",
                                           "meta-data", "name",
                                           applicationModeName,
                                           new[] {"value", "vr_only"},
                                           true);

            AndroidManifestHandler.UpdateManifestElement(applicationModeName, info);
        }
    }
}