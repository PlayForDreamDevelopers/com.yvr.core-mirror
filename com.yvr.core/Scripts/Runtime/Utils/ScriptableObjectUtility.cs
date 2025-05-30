// This script must be defined in Runtime folder, as some class like `ToTrackingImageCollectionSO` need to access it
#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;

namespace YVR.Core.XR
{
    public static class ScriptableObjectUtility
    {
        public static void CreateAsset<T>(T classdata, string path) where T : ScriptableObject
        {
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            string assetPathAndName = AssetDatabase.GenerateUniqueAssetPath(path + typeof(T).Name.ToString() + ".asset");
            AssetDatabase.CreateAsset(classdata, assetPathAndName);
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
        }
    }
}
#endif