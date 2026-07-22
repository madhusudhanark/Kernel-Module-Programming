##this code helps me in detecting adxl3345 on spi0.0 on beaglebone black from userspace , already tested and working, pins 17,18 ,20,21 ,
##using beaglebone provided spidev0 device tree overlay.

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"

#define SPI_MODE SPI_MODE_3
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED 1000000   // 1 MHz

#define ADXL345_DEVID_REG 0x00
#define READ_CMD          0x80

int main(void)
{
    int fd;
    uint8_t mode = SPI_MODE;
    uint8_t bits = SPI_BITS_PER_WORD;
    uint32_t speed = SPI_SPEED;

    fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
        perror("SPI mode");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
        perror("SPI bits");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        perror("SPI speed");
        return -1;
    }

    uint8_t tx[2];
    uint8_t rx[2];

    tx[0] = READ_CMD | ADXL345_DEVID_REG;
    tx[1] = 0x00;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = speed,
        .bits_per_word = bits,
        .delay_usecs = 0,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        perror("SPI transfer");
        close(fd);
        return -1;
    }

    printf("Device ID = 0x%02X\n", rx[1]);

    if (rx[1] == 0xE5)
        printf("ADXL345 detected!\n");
    else
        printf("Unexpected Device ID.\n");

    close(fd);
    return 0;
}
