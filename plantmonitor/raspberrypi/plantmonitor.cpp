// Copyright Victor Hurdugaci (http://victorhurdugaci.com). All rights reserved.
// Licensed under the Apache License, Version 2.0. See LICENSE in the project root for license information.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

#include <StreamSerialProtocol.h>

// Change if needed
#define SERIAL_DEVICE "/dev/ttyACM0"
#define DATA_FILE "sensors.csv"

// The structure is maked with __attribute((packed))
// because we don't want any structure padding. Otherwise we might
// get invalid data from the device
struct Sensors {
  uint16_t moisture;
  uint16_t light;
} __attribute__((packed)) sensorData;

int getch();

int main() 
{    
    int serial = open(
        SERIAL_DEVICE, 
        O_RDWR | O_NOCTTY | O_NDELAY);
	    
    if (serial == -1) 
    {
        printf("Failed to open serial port.\n");
        return -1;  
    }
    
    struct termios options; 
    tcgetattr(serial, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serial, TCIFLUSH);
    tcsetattr(serial, TCSANOW, &options); 

    // Create the stream protocol object
    StreamSerialProtocol protocol(serial, (uint8_t*)&sensorData, sizeof(sensorData));
    
    time_t now;
    struct tm * local_time;
        
    char data_row[100];
    char time_string[50];
    
    FILE* dataFile = fopen(DATA_FILE, "a");
    if (dataFile == NULL) 
    {
        printf("Failed to open the data file");
        return -1;
    }

    printf("Press q to end the program\n");
    
    while(getch() != 'q')
    {
        uint8_t receiveState = protocol.receive();
        
        if (receiveState == ProtocolState::SUCCESS)
        {
            time(&now);
            local_time = localtime(&now);
            strftime(time_string, 100, "%m/%d,%H:%M:%S", local_time);

            sprintf(data_row, 
                "%s,%d,%d\n",
                time_string, 
                sensorData.moisture,
                sensorData.light);
            
            fputs(data_row, dataFile);
            printf(data_row);
        }
        else
        {
            sleep(10);
        }
    }
    
    fclose(dataFile);

    close(serial);
    return 0;
}

// Non blocking method for getting a character from the console
int getch()
{
    int ch;
    struct termios oldt, newt;
    long oldf, newf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    newf = oldf | O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, newf);
    ch = getchar();
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
