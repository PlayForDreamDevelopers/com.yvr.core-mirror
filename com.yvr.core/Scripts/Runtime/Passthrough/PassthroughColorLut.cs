using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Unity.Collections;
using Unity.Jobs;
using UnityEngine;

namespace YVR.Core
{

    public class PassthroughColorLut : System.IDisposable
    {
        private const int RecomendedBatchSize = 128;

        /// <summary>
        /// The size of one edge of the color cube spanned by the LUT. For example, a resolution of 16
        /// means that the LUT encompasses 16 * 16 * 16 colors.
        /// This property is defined at construction time and cannot be modified.
        /// </summary>
        public uint Resolution { get; private set; }

        /// <summary>
        /// The color channels contained in each of the resulting colors. Can be `ColorChannels.Rgb` or
        /// `ColorChannels.Rgba`.
        /// This property is defined at construction time and cannot be modified.
        /// </summary>
        public PassthroughColorLutChannels Channels { get; private set; }

        public bool IsValid => m_CreateState != CreateState.Invalid;

        internal UInt64 m_ColorLutHandle;
        private GCHandle m_AllocHandle;
        private PassthroughColorLutData m_LutData;
        private int m_ChannelCount;
        private byte[] m_ColorBytes;
        private object m_Locker = new object();

        private CreateState m_CreateState = CreateState.Invalid;

        /// <summary>
        /// Initialize the color LUT data from a texture. Color channels are inferred from texture format.
        /// Use `UpdateFrom()` to update LUT data after construction.
        /// </summary>
        /// <param name="initialLutTexture">Texture to initialize the LUT from</param>
        /// <param name="flipY">Flag to inform whether the LUT texture should be flipped vertically. This is needed for LUT images which have color (0, 0, 0)
        /// in the top-left corner. Some color grading systems, e.g. Unity post-processing, have color (0, 0, 0) in the bottom-left corner,
        /// in which case flipping is not needed.</param>
        public PassthroughColorLut(Texture2D initialLutTexture, bool flipY = true)
            : this(GetTextureSize(initialLutTexture), GetChannelsForTextureFormat(initialLutTexture.format))
        {
            Create(CreateLutDataFromTexture(initialLutTexture, flipY));
        }

        /// <summary>
        /// Set the color LUT data from an array of `Color`. The resolution is
        /// inferred from the array size, thus the size needs to be a result of
        /// `resolution = size ^ 3 * numColorChannels`, where `numColorChannels` depends
        /// on `channels`.
        /// Use `UpdateFrom()` to update color LUT data after construction.
        /// </summary>
        /// <param name="initialColorLut">Color array to initialize the LUT from</param>
        /// <param name="channels">Color channels for one color LUT entry</param>
        public PassthroughColorLut(Color[] initialColorLut, PassthroughColorLutChannels channels)
            : this(GetArraySize(initialColorLut), channels)
        {
            Create(CreateLutDataFromArray(initialColorLut));
        }

        /// <summary>
        /// Set the color LUT data from an array of `Color`. The resolution is
        /// inferred from the array size, thus the size needs to be a result of
        /// `resolution = size ^ 3 * numColorChannels`, where `numColorChannels` depends
        /// on `channels`.
        /// Use `UpdateFrom()` to update color LUT data after construction.
        /// </summary>
        /// <param name="initialColorLut">Color32 array to initialize the LUT from</param>
        /// <param name="channels">Color channels for one color LUT entry</param>
        public PassthroughColorLut(Color32[] initialColorLut, PassthroughColorLutChannels channels)
            : this(GetArraySize(initialColorLut), channels)
        {
            Create(CreateLutDataFromArray(initialColorLut));
        }

        /// <summary>
        /// Set the color LUT data from an array of `Color`. The resolution is
        /// inferred from the array size, thus the size needs to be a result of
        /// `resolution = size ^ 3 * numColorChannels`, where `numColorChannels` depends
        /// on `channels`.
        /// Use `UpdateFrom()` to update color LUT data after construction.
        /// </summary>
        /// <param name="initialColorLut">Color byte array to initialize the LUT from</param>
        /// <param name="channels">Color channels for one color LUT entry</param>
        public PassthroughColorLut(byte[] initialColorLut, PassthroughColorLutChannels channels)
            : this(GetTextureSizeFromByteArray(initialColorLut, channels), channels)
        {
            Create(CreateLutDataFromArray(initialColorLut));
        }

        /// <summary>
        /// Updates color LUT data from an array of `Color`.
        /// The resolution (number of colors) must match the original specification at construction
        /// time. The alpha value is ignored if `ColorChannels` was set to `Rgb`.
        /// </summary>
        /// <param name="colors">Color array</param>
        public void UpdateFrom(Color[] colors)
        {
            if (IsValidLutUpdate(colors, m_ChannelCount))
            {
                WriteColorsAsBytes(colors, m_ColorBytes);
                YVRPlugin.Instance.UpdatePassthroughColorLut(m_ColorLutHandle, m_LutData);
            }
        }

        /// <summary>
        /// Updates color LUT data from an array of `Color32`.
        /// The resolution (number of colors) must match the original specification at construction
        /// time. The alpha value is ignored if `ColorChannels` was set to `Rgb`.
        /// </summary>
        /// <param name="colors">Color array</param>
        public void UpdateFrom(Color32[] colors)
        {
            if (IsValidLutUpdate(colors, m_ChannelCount))
            {
                WriteColorsAsBytes(colors, m_ColorBytes);
                YVRPlugin.Instance.UpdatePassthroughColorLut(m_ColorLutHandle, m_LutData);
            }
        }

        /// <summary>
        /// Updates color LUT data from an array of RGB(A) values.
        /// The resolution (number of colors) must match the original specification at construction
        /// time. The expected number of bytes per color is 3 if `ColorChannels` was set to `Rgb`, 4 if
        /// it was set to `Rgba`.
        /// </summary>
        /// <param name="colors">Array of consecutive RGB(A) color tuples</param>
        public void UpdateFrom(byte[] colors)
        {
            if (IsValidLutUpdate(colors, 1))
            {
                colors.CopyTo(m_ColorBytes, 0);
                YVRPlugin.Instance.UpdatePassthroughColorLut(m_ColorLutHandle, m_LutData);
            }
        }

        /// <summary>
        /// Update color LUT data from a texture.
        /// Color channels and resolution must match the original.
        /// </summary>
        /// <param name="lutTexture">Color LUT texture</param>
        /// <param name="flipY">Flag to inform whether the LUT texture should be flipped vertically. This is needed for LUT images which have color (0, 0, 0)
        /// in the top-left corner. Some color grading systems, e.g. Unity post-processing, have color (0, 0, 0) in the bottom-left corner,
        /// in which case flipping is not needed.</param>
        public void UpdateFrom(Texture2D lutTexture, bool flipY = true)
        {
            if (IsValidUpdateResolution(GetTextureSize(lutTexture), m_ChannelCount))
            {
                ColorLutTextureConverter.TextureToColorByteMap(lutTexture, m_ChannelCount, m_ColorBytes, flipY);
                YVRPlugin.Instance.UpdatePassthroughColorLut(m_ColorLutHandle, m_LutData);
            }
        }

        public void Dispose()
        {
            Destroy();
            FreeAllocHandle();
        }

        private void FreeAllocHandle()
        {
            if (m_AllocHandle != null && m_AllocHandle.IsAllocated)
            {
                m_AllocHandle.Free();
            }
        }

        /// <summary>
        /// Check if the given texture is formatted correctly for the use as color LUT.
        /// </summary>
        /// <param name="texture">Texture to check</param>
        /// <param name="errorMessage">Error message describing acceptance fail reason</param>
        /// <returns></returns>
        public static bool IsTextureSupported(Texture2D texture, out string errorMessage)
        {
            try
            {
                GetChannelsForTextureFormat(texture.format);
            }
            catch (System.ArgumentException e)
            {
                errorMessage = e.Message;
                return false;
            }

            if (!ColorLutTextureConverter.TryGetTextureLayout(texture.width, texture.height, out _, out _,
                    out var layoutMessage))
            {
                errorMessage = layoutMessage;
                return false;
            }

            var size = texture.width * texture.height;
            if (!IsResolutionAccepted(GetResolutionFromSize(size), size, out var resolutionMessage))
            {
                errorMessage = resolutionMessage;
                return false;
            }

            errorMessage = string.Empty;
            return true;
        }

        private PassthroughColorLut(int size, PassthroughColorLutChannels channels)
        {
            Channels = channels;
            Resolution = GetResolutionFromSize(size);
            m_ChannelCount = ChannelsToCount(channels);

            if (!IsResolutionAccepted(Resolution, size, out var message))
            {
                throw new System.ArgumentException(message);
            }
        }

        private bool IsValidUpdateResolution(int lutSize, int elementByteSize)
        {
            if (!IsValid)
            {
                Debug.LogError("Can not update an uninitialized lut object.");
                return false;
            }

            var resolution = GetResolutionFromSize(lutSize * elementByteSize / m_ChannelCount);

            if (resolution != Resolution)
            {
                Debug.LogError($"Can only update with the same resolution of {Resolution}.");
                return false;
            }

            return true;
        }

        private bool IsValidLutUpdate<T>(T[] colorArray, int elementByteSize)
        {
            var arraySize = GetArraySize(colorArray);

            if (!IsValidUpdateResolution(arraySize, elementByteSize))
            {
                return false;
            }

            if (arraySize * elementByteSize != m_ColorBytes.Length)
            {
                Debug.LogError("New color byte array doesn't match LUT size.");
                return false;
            }

            return true;
        }

        private static PassthroughColorLutChannels GetChannelsForTextureFormat(TextureFormat format)
        {
            switch (format)
            {
                case TextureFormat.RGB24:
                    return PassthroughColorLutChannels.Rgb;
                case TextureFormat.RGBA32:
                    return PassthroughColorLutChannels.Rgba;
                default:
                    throw new System.ArgumentException(
                        $"Texture format {format} not supported for Color LUTs. Supported formats are RGB24 and RGBA32.");
            }
        }

        private static int GetTextureSizeFromByteArray(byte[] initialColorLut, PassthroughColorLutChannels channels)
        {
            var arraySize = GetArraySize(initialColorLut);
            var channelCount = ChannelsToCount(channels);
            if (arraySize % channelCount != 0)
            {
                throw new System.ArgumentException(
                    $"Invalid byte array given, {channelCount} bytes required for each color for {channels} color channels.");
            }

            return initialColorLut.Length / channelCount;
        }

        private static int GetTextureSize(Texture2D texture)
        {
            if (texture == null)
            {
                throw new System.ArgumentNullException("Lut texture is undefined.");
            }

            return texture.width * texture.height;
        }

        private static int GetArraySize<T>(T[] array)
        {
            if (array == null)
            {
                throw new System.ArgumentNullException($"Lut {typeof(T).Name} array is undefined.");
            }

            return array.Length;
        }

        private static int ChannelsToCount(PassthroughColorLutChannels channels)
        {
            return channels == PassthroughColorLutChannels.Rgb ? 3 : 4;
        }

        private static bool IsResolutionAccepted(uint resolution, int size, out string errorMessage)
        {
            // if (!IsPowerOfTwo(resolution))
            // {
            //     errorMessage = "Color LUT texture resolution should be a power of 2.";
            //     return false;
            // }

            if (resolution * resolution * resolution != size)
            {
                errorMessage = "Unexpected LUT resolution, LUT size should be resolution in a power of 3.";
                return false;
            }

            errorMessage = string.Empty;
            return true;
        }

        private static bool IsPowerOfTwo(uint x)
        {
            return (x != 0) && ((x & (x - 1)) == 0);
        }

        private void Create(PassthroughColorLutData lutData)
        {
            m_LutData = lutData;
            InternalCreate();
        }

        private void RefreshIfInitialized(bool isInitialized)
        {
            if (isInitialized)
            {
                Recreate();
            }
        }

        private void Recreate()
        {
            Destroy();
            InternalCreate();
        }

        private void InternalCreate()
        {
            var result = YVRPlugin.Instance.CreatePassthroughColorLut(Channels,
                Resolution, m_LutData, out m_ColorLutHandle);
            m_CreateState = result ? CreateState.Created : CreateState.Invalid;
            if (!IsValid)
            {
                Debug.LogError("Failed to create Passthrough Color LUT.");
            }
        }

        private static uint GetResolutionFromSize(int size)
        {
            return (uint)Mathf.Round(Mathf.Pow(size, 1f / 3f));
        }

        private PassthroughColorLutData CreateLutData(out byte[] colorBytes)
        {
            PassthroughColorLutData lutData = default;
            lutData.BufferSize = (uint)(Resolution * Resolution * Resolution * m_ChannelCount);
            colorBytes = new byte[lutData.BufferSize];
            m_AllocHandle = GCHandle.Alloc(colorBytes, GCHandleType.Pinned);
            lutData.Buffer = m_AllocHandle.AddrOfPinnedObject();
            return lutData;
        }

        private PassthroughColorLutData CreateLutDataFromTexture(Texture2D lut, bool flipY)
        {
            var lutData = CreateLutData(out m_ColorBytes);
            ColorLutTextureConverter.TextureToColorByteMap(lut, m_ChannelCount, m_ColorBytes, flipY);
            return lutData;
        }

        private PassthroughColorLutData CreateLutDataFromArray(Color[] colors)
        {
            var lutData = CreateLutData(out m_ColorBytes);
            WriteColorsAsBytes(colors, m_ColorBytes);
            return lutData;
        }

        private PassthroughColorLutData CreateLutDataFromArray(Color32[] colors)
        {
            var lutData = CreateLutData(out m_ColorBytes);
            WriteColorsAsBytes(colors, m_ColorBytes);
            return lutData;
        }

        private PassthroughColorLutData CreateLutDataFromArray(byte[] colors)
        {
            var lutData = CreateLutData(out m_ColorBytes);
            colors.CopyTo(m_ColorBytes, 0);
            return lutData;
        }

        private void WriteColorsAsBytes(Color[] colors, byte[] target)
        {
            using var source = new NativeArray<Color>(colors, Allocator.TempJob);
            using var targetNative = new NativeArray<byte>(target, Allocator.TempJob);

            var job = new WriteColorsAsBytesJob
            {
                source = source,
                target = targetNative,
                channelCount = m_ChannelCount
            }.Schedule(source.Length, RecomendedBatchSize);

            job.Complete();
            targetNative.CopyTo(target);
        }

        private void WriteColorsAsBytes(Color32[] colors, byte[] target)
        {
            for (int i = 0; i < colors.Length; i++)
            {
                for (int c = 0; c < m_ChannelCount; c++)
                {
                    target[i * m_ChannelCount + c] = colors[i][c];
                }
            }
        }

        ~PassthroughColorLut()
        {
            Dispose();
        }

        private void Destroy()
        {
            if (m_CreateState == CreateState.Created)
            {
                lock (m_Locker)
                {
                    YVRPlugin.Instance.DestroyPassthroughColorLut(m_ColorLutHandle);
                }
            }

            m_CreateState = CreateState.Invalid;
        }

        private struct WriteColorsAsBytesJob : IJobParallelFor
        {
            [NativeDisableParallelForRestriction] [WriteOnly]
            public NativeArray<byte> target;

            [NativeDisableParallelForRestriction] [ReadOnly]
            public NativeArray<Color> source;

            public int channelCount;

            public void Execute(int index)
            {
                for (int c = 0; c < channelCount; c++)
                {
                    target[index * channelCount + c] = (byte)Mathf.Min(source[index][c] * 255.0f, 255.0f);
                }
            }
        }

        private static class ColorLutTextureConverter
        {
            /// <summary>
            /// Read colors from LUT texture and write them in the pre-allocated target byte array
            /// </summary>
            /// <param name="lut">Color LUT texture</param>
            /// <param name="channelCount">{RGB = 3, RGBA = 4}</param>
            /// <param name="target">Pre-allocated byte array that should fit all colors of the texture</param>
            /// <param name="flipY">Flag to inform whether the LUT texture should be flipped vertically</param>
            public static void TextureToColorByteMap(Texture2D lut, int channelCount, byte[] target, bool flipY)
            {
                MapColorValues(GetTextureSettings(lut, channelCount, flipY), lut.GetPixelData<byte>(0), target);
            }

            private static void MapColorValues(TextureSettings settings, NativeArray<byte> source, byte[] target)
            {
                using var targetNative = new NativeArray<byte>(target, Allocator.TempJob);
                var job = new MapColorValuesJob
                {
                    settings = settings,
                    source = source,
                    target = targetNative
                }.Schedule(settings.Resolution * settings.Resolution, settings.Resolution);

                job.Complete();
                targetNative.CopyTo(target);
            }

            private static TextureSettings GetTextureSettings(Texture2D lut, int channelCount, bool flipY)
            {
                int resolution, slicesPerRow;
                if (TryGetTextureLayout(lut.width, lut.height, out resolution, out slicesPerRow, out var message))
                {
                    return new TextureSettings(lut.width, lut.height, resolution, slicesPerRow, channelCount, flipY);
                }
                else
                {
                    throw new System.Exception(message);
                }
            }

            // Supports 2 formats:
            // - Square, where the z (blue) planes are arranged in a square (like this https://cdn.streamshark.io/obs-guide/img/neutral-lut.png)
            //   For that, assuming that x is the edge size of the LUT, width and height must be x * sqrt(x) (-> doesn't work for all edge sizes)
            // - Horizontal, where the z (blue) planes are arranged horizontally (like this http://www.thomashourdel.com/lutify/img/tut03.jpg)
            internal static bool TryGetTextureLayout(int width, int height, out int resolution, out int slicesPerRow,
                out string errorMessage)
            {
                resolution = -1;
                slicesPerRow = -1;

                if (width == height)
                {
                    float edgeLengthF = Mathf.Pow(width, 2.0f / 3.0f);
                    if (Mathf.Abs(edgeLengthF - Mathf.Round(edgeLengthF)) > 0.001)
                    {
                        errorMessage = "Texture layout is not compatible for color LUTs: " +
                                       "the dimensions don't result in a power-of-two resolution for the LUT. " +
                                       "Acceptable image sizes are e.g. 64 (for a LUT resolution of 16) or 512 (for a LUT resolution of 64).";
                        return false;
                    }

                    resolution = (int)Mathf.Round(edgeLengthF);
                    slicesPerRow = (int)Mathf.Sqrt(resolution);
                    Debug.Assert(width == resolution * slicesPerRow);
                }
                else
                {
                    if (width != height * height)
                    {
                        errorMessage = "Texture layout is not compatible for color LUTs: for horizontal layouts, " +
                                       "the Width is expected to be equal to Height * Height.";
                        return false;
                    }

                    resolution = height;
                    slicesPerRow = resolution;
                }

                errorMessage = string.Empty;
                return true;
            }

            private struct MapColorValuesJob : IJobParallelFor
            {
                public TextureSettings settings;

                [NativeDisableParallelForRestriction] [WriteOnly]
                public NativeArray<byte> target;

                [NativeDisableParallelForRestriction] [ReadOnly]
                public NativeArray<byte> source;

                public void Execute(int index)
                {
                    var bi = index / settings.Resolution;
                    var gi = index % settings.Resolution;
                    int bi_row = bi % settings.SlicesPerRow;
                    int bi_col = (int)Mathf.Floor(bi / settings.SlicesPerRow);
                    int sY = gi + bi_col * settings.Resolution;
                    int y = settings.FlipY ? settings.Height - sY - 1 : sY;
                    int sourceIndex = (bi_row * settings.Resolution + y * settings.Width) * settings.ChannelCount;
                    int targetIndex = (bi * settings.Resolution * settings.Resolution +
                                       gi * settings.Resolution) * settings.ChannelCount;

                    for (int i = 0; i < settings.Resolution * settings.ChannelCount; i++)
                    {
                        target[targetIndex + i] = source[sourceIndex + i];
                    }
                }
            }

            private struct TextureSettings
            {
                public int Width { get; }
                public int Height { get; }
                public int Resolution { get; }
                public int SlicesPerRow { get; }
                public int ChannelCount { get; }
                public bool FlipY { get; }

                public TextureSettings(int width, int height, int resolution, int slicesPerRow, int channelCount,
                    bool flipY)
                {
                    Width = width;
                    Height = height;
                    Resolution = resolution;
                    SlicesPerRow = slicesPerRow;
                    ChannelCount = channelCount;
                    FlipY = flipY;
                }
            }
        }

        private enum CreateState
        {
            Invalid,
            Pending,
            Created
        }
    }
}