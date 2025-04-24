//ThermostatMode.h
#include <cstdint>
#include <cstddef>


#pragma once

// HomeKit 恆溫器模式定義
#define HAP_MODE_OFF   0
#define HAP_MODE_HEAT  1
#define HAP_MODE_COOL  2
#define HAP_MODE_AUTO  3

// 大金空調模式定義
// 這些值用於與空調直接通訊
#define AC_MODE_FAN     6  // 送風模式
#define AC_MODE_HEAT    4  // 暖房模式
#define AC_MODE_COOL    3  // 冷房模式
#define AC_MODE_AUTO    1  // 自動模式
#define AC_MODE_AUTO_2  0  // 自動模式 2
#define AC_MODE_AUTO_3  7  // 自動模式 3
#define AC_MODE_DRY     2  // 除濕模式
#define AC_MODE_INVALID -1 // 無效模式

// 風扇速度定義 - 數值表示
#define FAN_AUTO    0  // 自動
#define FAN_SPEED_1 1  // 1檔
#define FAN_SPEED_2 2  // 2檔
#define FAN_SPEED_3 3  // 3檔
#define FAN_SPEED_4 4  // 4檔
#define FAN_SPEED_5 5  // 5檔
#define FAN_QUIET   6  // 安靜

// 風扇速度定義 - 協議字符表示
#define AC_FAN_AUTO    'A'  // 自動
#define AC_FAN_QUIET   'B'  // 安靜
#define AC_FAN_1       '3'  // 1檔
#define AC_FAN_2       '4'  // 2檔
#define AC_FAN_3       '5'  // 3檔
#define AC_FAN_4       '6'  // 4檔
#define AC_FAN_5       '7'  // 5檔
#define AC_FAN_INVALID -1   // 無效風扇速度

// HomeKit 模式轉換函數
static uint8_t convertHomeKitToACMode(uint8_t hapMode) {
    switch (hapMode) {
        case HAP_MODE_OFF:  return AC_MODE_INVALID;  // 關機狀態用自動模式
        case HAP_MODE_HEAT: return AC_MODE_HEAT;  // 製熱
        case HAP_MODE_COOL: return AC_MODE_COOL;  // 製冷
        case HAP_MODE_AUTO: return AC_MODE_AUTO;  // 自動
        default: return AC_MODE_AUTO;
    }
}

// 空調模式轉換為 HomeKit 模式
static uint8_t convertACToHomeKitMode(uint8_t acMode, bool isPowerOn) {
    // 如果電源關閉，始終返回 OFF 模式
    if (!isPowerOn) {
        return HAP_MODE_OFF;
    }
    
    // 只有在電源開啟時才進行正常的模式轉換
    switch (acMode) {
        case AC_MODE_HEAT: return HAP_MODE_HEAT;  // 製熱
        case AC_MODE_COOL: return HAP_MODE_COOL;  // 製冷
        case AC_MODE_AUTO:   // 自動模式
        case AC_MODE_AUTO_2: // 自動模式 2
        case AC_MODE_AUTO_3: // 自動模式 3
            return HAP_MODE_AUTO;  // 所有自動模式都映射為 HomeKit 自動
        case AC_MODE_DRY:  return HAP_MODE_AUTO;  // 除濕模式映射為自動
        case AC_MODE_FAN:  return HAP_MODE_AUTO;  // 送風模式映射為自動
        default: return HAP_MODE_AUTO;
    }
}

// 獲取 HomeKit 模式的文字描述
static const char* getHomeKitModeText(uint8_t hapMode) {
    switch (hapMode) {
        case HAP_MODE_OFF:  return "關機";
        case HAP_MODE_HEAT: return "製熱";
        case HAP_MODE_COOL: return "製冷";
        case HAP_MODE_AUTO: return "自動";
        default: return "未知";
    }
}

// 獲取空調模式的文字描述
static const char* getACModeText(uint8_t acMode) {
    switch (acMode) {
        case AC_MODE_FAN:  return "送風";
        case AC_MODE_HEAT: return "製熱";
        case AC_MODE_COOL: return "製冷";
        case AC_MODE_AUTO: return "自動";
        case AC_MODE_AUTO_2: return "自動 2";
        case AC_MODE_AUTO_3: return "自動 3";
        case AC_MODE_DRY:  return "除濕";
        default: return "未知";
    }
}

// 風扇速度轉換函數
static uint8_t convertFanSpeedToAC(uint8_t speed) {
    switch (speed) {
        case FAN_AUTO:    return AC_FAN_AUTO;   // 自動
        case FAN_QUIET:   return AC_FAN_QUIET;  // 安靜
        case FAN_SPEED_1: return AC_FAN_1;      // 1檔
        case FAN_SPEED_2: return AC_FAN_2;      // 2檔
        case FAN_SPEED_3: return AC_FAN_3;      // 3檔
        case FAN_SPEED_4: return AC_FAN_4;      // 4檔
        case FAN_SPEED_5: return AC_FAN_5;      // 5檔
        default:          return AC_FAN_AUTO;   // 默認自動
    }
}

static uint8_t convertACToFanSpeed(uint8_t acFan) {
    switch (acFan) {
        case AC_FAN_AUTO:  return FAN_AUTO;    // 自動
        case AC_FAN_QUIET: return FAN_QUIET;   // 安靜
        case AC_FAN_1:     return FAN_SPEED_1; // 1檔
        case AC_FAN_2:     return FAN_SPEED_2; // 2檔
        case AC_FAN_3:     return FAN_SPEED_3; // 3檔
        case AC_FAN_4:     return FAN_SPEED_4; // 4檔
        case AC_FAN_5:     return FAN_SPEED_5; // 5檔
        default:           return FAN_AUTO;    // 默認自動
    }
}

static const char* getFanSpeedText(uint8_t speed) {
    switch (speed) {
        case FAN_AUTO:    return "自動";
        case FAN_QUIET:   return "安靜";
        case FAN_SPEED_1: return "1檔";
        case FAN_SPEED_2: return "2檔";
        case FAN_SPEED_3: return "3檔";
        case FAN_SPEED_4: return "4檔";
        case FAN_SPEED_5: return "5檔";
        default:          return "未知";
    }
} 
