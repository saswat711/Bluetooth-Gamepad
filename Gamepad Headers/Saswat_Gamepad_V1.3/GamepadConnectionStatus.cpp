#include "GamepadConnectionStatus.h"

GamepadConnectionStatus::GamepadConnectionStatus(void)
{
}

void GamepadConnectionStatus::onConnect(NimBLEServer *pServer)
{
    this->connected = true;
}

void GamepadConnectionStatus::onDisconnect(NimBLEServer *pServer)
{
    this->connected = false;
}
