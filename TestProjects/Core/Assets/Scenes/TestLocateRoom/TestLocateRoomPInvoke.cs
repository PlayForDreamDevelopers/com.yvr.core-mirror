using System;
using System.Runtime.InteropServices;
using UnityEngine;

public class TestLocateRoomPInvoke : MonoBehaviour
{
    private const string LibraryName = "yvrplugin";
    private const int XR_TYPE_LOCATE_ROOM_INFO_YVR = 1000516002;
    private const int XR_UUID_SIZE_EXT = 16;

    [SerializeField] private string roomUuidHex = "";
    [SerializeField] private bool isLocated;
    [SerializeField] private ulong sequence;

    [StructLayout(LayoutKind.Sequential)]
    public struct XrUuidExt
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = XR_UUID_SIZE_EXT)]
        public byte[] data;

        public static XrUuidExt FromHex(string hex)
        {
            var uuid = new XrUuidExt { data = new byte[XR_UUID_SIZE_EXT] };
            if (string.IsNullOrWhiteSpace(hex))
            {
                return uuid;
            }

            var normalized = hex.Trim().Replace("-", string.Empty);
            if (normalized.Length != XR_UUID_SIZE_EXT * 2)
            {
                throw new ArgumentException($"UUID hex length must be {XR_UUID_SIZE_EXT * 2}.", nameof(hex));
            }

            for (var i = 0; i < XR_UUID_SIZE_EXT; i++)
            {
                uuid.data[i] = Convert.ToByte(normalized.Substring(i * 2, 2), 16);
            }

            return uuid;
        }

        public string ToHexString()
        {
            return data == null ? string.Empty : BitConverter.ToString(data).Replace("-", string.Empty);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XrLocateRoomInfoYVR
    {
        public int type;
        public IntPtr next;
        public XrUuidExt uuid;

        [MarshalAs(UnmanagedType.I1)]
        public bool isLocated;

        public ulong sequence;
    }

    [DllImport(LibraryName)]
    private static extern int YVRSetLocateRoom(ref XrLocateRoomInfoYVR roomInfo);

    [DllImport(LibraryName)]
    private static extern int YVRGetLocateRoom(ref XrLocateRoomInfoYVR roomInfo);

    [ContextMenu("Set Locate Room")]
    public void SetLocateRoom()
    {
        try
        {
            var roomInfo = CreateRoomInfo();
            var result = YVRSetLocateRoom(ref roomInfo);
            Debug.Log($"YVRSetLocateRoom result={result}, uuid={roomInfo.uuid.ToHexString()}, isLocated={roomInfo.isLocated}, sequence={roomInfo.sequence}");
        }
        catch (Exception ex)
        {
            Debug.LogError($"SetLocateRoom failed: {ex.Message}");
        }
    }

    [ContextMenu("Get Locate Room")]
    public void GetLocateRoom()
    {
        try
        {
            var roomInfo = CreateRoomInfo();
            var result = YVRGetLocateRoom(ref roomInfo);
            roomUuidHex = roomInfo.uuid.ToHexString();
            isLocated = roomInfo.isLocated;
            sequence = roomInfo.sequence;
            Debug.Log($"YVRGetLocateRoom result={result}, uuid={roomUuidHex}, isLocated={isLocated}, sequence={sequence}");
        }
        catch (Exception ex)
        {
            Debug.LogError($"GetLocateRoom failed: {ex.Message}");
        }
    }

    private XrLocateRoomInfoYVR CreateRoomInfo()
    {
        return new XrLocateRoomInfoYVR
        {
            type = XR_TYPE_LOCATE_ROOM_INFO_YVR,
            next = IntPtr.Zero,
            uuid = XrUuidExt.FromHex(roomUuidHex),
            isLocated = isLocated,
            sequence = sequence
        };
    }
}
