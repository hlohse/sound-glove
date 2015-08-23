#pragma once

#include "Export.h"
#include "Platform.h"
#include "SerialPort.h"
#include "BluetoothGuard.h"
#include <string>

#ifdef OS_LINUX
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#elif  OS_WINDOWS
#include <WinSock2.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#endif

class BluetoothSPP : public SerialPort {
public:
#ifdef OS_LINUX
    using Socket        = int;
    using SocketAddress = struct sockaddr_rc;
#elif  OS_WINDOWS
    using Socket        = SOCKET;
    using SocketAddress = SOCKADDR_BTH;
#endif
	static const int read_buffer_size = 1024;

	SOUNDGLOVE_API static bool isValidName(const std::string& name);

	SOUNDGLOVE_API BluetoothSPP(const std::string& name, const int timeout_ms);
	SOUNDGLOVE_API ~BluetoothSPP();

	// SerialPort
	SOUNDGLOVE_API void waitUntilAvailable(const int bytes);

	// SerialPort : ISerialPort
	SOUNDGLOVE_API virtual bool isOpen();

	SOUNDGLOVE_API virtual uint8_t peek(const int index);

	SOUNDGLOVE_API virtual void read(uint8_t* destination, const int length);
	SOUNDGLOVE_API virtual void write(uint8_t* source, const int length);

	SOUNDGLOVE_API virtual void progress();

	SOUNDGLOVE_API virtual void clear();

	SOUNDGLOVE_API virtual int bufferedReadLength();
	SOUNDGLOVE_API virtual int bufferedWriteLength();

	SOUNDGLOVE_API virtual bool hasReadData();
	SOUNDGLOVE_API virtual bool hasWriteData();

	SOUNDGLOVE_API virtual bool fits(const int write_data_size);

private:
    SocketAddress socket_address_;
    Socket        socket_;
    std::string   buffer_;
	bool          is_open_;

    // SerialPort
    virtual void open();
    virtual void close();
    
    SocketAddress socketAddressFromName() const;

    void setSocketBlocking() const;
    void setSocketNonBlocking() const;
    int  getSocketError() const;

	void readSocket();
};

