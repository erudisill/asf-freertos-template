#include <asf.h>
#include "conf_board.h"

#define TASK_MONITOR_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_MONITOR_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_LED_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define TASK_CLI_STACK_SIZE                (2048/sizeof(portSTACK_TYPE))
#define TASK_CLI_STACK_PRIORITY            (tskIDLE_PRIORITY)

static volatile bool bMonitorPaused = true;

extern void vApplicationMallocFailedHook(void) {
	taskDISABLE_INTERRUPTS();
	for (;;)
		;
}

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *) pcTaskName);
	for (;;)
		;
}

extern void vApplicationIdleHook(void) {
}

extern void vApplicationTickHook(void) {
}

static void task_monitor(void *pvParameters) {
	static portCHAR szList[256];
	UNUSED(pvParameters);

	for (;;) {
		if (bMonitorPaused == false) {
			printf("--- task ## %u\r\n", (unsigned int) uxTaskGetNumberOfTasks());
			vTaskList((signed portCHAR *) szList);
			printf(szList);
		}
		vTaskDelay(1000);
	}
}

static void task_led(void *pvParameters) {
	UNUSED(pvParameters);
	for (;;) {
		pio_toggle_pin(LED_STATUS_IDX);
		vTaskDelay(1000);
	}
}

// CHEAP CLI task to exercise getchar()
// A better solution is to use FreeRTOS+CLI
static void task_cli(void *pvParameters) {
	uint8_t c;
	UNUSED(pvParameters);
	for (;;) {
		c = getchar();
		if (c == 'm' || c == 'M') {
			bMonitorPaused = (bMonitorPaused ? false : true);
			printf("bMonitorPaused is now %d\r\n", (int)bMonitorPaused);
		}
	}
}

static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = { .baudrate = CONF_UART_BAUDRATE, .paritytype = CONF_UART_PARITY, };
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

int main(void) {
	sysclk_init();
	board_init();

	configure_console();

	/* Output demo infomation. */
	printf("-- Freertos Example --\n\r");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	printf("-- Press M to toggle Task Monitor\r\n");

	/* Create task to monitor processor activity */
	if (xTaskCreate(task_monitor, "Monitor", TASK_MONITOR_STACK_SIZE, NULL,TASK_MONITOR_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create Monitor task\r\n");
	}

	/* Create task to make led blink */
	if (xTaskCreate(task_led, "Led", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create test led task\r\n");
	}

	/* Create task for cheap CLI */
	if (xTaskCreate(task_cli, "CLI", TASK_CLI_STACK_SIZE, NULL, TASK_CLI_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create CLI task\r\n");
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}
