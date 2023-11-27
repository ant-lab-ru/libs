#pragma once
#include <stdint.h>

class ISpi
{
    public:
        virtual int read(uint8_t* buffer, uint32_t lenght) = 0;
        virtual int write(const uint8_t* data, uint32_t size) = 0;
        virtual int select() = 0;
        virtual int deselect() = 0;
};