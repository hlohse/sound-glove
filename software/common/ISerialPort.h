#ifndef ISERIALPORT_H_
#define ISERIALPORT_H_

#include <stdint.h>

class ISerialPort {
public:
	virtual bool isOpen() = 0;

	virtual uint8_t peek(const int index) = 0;

    virtual void read(uint8_t* destination, const int length) = 0;
    virtual void write(uint8_t* source, const int length) = 0;

	virtual void progress() = 0;

    virtual void clear() = 0;

    virtual int bufferedReadLength() = 0;
    virtual int bufferedWriteLength() = 0;

    virtual bool hasReadData() = 0;
    virtual bool hasWriteData() = 0;

    virtual bool fits(const int write_data_size) = 0;
};

#endif /* ISERIALPORT_H_ */
