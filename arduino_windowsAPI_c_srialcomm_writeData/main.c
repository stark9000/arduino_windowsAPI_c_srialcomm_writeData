#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

HANDLE COMM_PORT_HANDLE;
char COMM_PORT_NAME[] = "\\\\.\\COM13";
BOOL STATUS;

DCB DCB_PARAMS = {0};

void writeData(int CHOICE);

int CHOICE;

int main()
{

    COMM_PORT_HANDLE = CreateFile(COMM_PORT_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (COMM_PORT_HANDLE == INVALID_HANDLE_VALUE)
    {
        printf("Error in opening serial port");
        return 1;
    }
    else
    {
        printf("opening serial port successful");
        printf("\n");

        STATUS = FlushFileBuffers(COMM_PORT_HANDLE);
        if (!STATUS)
        {
            printf("Failed to flush serial port");
            CloseHandle(COMM_PORT_HANDLE);
        }

        do
        {
            printf("Enter 1 to turn on the LED");
            printf("\n");
            printf("Enter 0 to turn off the LED");
            printf("\n");
            printf("any other char to quit");
            printf("\n");
            scanf("%d", &CHOICE);
            printf("\n");

            printf("CHOICE = %d", CHOICE);
            printf("\n");

            if (CHOICE == 1)
            {
                printf("turning on LED");
                printf("\n");
                writeData(CHOICE);
            }
            else if (CHOICE == 0)
            {
                printf("turning off LED");
                printf("\n");
                writeData(CHOICE);
            }
            else
            {
                printf("exit !");
                CloseHandle(COMM_PORT_HANDLE);
                exit(0);
            }
        } while (1);
    }
}

void writeData(int CHOICE)
{

    STATUS = GetCommState(COMM_PORT_HANDLE, &DCB_PARAMS);

    DCB_PARAMS.DCBlength = sizeof(DCB);
    DCB_PARAMS.BaudRate = CBR_9600;
    DCB_PARAMS.ByteSize = 8;
    DCB_PARAMS.Parity = NOPARITY;
    DCB_PARAMS.StopBits = ONESTOPBIT;

    STATUS = SetCommState(COMM_PORT_HANDLE, &DCB_PARAMS);

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    char W_BUFFER[1];
    char TMP = '.';

    if (CHOICE == 1)
    {
        TMP = '1';
    }
    else if (CHOICE == 0)
    {
        TMP = '0';
    }

    size_t LENGTH_OF_BUFFER = strlen(W_BUFFER);
    snprintf((W_BUFFER + LENGTH_OF_BUFFER), sizeof(W_BUFFER - LENGTH_OF_BUFFER), "%c", TMP);

    DWORD NO_OF_BYTES_TO_WRITE;
    DWORD NO_OF_BYTES_WRITTEN = 0;

    NO_OF_BYTES_TO_WRITE = sizeof(W_BUFFER);

    STATUS = WriteFile(COMM_PORT_HANDLE, W_BUFFER, NO_OF_BYTES_TO_WRITE, &NO_OF_BYTES_WRITTEN, NULL);
}