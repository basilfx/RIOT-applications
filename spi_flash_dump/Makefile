APPLICATION = spi_flash_dump

# If no BOARD is found in the environment, use this default:
BOARD ?= ikea-tradfri

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

USEMODULE += mtd_spi_nor
USEMODULE += od

# Modules and packages:
FEATURES_REQUIRED += periph_spi

include $(RIOTBASE)/Makefile.include
