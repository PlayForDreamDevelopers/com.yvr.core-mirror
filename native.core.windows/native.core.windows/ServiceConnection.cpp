#include  "ServiceConnection.h"
#include "YVRUnityPlugin.h"

void ServiceConnection::onServiceConnected(IBinder* service)
{
	plugin.previewToolService->SetDeviceID();
	int width, height;
	plugin.previewToolService->GetDisplayResolution(width, height);
	plugin.renderConfigsMgr->setResolution(width, height);
	isConnected = true;
	Write("onServiceConnected true");
	plugin.previewToolService->SetTrankingSpace(plugin.previewToolService->trankingSpace);
}

void ServiceConnection::onServiceDisconnected()
{
	isConnected = false;
}