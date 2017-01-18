#ifndef NCP_PARAM_TYPES_H
#define NCP_PARAM_TYPES_H

enum param_type
{
    PARAM_INT               =   0x00,
    PARAM_UNSIGNED_INT      =   0x01,
    PARAM_STRING            =   0x02,

    PARAM_DATA_RAW          =   0x80,
    PARAM_DATA_UNSIGNED_8   =   0x81,
    PARAM_DATA_UNSIGNED_16  =   0x82,
    PARAM_DATA_UNSIGNED_32  =   0x83,
    PARAM_DATA_SIGNED_8     =   0x84,
    PARAM_DATA_SIGNED_16    =   0x85,
    PARAM_DATA_SIGNED_32    =   0x86
};

#endif
