#pragma once

#include <stdint.h>

typedef void (*spi_callback_t)(uint8_t *data);


void spi_init(spi_callback_t spi_callback);
