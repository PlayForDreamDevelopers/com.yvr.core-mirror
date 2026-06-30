using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using YVR.Utilities;

public class PremissionRequest : Singleton<PremissionRequest>
{

    public void ScenePermissionRequest()
    {
        const string spatialPermission = "com.yvr.permission.USE_SCENE";
        if (!UnityEngine.Android.Permission.HasUserAuthorizedPermission(spatialPermission))
        {
            var callbacks = new UnityEngine.Android.PermissionCallbacks();
            callbacks.PermissionDenied += Denied;
            callbacks.PermissionGranted += Granted;
            UnityEngine.Android.Permission.RequestUserPermission(spatialPermission, callbacks);
        }
    }
    private void Denied(string permission)  => Debug.Log($"{permission} Denied");
    private void Granted(string permission) => Debug.Log($"{permission} Granted");
}
