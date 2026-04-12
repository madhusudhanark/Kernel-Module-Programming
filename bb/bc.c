#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define UART_DEVICE "/dev/ttyS1"   // or /dev/ttyO1

int main() {
    int uart0_filestream = -1;
    int ret = -1;
    // Open UART
    uart0_filestream = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1) {
        perror("Error - Unable to open UART");
        return 1;
    }

    // Configure UART
    struct termios options;
    tcgetattr(uart0_filestream, &options);

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // 9600 baud
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    printf("UART initialized\n");

    // Send data
    char *tx_buffer = "Hello from BeagleBone\n";
    ret = write(uart0_filestream, tx_buffer, strlen(tx_buffer));
     
    if (ret == -1)
      {
         printf("error in writing to ttyS1\n");
	 return -1;
          }
     printf("bytes written tty: %d\n",ret);
    // Read data
    char rx_buffer[256];
    int rx_length;

    while (1) {
        rx_length = read(uart0_filestream, (void*)rx_buffer, 255);
        if(rx_length == -1)
       {
          printf("error in reading the file\n");
           } 

        if (rx_length > 0) {
            rx_buffer[rx_length] = '\0';
            printf("Received: %s\n", rx_buffer);
        }

        usleep(1000000);
    }

    close(uart0_filestream);
    return 0;
}
