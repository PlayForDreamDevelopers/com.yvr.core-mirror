#include "./Common.h"

std::string GetXrVersionString(XrVersion ver)
{
    return StrFormat("%d.%d.%d", XR_VERSION_MAJOR(ver), XR_VERSION_MINOR(ver), XR_VERSION_PATCH(ver));
}

std::string Uint8ToHexString(const uint8_t* data, int size)
{
    std::string res;
    res.reserve(size * 2);
    const char hex[] = "0123456789ABCDEF";
    for (uint32_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        res += hex[c >> 4];
        res += hex[c & 0xf];
    }
    return res;
}

uint8_t* HexStringToUint8(const std::string& hex_str)
{
    std::vector<uint8_t> data;
    if (hex_str.length() != XR_UUID_SIZE_EXT * 2) {
        YError("Invalid hex string length: %d", hex_str.length());
        return data.data();
    }
    for (size_t i = 0; i < hex_str.size(); i += 2) {
        uint8_t byte = static_cast<uint8_t>(std::stoi(hex_str.substr(i, 2), nullptr, 16));
        data.push_back(byte);
    }
    return data.data();
}