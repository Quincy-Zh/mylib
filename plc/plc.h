#ifndef LIB_PLC_H
#define LIB_PLC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct PLC;
typedef struct PLC sPLC;

typedef enum PLC_Type
{
    MITSUBISHI_FX,
    PANASONIC_,
} ePLC_Type;

typedef struct SerialParam
{
    int baudrate;
    char databits;
    char stopbits;
    char parity;
    char flowctrl;
} sSerialParam;

/** 
 * 打开串口函数函数指针
 * 
 * @param port 串口号
 * @param param 串口参数
 * 
 * @return 返回串口句柄，NULL 表示打开失败
 */
typedef void * (*fnSerialPortOpen)(const char *port, const sSerialParam *param);

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

struct PLC
{
    const char *cfg_filepath;

    fnSerialPortOpen  serialportOpen;
    fnSerialPortClose serialportClose;
    fnSerialPortRead  serialportRead;
    fnSerialPortWrite serialportWrite;

    void *priv;
};


/**
 * @brief 初始化 PLC 模块
 * 
 * @param plc PLC 句柄
 * @param type PLC 类型
 * 
 * @return 错误码
 */
int plc_init(sPLC *plc, ePLC_Type type);

/**
 * @brief 销毁 PLC 模块
 * 
 * @param plc PLC 句柄
 * 
 * @return 错误码
 */
int plc_deinit(sPLC *plc);


/**
 * @brief PLC 处理循环
 * 
 * @param plc PLC 句柄
 */
void plc_loop(sPLC *plc);

/**
 * @brief 读位
 * 
 * @param plc PLC 句柄
 * @param station PLC 站号
 * @param start 起始元件号
 * @param count 读取的元件个数
 * @param buff 用于保存读取的内容，请保证其长度不小于 count 字节
 * 
 * @return 错误码
 */
int plc_read_bits(sPLC *plc, int station, const char *start, int count, uint8_t *buff);

/**
 * @brief 读字/寄存器
 * 
 * @param plc PLC 句柄
 * @param station PLC 站号
 * @param start 起始元件号
 * @param count 读取的元件个数
 * @param buff 用于保存读取的内容，请保证其长度不小于 count*2 字节
 * 
 * @return 错误码
 */
int plc_read_words(sPLC *plc, int station, const char *start, int count, uint16_t *buff);

/**
 * @brief 写位
 * 
 * @param plc PLC 句柄
 * @param station PLC 站号
 * @param start 起始元件号
 * @param count 写入的元件个数
 * @param buff 需要写入的内容
 * 
 * @return 错误码
 */
int plc_write_bits(sPLC *plc, int station, const char *start, int count, const uint8_t *buff);

/**
 * @brief 写字/寄存器
 * 
 * @param plc PLC 句柄
 * @param station PLC 站号
 * @param start 起始元件号
 * @param count 写入的元件个数
 * @param buff 需要写入的内容
 * 
 * @return 错误码
 */
int plc_write_words(sPLC *plc, int station, const char *start, int count, const uint16_t *buff);


#ifdef __cplusplus
}
#endif

#endif // LIB_PLC_H

// EOF
