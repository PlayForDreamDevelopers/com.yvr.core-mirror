#pragma once

typedef enum PassthroughMode_
{
    None = 0,
    MonoToRgba = 1,
    MonoToMono = 2,
    BrightnessContrastSaturation = 3,
    ColorLut = 4,
    InterpolatedColorLut = 5

} PassthroughMode;
