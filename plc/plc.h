#ifndef LIB_PLC_H
#define LIB_PLC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../serialport/serialport_defs.h"

#include <stdint.h>

struct PLC;
typedef struct PLC sPLC;

typedef enum PLC_Type
{
    MITSUBISHI_FX,
    PANASONIC_,
} ePLC_Type;

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
