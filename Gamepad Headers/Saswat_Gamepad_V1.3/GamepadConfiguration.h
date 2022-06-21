#ifndef GAMEPAD_CONFIG_H
#define GAMEPAD_CONFIG_H

#define POSSIBLESPECIALBUTTONS 8
#define POSSIBLEAXES 8
#define POSSIBLESIMULATIONCONTROLS 5

#include <Arduino.h>

#define CONTROLLER_TYPE_JOYSTICK 0x04
#define CONTROLLER_TYPE_GAMEPAD 0x05
#define CONTROLLER_TYPE_MULTI_AXIS 0x08

#define BUTTON_1 0x1
#define BUTTON_2 0x2
#define BUTTON_3 0x3
#define BUTTON_4 0x4
#define BUTTON_5 0x5
#define BUTTON_6 0x6
#define BUTTON_7 0x7
#define BUTTON_8 0x8

#define BUTTON_9 0x9
#define BUTTON_10 0xa
#define BUTTON_11 0xb
#define BUTTON_12 0xc
#define BUTTON_13 0xd
#define BUTTON_14 0xe
#define BUTTON_15 0xf
#define BUTTON_16 0x10

#define BUTTON_17 0x11
#define BUTTON_18 0x12
#define BUTTON_19 0x13
#define BUTTON_20 0x14
#define BUTTON_21 0x15
#define BUTTON_22 0x16
#define BUTTON_23 0x17
#define BUTTON_24 0x18

#define BUTTON_25 0x19
#define BUTTON_26 0x1a
#define BUTTON_27 0x1b
#define BUTTON_28 0x1c
#define BUTTON_29 0x1d


#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define RX_AXIS 3
#define RY_AXIS 4
#define RZ_AXIS 5

#define START_BUTTON 0
#define SELECT_BUTTON 1

class GamepadConfiguration
{
private:
    uint8_t _controllerType;
    bool _autoReport;
    uint8_t _hidReportId;
    uint16_t _buttonCount;
    bool _whichSpecialButtons[POSSIBLESPECIALBUTTONS];
    bool _whichAxes[POSSIBLEAXES];
	uint16_t _vid;
	uint16_t _pid;

public:
    GamepadConfiguration();

    bool getAutoReport();
    uint8_t getControllerType();
    uint8_t getHidReportId();
    uint16_t getButtonCount();
    uint8_t getTotalSpecialButtonCount();
    uint8_t getDesktopSpecialButtonCount();
    uint8_t getConsumerSpecialButtonCount();
    uint8_t getAxisCount();
    bool getIncludeStart();
    bool getIncludeSelect();
    const bool *getWhichSpecialButtons() const;
    bool getIncludeXAxis();
    bool getIncludeYAxis();
    bool getIncludeZAxis();
    bool getIncludeRxAxis();
    bool getIncludeRyAxis();
    bool getIncludeRzAxis();
    const bool *getWhichAxes() const;
	uint16_t getVid();
	uint16_t getPid();

    void setControllerType(uint8_t controllerType);
    void setAutoReport(bool value);
    void setHidReportId(uint8_t value);
    void setButtonCount(uint16_t value);
    void setIncludeStart(bool value);
    void setIncludeSelect(bool value);
    void setWhichSpecialButtons(bool start, bool select);
    void setIncludeXAxis(bool value);
    void setIncludeYAxis(bool value);
    void setIncludeZAxis(bool value);
    void setIncludeRxAxis(bool value);
    void setIncludeRyAxis(bool value);
    void setIncludeRzAxis(bool value);
    void setWhichAxes(bool xAxis, bool yAxis, bool zAxis, bool rxAxis, bool ryAxis, bool rzAxis);
   
	void setVid(uint16_t value);
	void setPid(uint16_t value);
};

#endif