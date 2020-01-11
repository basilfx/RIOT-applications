/*
 * Utility for reading SPI flash JEDEC ID, to identify flash manufacturer.
 */

#include <stdio.h>

#include "periph/spi.h"

#define SPI_BUS     SPI_DEV(0)
#define SPI_EN_PIN  GPIO_PIN(PF, 3)
#define SPI_CS_PIN  GPIO_PIN(PB, 11)

int main(void)
{
    uint8_t buf[] = { 0x9F, 0x00, 0x00, 0x00 };

    puts("Initialize GPIO.");

    gpio_init(SPI_CS_PIN, GPIO_OUT);
    gpio_init(SPI_EN_PIN, GPIO_OUT);

    puts("Initialize SPI.");

    gpio_set(SPI_EN_PIN);
    spi_init(SPI_BUS);

    puts("Sending JEDEC READ ID command.");

    spi_acquire(SPI_BUS, SPI_CS_UNDEF, SPI_MODE_0, SPI_CLK_1MHZ);
    gpio_clear(SPI_CS_PIN);
    spi_transfer_bytes(SPI_BUS, GPIO_UNDEF, false, &buf, &buf, 4);
    gpio_set(SPI_CS_PIN);
    spi_release(SPI_BUS);

    printf("JEDEC ID: %x %x %x.\n", buf[1], buf[2], buf[3]);
}
