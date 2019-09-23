#ifndef PLC_PRIV_DEFS_H
#define PLC_PRIV_DEFS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "plc.h"

#include <stdint.h>

#ifndef SERIALPORT_NAME_MAX_LEN
#define SERIALPORT_NAME_MAX_LEN (128)
#endif

struct PLC_Priv;
typedef struct PLC_Priv sPLC_Priv;

typedef int (*fnPlcDeinit)(sPLC *plc);

typedef int (*fnPlcReadBits)(sPLC *plc, int station, const char *start, int count, uint8_t *buff);
typedef int (*fnPlcReadWords)(sPLC *plc, int station, const char *start, int count, uint16_t *buff);
typedef int (*fnPlcWriteBits)(sPLC *plc, int station, const char *start, int count, const uint8_t *buff);
typedef int (*fnPlcWriteWords)(sPLC *plc, int station, const char *start, int count, const uint16_t *buff);


struct PLC_Priv
{
    fnPlcDeinit deinit;
    fnPlcReadBits readBits;
    fnPlcReadWords readWords;
    fnPlcWriteBits writeBits;
    fnPlcWriteWords writeWords;

    char serialportName[SERIALPORT_NAME_MAX_LEN];
    sSerialportParam serialParam;
    void *serialport;

    void *priv;
};


#ifdef __cplusplus
}
#endif

#endif // PLC_PRIV_DEFS_H
// EOF
