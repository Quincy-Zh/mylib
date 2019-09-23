#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "serialport_param_defs.h"

typedef void (*fnDataArrived)(const char *data, int length);

struct SerialportHandle;
typedef struct SerialportHandle sSerialportHandle;

/**
 * @brief 打开串口
 * 
 * @param port 串口名称
 * @param param 串口参数 @sSerialportParam
 * * 
 * @return 打开成功返回串口句柄sSerialportHandle，否则返回 NULL
 */
sSerialportHandle * serialport_open(const char *port, const sSerialportParam *param);

/**
 * @brief 读取串口数据
 * 
 * @param handle serialport_open 返回的串口句柄
 * @param data 保存接收数据的buff
 * @param max_len buff长度
 * 
 * @return 实际读取数据的字节数，小于零发生错误
 */
int serialport_read(sSerialportHandle *handle, char *data, int max_len);

/**
 * @brief 写入串口数据
 * 
 * @param handle serialport_open 返回的串口句柄
 * @param data 需要发送的数据
 * @param max_len 需要发送的数据长度，单位字节
 * 
 * @return 实际发送的数据字节数，小于零发生错误
 */
int serialport_write(sSerialportHandle *handle, const char *data, int len);

/**
 * @brief Flush 串口
 * 
 * @param handle serialport_open 返回的串口句柄
 */
void serialport_flush(sSerialportHandle *handle);

/**
 * @brief 关闭串口
 * 
 * @param handle serialport_open 返回的串口句柄
 */
void serialport_close(sSerialportHandle *handle);


#ifdef __cplusplus
}
#endif

#endif /* SERIALPORT_H */
