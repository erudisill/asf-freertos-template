
#ifndef _CPH_BOARD_H
#define _CPH_BOARD_H

#include "compiler.h"
#include "system_sam4s.h"
#include "exceptions.h"
#include "pio.h"

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL        (32768U)
#define BOARD_FREQ_SLCK_BYPASS      (32768U)
#define BOARD_FREQ_MAINCK_XTAL      (12000000U)
#define BOARD_FREQ_MAINCK_BYPASS    (12000000U)

/** Master clock frequency */
#define BOARD_MCK                   CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US   15625

/** Name of the board */
#define BOARD_NAME "CPH-BOARD"
/** Board definition */
#define cphBoard
/** Family definition (already defined) */
#define sam4s
/** Core definition */
#define cortexm4



/*----------------------------------------------------------------------------*/
/*	CONSOLE																	  */
/*----------------------------------------------------------------------------*/

#define CONSOLE_UART        UART0
#define CONSOLE_UART_ID     ID_UART0

#define PINS_UART0_PIO 		PIOA
#define PINS_UART0_TYPE 	PIO_PERIPH_A
#define PINS_UART0_MASK 	PIO_PA9A_URXD0|PIO_PA10A_UTXD0
#define PINS_UART0_ATTR 	PIO_DEFAULT


/*----------------------------------------------------------------------------*/
/*	LEDS																	  */
/*----------------------------------------------------------------------------*/

#define LED_STATUS_IDX		PIO_PC12_IDX

#define PINS_LED0_PIO		PIOC
#define PINS_LED0_TYPE		PIO_OUTPUT_0
#define PINS_LED0_MASK		PIO_PC12
#define PINS_LED0_ATTR		PIO_DEFAULT






#endif  // _CPH_BOARD_H
