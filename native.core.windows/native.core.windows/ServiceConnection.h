#pragma once
#include "Headers/binder/IServiceConnection.h"

class ServiceConnection : public IServiceConnection
{
public :
	bool isConnected;
	void onServiceConnected(IBinder* service);
	void onServiceDisconnected();
};
