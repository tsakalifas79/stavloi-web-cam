#pragma once

#include "esp_log.h"
#include "esp_camera.h"
#include "camera_pin.h"
#include <stdlib.h>
#include <string.h>  // Include for memcpy
#include "esp_heap_caps.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define a structure for resolution
typedef struct {
    int width;
    int height;
} resolution_t;

// Function prototypes
resolution_t get_resolution(int framesize);
int get_bytes_per_pixel(pixformat_t pixel_format);
esp_err_t init_camera(int framesize);
esp_err_t camera_capture_RAM();
esp_err_t camera_capture(char* FileName, size_t* pictureSize);

// Global variables
extern camera_fb_t* fb;

#ifdef __cplusplus
}
#endif
