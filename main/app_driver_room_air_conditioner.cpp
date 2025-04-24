//app_driver_room_air_conditioner.cpp
#include "app_driver_room_air_conditioner.h"
#include "ThermostatController.h"
#include "S21Protocol.h"
#include "S21Utils.h"
#include "ThermostatMode.h"
#include "Debug.h"
#include "driver/uart.h"

#include <esp_log.h>
#include <esp_matter.h>
#include <esp_matter_attribute.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app_priv.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t room_air_conditioner_endpoint_id;

#define S21_RX_PIN 14
#define S21_TX_PIN 13
#define S21_UART_NUM UART_NUM_1

static S21Protocol* s21Protocol = nullptr;
static ThermostatController* thermostatController = nullptr;

static esp_err_t app_driver_room_air_conditioner_set_power(ThermostatController* ctrl, esp_matter_attr_val_t *val){
    return ctrl->setPower(val->val.b) ? ESP_OK : ESP_FAIL;
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    if (endpoint_id == room_air_conditioner_endpoint_id) {
        ThermostatController* ctrl = (ThermostatController*)driver_handle;
        if (cluster_id == OnOff::Id) {
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                err = app_driver_room_air_conditioner_set_power(ctrl, val);
            }
        } else if (cluster_id == Thermostat::Id) {
            if (attribute_id == Thermostat::Attributes::SystemMode::Id) {
                err = ctrl->setTargetMode(val->val.u8) ? ESP_OK : ESP_FAIL;
            } else if (attribute_id == Thermostat::Attributes::OccupiedCoolingSetpoint::Id) {
                float temp = (float)val->val.i16 / 100.0f; // Matter uses 0.01 \'b0C units
                err = ctrl->setTargetTemperature(temp) ? ESP_OK : ESP_FAIL;
            }
        }
    }
    return err;
}

esp_err_t app_driver_room_air_conditioner_set_defaults(uint16_t endpoint_id)
{
    esp_err_t err = ESP_OK;
    void *priv_data = endpoint::get_priv_data(endpoint_id);
    ThermostatController* ctrl = (ThermostatController*)priv_data;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    /* Setting power */
    attribute_t *attribute = attribute::get(endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);
    err |= app_driver_room_air_conditioner_set_power(ctrl, &val);

    return err;
}

app_driver_handle_t app_driver_room_air_conditioner_init()
{
    ESP_LOGI(TAG, "Initializing UART and S21Protocol");

    // UART config
    uart_config_t uart_config = {
        .baud_rate = 2400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(S21_UART_NUM, &uart_config);
    uart_set_pin(S21_UART_NUM, S21_TX_PIN, S21_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(S21_UART_NUM, 256, 0, 0, NULL, 0);

    vTaskDelay(pdMS_TO_TICKS(200));

    // Initialize protocol + controller
    s21Protocol = new S21Protocol(S21_UART_NUM);
    if (!s21Protocol->begin()) {
        ESP_LOGE(TAG, "S21 protocol init failed");
        return nullptr;
    }

    thermostatController = new ThermostatController(*s21Protocol);
    if (!thermostatController) {
        ESP_LOGE(TAG, "Failed to create ThermostatController");
        return nullptr;
    }

    ESP_LOGI(TAG, "A/C driver initialized");
    return (app_driver_handle_t)thermostatController;
}
