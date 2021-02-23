/**
 * This example takes a picture every 5s and print its size on serial monitor.
 */

// =============================== SETUP ======================================

// 1. Board setup (Uncomment):
// #define BOARD_WROVER_KIT
// #define BOARD_ESP32CAM_AITHINKER

/**
 * 2. Kconfig setup
 * 
 * If you have a Kconfig file, copy the content from
 *  https://github.com/espressif/esp32-camera/blob/master/Kconfig into it.
 * In case you haven't, copy and paste this Kconfig file inside the src directory.
 * This Kconfig file has definitions that allows more control over the camera and
 * how it will be initialized.
 */

/**
 * 3. Enable PSRAM on sdkconfig:
 * 
 * CONFIG_ESP32_SPIRAM_SUPPORT=y
 * 
 * More info on
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/kconfig.html#config-esp32-spiram-support
 */

// ================================ CODE ======================================

#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "SdCard.h"
#include "esp_cam.h"


static const char *TAG = "example:take_picture";

void app_main()
{
    initialize_sd_card();
    init_camera();

    while (1)
    {
        ESP_LOGI(TAG, "Taking picture...");
        static camera_fb_t *pic;
        pic = esp_camera_fb_get();
        // use pic->buf to access the image
        ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
        save_image(pic);
        vTaskDelay(50000 / portTICK_RATE_MS);
    }
}