#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <thread>
#include "wincodec.h"
#include <ScreenGrab.h>

using namespace std;

int Write(const char* pchLogMessage);

void SaveTexture(ID3D11DeviceContext* m_pD3D11Context, ID3D11Resource* pResource, string str);

LPCWSTR stringToLPCWSTR(std::string orig);

extern bool EnableLog;