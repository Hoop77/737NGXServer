#pragma once


#include "stdint.h"


union value32_t
{
    float f;
    bool b;

    uint8_t u8[ 4 ];
    uint16_t u16[ 2 ];
    uint32_t u32;

    int8_t i8[ 4 ];
    int16_t i16[ 2 ];
    int32_t i32;
};


typedef uint8_t entityId_t;
typedef uint16_t valueId_t;