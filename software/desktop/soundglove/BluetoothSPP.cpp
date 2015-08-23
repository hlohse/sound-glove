#include "BluetoothSPP.h"
#include "Formatter.h"
#include <stdexcept>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

#ifdef OS_LINUX
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif

bool BluetoothSPP::isValidName(const string& name)
{
    int buffer[6];
#ifdef OS_LINUX
    return sscanf(name.c_str(),
#elif  OS_WINDOWS
    return sscanf_s(name.c_str(),
#endif
        "%02x:%02x:%02x:%02x:%02x:%02x",
        &buffer[0],
        &buffer[1],
        &buffer[2],
        &buffer[3],
        &buffer[4],
        &buffer[5]) == 6;
}

BluetoothSPP::BluetoothSPP(const std::string& name, const int timeout_ms)
	: SerialPort(name, BaudRate::BPS_NONE, timeout_ms),
    socket_address_(socketAddressFromName()),
#ifdef OS_LINUX
    socket_(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)),
#elif  OS_WINDOWS
    socket_(socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)),
#endif
	is_open_(false)
{
    try {
        open();
    } catch (...) { throw; }
}

BluetoothSPP::~BluetoothSPP()
{
	if (isOpen()) {
        close();
    }
}

bool BluetoothSPP::isOpen()
{
	return is_open_;
}

void BluetoothSPP::read(uint8_t* destination, const int length)
{
	waitUntilAvailable(length);
	memcpy(destination, buffer_.c_str(), length);
	buffer_.erase(0, length);
}

uint8_t BluetoothSPP::peek(const int index)
{
	waitUntilAvailable(index + 1);
	return buffer_[index];
}

void BluetoothSPP::write(uint8_t* source, const int length)
{
	int bytes_written = 0;
	while (bytes_written < length) {
#ifdef OS_LINUX
		const int written = write(socket_,
			(const char*) source + bytes_written,
			length);
#elif  OS_WINDOWS
		const int written = send(socket_,
			(const char*) source + bytes_written,
			length,
			0);
#endif
		if (written < 0) {
			throw runtime_error(Formatter()
				<< "Error " << errno << " while writing socket data");
		}
		bytes_written += written;
	}
}

void BluetoothSPP::progress()
{
	waitUntilAvailable(bufferedReadLength() + 1);
}

void BluetoothSPP::clear()
{
	buffer_.clear();
}

int BluetoothSPP::bufferedReadLength()
{
	return (int) buffer_.length();
}

int BluetoothSPP::bufferedWriteLength()
{
	return 0;
}

bool BluetoothSPP::hasReadData()
{
	return bufferedReadLength() > 0;
}

bool BluetoothSPP::hasWriteData()
{
	return false;
}

bool BluetoothSPP::fits(const int write_data_size)
{
	// Trivially, since we are blocking on write until socket got everything
	return true;
}
void BluetoothSPP::close()
{
#ifdef OS_LINUX
    shutdown(socket_, SHUT_RDWR);
#elif  OS_WINDOWS
    shutdown(socket_, SD_BOTH);
#endif
	is_open_ = false;
}

void BluetoothSPP::open()
{
    struct timeval timeout;
    fd_set sockets;

    timeout.tv_sec  = (long) (timeoutMs() / 1e3);
    timeout.tv_usec = (long) (timeoutMs() * 1e3 - timeout.tv_sec * 1e6);
    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);

    setSocketNonBlocking();
    connect(socket_, (sockaddr*) &socket_address_, sizeof(socket_address_));
    
    switch (select((int) socket_ + 1, NULL, &sockets, NULL, &timeout)) {
    case 1: {
        int error = getSocketError();
        if (error != 0) {
            throw runtime_error(Formatter()
                << "Error " << error << " on socket");
        }
        break;
    }
    case 0:
        throw runtime_error(Formatter()
            << "Timeout while connecting socket");
        break;
    default:
        throw runtime_error(Formatter()
#ifdef OS_LINUX
            << "Error " << errno << " while connecting socket");
#elif  OS_WINDOWS
            << "Error " << WSAGetLastError() << " while connecting socket");
#endif
        break;
    }

    setSocketBlocking();
	is_open_ = true;
}

void BluetoothSPP::setSocketBlocking() const
{
#ifdef OS_LINUX
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags ^= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
#elif OS_WINDOWS
    ULONG non_blocking = 0;
    ioctlsocket(socket_, FIONBIO, &non_blocking);
#endif
}

void BluetoothSPP::setSocketNonBlocking() const
{
#ifdef OS_LINUX
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags |= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
#elif OS_WINDOWS
    ULONG non_blocking = 1;
    ioctlsocket(socket_, FIONBIO, &non_blocking);
#endif
}

int BluetoothSPP::getSocketError() const
{
    int error;
#ifdef __linux__
    socklen_t length = sizeof(error);
#elif _WIN32
    int length = sizeof(error);
#endif
    getsockopt(socket_, SOL_SOCKET, SO_ERROR, (char*) &error, &length);
    return error;
}

void BluetoothSPP::readSocket()
{
    char buffer[BluetoothSPP::read_buffer_size];
    int bytes_read;
    
#ifdef OS_LINUX
    bytes_read = read(socket_, buffer, BluetoothSPP::read_buffer_size);
#elif OS_WINDOWS
    bytes_read = recv(socket_, buffer, BluetoothSPP::read_buffer_size, 0);
#endif
    
    if (bytes_read < 0) {
        throw runtime_error(Formatter()
            << "Error " << errno << " while reading socket data");
    }

    buffer_ += string(buffer, bytes_read);
}

BluetoothSPP::SocketAddress BluetoothSPP::socketAddressFromName() const
{
    SocketAddress socket_address;
#ifdef OS_LINUX
    if (str2ba(Name().c_str(), &socket_address.rc_bdaddr) != 0) {
        throw runtime_error(Formatter()
            << "Invalid Bluetooth address \"" << Name() << "\"");
    }
    socket_address.rc_family  = AF_BLUETOOTH;
    socket_address.rc_channel = 1;
#elif  OS_WINDOWS
    ULONG addressdata[6];

    if (sscanf_s(name().c_str(),
        "%02x:%02x:%02x:%02x:%02x:%02x",
        &addressdata[0],
        &addressdata[1],
        &addressdata[2],
        &addressdata[3],
        &addressdata[4],
        &addressdata[5]) != 6)
    {
        throw runtime_error(Formatter()
            << "Invalid Bluetooth address \"" << name() << "\"");
    }

    socket_address.btAddr = 0;
    for (int i = 0; i < 6; ++i) {
        socket_address.btAddr = (socket_address.btAddr << 8)
			+ ((BTH_ADDR)(addressdata[i] & 0xFF));
    }

    socket_address.addressFamily  = AF_BTH;
    socket_address.serviceClassId = GUID_NULL;
	socket_address.port = 1;
#endif
    return socket_address;
}

void BluetoothSPP::waitUntilAvailable(const int bytes)
{
	fd_set sockets;
	FD_ZERO(&sockets);
	FD_SET(socket_, &sockets);

	while ((int)buffer_.length() < bytes) {
		if (select((int) socket_ + 1, NULL, &sockets, NULL, NULL) == 1) {
			int error = getSocketError();
			if (error != 0) {
				throw runtime_error(Formatter()
					<< "Error " << error << " on socket while waiting for data");
			}
			try {
				readSocket();
			}
			catch (...) { throw; }
		}
		else {
			throw runtime_error(Formatter() <<
				"Error while waiting for socket data");
		}
	}
}

