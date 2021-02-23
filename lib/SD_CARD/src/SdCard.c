#include "SdCard.h"


sdmmc_card_t* gCard;

static const char *TAG = "SD_CARD_SYSTEM";


esp_err_t initialize_sd_card()
{

    sdmmc_host_t gHost = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t gSlotConfig = SDMMC_SLOT_CONFIG_DEFAULT();

    esp_vfs_fat_sdmmc_mount_config_t gMountConfig = {
        .format_if_mount_failed = false,
        .max_files = 1,
        .allocation_unit_size = 16 * 1024
    };


    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    // GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
    // Internal pull-ups are not sufficient. However, enabling internal pull-ups
    // does make a difference some boards, so we do that here.
    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &gHost, &gSlotConfig, &gMountConfig, &gCard);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, gCard);

    return ESP_OK;
}

esp_err_t save_image(camera_fb_t *img)
{
    static int counting;
    char file_name[25];
    counting += 1;
    snprintf(file_name, 25, "/sdcard/%d.jpg", counting);

    FILE* f = fopen(file_name, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fwrite(img->buf, 1, img->len, f);
    fclose(f);
    return ESP_OK;
}