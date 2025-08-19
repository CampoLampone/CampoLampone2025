/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * Edited to suit LampOne's board - Plajta 2025 - Salivon Andrii
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

#ifndef _BOARDS_LAMPONE_H
#define _BOARDS_LAMPONE_H

// For board detection
#define LAMPONE

//---------------------------//
//   BOARD PIN DEFINITIONS   //
//---------------------------//
#include "boards/pico.h" // IWYU pragma: keep

// --- MOTOR PINS ---
#define BOARD_MOTOR_EEP 10
#define BOARD_MOTOR_PIN_1 9 // LEFT
#define BOARD_MOTOR_PIN_2 8
#define BOARD_MOTOR_PIN_3 7 // RIGHT
#define BOARD_MOTOR_PIN_4 6

// --- ENCODER PINS ---
#define BOARD_ENCODER1_PIN_A 2 // LEFT
#define BOARD_ENCODER1_PIN_B 3
#define BOARD_ENCODER2_PIN_A 13 // RIGHT
#define BOARD_ENCODER2_PIN_B 14

#endif //_BOARDS_LAMPONE_H
