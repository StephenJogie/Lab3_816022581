#include "app_main.h"
/*
NOTE about Q2.b
The mutex already utilizes Priority Inheritance.
Therefore, no addition change was needed
===============================================================================
===============================================================================
===============================================================================
NOTE about q2 DISCUSS -> BRANCH: q2_discuss
main resource used is vTaskGetRunTimeStats in task.h
https://www.freertos.org/a00021.html#vTaskGetRunTimeStats for documentation
From the above link it specifies that this disables interrupts
This is also not intended for notmal application usage, only for debugging.
Therefore, a separate branch is used for debugging specifically with this function.

*/
static const char *TAG = "main";

static void task_1(void *arg)
{   
    bool output_high = 1;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE) //had ==pdTRUE before
        {   
            printf("task 1 took semaphore thing \n"); //testline
            //bool output_level = 1;
            gpio_set_level(GPIO_OUTPUT_IO_0, output_high);

            //Active delay for 500ms (ish)
            printf("Active delay for 500ms \n");
            for (uint32_t i=0;i<6500000;i++){

            }   
        
            //Task delay for 1000ms

                
            //Give back the mutex after critical task section complete
            printf("Going to attempt to give back the mutex after resisting tempation (task1) \n");
            xSemaphoreGive(xMutex);
            printf("task1 gave back the mutex despite the temptations of inner greed \n");
            printf("task1 task delay for 1000ms \n");
            vTaskDelay(1000 / portTICK_RATE_MS);
        } 
        else
        {
            //Did not take the mutex
            printf("Could not take the mutex (task 1 btw) \n");
        }
    }
}

/*
This below task is the same as task 1, but rather than active delay it deletes itself
NOTE: Note about using xTaskGetTickCount() to verify delay. 
    The tickrate is 100Hz (see freertos config).
    Acceptable value for success is 49-51 (due to slightly above and slightly below 500ms).
    It displayed 49 ticks as the active delay, this was verified to be the case with the oscilloscope which showed around 490-500ms
    Therefore this is a success. 
*/
static void task_1_test_version(void *arg)
{   
    TickType_t start_tick, end_tick, tick_difference;
    bool output_high = 1;
    //mutex_reject_cnt used to track times this task could not take mutex
    //Will delete itself after 10 times it could not take mutex
    uint8_t mutex_reject_cnt = 0; 
    for(;;){
        
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE) //had ==pdTRUE before
        {   
            mutex_reject_cnt = 0; 
            //reset the count as the mutex was taken

            printf("task 1 took semaphore thing \n"); //testline
            //bool output_level = 1;
            gpio_set_level(GPIO_OUTPUT_IO_0, output_high);

            //Active delay for 500ms (ish)
            printf("Active delay for 500ms \n");
            for (uint32_t i=0;i<6500000;i++){

            }   
   
            //Give back the mutex after critical task section complete
            printf("Going to attempt to give back the mutex after resisting tempation (task1) \n");
            xSemaphoreGive(xMutex);
            printf("task1 gave back the mutex despite the temptations of inner greed \n");
            printf("task1 deleted itself \n");
            //vTaskDelay(1000 / portTICK_RATE_MS);
            vTaskDelete(NULL);
        } 
        else
        {
            //Did not take the mutex
            mutex_reject_cnt++;
            printf("Could not take the mutex (task 1 delete prone version btw)\n");
            printf("Mutex reject count: %d \n \n", mutex_reject_cnt);
            vTaskDelay(100/portTICK_RATE_MS); 
            //Added a delay so as to not fill console screen for unit testing
            if(mutex_reject_cnt == 10)
            {
                printf("task 1 test version will delete itself as it has not been able to run 10 times \n \n");
                vTaskDelete(NULL);
            }
        }
    }
}

/*
This is the unit test for task1 as specified in the readme
Only test not implemented was verifying the task delay by waiting for task1_test_version to delete itself
This was intentionally left out for now as the task delay has been verified already
*/
static void task_1_unit_test(void *arg)
{   
    bool output_high = 1;
    bool output_low = 0;
    TickType_t start_tick, end_tick, tick_difference;
    uint8_t success_count = 0, total_count = 0;
    for(;;)
    {
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE) //had ==pdTRUE before
        {   
            printf("task 1 unit test took semaphore thing \n"); //testline
            
            gpio_set_level(GPIO_OUTPUT_IO_0, output_low);
            //Currently has mutex
            xSemaphoreGive(xMutex);
            //Mutex is now available
            start_tick = xTaskGetTickCount();
            xTaskCreate(task_1_test_version, "self-deleting task 1", 2048, NULL, 10, NULL);
            end_tick = xTaskGetTickCount();
            tick_difference = end_tick - start_tick;
            printf("Time Taken for unit test for task 1 part 1: %d", (tick_difference));
            total_count++;
            if((tick_difference<=51) && (tick_difference>=49))
            {
                printf("ACTIVE DELAY WAS SUCCEDSSFUL WITHIN 10MS ACCURACY \n \n");
                success_count++;
            }

            total_count++;
            if(gpio_get_level(GPIO_OUTPUT_IO_0) == output_high)
            {
                success_count++;
                printf("TASK 1 SET HIGH SUCCESS \n \n");
            }

            //At this point in time it is known that the mutex is free
            //But I will nest another check I guess
            //Will increment totalCount before checking
            //This is because I can consider the test a fail if the mutex was not given back by task1
            total_count++;
            if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE) 
            {
                printf("task 1 unit test took semaphore thing AGAIN\n"); //testline
                gpio_set_level(GPIO_OUTPUT_IO_0, output_low);
                //Creating Task 1 test version again as previous one deleted itself
                xTaskCreate(task_1_test_version, "self-deleting task 1", 2048, NULL, 10, NULL);
                vTaskDelay(1500/portTICK_RATE_MS);
                if(gpio_get_level(GPIO_OUTPUT_IO_0) == output_low)
                {
                    success_count++;
                    printf("TASK 1 DID NOT TOUCH PIN WHEN MUTEX UNAVAILABLE SUCCESS \n \n");
                }

            }
            else
            {
                printf("Task 1 unit test could not take mutex a second time? Kindof strange huh?! \n");
            }

            printf("TESTING COMPLETE: \n ");
            printf("RESULTS: %d SUCCESS/ %d TOTAL TESTS \n \n \n", success_count,total_count);
            vTaskDelete(NULL);
        } 
        else
        {
            //Did not take the mutex
            printf("Could not take the mutex (task 1 unit test btw) \n");
        }
    }
}

static void task_2(void *arg)
{
    bool output_low = 0;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE)
        {
            // bool output_level = 0;
            printf("TASK 2 took semaphore \n");
            gpio_set_level(GPIO_OUTPUT_IO_0, output_low);

            //Active delay for 500ms (ish)
            //NOTE: Replace with a function using xTaskGetTickCount
            for (uint32_t i=0;i<6500000;i++){
                //This is around 500ms.
            }   
        
            //Give back the mutex after critical task section complete
            xSemaphoreGive(xMutex);

            //Task delay for 1000ms
            vTaskDelay(1000 / portTICK_RATE_MS);
                
        }
        else
        {
            //Did not take the mutex
            printf("could not take the mutex (task2 btw) \n");
        }
    }
}

static void task_3(void *arg)
{
    uint8_t pin_state;
    for(;;){
    
        //Take the mutex before performing task
        //We want a non-blocking task, so we immediately return after 0 ticks check
        if (xSemaphoreTake(xMutex, (TickType_t)1)==pdTRUE)
        {   
            printf("task 3 took mutex \n");
            //Print the output level of the pin if mutex is taken
            pin_state= gpio_get_level(GPIO_OUTPUT_IO_0);
            printf("The output level of the PIN: %d \n", pin_state); 
            
                
            //Give back the mutex after critical task section complete
            xSemaphoreGive(xMutex);
            //Task delay for 1000ms
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
        else
        {
         printf("Task3 can't take the mutex :( \n");   //Did not take the mutex
        }
    }
}

/*
The getStatistics task will use the vTaskGetRunTimeStats function
The parameter pcWriteBuffer will need to be approximately 40 bytes per task 
The report of execution times will be written into this buffer

*/
static void getStatistics(void *arg)
{
    for(;;){
        //The pcWriteBuffer is defined above as a char array [2000], this is to ensure it has enough space
        //This will get the runtime statistics every ten seconds
        vTaskDelay(10000/portTICK_RATE_MS);
        vTaskGetRunTimeStats(pcWriteBuffer);
        printf("\n \n***************************************************************\n");
        printf("***************************************************************\n");
        printf(pcWriteBuffer);
        printf("\n \n***************************************************************\n");
        printf("***************************************************************\n \n");
    }
}

void app_main(void)
{
    xMutex = xSemaphoreCreateMutex();
    if(xMutex != NULL)
    {
        printf("MUTEX CREATED SUCCESSFULLY \n");
    }

    //NOTE: Configuring outside of tasks for now, will try to configure within tasks next, or make configure task 
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO 2
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    printf("GPIO 2 CONFIGURED AS OUTPUT \n");

    //Start the three tasks
    printf("Going to attempt to make the tasks now \n");
    
    // xHandle_1 = xTaskCreate(task_1, "task_1", 2048, NULL, 10, NULL);
    // printf("TASK1 made  \n");
    // xHandle_2 = xTaskCreate(task_2, "task_2", 2048, NULL, 10, NULL);
    // printf("TASK2 made  \n");
    // xHandle_3 = xTaskCreate(task_3, "task_3", 2048, NULL, 10, NULL); 
    // printf("TASK3 made  \n");
    // xHandle_4 = xTaskCreate(getStatistics, "getStatistics", 2048, NULL, 10, NULL); 
    // printf("GET STATS TASK made  \n");

    xHandle_test_1 = xTaskCreate(task_1_unit_test, "Unit test for task 1", 2048, NULL, 5, NULL);


    int cnt = 0;

    //While loop
    while(1){
        //Put main loop function here
        //Will put nothing here as I just want my tasks to do their thing
        cnt++;
        //ESP_LOGI(TAG, "cnt: %d \n", cnt);
        printf("COUNT = %d \n \n",cnt);
        printf("***************************************************************\n");
        printf("***************************************************************\n");
        printf("ticks = %d \n", xTaskGetTickCount());
        printf("***************************************************************\n");
        printf("***************************************************************\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

}


