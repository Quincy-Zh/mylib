#include "fx.h"

#include "../plc_priv_defs.h"

#include <stdlib.h>
#include <string.h>


#define FX_ENQ 0x05 // Enquiry
#define FX_ACK 0x06 // Acknowledge
#define FX_NAK 0x15 // Nagative ack
#define FX_STX 0x02 // Start of text
#define FX_ETX 0x03 // End of text


typedef struct FxPriv
{
    char send_buff[128];
    char recv_buff[256];
} sFxPriv;

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

static sSerialParam _serial_param = {9600, 8, 1, 0, 0};

int _fx_load_cfg(const char *filepath, sPLC_Priv *priv)
{
    memset(priv->serialportName, 0, SERIALPORT_NAME_MAX_LEN);
    strncpy(priv->serialportName, "COM64", SERIALPORT_NAME_MAX_LEN);

    memcpy(&priv->serialParam, &_serial_param, sizeof(sSerialParam));

    return 0;
}

int fx_deinit(sPLC *plc)
{
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // ...

    // 释放各自内存
    free(myself);

    return 0;
}

int fx_read_bits(sPLC *plc, int station, const char *start, int count, uint8_t *buff)
{
    int packet_size = 0;
    sPLC_Priv *plc_priv = (sPLC_Priv *)plc->priv;
    sFxPriv *myself = (sFxPriv *)plc_priv->priv;

    // packed packet -> send_buff
    
    if(plc->serialportWrite(plc_priv->serialport, myself->send_buff, packet_size))
    {
        // 发送失败...

        return -1;
    }

    // responed...

    return 0;
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
