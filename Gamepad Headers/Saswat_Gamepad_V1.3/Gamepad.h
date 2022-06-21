#ifndef GAMEPAD_H
#define GAMEPAD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "GamepadConnectionStatus.h"
#include "NimBLEHIDDevice.h"
#include "NimBLECharacteristic.h"
#include "GamepadConfiguration.h"

class Gamepad
{
private:
    uint8_t _buttons[16]; // 8 bits x 16 --> 128 bits
    uint8_t _specialButtons;
    int16_t _x;
    int16_t _y;
    int16_t _z;
    int16_t _rZ;
    int16_t _rX;
    int16_t _rY;

    GamepadConfiguration configuration;

    GamepadConnectionStatus *connectionStatus;

    NimBLEHIDDevice *hid;
    NimBLECharacteristic *inputGamepad;

    void rawAction(uint8_t msg[], char msgSize);
    static void taskServer(void *pvParameter);
    uint8_t specialButtonBitPosition(uint8_t specialButton);

public:
    Gamepad(std::string deviceName = "Saswat Gamepad", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
    void begin(GamepadConfiguration *config = new GamepadConfiguration());
    void end(void);
    void setAxes(int16_t x = 0, int16_t y = 0, int16_t z = 0, int16_t rZ = 0, int16_t rX = 0, int16_t rY = 0);
    void press(uint8_t b = BUTTON_1);   // press BUTTON_1 by default
    void release(uint8_t b = BUTTON_1); // release BUTTON_1 by default
    void pressSpecialButton(uint8_t b);
    void releaseSpecialButton(uint8_t b);
    void pressStart();
    void releaseStart();
    void pressSelect();
    void releaseSelect();
    void setLeftThumb(int16_t x = 0, int16_t y = 0);
    void setRightThumb(int16_t z = 0, int16_t rZ = 0);
    void setLeftTrigger(int16_t rX = 0);
    void setRightTrigger(int16_t rY = 0);
    void setTriggers(int16_t rX = 0, int16_t rY = 0);
    void setX(int16_t x = 0);
    void setY(int16_t y = 0);
    void setZ(int16_t z = 0);
    void setRZ(int16_t rZ = 0);
    void setRX(int16_t rX = 0);
    void setRY(int16_t rY = 0);
    void sendReport();
    bool isPressed(uint8_t b = BUTTON_1); // check BUTTON_1 by default
    bool isConnected(void);
    void resetButtons();
    void setBatteryLevel(uint8_t level);
    uint8_t batteryLevel;
    std::string deviceManufacturer;
    std::string deviceName;

protected:
    virtual void onStarted(NimBLEServer *pServer){};
};

#endif // CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_GAMEPAD_H
