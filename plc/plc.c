#include "plc.h"
#include "plc_priv_defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int plc_init(sPLC *plc, ePLC_Type type)
{
    if(plc == NULL)
    {
        return -1;
    }

    sPLC_Priv *priv = malloc(sizeof(sPLC_Priv));

    // 不能创建处理句柄，失败
    if(priv == NULL)
    {
        return -1;
    }

    int ret = -1;
    if(type == MITSUBISHI_FX)
    {
        ret = fx_init(priv);
    }
    else if(type == PANASONIC_)
    {
        //ret = xx_init(priv);
    }
    
    if(ret != 0)
    {
        // 处理句柄初始化失败，释放内存
        free(priv);

        return -1;
    }

    if(plc->serialportOpen(priv->serialportName, &priv->serialParam))
    {
        // 串口打开失败
        // 关闭处理句柄，释放内存
        priv->deinit(plc);

        free(priv);

        return -1;
    }

    plc->priv = priv;
    
    return 0;
}

int plc_deinit(sPLC *plc)
{
    sPLC_Priv * priv = (sPLC_Priv *)plc->priv;

    // 关闭处理句柄，释放内存
    priv->deinit(plc);
    plc->serialportClose(priv->serialport);

    free(priv);
    plc->priv = NULL;

    return 0;
}

int plc_read_bits(sPLC *plc, int station, const char *start, int count, uint8_t *buff)
{
    sPLC_Priv * plc_priv = (sPLC_Priv *)plc->priv;

    return plc_priv->readBits(plc, station, start, count, buff);
}

int plc_read_words(sPLC *plc, int station, const char *start, int count, uint16_t *buff)
{
    sPLC_Priv * plc_priv = (sPLC_Priv *)plc->priv;

    return plc_priv->readWords(plc, station, start, count, buff);
}

int plc_write_bits(sPLC *plc, int station, const char *start, int count, const uint8_t *buff)
{
    sPLC_Priv * plc_priv = (sPLC_Priv *)plc->priv;

    return plc_priv->writeBits(plc, station, start, count, buff);
}

int plc_write_words(sPLC *plc, int station, const char *start, int count, const uint16_t *buff)
{
    sPLC_Priv * plc_priv = (sPLC_Priv *)plc->priv;

    return plc_priv->writeWords(plc, station, start, count, buff);
}

// EOF
