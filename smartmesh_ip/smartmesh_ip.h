#ifndef SMARTMESH_IP_DEFS_H
#define SMARTMESH_IP_DEFS_H


#define SMIP_HDLC_FLAG                   0x7e
#define SMIP_HDLC_ESCAPE                 0x7d
#define SMIP_HDLC_ESCAPE_MASK            0x20

#define SMIP_HDLC_CRCINIT                0xffff
#define SMIP_HDLC_CRCGOOD                0xf0b8

#define DN_HDLC_MAX_FRAME_LENGTH       128
#define DN_HDLC_INPUT_BUFFER_SIZE      DN_HDLC_MAX_FRAME_LENGTH


#include "../serialport/serialport_defs.h"
#include <stdint.h>

struct SmartMeshIP;
typedef struct SmartMeshIP sSmartMeshIP;


struct SmartMeshIP
{
    const char *cfg_filepath;

    fnSerialPortOpen  serialportOpen;
    fnSerialPortClose serialportClose;
    fnSerialPortRead  serialportRead;
    fnSerialPortWrite serialportWrite;

    void *serialport;
    void *priv;
};

#ifdef __cplusplus
extern "C" {
#endif

uint16_t hdlc_crc16(const uint8_t *data, int length);

#ifdef __cplusplus
}
#endif

#endif // SMARTMESH_IP_DEFS_H
// EOF
