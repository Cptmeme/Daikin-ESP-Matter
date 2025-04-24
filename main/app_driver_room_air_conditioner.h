//app_driver_room_air_conditioner.h
#pragma once

#include <stdint.h>
#include <esp_err.h>
#include <esp_matter_attribute.h>

typedef void* app_driver_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

app_driver_handle_t app_driver_room_air_conditioner_init();
esp_err_t app_driver_room_air_conditioner_set_defaults(uint16_t endpoint_id);
esp_err_t app_driver_attribute_update(app_driver_handle_t handle,
    uint16_t endpoint_id,
    uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val);

#ifdef __cplusplus
}
#endif
