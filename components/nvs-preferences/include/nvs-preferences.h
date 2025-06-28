#ifndef NVS_PREFERENCES_H
#define NVS_PREFERENCES_H

#include <esp_err.h>

static const char *NVS_PREFERENCES_TAG = "nvs_preferences";
static const char *NVS_PREFERENCES_NAMESPACE = "nvs_preferences";

esp_err_t setup_nvs_preferences(void);

esp_err_t load_i32(const char *key, int32_t *value);
esp_err_t save_i32(const char *key, const int32_t value);

esp_err_t load_u32(const char *key, uint32_t *value);
esp_err_t save_u32(const char *key, const uint32_t value);

esp_err_t load_float(const char *key, float *value);
esp_err_t save_float(const char *key, const float value);

esp_err_t load_str(const char *key, char *value);
esp_err_t save_str(const char *key, const char *value);

#endif // NVS_PREFERENCES_H