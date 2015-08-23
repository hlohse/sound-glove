#pragma once

#include "ISerialPort.h"
#include <string>

enum BaudRate {
	BPS_NONE,
	BPS_9600,
	BPS_19200,
	BPS_38400,
	BPS_57600,
	BPS_115200,
	BPS_230400,
	BPS_460800,
	BPS_921600
};

class SerialPort : public ISerialPort {
public:
    static const int max_line_bytes = 1024;

    SerialPort(const std::string& name,
               const enum BaudRate baud_rate,
               const int timeout_ms);
    virtual ~SerialPort();

    std::string name() const;
    enum BaudRate baudRate() const;
    int timeoutMs() const;
    
	static int baudRateNumber(const enum BaudRate baud_rate_enum);
	static enum BaudRate baudRateEnum(const int baud_rate_number);

	std::string read(const int length);
	std::string readLine();

	virtual void waitUntilAvailable(const int bytes) = 0;

	// ISerialPort
	virtual bool isOpen() = 0;

	virtual void read(uint8_t* destination, const int length) = 0;
	virtual void write(uint8_t* source, const int length) = 0;

	virtual int bufferedReadLength() = 0;
	virtual int bufferedWriteLength() = 0;

	virtual bool hasReadData() = 0;
	virtual bool hasWriteData() = 0;

protected:
	virtual void open() = 0;
    virtual void close() = 0;

private:
    std::string   name_;
    enum BaudRate baud_rate_;
    int           timeout_ms_;
};

