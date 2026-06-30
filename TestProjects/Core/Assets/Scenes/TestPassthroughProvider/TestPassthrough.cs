using UnityEngine;
using UnityEngine.XR;
using YVR.Core;
using YVR.Utilities;

public class TestPassthrough : MonoBehaviour
{
    public Material mat;
    public Renderer rendererComponent;
    RenderTexture rt;
    private int destId;
    private static Matrix4x4[] s_ViewMatrixArray = new Matrix4x4[2];

    // Start is called before the first frame update
    void Start()
    {
        //YVRPlugin.Instance.SetPassthrough(true);
        rt = new RenderTexture(XRSettings.eyeTextureDesc);
        rt.Create();
        destId = (int)rt.GetNativeTexturePtr();
    }

    // Update is called once per frame
    void Update()
    {
        Vector2 resolution = YVRManager.instance.cameraRenderer.GetEyeRenderDesc(EyeSide.Left).resolution;
        float verticalHalfRad = YVRManager.instance.cameraRenderer.GetEyeRenderDesc(EyeSide.Left).fov.y / 2f;
        float height = Mathf.Tan(verticalHalfRad) * this.transform.localPosition.z * 2f;
        float width = (resolution.x / resolution.y) * height;

        //Debug.LogError("LQR Fov " + Camera.main.fieldOfView + " " + YVRManager.instance.cameraRenderer.GetEyeRenderDesc(EyeSide.Left).fov + " " + Camera.main.aspect + " " + (resolution.x / resolution.y));

        this.transform.localScale = new Vector3(width, height, 1f);

        mat.SetTexture("_MainTex", rt);
        if (YVRInput.GetDown(YVRInput.RawButton.A))
        {
            GetComponent<Renderer>().enabled = !GetComponent<Renderer>().enabled;
        }
    }

    void LateUpdate()
    {
        GfxHelper.instance.TriggerGfxThreadCallback(() =>
        {
            int vstTextureId = YVRPlugin.Instance.GetPassthroughSwapchainImageIndex();
            Quaternion leftEyePassthroughRotation = YVRPlugin.Instance.GetPassthroughImageDiffRotation(0);
            Quaternion rightEyePassthroughRotation = YVRPlugin.Instance.GetPassthroughImageDiffRotation(1);

            YVRCameraRig.centerEyeDevice.TryGetFeatureValue(UnityEngine.XR.CommonUsages.leftEyeRotation, out Quaternion leftEyeRotationQuat);
            Quaternion leftDiff = Quaternion.Inverse(leftEyePassthroughRotation) * leftEyeRotationQuat;

            YVRCameraRig.centerEyeDevice.TryGetFeatureValue(UnityEngine.XR.CommonUsages.rightEyeRotation, out Quaternion rightEyeRotationQuat);
            Quaternion rightDiff = Quaternion.Inverse(rightEyePassthroughRotation) * rightEyeRotationQuat;

            Matrix4x4 leftDiffMatrix = Matrix4x4.Rotate(leftDiff);
            Matrix4x4 rightDiffMatrix = Matrix4x4.Rotate(rightDiff);

            Matrix4x4 modelMatrix = Matrix4x4.TRS(Vector3.back, Quaternion.identity, new Vector3(1.08108f, 1.0f, 1.0f));

            Matrix4x4 projMat = Matrix4x4.Perspective(90f, 1.08108f, 0.1f, 10f);

            s_ViewMatrixArray[0] = projMat * leftDiffMatrix * modelMatrix;
            s_ViewMatrixArray[1] = projMat * rightDiffMatrix * modelMatrix;


            GfxHelper.instance.CopyToTextureArrayMultiview(vstTextureId, destId, s_ViewMatrixArray);
            //GfxHelper.instance.CopyTexture(textureId, destId);
        });
    }
}
