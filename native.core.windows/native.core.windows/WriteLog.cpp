#include "WriteLog.h"
#include <d3d11.h>
#include "timer.h"

Timer timer{};
bool EnableLog = false;
int Write(const char* pchLogMessage)
{
	if (!EnableLog)
	{
		return 0;
	}

	ofstream location_out;
	location_out.open("location_out_unity.txt", std::ios::out | std::ios::app);
	if (!location_out.is_open())
	{
		return 0;
	}
	time_t timep;
	time(&timep);
	char tmp[256];
	uint64_t timeSpan = timer.elapsed();
	strftime(tmp, sizeof(tmp), "%F %T", localtime(&timep));
	thread::id this_thread_id = this_thread::get_id(); // 获取本线程的id
	location_out << timeSpan << ": " << this_thread_id << ": " << pchLogMessage << "\n";
	location_out.close();
	return 1;
}

void SaveTexture(ID3D11DeviceContext* m_pD3D11Context, ID3D11Resource* pResource, string str)
{
	string name = str + ".png";
	LPCWSTR  filename = stringToLPCWSTR(name);
	HRESULT hr = DirectX::SaveWICTextureToFile(m_pD3D11Context, pResource,GUID_ContainerFormatPng, filename);
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
	wchar_t* wcstring = 0;
	try
	{
		size_t origsize = orig.length() + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		if (orig == "")
		{
			wcstring = (wchar_t*)malloc(0);
			mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
		}
		else
		{
			wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
			mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
		}
	}
	catch (std::exception e)
	{
	}
	return wcstring;
}