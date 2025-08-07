#include "encoder.h"
#include "hardware/pio.h"

#include "quadrature_encoder.pio.h"
#include <stdint.h>

static PIO pio = pio0;


void encoder_init(){
    for (int encoder = 0; encoder < ENCODER_COUNT; encoder++){
        pio_add_program(pio, &quadrature_encoder_program);
        quadrature_encoder_program_init(pio, GET_ENCODER_SM(encoder), GET_ENCODER_PIN(encoder), 0);
    }
}
int encoder_get_count(uint8_t encoder){
    return quadrature_encoder_get_count(pio, GET_ENCODER_SM(encoder));
    // minus is used to invert count to match default board setup
}
