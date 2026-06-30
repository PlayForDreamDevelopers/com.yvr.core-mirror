using System;
using System.Collections.Generic;
using System.Globalization;
using UnityEngine;

public class CubeLUTParser
{
    public static Color32[] ParseCubeLines(string[] lines)
    {
        List<Color32> colors = new List<Color32>();
        int lutSize = 0;

        foreach (var line in lines)
        {
            var trimmedLine = line.Trim();
            if (string.IsNullOrEmpty(trimmedLine) || trimmedLine.StartsWith("#"))
                continue;

            if (trimmedLine.StartsWith("LUT_3D_SIZE"))
            {
                var parts = trimmedLine.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                lutSize = int.Parse(parts[1]);
                continue;
            }

            if (trimmedLine.StartsWith("DOMAIN_MIN") || trimmedLine.StartsWith("DOMAIN_MAX"))
                continue;

            var rgbParts = trimmedLine.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if (rgbParts.Length == 3)
            {
                float r = float.Parse(rgbParts[0], CultureInfo.InvariantCulture);
                float g = float.Parse(rgbParts[1], CultureInfo.InvariantCulture);
                float b = float.Parse(rgbParts[2], CultureInfo.InvariantCulture);

                colors.Add(new Color32(
                    (byte)(Mathf.Clamp01(r) * 255),
                    (byte)(Mathf.Clamp01(g) * 255),
                    (byte)(Mathf.Clamp01(b) * 255),
                    255
                ));
            }
        }

        if (lutSize == 0 || colors.Count != lutSize * lutSize * lutSize)
        {
            throw new Exception($"Invalid LUT data. LUT size: {lutSize}, entries: {colors.Count}");
        }

        return colors.ToArray();
    }
}
