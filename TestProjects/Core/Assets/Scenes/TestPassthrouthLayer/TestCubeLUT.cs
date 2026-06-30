using System;
using System.IO;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;
using YVR.Core;

public class TestCubeLUT : MonoBehaviour
{
    public string[] cubeFileNameArray;
    public PassthroughLayer passthroughLayer;
    private Color32[] lutColors;
    public int m_FileIndex = 0;
    public string m_CubeFileName;
    public Text fileNameText;
    public Slider lutWeightSlider;

    private void Start()
    {
        lutWeightSlider.onValueChanged.AddListener((value) =>
        {
            passthroughLayer.lutWeight = value;
            fileNameText.text = $"{m_CubeFileName} \n" +
                                $"LUTWeight:{value*100f}";
        });
    }

    private void Update()
    {
        if (YVRInput.GetDown(YVRInput.VirtualButton.One) || Input.GetKeyDown(KeyCode.A))
        {
            m_CubeFileName = cubeFileNameArray[m_FileIndex];
            m_FileIndex = m_FileIndex++ >= cubeFileNameArray.Length - 1 ? 0 : m_FileIndex;

            _ = LoadCubeFromStreamingAssetsAsync(() =>
            {
                passthroughLayer.SetColorLut(
                    new PassthroughColorLut(lutColors, PassthroughColorLutChannels.Rgb));
                fileNameText.text = $"{m_CubeFileName} \n" +
                                    $"LUTWeight:{lutWeightSlider.value*100f}";
            });
        }

        if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            lutWeightSlider.value += 0.1f;
        }
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            lutWeightSlider.value -= 0.1f;
        }
    }

    private async Task LoadCubeFromStreamingAssetsAsync(Action callback)
    {
        string filePath = Path.Combine(Application.streamingAssetsPath, m_CubeFileName);
        UnityWebRequest request = UnityWebRequest.Get(filePath);

        var operation = request.SendWebRequest();
        while (!operation.isDone)
            await Task.Yield();

        if (request.result != UnityWebRequest.Result.Success)
        {
            Debug.LogError("Failed to load LUT: " + request.error);
            return;
        }

        string content = request.downloadHandler.text;
        try
        {
            lutColors = await Task.Run(() =>
            {
                string[] lines = content.Split(new[] { "\r\n", "\n" }, StringSplitOptions.None);
                return CubeLUTParser.ParseCubeLines(lines);
            });

            Debug.Log("LUT loaded and parsed: " + lutColors.Length + " colors");
            callback?.Invoke();
        }
        catch (Exception ex)
        {
            Debug.LogError("LUT parse failed: " + ex.Message);
        }
    }
}