#include "smartmesh_ip_mngr.h"

#include <stdio.h>
#include <string.h>

#include <windows.h>

#define BUFF_LEN (64)


static int hdlc_unpacket(sSmartMeshIP *mngr, const uint8_t *data_in, int length);
static int _new_frame_recv(sSmartMeshIP *mngr);

static int hdlc_packet(sSmartMeshIP *mngr, const uint8_t *data_in, int length);
static int _send_hello_mngr(sSmartMeshIP *mngr);

static sSerialportParam _serial_param = {115200, SP_DATA_BITS_8, SP_STOP_BITS_1, SP_PARITY_NONE, SP_FLOWCTRL_NONE};

typedef struct Mote_Priv
{
    int status;
    int p_index;

    uint8_t rpacked[DN_HDLC_INPUT_BUFFER_SIZE];

    uint8_t recv[BUFF_LEN];

} sMote_Priv;

static const uint8_t _init_packed[] = {0x7e, 0x00, 0x01, 0x01, 0x03, 0x04, 0x00, 0x00, 0xb3, 0xc5, 0x7e};

int smip_mngr_init(sSmartMeshIP *mngr)
{
    char portname[64] = {0};
    FILE *cfg = fopen(mngr->cfg_filepath, "r");
    if(cfg == NULL)
    {
        printf(":: FAIL Open file: %s.\n", mngr->cfg_filepath);
        return -1;
    }

    int bytes = fread(portname, 1, sizeof(portname), cfg);
    fclose(cfg);

    if(bytes < 0 || bytes == sizeof(portname))
    {
        printf(":: FAIL read: %d.\n", bytes);
        return -1;
    }
    
    int i = 0;
    for(int i = 0; i < bytes; ++i)
    {
        if(!isdigit(portname[i]) && !isalpha(portname[i]))
        {
            portname[i] = 0;
        }
    }

    if(i == bytes)
    {
        return -1;
    }

    sMote_Priv *myself = malloc(sizeof(sMote_Priv));

    if(myself == NULL)
    {
        return -1;
    }

    printf(":: Open port: %s.\n", portname);
    mngr->serialport = mngr->serialportOpen(portname, &_serial_param);
    if(mngr->serialport == NULL)
    {
        printf(":: FAIL open port: %s.\n", portname);
        free(mngr->priv);

        return -1;
    }

    memset(myself, 0, sizeof(sMote_Priv));
    mngr->priv = myself;

    mngr->serialportWrite(mngr->serialport, _init_packed, sizeof(_init_packed));
    
    return 0;
}

int smip_mngr_deinit(sSmartMeshIP *mngr)
{
    mngr->serialportClose(mngr->serialport);

    free(mngr->priv);
    
    mngr->priv = 0;
    mngr->serialport = 0;

    return -1;
}

void smip_mngr_loop(sSmartMeshIP *mngr)
{
    int read_bytes = 0;
    sMote_Priv *myself = mngr->priv;
    
    read_bytes = mngr->serialportRead(mngr->serialport, myself->recv, BUFF_LEN);
    if(read_bytes > 0)
    {
        printf(":: Read %d-byte.\n", read_bytes);

        hdlc_unpacket(mngr, myself->recv, read_bytes);
    }
    
    ;
}

int hdlc_unpacket(sSmartMeshIP *mngr, const uint8_t *data_in, int length)
{
    sMote_Priv *mngr_priv = mngr->priv;

    int ch;

    for(int i = 0; i < length; ++i)
    {
        ch = data_in[i];

        if(mngr_priv->status == 1)
        {
            if(ch == SMIP_HDLC_ESCAPE)
            {
                mngr_priv->status = 2;
            }
            else if(ch == SMIP_HDLC_FLAG)
            {
                uint16_t crc_calc = hdlc_crc16(mngr_priv->rpacked, mngr_priv->p_index - 2);
                uint16_t crc_recv = mngr_priv->rpacked[mngr_priv->p_index - 2] | 
                    (mngr_priv->rpacked[mngr_priv->p_index - 1] << 8); 

                if(crc_calc == crc_recv)
                {
                    // remove fcs
                    mngr_priv->p_index -= 2;

                    _new_frame_recv(mngr);
                }
                else
                {
                    printf(":: FCS FAIL.\n");
                }

                mngr_priv->status = 0;
            }
            else
            {
                mngr_priv->rpacked[mngr_priv->p_index] = ch;
                mngr_priv->p_index += 1;
            }
        }
        else if(mngr_priv->status == 2)
        {
            mngr_priv->rpacked[mngr_priv->p_index] = ch ^ 0x20;
            mngr_priv->p_index += 1;
            mngr_priv->status = 1;
        }
        else if(mngr_priv->status == 0)
        {
            if(ch == SMIP_HDLC_FLAG)
            {
                mngr_priv->status = 1;
                mngr_priv->p_index = 0;
            }
        }
    }
}

int _new_frame_recv(sSmartMeshIP *mngr)
{
    sMote_Priv *mngr_priv = mngr->priv;

    printf(":: Done!\n");

    return 0;
}

// EOF
