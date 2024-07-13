#if CONFIG_SHUTTER_AUTO


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "cmd.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"



extern QueueHandle_t xQueueCmd;

static const char *TAG = "AUTO";

uint64_t interval_us = 500000;

CMD_t cmdBuf;

esp_timer_handle_t periodic_timer;


static void periodic_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
    
    if (xQueueSend(xQueueCmd, &cmdBuf, 10) != pdPASS) {
        ESP_LOGE(TAG, "xQueueSend fail");
    }
    ESP_LOGE(TAG, "xQueueSend SUCCESS");
}


void setup_timer(){
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "acuisition_timer"
    };

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */

    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, interval_us));
    ESP_LOGI(TAG, "Started timer, time since boot: %lld us", esp_timer_get_time());
}

void print_timer_state(){
    ESP_ERROR_CHECK(esp_timer_dump(stdout));
}

void delete_timer(){
    /* Clean up and finish the example */
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
    ESP_LOGI(TAG, "Stopped and deleted timer");
}





void auto_shutter(void *pvParameter)
{
	ESP_LOGI(TAG, "Start CONFIG_SHUTTER_AUTO");
	cmdBuf.taskHandle = xTaskGetCurrentTaskHandle();
	cmdBuf.command = CMD_TAKE;

    setup_timer();

	while(1) {
        print_timer_state();
		vTaskDelay(10000/portTICK_PERIOD_MS);
	}

    delete_timer();
	/* Never reach */
	vTaskDelete( NULL );
}
#endif

