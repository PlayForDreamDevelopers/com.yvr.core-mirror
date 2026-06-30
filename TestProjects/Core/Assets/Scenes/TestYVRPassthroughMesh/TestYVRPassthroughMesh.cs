using System;
using System.Runtime.InteropServices;
using UnityEngine;
using YVR.Core;

public class TestYVRPassthroughMesh : MonoBehaviour
{
    [StructLayout(LayoutKind.Sequential)]
    public struct PersistentPassthroughMeshModeInfo
    {
        public bool enabled;
        public bool followMode;
        public int controller; // 0:LeftController; 1:RightController
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PersistentPassthroughMeshTransform
    {
        public long time;
        public XRPose pose;
        public Vector3 scale;
    }

    // DllImports
    private const string LibraryName = "yvrplugin";

    [DllImport(LibraryName)]
    private static extern void YVRSetPersistentPassthroughMeshInfoYVR(PersistentPassthroughMeshModeInfo setInfo);

    [DllImport(LibraryName)]
    private static extern void YVRGetPersistentPassthroughMeshInfoYVR(ref PersistentPassthroughMeshModeInfo getInfo);

    [DllImport(LibraryName)]
    private static extern void YVRSetPersistentPassthroughMeshYVR(ref YVRAnchorTriangleMeshInternal mesh);

    [DllImport(LibraryName)]
    private static extern void YVRSetPersistentPassthroughTransformYVR(PersistentPassthroughMeshTransform transform);

    void Start()
    {
        Debug.Log("TestYVRPassthroughMesh Start");
        TestSetPersistentPassthroughMeshInfo();
        TestGetPersistentPassthroughMeshInfo();
        TestSetPersistentPassthroughMesh();
        TestSetPersistentPassthroughTransform();
    }

    public void TestSetPersistentPassthroughMeshInfo()
    {
        PersistentPassthroughMeshModeInfo modeInfo = new PersistentPassthroughMeshModeInfo
        {
            enabled = true,
            followMode = false,
            controller = 0
        };

        Debug.Log($"Setting Mesh Info: Enable={modeInfo.enabled}, Follow={modeInfo.followMode}");
        YVRSetPersistentPassthroughMeshInfoYVR(modeInfo);
    }

    public void TestGetPersistentPassthroughMeshInfo()
    {
        PersistentPassthroughMeshModeInfo getInfo = new PersistentPassthroughMeshModeInfo();
        YVRGetPersistentPassthroughMeshInfoYVR(ref getInfo);
        Debug.Log($"Got Mesh Info: Enable={getInfo.enabled}");
    }

    public void TestSetPersistentPassthroughMesh()
    {
        int vertexCount = 3;
        int indexCount = 3;
        int vertexSize = Marshal.SizeOf<Vector3>();
        int indexSize = sizeof(int);

        IntPtr verticesPtr = Marshal.AllocHGlobal(vertexSize * vertexCount);
        IntPtr indicesPtr = Marshal.AllocHGlobal(indexSize * indexCount);

        try
        {
            Vector3[] vertices = new Vector3[vertexCount];
            vertices[0] = new Vector3 { x = 0, y = 0, z = 0 };
            vertices[1] = new Vector3 { x = 1, y = 0, z = 0 };
            vertices[2] = new Vector3 { x = 0, y = 1, z = 0 };

            int[] indices = new int[] { 0, 1, 2 };

            for (int i = 0; i < vertexCount; i++)
            {
                IntPtr indexPtr = IntPtr.Add(verticesPtr, i * vertexSize);
                Marshal.StructureToPtr(vertices[i], indexPtr, false);
            }

            Marshal.Copy(indices, 0, indicesPtr, indices.Length);
            YVRAnchorTriangleMeshInternal meshInternal = new YVRAnchorTriangleMeshInternal
            {
                vertexCapacityInput = vertexCount,
                vertexCountOutput = vertexCount,
                verticesIntPtr = verticesPtr,
                indexCapacityInput = indexCount,
                indexCountOutput = indexCount,
                indicesIntPtr = indicesPtr
            };

            Debug.Log("Setting Persistent Passthrough Mesh (Dummy)");
            YVRSetPersistentPassthroughMeshYVR(ref meshInternal);
        }
        finally
        {
            Marshal.FreeHGlobal(verticesPtr);
            Marshal.FreeHGlobal(indicesPtr);
        }
    }

    public void TestSetPersistentPassthroughTransform()
    {
        PersistentPassthroughMeshTransform transformYVR = new PersistentPassthroughMeshTransform
        {
            pose = new XRPose(Vector3.zero,Quaternion.identity),
            scale = new Vector3 { x = 1, y = 1, z = 1 }
        };

        YVRSetPersistentPassthroughTransformYVR(transformYVR);
    }
}
