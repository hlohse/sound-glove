#include "BluetoothGuard.h"
#include "Platform.h"
#include "Formatter.h"
#include <stdexcept>
#ifdef  OS_WINDOWS
#include <WinSock2.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#endif
using namespace std;

BluetoothGuard::BluetoothGuard()
{
#ifdef OS_WINDOWS
    // Use winsock version 2.2
    const int version_major = 2;
    const int version_minor = 2;
    const WORD required_version = MAKEWORD(version_major, version_minor);
    WSADATA winsock_data;

    const int status = WSAStartup(required_version, &winsock_data);

    if (status != 0) {
        throw runtime_error(Formatter()
            << "WSAStartup failed with return code " << status);
    }

    const int actual_version_major = HIBYTE(winsock_data.wVersion);
    const int actual_version_minor = LOBYTE(winsock_data.wVersion);

    if (actual_version_major != version_major
    ||  actual_version_minor != version_minor)
    {
        throw runtime_error(Formatter()
            << "Used winsock version "
            << actual_version_major << "." << actual_version_minor
            << " is not desired " << version_major << "." << version_minor);
    }
#endif
}

BluetoothGuard::~BluetoothGuard()
{
#ifdef OS_WINDOWS
    const int status = WSACleanup();
    if (status != 0) {
        throw runtime_error(Formatter()
            << "WSACleanup failed with return code " << status);
    }
#endif
}

