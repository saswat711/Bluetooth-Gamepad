#ifndef GAMEPAD_CONNECTION_STATUS_H
#define GAMEPAD_CONNECTION_STATUS_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include <NimBLEServer.h>
#include "NimBLECharacteristic.h"

class GamepadConnectionStatus : public NimBLEServerCallbacks
{
public:
    GamepadConnectionStatus(void);
    bool connected = false;
    void onConnect(NimBLEServer *pServer);
    void onDisconnect(NimBLEServer *pServer);
    NimBLECharacteristic *inputGamepad;
};

#endif // CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_CONNECTION_STATUS_H
