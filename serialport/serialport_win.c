/**
 * @file serialport_win.c
 * 
 * @author  Quincy-Zh (wangqy31@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-18
 * 
 */

#if defined(__WIN32)

#include "serialport.h"

#include <stdio.h>
#include <windows.h>

#ifdef _DEBUG

#ifndef DBG_PRINTF
#define DBG_PRINTF printf
#endif

#else
#define DBG_PRINTF
#endif

struct SerialportHandle
{
    HANDLE hComm;
};

static int _allowed_baudrate[] =
    {
        CBR_110,
        CBR_300,
        CBR_600,
        CBR_1200,
        CBR_2400,
        CBR_4800,
        CBR_9600,
        CBR_14400,
        CBR_19200,
        CBR_38400,
        CBR_56000,
        CBR_57600,
        CBR_115200,
        CBR_128000,
        CBR_256000,
};

sSerialportHandle *serialport_open(const char *port, const sSerialportParam *param)
{
    char portname[16] = {0};
    char *end;

    int i = 0;
    sSerialportHandle *handle;

    i = strtol(port + 3, &end, 10);
    if ((end != (port + strlen(port))) || i < 0)
    {
        DBG_PRINTF(":: 'port': %s is invalid.\n", port);
        return NULL;
    }

    snprintf(portname, sizeof(portname), "\\\\.\\COM%d", i);

    for (i = 0; i < sizeof(_allowed_baudrate) / sizeof(int); ++i)
    {
        if (param->baudrate == _allowed_baudrate[i])
        {
            break;
        }
    }

    if (i == sizeof(_allowed_baudrate) / sizeof(int))
    {
        DBG_PRINTF(":: 'baudrate': %d is not allowed.\n", param->baudrate);
        return NULL;
    }

    if (param->databits < SP_DATA_BITS_5 || param->databits > SP_DATA_BITS_8)
    {
        DBG_PRINTF(":: 'databits': %d is not allowed.\n", param->databits);
        return NULL;
    }

    if (param->stopbits < SP_STOP_BITS_1 || param->stopbits > SP_STOP_BITS_2)
    {
        DBG_PRINTF(":: 'stopbits': %d is not allowed.\n", param->stopbits);
        return NULL;
    }

    if (param->parity < SP_PARITY_NONE || param->parity > SP_PARITY_SPACE)
    {
        DBG_PRINTF(":: 'parity': %d is not allowed.\n", param->parity);
        return NULL;
    }

    handle = malloc(sizeof(sSerialportHandle));
    if (handle == NULL)
    {
        DBG_PRINTF(":: Out of memory.\n");

        return NULL;
    }

    handle->hComm = CreateFile(portname,
                               GENERIC_READ | GENERIC_WRITE,                 //Read/Write
                               0,                                            // No Sharing
                               NULL,                                         // No Security
                               OPEN_EXISTING,                                // Open existing port only
                               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // Overlapped I/O
                               NULL);                                        // Null for Comm Devices

    if (handle->hComm == INVALID_HANDLE_VALUE)
    {
        free(handle);

        DBG_PRINTF(":: Cannot open serial port \"%s\".\n", port);

        return NULL;
    }

    // 设置缓存区长度
    SetupComm(handle->hComm, 1024, 1024);

    // configure serial port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(handle->hComm, &dcbSerialParams))
    {
        CloseHandle(handle->hComm);
        free(handle);

        DBG_PRINTF(":: Cannot config serial port \"%s\".\n", port);

        return NULL;
    }

    dcbSerialParams.BaudRate = param->baudrate;
    dcbSerialParams.ByteSize = param->databits;
    dcbSerialParams.StopBits = param->stopbits;
    dcbSerialParams.Parity = param->parity;

    if (!SetCommState(handle->hComm, &dcbSerialParams))
    {
        CloseHandle(handle->hComm);
        free(handle);

        DBG_PRINTF(":: Cannot config serial port \"%s\".\n", port);

        return NULL;
    }

    // Setting Timeouts
    COMMTIMEOUTS timeouts = {0};

    // 在读一次输入缓冲区的内容后读操作就立即返回，
    // 而不管是否读入了要求的字符。
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(handle->hComm, &timeouts))
    {
        CloseHandle(handle->hComm);
        free(handle);

        DBG_PRINTF(":: Cannot set timeout serial port \"%s\".\n", port);

        return NULL;
    }

    PurgeComm(handle->hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

    return handle;
}

int serialport_read(sSerialportHandle *handle, char *data, int max_len)
{
    DWORD dwBytesRead = max_len;
    DWORD dwErrorFlags;
    OVERLAPPED m_osRead;

    memset(&m_osRead, 0, sizeof(OVERLAPPED));
    m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (ClearCommError(handle->hComm, &dwErrorFlags, NULL))
    {
        PurgeComm(handle->hComm, PURGE_TXABORT | PURGE_TXCLEAR);
    }

    if (!ReadFile(handle->hComm, data, dwBytesRead, &dwBytesRead, &m_osRead))
    {
        DWORD err = GetLastError();
        if (err == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(handle->hComm, &m_osRead, &dwBytesRead, TRUE))
            {
                Sleep(10);
            }

            return dwBytesRead;
        }
        else
        {
            DBG_PRINTF(":: Error: %x.\n", err);
            return -1;
        }
    }

    return dwBytesRead;
}

int serialport_write(sSerialportHandle *handle, const char *data, int len)
{
    DWORD dwBytesWritten = len;
    DWORD dwRealSend = 0;
    DWORD dwErrorFlags;
    COMSTAT ComStat;
    OVERLAPPED m_osWrite;
    BOOL bWriteStat;

    memset(&m_osWrite, 0, sizeof(OVERLAPPED));
    m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (ClearCommError(handle->hComm, &dwErrorFlags, NULL))
    {
        PurgeComm(handle->hComm, PURGE_TXABORT | PURGE_TXCLEAR);
    }

    if (!WriteFile(handle->hComm, data, dwBytesWritten, &dwRealSend, &m_osWrite))
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            while (!GetOverlappedResult(handle->hComm, &m_osWrite, &dwRealSend, FALSE))
            {
                if (GetLastError() == ERROR_IO_INCOMPLETE)
                {
                    continue;
                }
                else
                {
                    ClearCommError(handle->hComm, &dwErrorFlags, NULL);

                    return -1;
                }
            }

            return dwRealSend;
        }

        ClearCommError(handle->hComm, &dwErrorFlags, NULL);
        return -1;
    }

    return dwRealSend;
}

void serialport_flush(sSerialportHandle *handle)
{
    PurgeComm(handle->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}

void serialport_close(sSerialportHandle *handle)
{
    if (handle)
    {
        CloseHandle(handle->hComm);

        free(handle);
    }
}

#endif

// EOF
