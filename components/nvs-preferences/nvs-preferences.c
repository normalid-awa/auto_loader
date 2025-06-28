#include "nvs-preferences.h"

#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_log.h"

#include "nvs_flash.h"

static nvs_handle_t app_nvs_handle;

esp_err_t setup_nvs_preferences(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_RETURN_ON_ERROR(err, NVS_PREFERENCES_TAG, "Failed to initialize NVS");

    ESP_RETURN_ON_ERROR(nvs_open(NVS_PREFERENCES_NAMESPACE, NVS_READWRITE, &app_nvs_handle), NVS_PREFERENCES_TAG, "Failed to open NVS");

    ESP_LOGI(NVS_PREFERENCES_TAG, "NVS initialized");

    uint32_t startup_count = 0;
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_get_u32(app_nvs_handle, "startup_count", &startup_count));
    ESP_ERROR_CHECK(nvs_set_u32(app_nvs_handle, "startup_count", ++startup_count));
    ESP_ERROR_CHECK(nvs_commit(app_nvs_handle));

    ESP_LOGI(NVS_PREFERENCES_TAG, "This is the %" PRIu32 "-th startup", startup_count);

    return ESP_OK;
}

esp_err_t __nvs_commit()
{
    ESP_RETURN_ON_ERROR(nvs_commit(app_nvs_handle), NVS_PREFERENCES_TAG, "Failed to commit NVS");
    return ESP_OK;
}

esp_err_t load_i32(const char *key, int32_t *value)
{
    esp_err_t err = nvs_get_i32(app_nvs_handle, key, value);
    if (err == ESP_ERR_NVS_NOT_FOUND)
        save_i32(key, *value);
    else
        ESP_RETURN_ON_ERROR(err, NVS_PREFERENCES_TAG, "Failed to get i32 %s", key);
    return ESP_OK;
}

esp_err_t save_i32(const char *key, const int32_t value)
{
    ESP_RETURN_ON_ERROR(nvs_set_i32(app_nvs_handle, key, value), NVS_PREFERENCES_TAG, "Failed to set i32 %s to %" PRIi32, key, value);
    return __nvs_commit(app_nvs_handle);
}

esp_err_t load_u32(const char *key, uint32_t *value)
{
    // ESP_RETURN_ON_ERROR(nvs_get_u32(app_nvs_handle, key, value), NVS_PREFERENCES_TAG, "Failed to get u32 %s", key);
    esp_err_t err = nvs_get_u32(app_nvs_handle, key, value);
    if (err == ESP_ERR_NVS_NOT_FOUND)
        save_u32(key, *value);
    else
        ESP_RETURN_ON_ERROR(err, NVS_PREFERENCES_TAG, "Failed to get u32 %s", key);
    return ESP_OK;
}

esp_err_t save_u32(const char *key, const uint32_t value)
{
    ESP_RETURN_ON_ERROR(nvs_set_u32(app_nvs_handle, key, value), NVS_PREFERENCES_TAG, "Failed to set u32 %s to %" PRIu32, key, value);
    return __nvs_commit(app_nvs_handle);
}

esp_err_t load_float(const char *key, float *value)
{
    size_t required_size;
    esp_err_t err = nvs_get_str(app_nvs_handle, key, NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        save_float(key, *value);
        return ESP_OK;
    }
    else
        ESP_RETURN_ON_ERROR(err, NVS_PREFERENCES_TAG, "Failed to get size of str %s", key);
    char *temp_val = malloc(required_size);
    ESP_RETURN_ON_ERROR(nvs_get_str(app_nvs_handle, key, temp_val, &required_size), NVS_PREFERENCES_TAG, "Failed to get str %s", key);
    *value = (float)atof(temp_val);
    free(temp_val);
    return ESP_OK;
}

esp_err_t save_float(const char *key, const float value)
{
    int len = snprintf(NULL, 0, "%f", value);
    char temp_val[len + 1];
    snprintf(temp_val, len + 1, "%f", value);
    ESP_RETURN_ON_ERROR(nvs_set_str(app_nvs_handle, key, temp_val), NVS_PREFERENCES_TAG, "Failed to set float %s to %f", key, value);
    return __nvs_commit(app_nvs_handle);
}

esp_err_t load_str(const char *key, char *value)
{
    size_t required_size;
    esp_err_t err = nvs_get_str(app_nvs_handle, key, NULL, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        save_str(key, value);
        return ESP_OK;
    }
    else
        ESP_RETURN_ON_ERROR(err, NVS_PREFERENCES_TAG, "Failed to get size of str %s", key);
    value = malloc(required_size);
    ESP_RETURN_ON_ERROR(nvs_get_str(app_nvs_handle, key, value, &required_size), NVS_PREFERENCES_TAG, "Failed to get str %s", key);
    return ESP_OK;
}

esp_err_t save_str(const char *key, const char *value)
{
    ESP_RETURN_ON_ERROR(nvs_set_str(app_nvs_handle, key, value), NVS_PREFERENCES_TAG, "Failed to set str %s to %s", key, value);
    return __nvs_commit(app_nvs_handle);
}