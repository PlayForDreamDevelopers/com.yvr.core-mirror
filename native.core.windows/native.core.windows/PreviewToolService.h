#include "Headers/interface/IPreviewToolService.h"
#include "WriteLog.h"
#include "ServiceConnection.h"
#include "Headers/proxy/ProxyService.h"
#include <wrl/client.h>

class YVRUnityPlugin;

class PreviewToolService
{
public:
	PreviewToolService();
	ServiceConnection* serviceConnection;
	int trankingSpace;
	const char* GetPreviewToolService();
	void Present(uint64_t sharedTexture);
	void Present(uint64_t sharedTexture, uint64_t timestamp);
	bool GetTrackingData(void* outBuffer);
	UINT GetDirect3DDeviceID(ID3D11Device* device);
	void SetDeviceID();
	void GetDisplayResolution(int& width, int& height);
	void StopService();
	void SetTrankingSpace(int type);
private:
	IPreviewToolService* previceService;
	UINT deviceID;
};