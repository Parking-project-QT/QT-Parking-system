#include "protocol.h"
#include "led.h"
#include "uart.h"

#include <string.h>

#define COMMAND_BUFFER_SIZE 64U

static char command_buffer[COMMAND_BUFFER_SIZE];
static unsigned int command_length;
static int command_overflow;

static void ProcessCommand(char *command)
{
    unsigned int index;

    for (index = 0U; command[index] != '\0'; ++index)
    {
        if ((command[index] >= 'a') && (command[index] <= 'z'))
        {
            command[index] = (char)(command[index] - ('a' - 'A'));
        }
    }

    if (strcmp(command, "PING") == 0)
    {
        Uart1_WriteLine("OK PONG");
    }
    else if (strcmp(command, "LED 1 ON") == 0)
    {
        Led_Set(1U, 1);
        Uart1_WriteLine("OK LED 1 ON");
    }
    else if (strcmp(command, "LED 1 OFF") == 0)
    {
        Led_Set(1U, 0);
        Uart1_WriteLine("OK LED 1 OFF");
    }
    else if (strcmp(command, "LED 2 ON") == 0)
    {
        Led_Set(2U, 1);
        Uart1_WriteLine("OK LED 2 ON");
    }
    else if (strcmp(command, "LED 2 OFF") == 0)
    {
        Led_Set(2U, 0);
        Uart1_WriteLine("OK LED 2 OFF");
    }
    else if (command[0] != '\0')
    {
        Uart1_WriteLine("ERR UNKNOWN_COMMAND");
    }
}

void Protocol_Init(void)
{
    command_length = 0U;
    command_overflow = 0;
}

void Protocol_ReceiveByte(unsigned char byte)
{
    if (byte == '\r')
    {
        return;
    }

    if (byte == '\n')
    {
        if (command_overflow)
        {
            Uart1_WriteLine("ERR LINE_TOO_LONG");
        }
        else
        {
            command_buffer[command_length] = '\0';
            ProcessCommand(command_buffer);
        }

        command_length = 0U;
        command_overflow = 0;
        return;
    }

    if (command_length < (COMMAND_BUFFER_SIZE - 1U))
    {
        command_buffer[command_length++] = (char)byte;
    }
    else
    {
        command_overflow = 1;
    }
}
