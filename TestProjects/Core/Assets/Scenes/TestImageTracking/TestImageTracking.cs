using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using UnityEngine;
using UnityEngine.Serialization;
using UnityEngine.UI;
using YVR.Core;
using YVR.Core.ImageTracking;
using YVR.Utilities;

public class TestImageTracking : MonoBehaviour
{
    public Text text = null;
    [FormerlySerializedAs("cubeTemplate")] public GameObject cubePrefab = null;

    private Dictionary<string, TrackedImageInfo> m_Id2InfoDict = new();
    private Dictionary<string, Transform> m_Id2CubeDict = new();

    private void Start()
    {
        YVRManager.instance.hmdManager.SetPassthrough(true);
        ImageTrackingMgr.instance.SwitchImageTracking(true);
        ImageTrackingMgr.instance.RegisterTrackImageLibrary();
        ImageTrackingMgr.instance.RegisterImageTrackingUpdateCallback(OnImageTrackingUpdate);
        ImageTrackingMgr.instance.UnRegisterImageTemplate("123");

        // Load content from /sdcard/ToTrackImage.json
        string jsonFilePath = Path.Combine(Application.persistentDataPath, "ToTrackImage.json");
        if (File.Exists(jsonFilePath))
        {
            string jsonContent = File.ReadAllText(jsonFilePath);
            var ret = ScriptableObject.CreateInstance<ToTrackImagesCollectionSO>();
            JsonConvert.PopulateObject(jsonContent, ret);
            Debug.Log($"sss ret size is {ret.toTrackImages.Count}");

            ret.toTrackImages.ForEach(image => ImageTrackingMgr.instance.RegisterTemplateTemplate(image));
        }
        else
        {
            Debug.Log($"sss json path is {jsonFilePath}");
        }
    }

    private Dictionary<string, Color> m_Id2ColorDict = new()
    {
        {"0001", Color.red},
        {"jetPost", Color.green},
    };

    private void OnImageTrackingUpdate(TrackedImageInfo info)
    {
        Debug.Log($"sss Receive image info {info.imageId}");
        if (!m_Id2CubeDict.TryGetValue(info.imageId, out Transform cube))
        {
            Debug.Log($"sss Create cube for  {info.imageId}");
            cube = Instantiate(cubePrefab).transform;
            m_Id2CubeDict[info.imageId] = cube;
            cube.GetComponent<MeshRenderer>().material.color
                = m_Id2ColorDict.TryGetValue(info.imageId, out Color color) ? color : Color.white;
        }

        Debug.Log($"sss update cube for  {info.imageId}, {info.pose.position}, {info.pose.orientation}, {info.size}");
        cube.position = info.pose.position;
        cube.rotation = info.pose.orientation;
        cube.localScale = new Vector3(info.size.x, info.size.y, 0.05f);

        m_Id2InfoDict[info.imageId] = info;

        text.text = "";
        m_Id2InfoDict.Values.ForEach(imageInfo =>
        {
            text.text
                += $"Image: {imageInfo.imageId} is active {imageInfo.isActive == 1}, size {imageInfo.size}, confidence {imageInfo.confidence} \n";
        });
    }
}