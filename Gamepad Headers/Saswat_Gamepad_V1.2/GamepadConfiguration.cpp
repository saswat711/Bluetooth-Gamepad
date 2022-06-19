#include "GamepadConfiguration.h"

GamepadConfiguration::GamepadConfiguration() : _controllerType(CONTROLLER_TYPE_GAMEPAD),
                                                     _autoReport(true),
                                                     _hidReportId(3),
                                                     _buttonCount(16),
                                                     _hatSwitchCount(1),
                                                     _whichSpecialButtons{true, true, false, false, false, false, false, false},
                                                     _whichAxes{true, true, true, true, true, true, true, true},
                                                     _whichSimulationControls{false, false, false, false, false},
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

uint8_t GamepadConfiguration::getSimulationCount()
{
    int count = 0;
    for (int i = 0; i < POSSIBLESIMULATIONCONTROLS; i++)
    {
        count += (int)_whichSimulationControls[i];
    }

    return count;
}

uint16_t GamepadConfiguration::getVid(){ return _vid; }
uint16_t GamepadConfiguration::getPid(){ return _pid; }
uint8_t GamepadConfiguration::getControllerType() { return _controllerType; }
uint8_t GamepadConfiguration::getHidReportId() { return _hidReportId; }
uint16_t GamepadConfiguration::getButtonCount() { return _buttonCount; }
uint8_t GamepadConfiguration::getHatSwitchCount() { return _hatSwitchCount; }
bool GamepadConfiguration::getAutoReport() { return _autoReport; }
bool GamepadConfiguration::getIncludeStart() { return _whichSpecialButtons[START_BUTTON]; }
bool GamepadConfiguration::getIncludeSelect() { return _whichSpecialButtons[SELECT_BUTTON]; }
bool GamepadConfiguration::getIncludeMenu() { return _whichSpecialButtons[MENU_BUTTON]; }
bool GamepadConfiguration::getIncludeHome() { return _whichSpecialButtons[HOME_BUTTON]; }
bool GamepadConfiguration::getIncludeBack() { return _whichSpecialButtons[BACK_BUTTON]; }
bool GamepadConfiguration::getIncludeVolumeInc() { return _whichSpecialButtons[VOLUME_INC_BUTTON]; }
bool GamepadConfiguration::getIncludeVolumeDec() { return _whichSpecialButtons[VOLUME_DEC_BUTTON]; }
bool GamepadConfiguration::getIncludeVolumeMute() { return _whichSpecialButtons[VOLUME_MUTE_BUTTON]; }
const bool *GamepadConfiguration::getWhichSpecialButtons() const { return _whichSpecialButtons; }
bool GamepadConfiguration::getIncludeXAxis() { return _whichAxes[X_AXIS]; }
bool GamepadConfiguration::getIncludeYAxis() { return _whichAxes[Y_AXIS]; }
bool GamepadConfiguration::getIncludeZAxis() { return _whichAxes[Z_AXIS]; }
bool GamepadConfiguration::getIncludeRxAxis() { return _whichAxes[RX_AXIS]; }
bool GamepadConfiguration::getIncludeRyAxis() { return _whichAxes[RY_AXIS]; }
bool GamepadConfiguration::getIncludeRzAxis() { return _whichAxes[RZ_AXIS]; }
bool GamepadConfiguration::getIncludeSlider1() { return _whichAxes[SLIDER1]; }
bool GamepadConfiguration::getIncludeSlider2() { return _whichAxes[SLIDER2]; }
const bool *GamepadConfiguration::getWhichAxes() const { return _whichAxes; }
bool GamepadConfiguration::getIncludeRudder() { return _whichSimulationControls[RUDDER]; }
bool GamepadConfiguration::getIncludeThrottle() { return _whichSimulationControls[THROTTLE]; }
bool GamepadConfiguration::getIncludeAccelerator() { return _whichSimulationControls[ACCELERATOR]; }
bool GamepadConfiguration::getIncludeBrake() { return _whichSimulationControls[BRAKE]; }
bool GamepadConfiguration::getIncludeSteering() { return _whichSimulationControls[STEERING]; }
const bool *GamepadConfiguration::getWhichSimulationControls() const { return _whichSimulationControls; }

void GamepadConfiguration::setWhichSpecialButtons(bool start, bool select, bool menu, bool home, bool back, bool volumeInc, bool volumeDec, bool volumeMute)
{
    _whichSpecialButtons[START_BUTTON] = start;
    _whichSpecialButtons[SELECT_BUTTON] = select;
    _whichSpecialButtons[MENU_BUTTON] = menu;
    _whichSpecialButtons[HOME_BUTTON] = home;
    _whichSpecialButtons[BACK_BUTTON] = back;
    _whichSpecialButtons[VOLUME_INC_BUTTON] = volumeInc;
    _whichSpecialButtons[VOLUME_DEC_BUTTON] = volumeDec;
    _whichSpecialButtons[VOLUME_MUTE_BUTTON] = volumeMute;
}

void GamepadConfiguration::setWhichAxes(bool xAxis, bool yAxis, bool zAxis, bool rxAxis, bool ryAxis, bool rzAxis, bool slider1, bool slider2)
{
    _whichAxes[X_AXIS] = xAxis;
    _whichAxes[Y_AXIS] = yAxis;
    _whichAxes[Z_AXIS] = zAxis;
    _whichAxes[RX_AXIS] = rxAxis;
    _whichAxes[RY_AXIS] = ryAxis;
    _whichAxes[RZ_AXIS] = rzAxis;
}

void GamepadConfiguration::setWhichSimulationControls(bool rudder, bool throttle, bool accelerator, bool brake, bool steering)
{
    _whichSimulationControls[RUDDER] = rudder;
    _whichSimulationControls[THROTTLE] = throttle;
    _whichSimulationControls[ACCELERATOR] = accelerator;
    _whichSimulationControls[BRAKE] = brake;
    _whichSimulationControls[STEERING] = steering;
}

void GamepadConfiguration::setControllerType(uint8_t value) { _controllerType = value; }
void GamepadConfiguration::setHidReportId(uint8_t value) { _hidReportId = value; }
void GamepadConfiguration::setButtonCount(uint16_t value) { _buttonCount = value; }
void GamepadConfiguration::setHatSwitchCount(uint8_t value) { _hatSwitchCount = value; }
void GamepadConfiguration::setAutoReport(bool value) { _autoReport = value; }
void GamepadConfiguration::setIncludeStart(bool value) { _whichSpecialButtons[START_BUTTON] = value; }
void GamepadConfiguration::setIncludeSelect(bool value) { _whichSpecialButtons[SELECT_BUTTON] = value; }
void GamepadConfiguration::setIncludeMenu(bool value) { _whichSpecialButtons[MENU_BUTTON] = value; }
void GamepadConfiguration::setIncludeHome(bool value) { _whichSpecialButtons[HOME_BUTTON] = value; }
void GamepadConfiguration::setIncludeBack(bool value) { _whichSpecialButtons[BACK_BUTTON] = value; }
void GamepadConfiguration::setIncludeVolumeInc(bool value) { _whichSpecialButtons[VOLUME_INC_BUTTON] = value; }
void GamepadConfiguration::setIncludeVolumeDec(bool value) { _whichSpecialButtons[VOLUME_DEC_BUTTON] = value; }
void GamepadConfiguration::setIncludeVolumeMute(bool value) { _whichSpecialButtons[VOLUME_MUTE_BUTTON] = value; }
void GamepadConfiguration::setIncludeXAxis(bool value) { _whichAxes[X_AXIS] = value; }
void GamepadConfiguration::setIncludeYAxis(bool value) { _whichAxes[Y_AXIS] = value; }
void GamepadConfiguration::setIncludeZAxis(bool value) { _whichAxes[Z_AXIS] = value; }
void GamepadConfiguration::setIncludeRxAxis(bool value) { _whichAxes[RX_AXIS] = value; }
void GamepadConfiguration::setIncludeRyAxis(bool value) { _whichAxes[RY_AXIS] = value; }
void GamepadConfiguration::setIncludeRzAxis(bool value) { _whichAxes[RZ_AXIS] = value; }
void GamepadConfiguration::setIncludeSlider1(bool value) { _whichAxes[SLIDER1] = value; }
void GamepadConfiguration::setIncludeSlider2(bool value) { _whichAxes[SLIDER2] = value; }
void GamepadConfiguration::setIncludeRudder(bool value) { _whichSimulationControls[RUDDER] = value; }
void GamepadConfiguration::setIncludeThrottle(bool value) { _whichSimulationControls[THROTTLE] = value; }
void GamepadConfiguration::setIncludeAccelerator(bool value) { _whichSimulationControls[ACCELERATOR] = value; }
void GamepadConfiguration::setIncludeBrake(bool value) { _whichSimulationControls[BRAKE] = value; }
void GamepadConfiguration::setIncludeSteering(bool value) { _whichSimulationControls[STEERING] = value; }
void GamepadConfiguration::setVid(uint16_t value) { _vid = value; }
void GamepadConfiguration::setPid(uint16_t value) { _pid = value; }