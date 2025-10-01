#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int main() 
{
    int uart_fd;
    struct termios options;
    char buffer[256];
    uart_fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY);
    if (uart_fd < 0) 
    {
        perror("Failed To Open UART");
        return -1;
    }
    // Configure UART
    tcgetattr(uart_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(uart_fd, TCSANOW, &options);
    // Send data
    strcpy(buffer, "Hello From BeagleBone Black UART\n");
    write(uart_fd, buffer, strlen(buffer));
    // Read response
    int bytes = read(uart_fd, buffer, sizeof(buffer));
    if (bytes > 0) 
    {
        printf("Received : %.*s", bytes, buffer);
    }
    close(uart_fd);
    return 0;
}