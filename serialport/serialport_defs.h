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



typedef void (*fnDataArrived)(const char *data, int length);
/** 
 * 打开串口函数函数指针
 * 
 * @param port 串口号
 * @param param 串口参数
 * 
 * @return 返回串口句柄，NULL 表示打开失败
 */
typedef void * (*fnSerialPortOpen)(const char *port, const sSerialportParam *param);

/** 
 * 串口读取函数函数指针
 * 
 * @param handle 串口句柄
 * @param buff 缓存区
 * @param 缓存区长度
 * 
 * @return 实际读取到的数据长度，小于0表示发生错误
 */
typedef int (*fnSerialPortRead)(void *handle, char *buff, int max_length);

/** 
 * 串口写入函数函数指针
 * 
 * @param handle 串口句柄
 * @param buff 缓存区
 * @param 缓存区长度
 * 
 * @return 实际写入的数据长度，小于0表示发生错误
 */
typedef int (*fnSerialPortWrite)(void *handle, const char *buff, int length);

/** 
 * 关闭串口函数函数指针
 * 
 * @param handel 串口句柄
 */
typedef void (*fnSerialPortClose)(void *handel);

#endif /* SERIALPORT_PARAM_DEFS_H */

// EOF
