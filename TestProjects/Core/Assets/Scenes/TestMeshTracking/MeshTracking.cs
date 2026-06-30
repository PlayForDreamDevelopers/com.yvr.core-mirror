using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using YVR.Core;

public class MeshTracking : MonoBehaviour
{
    public GameObject meshBlockPrefab;
    public Transform target;
    private Dictionary<ulong, MeshFilter> m_MeshBlockDic = new Dictionary<ulong, MeshFilter>();
    // Start is called before the first frame update
    void Start()
    {
        YVRPlugin.Instance.SetPassthrough(true);
        YVRMeshTracking.instance.CreateMeshDetector();
        YVRMeshTracking.updateMeshBlockAction += UpdateMeshBlock;
    }

    private void UpdateMeshBlock(ulong xrSpace, YVRMeshBlockChangeState blockChangeState)
    {
        switch (blockChangeState)
        {
            case YVRMeshBlockChangeState.Added:
            case YVRMeshBlockChangeState.Updated:
                GetMeshBlockData(xrSpace);
                break;
            case YVRMeshBlockChangeState.Removed:
                if (m_MeshBlockDic.TryGetValue(xrSpace, out var value))
                    Destroy(value);
                break;
        }
    }

    private void Update()
    {
        foreach (var meshBlock in m_MeshBlockDic)
        {
            bool result = YVRSpatialAnchor.instance.GetSpatialAnchorPose(meshBlock.Key, out Vector3 position, out Quaternion rotation, out YVRAnchorLocationFlags flags);
            if (result)
            {
                if (meshBlock.Value != null && position != meshBlock.Value.transform.position)
                {
                    meshBlock.Value.transform.position = position;
                    meshBlock.Value.transform.rotation = rotation;
                }
            }
        }
    }

    public bool GetMeshBlockData(ulong xrSpace)
    {
        bool result = YVRSceneAnchor.instance.GetAnchorTriangleMesh(xrSpace, out YVRTriangleMesh triangleMesh);
        if (result && triangleMesh.indices.Count > 0 && triangleMesh.vertices.Count > 0)
        {
            if (!m_MeshBlockDic.ContainsKey(xrSpace))
            {
                MeshFilter meshFilter = Instantiate(meshBlockPrefab, target).AddComponent<MeshFilter>();
                meshFilter.mesh = new Mesh();
                m_MeshBlockDic.Add(xrSpace, meshFilter);
            }

            GenerateMesh(m_MeshBlockDic[xrSpace].mesh,triangleMesh);
        }
        else
            Debug.LogError($"GetMeshBlockData failed");

        return result;
    }

    private void GenerateMesh(Mesh mesh, YVRTriangleMesh meshBlockData)
    {
        mesh.Clear();
        mesh.vertices = meshBlockData.vertices.ToArray();
        mesh.triangles = meshBlockData.indices.ToArray();
        mesh.RecalculateNormals();
        mesh.RecalculateBounds();
    }
}
