#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DATA_BITS_5 = 5,
    DATA_BITS_6 = 6,
    DATA_BITS_7 = 7,
    DATA_BITS_8 = 8
} eDataBits;

typedef enum 
{
    STOP_BITS_1   = 1,
    STOP_BITS_2   = 2,
    STOP_BITS_1P5 = 3
} eStopBits;

typedef enum 
{
    PARITY_NONE = 0,
    PARITY_ODD  = 1,
    PARITY_EVEN = 2,
    PARITY_MARK = 3,
    PARITY_SPACE = 4
} eParity;

typedef enum 
{
    FLOWCTRL_NONE = 0,
    FLOWCTRL_SW  = 1,
    FLOWCTRL_HW = 2
} eFlowCtrl;

typedef struct SerialportParam
{
    int baudrate;
    eDataBits databits;
    eStopBits stopbits;
    eParity   parity;
    eFlowCtrl flowctrl;
} sSerialportParam;

typedef void (*fnDataArrived)(const char *data, int length);

struct SerialportHandle;
typedef struct SerialportHandle sSerialportHandle;

sSerialportHandle * serialport_open(const char *port, const sSerialportParam *param);
int serialport_read(sSerialportHandle *handle, char *data, int max_len);
int serialport_write(sSerialportHandle *handle, const char *data, int len);
void serialport_flush(sSerialportHandle *handle);
void serialport_close(sSerialportHandle *handle);


#ifdef __cplusplus
}
#endif

#endif /* SERIALPORT_H */
