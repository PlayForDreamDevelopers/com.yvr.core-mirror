# 1. Find the openxr_loader_xxxxxx.aar file in the same directory
$aarFile = Get-ChildItem -Path . -Filter "openxr_loader_*.aar" | Select-Object -First 1

if ($null -eq $aarFile) {
    Write-Host "No openxr_loader_xxxxxx.aar file found."
    exit 1
}

# 2. Use 7z tool to extract the aar file
& "C:\Program Files\7-Zip\7z.exe" x $aarFile.FullName -o"$($aarFile.DirectoryName)\extracted" -y

# 4. Copy .h files to the target directory
$headerFiles = Get-ChildItem -Path "$($aarFile.DirectoryName)\extracted\prefab\modules\headers\include\openxr" -Filter "*.h"
$targetHeaderDir = "$($aarFile.DirectoryName)\..\OpenXRLoader\openxr_loader\Include\openxr"

foreach ($file in $headerFiles) {
    Copy-Item -Path $file.FullName -Destination $targetHeaderDir
}

# 5. Copy libopenxr_loader.so for android.arm64-v8a
$libArm64Files = Get-ChildItem -Path "$($aarFile.DirectoryName)\extracted\prefab\modules\openxr_loader\libs\android.arm64-v8a" -Filter "libopenxr_loader.so"
$targetLibArm64DebugDir = "$($aarFile.DirectoryName)\..\OpenXRLoader\openxr_loader\Libs\Android\arm64-v8a\Debug"
$targetLibArm64ReleaseDir = "$($aarFile.DirectoryName)\..\OpenXRLoader\openxr_loader\Libs\Android\arm64-v8a\Release"

foreach ($file in $libArm64Files) {
    Copy-Item -Path $file.FullName -Destination $targetLibArm64DebugDir
    Copy-Item -Path $file.FullName -Destination $targetLibArm64ReleaseDir
}

# 6. Copy libopenxr_loader.so for android.armeabi-v7a
$libArmeabiFiles = Get-ChildItem -Path "$($aarFile.DirectoryName)\extracted\prefab\modules\openxr_loader\libs\android.armeabi-v7a" -Filter "libopenxr_loader.so"
$targetLibArmeabiDebugDir = "$($aarFile.DirectoryName)\..\OpenXRLoader\openxr_loader\Libs\Android\armeabi-v7a\Debug"
$targetLibArmeabiReleaseDir = "$($aarFile.DirectoryName)\..\OpenXRLoader\openxr_loader\Libs\Android\armeabi-v7a\Release"

foreach ($file in $libArmeabiFiles) {
    Copy-Item -Path $file.FullName -Destination $targetLibArmeabiDebugDir
    Copy-Item -Path $file.FullName -Destination $targetLibArmeabiReleaseDir
}