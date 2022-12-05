#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
//For this question, we will use a mutex 

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_sleep.h"

#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0))
#define GPIO_INPUT_IO_0     0
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0))

//Use pre-emptive scheduling. Q2.a addition
#define configUSE_PREEMPTION 1;


static char pcWriteBuffer[2000];
//Will reduce size

static SemaphoreHandle_t xMutex = NULL;
static TaskHandle_t xHandle_1 = NULL;
static TaskHandle_t xHandle_test_1 = NULL;
static TaskHandle_t xHandle_2 = NULL;
static TaskHandle_t xHandle_3 = NULL;
static TaskHandle_t xHandle_4 = NULL;

static void task_1(void *arg);
static void task_1_test_version(void *arg);
static void task_1_unit_test(void *arg);

static void task_2(void *arg);
static void task_3(void *arg);

static char pcWriteBuffer[2000];
static void getStatistics(void *arg);

/*
Creates the mutex. 
Configures GPIO2 as an output. 
Creates the tasks from mytasks.h 
Infinite loop which displays a count every second. 
*/
void app_main(void);

