#include "camera_helpers.h"

static const char* TAG_CAMERA = "camera_helpers";

camera_fb_t* fb;

// Define camera configuration structure
camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sscb_sda = CAM_PIN_SIOD,
    .pin_sscb_scl = CAM_PIN_SIOC,
    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 12,
    .fb_count = 1
};

// Function to initialize the camera
esp_err_t init_camera(int framesize) {
    // Set frame size
    camera_config.frame_size = framesize;

    // Initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_CAMERA, "Camera Init Failed");
        return err;
    }

    ESP_LOGI(TAG_CAMERA, "Camera Ready");
    return ESP_OK;
}

// Function to capture an image to RAM
esp_err_t camera_capture_RAM() {
    // Acquire a frame
    fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG_CAMERA, "Camera Capture Failed");
        return ESP_FAIL;
    }

    // Return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}

// Function to capture an image to a file
esp_err_t camera_capture(char* FileName, size_t* pictureSize) {
    // Clear internal queue
    for (int i = 0; i < 1; i++) {
        camera_fb_t* fb = esp_camera_fb_get();
        ESP_LOGI(TAG_CAMERA, "fb->len=%d", fb->len);
        esp_camera_fb_return(fb);
    }

    // Acquire a frame
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG_CAMERA, "Camera Capture Failed");
        return ESP_FAIL;
    }

    // Replace this with your own function
    // Process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
    FILE* f = fopen(FileName, "wb");
    if (f == NULL) {
        ESP_LOGE(TAG_CAMERA, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fwrite(fb->buf, fb->len, 1, f);
    ESP_LOGI(TAG_CAMERA, "fb->len=%d", fb->len);
    *pictureSize = (size_t)fb->len;
    fclose(f);

    // Return the frame buffer back to the driver for reuse
    esp_camera_fb_return(fb);
    return ESP_OK;
}
