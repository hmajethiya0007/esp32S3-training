#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define QUEUE_LENGTH 10
#define QUEUE_ITEM_SIZE sizeof(int)

QueueHandle_t queue;

void sender_task(void *param) {
    int task_id = (int)param;
    int data = task_id * 10;  // Unique data for each task

    while (1) {
        if (xQueueSend(queue, &data, pdMS_TO_TICKS(100)) == pdPASS) {
            printf("Sender Task %d sent: %d\n", task_id, data);
        } else {
            printf("Sender Task %d failed to send\n", task_id);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay before sending again
    }
}

void receiver_task(void *param) {
    int received_data;

    while (1) {
        if (xQueueReceive(queue, &received_data, portMAX_DELAY)) {
            printf("Receiver received: %d\n", received_data);
        }
    }
}

void app_main() {
    queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    if (queue == NULL) {
        printf("Failed to create queue\n");
        return;
    }

    // Create 5 sender tasks
    for (int i = 1; i <= 5; i++) {
        char task_name[20];
        snprintf(task_name, sizeof(task_name), "SenderTask%d", i);
        xTaskCreate(sender_task, task_name, 2048, (void *)i, 1, NULL);
    }

    // Create receiver task
    xTaskCreate(receiver_task, "ReceiverTask", 2048, NULL, 2, NULL);
}
