#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
using System;
using System.Collections.Generic;
using System.Threading;
using Unity.Collections;
using UnityEngine;
using UnityEngine.XR.ARSubsystems;

namespace YVR.Core.ARFoundation.Anchor
{
    public partial class YVRAnchorProvider : XRAnchorSubsystem.Provider
    {
        private static Dictionary<TrackableId, ulong> m_TrackableIdToHandleMap;
        private static Dictionary<ulong, XRAnchor> m_HandleToXRAnchorMap;

        private YVRSpatialAnchor m_SpatialAnchor;

        public YVRAnchorProvider()
        {
            m_SpatialAnchor = YVRSpatialAnchor.instance;
        }

        public override void Start()
        {
            m_TrackableIdToHandleMap = new();
            m_HandleToXRAnchorMap = new();
        }

        public override void Stop()
        {
        }

        public override void Destroy()
        {
            m_TrackableIdToHandleMap.Clear();
            m_HandleToXRAnchorMap.Clear();
        }

        public override TrackableChanges<XRAnchor> GetChanges(XRAnchor defaultAnchor, Allocator allocator)
        {
            var added = new NativeArray<XRAnchor>();
            var updated = new NativeArray<XRAnchor>(m_TrackableIdToHandleMap.Count, allocator);
            var removed = new NativeArray<TrackableId>();
            int updateIndex = 0;

            foreach (var pair in m_HandleToXRAnchorMap)
            {
                Pose pose;
                XRAnchor xrAnchor;
                if (m_SpatialAnchor.GetSpatialAnchorPose(pair.Key, out var position, out var rotation,
                        out var locationFlags))
                {
                    pose = new Pose(position, rotation);
                    xrAnchor = CreateXRAnchor(pair.Value.trackableId, pose, pair.Key, TrackingState.Tracking);
                }
                else
                {
                    pose = new Pose();
                    xrAnchor = CreateXRAnchor(pair.Value.trackableId, pose, pair.Key, TrackingState.None);
                }

                updated[updateIndex] = xrAnchor;
                updateIndex++;
            }

            return TrackableChanges<XRAnchor>.CopyFrom(added, updated, removed, allocator);
        }

        private TrackableId GenerateTrackableId(char[] uuid)
        {
            Guid guid = new(new string(uuid));
            byte[] bytes = guid.ToByteArray();
            TrackableId trackableId = new TrackableId(BitConverter.ToUInt64(bytes, 0), BitConverter.ToUInt64(bytes, 8));
            return trackableId;
        }

        private char[] RestoreUuidFromTrackableId(TrackableId trackableId)
        {
            ulong part1 = trackableId.subId1;
            ulong part2 = trackableId.subId2;

            byte[] bytes = new byte[16];
            Array.Copy(BitConverter.GetBytes(part1), 0, bytes, 0, 8);
            Array.Copy(BitConverter.GetBytes(part2), 0, bytes, 8, 8);

            Guid guid = new Guid(bytes);

            char[] uuidChars = guid.ToString("N").ToUpper().ToCharArray();

            return uuidChars;
        }

        private XRAnchor CreateXRAnchor(TrackableId id, Pose pose, ulong anchorHandle, TrackingState trackingState)
        {
            return new XRAnchor(
                id,
                pose,
                trackingState,
                new IntPtr((long)anchorHandle)
            );
        }

#if XR_ARFOUNDATION_6
            public override Awaitable<Result<XRAnchor>> TryAddAnchorAsync(Pose pose)
            {
                AwaitableCompletionSource<Result<XRAnchor>> completionSource = new();
                m_SpatialAnchor.CreateSpatialAnchor(pose.position,pose.rotation,onCreateResult);
                void onCreateResult(YVRSpatialAnchorResult anchorResult,bool success)
                {
                    var synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnknownError);
                    var result = new Result<XRAnchor>(synchronousResultStatus, XRAnchor.defaultValue);
                    if (success)
                    {
                        synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnqualifiedSuccess);
                        XRAnchor createAnchor =
 CreateXRAnchor(GenerateTrackableId(anchorResult.uuid), pose, anchorResult.anchorHandle,TrackingState.Tracking);
                        result = new Result<XRAnchor>(synchronousResultStatus, createAnchor);
                        m_TrackableIdToHandleMap.TryAdd(createAnchor.trackableId, anchorResult.anchorHandle);
                        m_HandleToXRAnchorMap.TryAdd(anchorResult.anchorHandle, createAnchor);
                    }

                    completionSource.SetResult(result);
                }

                return completionSource.Awaitable;
            }

            public override Awaitable<Result<SerializableGuid>> TrySaveAnchorAsync(
                TrackableId anchorId, CancellationToken cancellationToken = default)
            {
                var synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnknownError);
                var completionSource = new AwaitableCompletionSource<Result<SerializableGuid>>();
                var returnResult = new Result<SerializableGuid>(synchronousResultStatus, default);

                if (m_TrackableIdToHandleMap.TryGetValue(anchorId, out ulong handle))
                {
                    var saveInfo = new YVRSpatialAnchorSaveInfo
                    {
                        anchorHandle = handle,
                        storageLocation = YVRSpatialAnchorStorageLocation.Local
                    };

                    m_SpatialAnchor.SaveSpatialAnchor(saveInfo, (saveResult, success) =>
                    {
                        if (success)
                        {
                            synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnqualifiedSuccess);
                            returnResult = new Result<SerializableGuid>(synchronousResultStatus, anchorId);
                        }
                        completionSource.SetResult(returnResult);
                    });
                }
                else
                {
                    Debug.LogError($"m_TrackableIdToHandleMap not has anchorId:{anchorId}");
                    completionSource.SetResult(returnResult);
                }

                return completionSource.Awaitable;
            }

            public override Awaitable<XRResultStatus> TryEraseAnchorAsync(SerializableGuid savedAnchorGuid,
                CancellationToken cancellationToken = default)
            {
                var synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnknownError);
                var completionSource = new AwaitableCompletionSource<XRResultStatus>();
                if (m_TrackableIdToHandleMap.TryGetValue(savedAnchorGuid, out var anchorHandle))
                {
                    m_SpatialAnchor.EraseSpatialAnchor(anchorHandle, YVRSpatialAnchorStorageLocation.Local,
                        (result, success) =>
                        {
                            if (success)
                            {
                                synchronousResultStatus =
                                    new XRResultStatus(XRResultStatus.StatusCode.UnqualifiedSuccess);
                                m_TrackableIdToHandleMap.Remove(savedAnchorGuid);
                                if (m_HandleToXRAnchorMap.ContainsKey(anchorHandle))
                                    m_HandleToXRAnchorMap.Remove(anchorHandle);
                            }

                            completionSource.SetResult(synchronousResultStatus);
                        });
                }
                else
                {
                    Debug.LogError($"m_TrackableIdToHandleMap not has savedAnchorGuid:{savedAnchorGuid}");
                    completionSource.SetResult(synchronousResultStatus);
                }

                return completionSource.Awaitable;
            }

            public override Awaitable<Result<XRAnchor>> TryLoadAnchorAsync(SerializableGuid savedAnchorGuid,
                CancellationToken cancellationToken = default)
            {
                var synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnknownError);
                var completionSource = new AwaitableCompletionSource<Result<XRAnchor>>();
                var anchor = XRAnchor.defaultValue;
                var uuid = new YVRSpatialAnchorUUID();
                uuid.Id = RestoreUuidFromTrackableId(savedAnchorGuid);
                YVRSpatialAnchorQueryInfo queryInfo = new YVRSpatialAnchorQueryInfo
                {
                    storageLocation = YVRSpatialAnchorStorageLocation.Local,
                    numIds = 1,
                    ids = new[] {uuid}
                };

                m_SpatialAnchor.QuerySpatialAnchor(queryInfo, (result) =>
                {
                    synchronousResultStatus = new XRResultStatus(XRResultStatus.StatusCode.UnqualifiedSuccess);
                    if (result != null)
                    {
                        for (int i = 0; i < result.Count; i++)
                        {
                            var nativePtr = new IntPtr((long)result[i].anchorHandle);
                            m_SpatialAnchor.GetSpatialAnchorPose(result[i].anchorHandle, out var position,
                                out var rotation, out var locationFlags);
                            anchor = new XRAnchor(savedAnchorGuid, new Pose(position, rotation), TrackingState.Tracking,
                                nativePtr);
                            m_TrackableIdToHandleMap[savedAnchorGuid] = result[i].anchorHandle;
                            m_HandleToXRAnchorMap[result[i].anchorHandle] = anchor;
                        }
                    }

                    var returnResult = new Result<XRAnchor>(synchronousResultStatus, anchor);
                    completionSource.SetResult(returnResult);
                });

                return completionSource.Awaitable;
            }
#endif
    }
}
#endif