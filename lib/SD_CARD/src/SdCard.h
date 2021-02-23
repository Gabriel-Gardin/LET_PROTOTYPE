#ifndef SDCARD_H
#define SDCARD_H

#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_camera.h"



esp_err_t initialize_sd_card();

esp_err_t save_image(camera_fb_t *img);

#endif