#include "SerialPort.h"
#include "Formatter.h"
#include <stdexcept>
using namespace std;

SerialPort::SerialPort(const std::string& name,
                       const enum BaudRate baud_rate,
                       const int timeout_ms)
:   name_(name),
    baud_rate_(baud_rate),
    timeout_ms_(timeout_ms)
{
}

SerialPort::~SerialPort()
{
}

string SerialPort::read(const int length)
{
	char* buffer = new char[length];
	read((uint8_t*) buffer, length);
	const string read_values(buffer, length);
	delete buffer;
	return read_values;
}

std::string SerialPort::name() const
{
    return name_;
}

enum BaudRate SerialPort::baudRate() const
{
    return baud_rate_;
}

int SerialPort::timeoutMs() const
{
    return timeout_ms_;
}

int SerialPort::baudRateNumber(const enum BaudRate baud_rate_enum)
{
    switch (baud_rate_enum) {
	case BaudRate::BPS_9600:   return 9600;
	case BaudRate::BPS_19200:  return 19200;
	case BaudRate::BPS_38400:  return 38400;
	case BaudRate::BPS_57600:  return 57600;
	case BaudRate::BPS_115200: return 115200;
	case BaudRate::BPS_230400: return 230400;
	case BaudRate::BPS_460800: return 460800;
	case BaudRate::BPS_921600: return 921600;
    default: throw std::runtime_error(Formatter()
        << "No baud rate specified");
    }
}

enum BaudRate SerialPort::baudRateEnum(const int baud_rate_number)
{
    switch (baud_rate_number) {
	case 9600:   return BaudRate::BPS_9600;
	case 19200:  return BaudRate::BPS_19200;
	case 38400:  return BaudRate::BPS_38400;
	case 57600:  return BaudRate::BPS_57600;
	case 115200: return BaudRate::BPS_115200;
	case 230400: return BaudRate::BPS_230400;
	case 460800: return BaudRate::BPS_460800;
	case 921600: return BaudRate::BPS_921600;
    default:     throw std::runtime_error(Formatter()
        << "Invalid baud rate " << baud_rate_number);
    }
}

