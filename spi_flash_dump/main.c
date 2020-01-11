/*
 * Utility for dumping SPI flash memory.
 */

#include <stdio.h>
#include <stdbool.h>

#include "od.h"
#include "mtd_spi_nor.h"

#include "periph/spi.h"

#define SPI_BUS     SPI_DEV(0)
#define SPI_EN_PIN  GPIO_PIN(PF, 3)
#define SPI_CS_PIN  GPIO_PIN(PB, 11)

static mtd_spi_nor_t board_nor_dev = {
    .base = {
        .driver = &mtd_spi_nor_driver,
        .page_size = 256,
        .pages_per_sector = 16,
        .sector_count = 64,
    },
    .opcode = &mtd_spi_nor_opcode_default,
    .spi = SPI_BUS,
    .cs = SPI_CS_PIN,
    .addr_width = 3,
    .mode = SPI_MODE_0,
    .clk = SPI_CLK_10MHZ,
};

static mtd_dev_t *mtd0 = (mtd_dev_t *)&board_nor_dev;

static uint8_t buf[256];

/**
 * @brief   Return true if at least one byte is different than 0xff.
 */
static bool check_buf(uint8_t *buf, size_t len)
{
    for (unsigned i = 0; i < len; i++) {
        if (buf[i] != 0xff) {
            return true;
        }
    }

    return false;
}

int main(void)
{
    puts("Initialize GPIO.");

    gpio_init(SPI_EN_PIN, GPIO_OUT);

    puts("Initialize SPI.");

    gpio_set(SPI_EN_PIN);

    puts("Initializing NOR Flash.");

    int res = mtd_init(mtd0);

    if (res < 0) {
        puts("Failed");
        return 1;
    }

    /* flash memory is an IS25LQ020B (2MBit/256KiB) */
    int addr = 0;

    for (unsigned j = 0; j < (256 * 1024) / sizeof(buf); j++) {
        printf("Reading address %04x\n", addr);

        int res2 = mtd_read(mtd0, buf, addr, sizeof(buf));

        if (res2 < 0) {
            puts("Failed");
            return 1;
        }

        if (check_buf(buf, sizeof(buf))) {
            od_hex_dump(buf, sizeof(buf), OD_WIDTH_DEFAULT);

            printf("\n");
        }

        addr += sizeof(buf);
    }

    puts("Done.");
    return 0;
}
