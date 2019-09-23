#ifndef SERIALPORT_PARAM_DEFS_H
#define SERIALPORT_PARAM_DEFS_H

#define SP_DATA_BITS_5 5
#define SP_DATA_BITS_6 6
#define SP_DATA_BITS_7 7
#define SP_DATA_BITS_8 8


#define SP_STOP_BITS_1   0
#define SP_STOP_BITS_1P5 1
#define SP_STOP_BITS_2   2


#define SP_PARITY_NONE  0
#define SP_PARITY_ODD   1
#define SP_PARITY_EVEN  2
#define SP_PARITY_MARK  3
#define SP_PARITY_SPACE 4


#define SP_FLOWCTRL_NONE 0
#define SP_FLOWCTRL_SW   1
#define SP_FLOWCTRL_HW   2

typedef struct SerialportParam
{
    int baudrate;
    int databits;
    int stopbits;
    int   parity;
    int flowctrl;
} sSerialportParam;

#endif /* SERIALPORT_PARAM_DEFS_H */

// EOF
