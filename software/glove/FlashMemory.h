#ifndef FLASHMEMORY_H_
#define FLASHMEMORY_H_

#include "FlashMemoryLayout.h"
#include "Error.h"
#include "flashctl.h"
#include <stdint.h>
#include <string.h>

// Use defines for size to avoid VLA memory pool allocations
class FlashMemory {
public:
    const int address;

	static FlashMemory& infoA();
	static FlashMemory& infoB();
	static FlashMemory& infoC();
	static FlashMemory& infoD();

template <typename T>
T read(const int offset) const
{
	if (offset >= FLASH_MEMORY_INFO_SIZE) {
		Error::instance().set(Error::TYPE_FLASH_READ_EXCEED, offset - FLASH_MEMORY_INFO_SIZE);
		return 0;
	}
    return *((T*) ((uint8_t*) address + offset));
}

template <typename T>
void read(const int offset, T* buffer, const int length) const
{
	const int end = offset + length;
	if (end > FLASH_MEMORY_INFO_SIZE) {
		Error::instance().set(Error::TYPE_FLASH_READ_EXCEED, end - FLASH_MEMORY_INFO_SIZE);
	    memset(buffer, 0, length * sizeof(T));
	}
	else {
		memcpy(buffer, (uint8_t*) address + offset, length * sizeof(T));
	}
}

template <typename T>
void write(const int offset, const T value)
{
	if (offset >= FLASH_MEMORY_INFO_SIZE) {
		Error::instance().set(Error::TYPE_FLASH_WRITE_EXCEED, offset - FLASH_MEMORY_INFO_SIZE);
	}
	else {
		uint8_t segment[FLASH_MEMORY_INFO_SIZE];
		read<uint8_t>(0, segment, FLASH_MEMORY_INFO_SIZE);
		*((T*) (segment + offset)) = value;
		setSegment(segment);
	}
}

template <typename T>
void write(const int offset, const T* data, const int length)
{
	const int end = offset + length;
	if (end > FLASH_MEMORY_INFO_SIZE) {
		Error::instance().set(Error::TYPE_FLASH_WRITE_EXCEED, end - FLASH_MEMORY_INFO_SIZE);
	}
	else {
		uint8_t segment[FLASH_MEMORY_INFO_SIZE];
		read<uint8_t>(0, segment, FLASH_MEMORY_INFO_SIZE);
		memcpy(segment + offset, data, length * sizeof(T));
		setSegment(segment);
	}
}


private:

FlashMemory(const int address)
:	address(address)
{
}

void setSegment(uint8_t* data)
{
	/* According to the MSP430xxxDatasheet, information memory segment A has
	 * to be unlocked prior to an erase operation. Other segments seem to work
	 * without unlocking. (Tested with information memory segment D.)
	 *
	 */
    FlashCtl_unlockInfoA();
    do {
        FlashCtl_segmentErase((uint8_t*) address);
    } while (FlashCtl_eraseCheck((uint8_t*) address, FLASH_MEMORY_INFO_SIZE) != STATUS_SUCCESS);
    FlashCtl_write32((uint32_t*) data, (uint32_t*) address, FLASH_MEMORY_INFO_SIZE / sizeof(uint32_t));
    FlashCtl_lockInfoA();
}

// Disallow copies; (do not implement!)
FlashMemory(const FlashMemory&);
void operator=(const FlashMemory&);

};

#endif /* FLASHMEMORY_H_ */
