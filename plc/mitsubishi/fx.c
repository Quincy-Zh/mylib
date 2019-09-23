#include "fx.h"

#include "../plc_priv_defs.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define FX_ENQ 0x05 // Enquiry
#define FX_ACK 0x06 // Acknowledge
#define FX_NAK 0x15 // Nagative ack
#define FX_STX 0x02 // Start of text
#define FX_ETX 0x03 // End of text

#define PC_NUMBER (0xff)

#define PROTOCOL_FORMAT_1 (1)
#define PROTOCOL_FORMAT_4 (4)

typedef struct FxPriv
{
    int protocol; // 协议类型：PROTOCOL_FORMAT_1 or PROTOCOL_FORMAT_4
    int my_number; // PC 号0xff
    int timeout; // 报文等待时间，单位为10ms，值得范围0~15，也就是0~150ms

    // ...
    uint8_t send_buff[128]; // 
    uint8_t recv_buff[256];

    // Busy 标志，标志有效时，等待 PLC 响应中，暂不能处理其他指令
    // 建议采用平台自身事件同步实现
#if defined(__WIN32)
    // TODO WIN32 平台互斥量
#elif defined(linux)
    // TODO Linux 平台互斥量
#else
    int busy; 
#endif

    // 
} sFxPriv;

/**
 * @brief Busy 标志初始化
 * 
 * @param p sFxPriv 指针
 * @return 0 表示成功，其他表示值失败
 */
static inline int __busy_flag_init(sFxPriv * p);

/**
 * @brief Busy 标志判定
 * 
 * @param p sFxPriv 指针
 * @return int 0 表示成功，其他表示值失败
 */
static inline int __entry(sFxPriv * p);

/**
 * @brief  Busy 标志清除
 * 
 * @param p sFxPriv 指针
 * @return 0 表示成功，其他表示值失败
 */
static inline int __quit(sFxPriv * p);

static int fx_deinit(sPLC *plc);
static int fx_read_bits(sPLC *plc, int station, const char *start, int count, uint8_t *buff);
static int fx_read_words(sPLC *plc, int station, const char *start, int count, uint16_t *buff);
static int fx_write_bits(sPLC *plc, int station, const char *start, int count, const uint8_t *buff);
static int fx_write_words(sPLC *plc, int station, const char *start, int count, const uint16_t *buff);

static int _fx_load_cfg(const char *filepath, sPLC_Priv *priv);

int fx_init(sPLC *plc)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;

    sFxPriv *myself = malloc(sizeof(sFxPriv));
    if(myself == NULL)
    {
        return -1;
    }

    memset(myself, 0, sizeof(sFxPriv));

    if(__busy_flag_init(myself))
    {
        // 初始化 Busy 标志 失败
        free(myself);

        return -1;
    }

    if(_fx_load_cfg(plc->cfg_filepath, plc_priv))
    {
        // 加载配置文件失败
        free(myself);

        return -1;
    }

    plc_priv->priv = myself;
    plc_priv->deinit = fx_deinit;
    plc_priv->readBits = fx_read_bits;
    plc_priv->readWords = fx_read_words;
    plc_priv->writeBits = fx_write_bits;
    plc_priv->writeWords = fx_write_words;

    return 0;
}

int __busy_flag_init(sFxPriv * p)
{
#if defined(__WIN32)
    // TODO WIN32 平台互斥量初始化
#elif defined(linux)
    // TODO Linux 平台互斥量初始化
#else
    p->busy = 0;
#endif
    
    return 0;
}

int __entry(sFxPriv * p)
{
#if defined(__WIN32)
    // TODO WIN32 平台获取互斥量
#elif defined(linux)
    // TODO Linux 平台获取互斥量
#else
    if(p->busy)
    {
        return 1;
    }

    p->busy = 1;
#endif

    return 0;
}

int __quit(sFxPriv * p)
{
#if defined(__WIN32)
    // TODO WIN32 平台释放互斥量
#elif defined(linux)
    // TODO Linux 平台释放互斥量
#else
    if(p->busy)
    {
        return 1;
    }
    p->busy = 0;
#endif

    return 0;
}

static sSerialportParam _serial_param = {9600, SP_DATA_BITS_8, SP_STOP_BITS_1, SP_PARITY_NONE, SP_PARITY_NONE};

int _fx_load_cfg(const char *filepath, sPLC_Priv *priv)
{
    // TODO 实现从文件加载配置，设置相关内容
    sFxPriv *myself = (sFxPriv *)priv->priv;

    memset(priv->serialportName, 0, SERIALPORT_NAME_MAX_LEN);
    strncpy(priv->serialportName, "COM64", SERIALPORT_NAME_MAX_LEN);

    memcpy(&priv->serialParam, &_serial_param, sizeof(sSerialportParam));

    //
    myself->my_number = PC_NUMBER;
    myself->protocol = PROTOCOL_FORMAT_4;
    myself->timeout = 10; // 100ms 等待时间

    return 0;
}

int fx_deinit(sPLC *plc)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // ...
    free(myself);

    return 0;
}

int fx_read_bits(sPLC *plc, int station, const char *start, int count, uint8_t *buff)
{
    int ret_val = 0;
    int packet_size = 0;
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    if(__entry(myself))
    {
        return -1;
    }

    // packed packet -> send_buff
    if(plc->serialportWrite(plc_priv->serialport, myself->send_buff, packet_size))
    {
        // 发送失败...

        ret_val = -1;
        goto _exit;
    }

    // responed...
    ;

_exit:
    __quit(myself);

    return ret_val;
}

int fx_read_words(sPLC *plc, int station, const char *start, int count, uint16_t *buff)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // ...

    return -1;
}

int fx_write_bits(sPLC *plc, int station, const char *start, int count, const uint8_t *buff)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // ...

    return -1;
}

int fx_write_words(sPLC *plc, int station, const char *start, int count, const uint16_t *buff)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // ...

    return -1;
}

// EOF
