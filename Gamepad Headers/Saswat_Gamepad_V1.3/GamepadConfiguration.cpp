#include "GamepadConfiguration.h"

GamepadConfiguration::GamepadConfiguration() : _controllerType(CONTROLLER_TYPE_GAMEPAD),
                                                     _autoReport(true),
                                                     _hidReportId(3),
                                                     _buttonCount(16),
                                                     _whichSpecialButtons{true, true},
                                                     _whichAxes{true, true, true, true, true, true, true, true},
													 _vid(0xe502),
													 _pid(0xbbab)
{
}

uint8_t GamepadConfiguration::getTotalSpecialButtonCount()
{
    int count = 0;
    for (int i = 0; i < POSSIBLESPECIALBUTTONS; i++)
    {
        count += (int)_whichSpecialButtons[i];
    }

    return count;
}

uint8_t GamepadConfiguration::getDesktopSpecialButtonCount()
{
    int count = 0;
    for (int i = 0; i < 3; i++)
    {
        count += (int)_whichSpecialButtons[i];
    }

    return count;
}

uint8_t GamepadConfiguration::getConsumerSpecialButtonCount()
{
    int count = 0;
    for (int i = 3; i < 8; i++)
    {
        count += (int)_whichSpecialButtons[i];
    }

    return count;
}

uint8_t GamepadConfiguration::getAxisCount()
{
    int count = 0;
    for (int i = 0; i < POSSIBLEAXES; i++)
    {
        count += (int)_whichAxes[i];
    }

    return count;
}


uint16_t GamepadConfiguration::getVid(){ return _vid; }
uint16_t GamepadConfiguration::getPid(){ return _pid; }
uint8_t GamepadConfiguration::getControllerType() { return _controllerType; }
uint8_t GamepadConfiguration::getHidReportId() { return _hidReportId; }
uint16_t GamepadConfiguration::getButtonCount() { return _buttonCount; }
bool GamepadConfiguration::getAutoReport() { return _autoReport; }
bool GamepadConfiguration::getIncludeStart() { return _whichSpecialButtons[START_BUTTON]; }
bool GamepadConfiguration::getIncludeSelect() { return _whichSpecialButtons[SELECT_BUTTON]; }
const bool *GamepadConfiguration::getWhichSpecialButtons() const { return _whichSpecialButtons; }
bool GamepadConfiguration::getIncludeXAxis() { return _whichAxes[X_AXIS]; }
bool GamepadConfiguration::getIncludeYAxis() { return _whichAxes[Y_AXIS]; }
bool GamepadConfiguration::getIncludeZAxis() { return _whichAxes[Z_AXIS]; }
bool GamepadConfiguration::getIncludeRxAxis() { return _whichAxes[RX_AXIS]; }
bool GamepadConfiguration::getIncludeRyAxis() { return _whichAxes[RY_AXIS]; }
bool GamepadConfiguration::getIncludeRzAxis() { return _whichAxes[RZ_AXIS]; }
const bool *GamepadConfiguration::getWhichAxes() const { return _whichAxes; }

void GamepadConfiguration::setWhichSpecialButtons(bool start, bool select)
{
    _whichSpecialButtons[START_BUTTON] = start;
    _whichSpecialButtons[SELECT_BUTTON] = select;
}

void GamepadConfiguration::setWhichAxes(bool xAxis, bool yAxis, bool zAxis, bool rxAxis, bool ryAxis, bool rzAxis)
{
    _whichAxes[X_AXIS] = xAxis;
    _whichAxes[Y_AXIS] = yAxis;
    _whichAxes[Z_AXIS] = zAxis;
    _whichAxes[RX_AXIS] = rxAxis;
    _whichAxes[RY_AXIS] = ryAxis;
    _whichAxes[RZ_AXIS] = rzAxis;
}



void GamepadConfiguration::setControllerType(uint8_t value) { _controllerType = value; }
void GamepadConfiguration::setHidReportId(uint8_t value) { _hidReportId = value; }
void GamepadConfiguration::setButtonCount(uint16_t value) { _buttonCount = value; }
void GamepadConfiguration::setAutoReport(bool value) { _autoReport = value; }
void GamepadConfiguration::setIncludeStart(bool value) { _whichSpecialButtons[START_BUTTON] = value; }
void GamepadConfiguration::setIncludeSelect(bool value) { _whichSpecialButtons[SELECT_BUTTON] = value; }
void GamepadConfiguration::setIncludeXAxis(bool value) { _whichAxes[X_AXIS] = value; }
void GamepadConfiguration::setIncludeYAxis(bool value) { _whichAxes[Y_AXIS] = value; }
void GamepadConfiguration::setIncludeZAxis(bool value) { _whichAxes[Z_AXIS] = value; }
void GamepadConfiguration::setIncludeRxAxis(bool value) { _whichAxes[RX_AXIS] = value; }
void GamepadConfiguration::setIncludeRyAxis(bool value) { _whichAxes[RY_AXIS] = value; }
void GamepadConfiguration::setIncludeRzAxis(bool value) { _whichAxes[RZ_AXIS] = value; }
void GamepadConfiguration::setVid(uint16_t value) { _vid = value; }
void GamepadConfiguration::setPid(uint16_t value) { _pid = value; }