#include "Configuration.h"
#include "FlashMemoryLayout.h"
#include "Bluetooth/Bond.h"
#include <string.h>

Configuration& Configuration::instance()
{
	static Configuration instance;
	return instance;
}

const char* Configuration::defaultName()
{
	static const char default_name[] = "Sound Glove";
	return default_name;
}

Configuration::Configuration()
{
	if (!isInitialized()) {
		Calibration null_calibration;
		memset(&null_calibration, 0, sizeof(null_calibration));
		setName(defaultName(), strlen(defaultName()));
		setRemoteAddress(Bluetooth::Bond::invalid_remote_address);
		setLinkKey(Bluetooth::Bond::invalid_link_key);
		setImuCalibration(MARG::Hand, null_calibration);
		setImuCalibration(MARG::Forearm, null_calibration);
		setImuCalibration(MARG::UpperArm, null_calibration);
		setInitialized();
	}
}

bool Configuration::isInitialized() const
{
	const char* expected = FLASH_MEMORY_INFO_A_MAGIC;
	const char* actual = (char*) FLASH_MEMORY_INFO_A;
	return memcmp(actual, expected, FLASH_MEMORY_INFO_A_SIZE_MAGIC) == 0;
}

void Configuration::setInitialized() const
{
	char* magic = FLASH_MEMORY_INFO_A_MAGIC;
	FlashMemory::infoA().write<char>(0, magic, FLASH_MEMORY_INFO_A_SIZE_MAGIC);
}

char* Configuration::name() const
{
	return (char*) (FLASH_MEMORY_INFO_A + FLASH_MEMORY_INFO_A_OFFSET_NAME);
}

void Configuration::setName(const char* name, const int length)
{
	char name_buffer[FLASH_MEMORY_INFO_A_SIZE_NAME + 1] = {0};
	const uint8_t capped_length = length > FLASH_MEMORY_INFO_A_SIZE_NAME ?
		FLASH_MEMORY_INFO_A_SIZE_NAME :
		length;
	memcpy(name_buffer, name, capped_length);
	FlashMemory::infoA().write<char>(FLASH_MEMORY_INFO_A_OFFSET_NAME, name_buffer, FLASH_MEMORY_INFO_A_SIZE_NAME + 1);
	FlashMemory::infoA().write<uint8_t>(FLASH_MEMORY_INFO_A_OFFSET_NAME_LENGTH, capped_length);
}

int Configuration::nameLength() const
{
	return (int) *((uint8_t*) (FLASH_MEMORY_INFO_A + FLASH_MEMORY_INFO_A_OFFSET_NAME_LENGTH));
}

BD_ADDR_t& Configuration::remoteAddress() const
{
	return *((BD_ADDR_t*) (FLASH_MEMORY_INFO_A + FLASH_MEMORY_INFO_A_OFFSET_REMOTE_ADDR));
}

void Configuration::setRemoteAddress(const BD_ADDR_t& remote_address)
{
	FlashMemory::infoA().write<uint8_t>(FLASH_MEMORY_INFO_A_OFFSET_REMOTE_ADDR,
										(uint8_t*) &remote_address,
										FLASH_MEMORY_INFO_A_SIZE_REMOTE_ADDR);
}

Link_Key_t& Configuration::linkKey() const
{
	return *((Link_Key_t*) (FLASH_MEMORY_INFO_A + FLASH_MEMORY_INFO_A_OFFSET_LINK_KEY));
}

void Configuration::setLinkKey(const Link_Key_t& link_key)
{
	FlashMemory::infoA().write<uint8_t>(FLASH_MEMORY_INFO_A_OFFSET_LINK_KEY,
										(uint8_t*) &link_key,
										FLASH_MEMORY_INFO_A_SIZE_LINK_KEY);
}

Calibration& Configuration::margCalibration(const MARG::Placement marg) const
{
	FlashMemory& flash_memory = margCalibrationSource(marg);
	return *((Calibration*) flash_memory.address);
}

void Configuration::setImuCalibration(const MARG::Placement marg,
					           const Calibration& calibration)
{
	FlashMemory& flash_memory = margCalibrationSource(marg);
	flash_memory.write<uint8_t>(0, (uint8_t*)&calibration, Calibration::serialized_size);
}

FlashMemory& Configuration::margCalibrationSource(const MARG::Placement marg) const
{
	if (marg == MARG::Hand) {
		return FlashMemory::infoB();
	}
	else if (marg == MARG::Forearm) {
		return FlashMemory::infoC();
	}
	else {
		return FlashMemory::infoD();
	}
}
