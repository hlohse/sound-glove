#include "FlashMemory.h"

FlashMemory& FlashMemory::infoA()
{
    static FlashMemory instance(FLASH_MEMORY_INFO_A);
    return instance;
}

FlashMemory& FlashMemory::infoB()
{
    static FlashMemory instance(FLASH_MEMORY_INFO_B);
    return instance;
}

FlashMemory& FlashMemory::infoC()
{
    static FlashMemory instance(FLASH_MEMORY_INFO_C);
    return instance;
}

FlashMemory& FlashMemory::infoD()
{
    static FlashMemory instance(FLASH_MEMORY_INFO_D);
    return instance;
}
