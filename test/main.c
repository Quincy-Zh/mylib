#include "serialport.h"
#include "smartmesh_ip.h"
#include "smartmesh_ip_mngr.h"
#include "smartmesh_ip_mngr.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

static void * serialPortOpen(const char *port, const sSerialportParam *param);
static int serialPortRead(void *handle, char *buff, int max_length);
static int serialPortWrite(void *handle, const char *buff, int length);
static void serialPortClose(void *handel);

static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
static int keep_running = 1;


int main(int argc, char * argv[])
{
    const char *cfg_filepath = "cfg.ini";

    uint8_t x_val[5] = {0};

    sSmartMeshIP smip_mngr = {
        cfg_filepath,
        serialPortOpen,
        serialPortClose,
        serialPortRead,
        serialPortWrite,
        NULL,
        NULL
    };

    SetConsoleCtrlHandler(CtrlHandler, TRUE);

    if(smip_mngr_init(&smip_mngr))
    {
        printf(":: FAIL 'smip_mngr_init'.\n");

        return 1;
    }

    while(keep_running)
    {
        smip_mngr_loop(&smip_mngr);

        Sleep(200);
    }
    
    smip_mngr_deinit(&smip_mngr);

    return 0;
}

void * serialPortOpen(const char *port, const sSerialportParam *param)
{
    return serialport_open(port, param);
}

int serialPortRead(void *handle, char *buff, int max_length)
{
    if(handle == NULL)
    {
        return -1;
    }

    return serialport_read((sSerialportHandle *)handle, buff, max_length);
}

int serialPortWrite(void *handle, const char *buff, int length)
{
    if(handle == NULL)
    {
        return -1;
    }

    return serialport_write((sSerialportHandle *)handle, buff, length);
}

void serialPortClose(void *handle)
{
    if(handle == NULL)
    {
        return;
    }

    serialport_close((sSerialportHandle *)handle);
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal. 
    case CTRL_C_EVENT:
        printf("Ctrl-C event\n\n");
        Beep(750, 300);

        keep_running = 0;

        return TRUE;

        // CTRL-CLOSE: confirm that the user wants to exit. 
    case CTRL_CLOSE_EVENT:
        Beep(600, 200);
        printf("Ctrl-Close event\n\n");
        return TRUE;

        // Pass other signals to the next handler. 
    case CTRL_BREAK_EVENT:
        Beep(900, 200);
        printf("Ctrl-Break event\n\n");
        return FALSE;

    case CTRL_LOGOFF_EVENT:
        Beep(1000, 200);
        printf("Ctrl-Logoff event\n\n");
        return FALSE;

    case CTRL_SHUTDOWN_EVENT:
        Beep(750, 500);
        printf("Ctrl-Shutdown event\n\n");
        return FALSE;

    default:
        return FALSE;
    }
}

// EOF
