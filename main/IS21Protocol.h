//IS21Protocol.h
#include <cstdint>
#include <cstddef>


#pragma once

// S21 協議版本定義
enum class S21ProtocolVersion {
    UNKNOWN = 0,
    V1 = 1,
    V2 = 2,
    V3_00 = 300,
    V3_10 = 310,
    V3_20 = 320,
    V3_40 = 340
};

// 功能支援標誌
struct S21Features {
    bool hasAutoMode : 1;        // 支援自動模式
    bool hasDehumidify : 1;      // 支援除濕模式
    bool hasFanMode : 1;         // 支援送風模式
    bool hasPowerfulMode : 1;    // 支援強力模式
    bool hasEcoMode : 1;         // 支援節能模式
    bool hasTemperatureDisplay : 1;  // 支援溫度顯示
    
    S21Features() : 
        hasAutoMode(false),
        hasDehumidify(false),
        hasFanMode(false),
        hasPowerfulMode(false),
        hasEcoMode(false),
        hasTemperatureDisplay(false) {}
};

class IS21Protocol {
public:
    virtual ~IS21Protocol() = default;
    
    // 初始化並探測協議版本和功能
    virtual bool begin() = 0;
    
    // 發送命令並等待確認
    virtual bool sendCommand(char cmd0, char cmd1, const uint8_t* payload = nullptr, size_t len = 0) = 0;
    
    // 解析回應
    virtual bool parseResponse(uint8_t& cmd0, uint8_t& cmd1, uint8_t* payload, size_t& payloadLen) = 0;
    
    // 獲取協議版本和功能支援信息
    virtual S21ProtocolVersion getProtocolVersion() const = 0;
    virtual const S21Features& getFeatures() const = 0;
    virtual bool isFeatureSupported(const S21Features& feature) const = 0;
    
    // 檢查命令是否被當前協議版本支援
    virtual bool isCommandSupported(char cmd0, char cmd1) const = 0;
}; 
