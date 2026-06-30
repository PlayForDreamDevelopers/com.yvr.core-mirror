#include "PreviewToolService.h"

PreviewToolService::PreviewToolService()
{
	trankingSpace = 2;
	ProxyService::instance().start();
	previceService = dynamic_cast<IPreviewToolService*>(ProxyService::instance().getService(SERVICE_PREVIEW_TOOL));
	serviceConnection = new ServiceConnection();
	ProxyService::instance().bindService(SERVICE_PREVIEW_TOOL, serviceConnection);
}

const char* PreviewToolService::GetPreviewToolService()
{
	if (serviceConnection->isConnected)
	{
		std::string v = previceService->getVersionName();
		std::string strVersion = "GetPreviewToolService version:" + v;
		Write(strVersion.c_str());
		return v.c_str();
	}
}

void PreviewToolService::Present(uint64_t sharedTexture)
{
	if (serviceConnection->isConnected)
	{
		previceService->present(sharedTexture);
	}
}

void PreviewToolService::Present(uint64_t sharedTexture, uint64_t timestamp)
{
	if (serviceConnection->isConnected)
	{
		previceService->present(sharedTexture, timestamp);
	}
}

bool PreviewToolService::GetTrackingData(void* out_buffer)
{
	if (serviceConnection->isConnected)
	{
		return previceService->getTrackingData(out_buffer);
	}

	return false;
}

void PreviewToolService::SetDeviceID()
{
	previceService->setGraphicsDevice(deviceID);
}

void PreviewToolService::GetDisplayResolution(int& width, int& height)
{
	previceService->getDisplayResolution(width, height);
}

void PreviewToolService::StopService()
{
	ProxyService::instance().stop();
}

void PreviewToolService::SetTrankingSpace(int type) {
	if (serviceConnection->isConnected)
	{
		std::string v = std::to_string(type);
		std::string logMsg = "SetTrankingSpace:" + v;
		Write(logMsg.c_str());
		previceService->setXrReferenceSpaceType(type);
	}
}

UINT PreviewToolService::GetDirect3DDeviceID(ID3D11Device* device)
{
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dsgIAdapter = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	d3dDevice = device;
	d3dDevice.As(&dxgiDevice);
	dxgiDevice->GetAdapter(dsgIAdapter.GetAddressOf());
	dsgIAdapter->GetDesc(&adapterDesc);
	deviceID = adapterDesc.DeviceId;
	return adapterDesc.DeviceId;
}

