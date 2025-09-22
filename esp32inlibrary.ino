#include <BLEDevice.h>
#include <BLEServer.h>
#include "esp_gap_ble_api.h"

// 原始 iBeacon 广播数据
uint8_t bleRaw[] = {
  0x02,0x01,0x06,
  0x1A,0xFF,
  0x4C,0x00,
  0x02,0x15,
  0xFD,0xA5,0x06,0x93,0xA4,0xE2,0x4F,0xB1,0xAF,0xCF,0xC6,0xEB,0x07,0x64,0x78,0x25,
  0x27,0xD7,
  0xA5,0xEA,
  0xC5
};

// 广播参数 **全局定义**
esp_ble_adv_params_t adv_params = {
  .adv_int_min        = 0x20,
  .adv_int_max        = 0x40,
  .adv_type           = ADV_TYPE_NONCONN_IND, // iBeacon 不可连接
  .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
  .peer_addr          = {0},
  .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC,
  .channel_map        = ADV_CHNL_ALL,
  .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("签到成功辽~");
    // 重新开始广播
    esp_ble_gap_start_advertising(&adv_params);
  }
  void onDisconnect(BLEServer* pServer) {}
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-iBeacon");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // 设置原始广播数据
  esp_err_t rc = esp_ble_gap_config_adv_data_raw(bleRaw, sizeof(bleRaw));
  if (rc != ESP_OK) {
    Serial.printf("esp_ble_gap_config_adv_data_raw failed: %d\n", rc);
  }

  // 开始广播
  esp_ble_gap_start_advertising(&adv_params);
  Serial.println("iBeacon broadcasting...");
}

void loop() {
  // 持续广播
}
