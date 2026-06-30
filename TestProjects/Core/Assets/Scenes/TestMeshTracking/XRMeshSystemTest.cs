using System.Collections.Generic;
using Unity.Collections;
using UnityEngine;
using UnityEngine.XR;
using YVR.Core;

public class XRMeshSystemTest : MonoBehaviour
{
    public GameObject emptyMeshPrefab;
    public Transform target;
    private static XRMeshSubsystem s_MeshSubsystem;
    private static List<MeshInfo> s_MeshInfos = new List<MeshInfo>();
    private Dictionary<MeshId, MeshFilter> m_MeshIdToGo = new Dictionary<MeshId, MeshFilter>();

    // Start is called before the first frame update
    void Start()
    {
        PremissionRequest.instance.ScenePermissionRequest();
        YVRMeshTracking.instance.CreateMeshDetector();
        YVRPlugin.Instance.SetPassthrough(true);
        var meshSubsystems = new List<XRMeshSubsystem>();
        SubsystemManager.GetInstances(meshSubsystems);
        if (meshSubsystems.Count == 1)
        {
            s_MeshSubsystem = meshSubsystems[0];
        }
    }

    private void Update()
    {
        if (s_MeshSubsystem == null)
        {
            Debug.LogError("s_MeshSubsystem is null");
            return;
        }
        if (s_MeshSubsystem.TryGetMeshInfos(s_MeshInfos))
        {
            foreach (var meshInfo in s_MeshInfos)
            {
                switch (meshInfo.ChangeState)
                {
                    case MeshChangeState.Added:
                    case MeshChangeState.Updated:
                        AddToQueueIfNecessary(meshInfo);
                        break;
                    case MeshChangeState.Removed:
                        if (m_MeshIdToGo.TryGetValue(meshInfo.MeshId, out var meshGo))
                        {
                            Destroy(meshGo);
                            m_MeshIdToGo.Remove(meshInfo.MeshId);
                        }
                        break;
                    default:
                        break;
                }
            }

            UpdateMeshTransform();
        }
    }

    private void UpdateMeshTransform()
    {
        NativeArray<MeshTransform> meshTransforms = s_MeshSubsystem.GetUpdatedMeshTransforms(Allocator.Temp);
        foreach (var item in meshTransforms)
        {
            if (m_MeshIdToGo.TryGetValue(item.MeshId, out MeshFilter meshFilter))
            {
                if (meshFilter.transform.position != item.Position)
                {
                    meshFilter.transform.position = item.Position;
                    meshFilter.transform.rotation = item.Rotation;
                }
            }
        }
    }

    private void AddToQueueIfNecessary(MeshInfo meshInfo)
    {
        if (!m_MeshIdToGo.TryGetValue(meshInfo.MeshId, out var meshFilter))
        {
            meshFilter = Instantiate(emptyMeshPrefab, target, false).AddComponent<MeshFilter>();
            m_MeshIdToGo[meshInfo.MeshId] = meshFilter;
        }

        var mesh = meshFilter.mesh;
        s_MeshSubsystem.GenerateMeshAsync(meshInfo.MeshId, mesh, null, MeshVertexAttributes.None, (result) =>
        {
            if (result.Status == MeshGenerationStatus.Success)
            {
                if (meshFilter.transform.position != result.Position)
                {
                    meshFilter.transform.position = result.Position;
                    meshFilter.transform.rotation = result.Rotation;
                }
            }
        }, MeshGenerationOptions.ConsumeTransform);
    }
}
