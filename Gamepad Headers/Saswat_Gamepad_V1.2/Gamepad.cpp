#include <NimBLEDevice.h>
#include <NimBLEUtils.h>
#include <NimBLEServer.h>
#include "NimBLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

#include "GamepadConnectionStatus.h"
#include "Gamepad.h"
#include "GamepadConfiguration.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define LOG_TAG "SaswatGamepad"
#else
#include "esp_log.h"
static const char *LOG_TAG = "SaswatGamepad";
#endif

uint8_t tempHidReportDescriptor[150];
int hidReportDescriptorSize = 0;
uint8_t reportSize = 0;
uint8_t numOfButtonBytes = 0;
uint16_t vid;
uint16_t pid; 

Gamepad::Gamepad(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) : _buttons(),
                                                                                                       _specialButtons(0),
                                                                                                       _x(0),
                                                                                                       _y(0),
                                                                                                       _z(0),
                                                                                                       _rZ(0),
                                                                                                       _rX(0),
                                                                                                       _rY(0),
                                                                                                       _slider1(0),
                                                                                                       _slider2(0),
                                                                                                       _rudder(0),
                                                                                                       _throttle(0),
                                                                                                       _accelerator(0),
                                                                                                       _brake(0),
                                                                                                       _steering(0),
                                                                                                       _hat1(0),
                                                                                                       _hat2(0),
                                                                                                       _hat3(0),
                                                                                                       _hat4(0),
                                                                                                       hid(0)
{
    this->resetButtons();
    this->deviceName = deviceName;
    this->deviceManufacturer = deviceManufacturer;
    this->batteryLevel = batteryLevel;
    this->connectionStatus = new GamepadConnectionStatus();
}

void Gamepad::resetButtons()
{
    memset(&_buttons, 0, sizeof(_buttons));
}

void Gamepad::begin(GamepadConfiguration *config)
{
    configuration = *config; // we make a copy, so the user can't change actual values midway through operation, without calling the begin function again
	
	vid = configuration.getVid();
	pid = configuration.getPid();
	
	uint8_t high = highByte(vid);
	uint8_t low = lowByte(vid);
	
	vid = low << 8 | high;
	
	high = highByte(pid);
	low = lowByte(pid);
	
	pid = low << 8 | high;

    uint8_t buttonPaddingBits = 8 - (configuration.getButtonCount() % 8);
    if (buttonPaddingBits == 8)
    {
        buttonPaddingBits = 0;
    }
    uint8_t specialButtonPaddingBits = 8 - (configuration.getTotalSpecialButtonCount() % 8);
    if (specialButtonPaddingBits == 8)
    {
        specialButtonPaddingBits = 0;
    }
    uint8_t numOfAxisBytes = configuration.getAxisCount() * 2;
    uint8_t numOfSimulationBytes = configuration.getSimulationCount() * 2;

    numOfButtonBytes = configuration.getButtonCount() / 8;
    if (buttonPaddingBits > 0)
    {
        numOfButtonBytes++;
    }

    uint8_t numOfSpecialButtonBytes = configuration.getTotalSpecialButtonCount() / 8;
    if (specialButtonPaddingBits > 0)
    {
        numOfSpecialButtonBytes++;
    }

    reportSize = numOfButtonBytes + numOfSpecialButtonBytes + numOfAxisBytes + numOfSimulationBytes + configuration.getHatSwitchCount();

    // USAGE_PAGE (Generic Desktop)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE (Joystick - 0x04; Gamepad - 0x05; Multi-axis Controller - 0x08)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
    tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getControllerType();

    // COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xa1;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (Default: 3)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getHidReportId();

    if (configuration.getButtonCount() > 0)
    {

        // USAGE_PAGE (Button)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;

        // LOGICAL_MINIMUM (0)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        // LOGICAL_MAXIMUM (1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // REPORT_SIZE (1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // USAGE_MINIMUM (Button 1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // USAGE_MAXIMUM (Up to 128 buttons possible)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
        tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getButtonCount();

        // REPORT_COUNT (# of buttons)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
        tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getButtonCount();

        // INPUT (Data,Var,Abs)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

        if (buttonPaddingBits > 0)
        {

            // REPORT_SIZE (1)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

            // REPORT_COUNT (# of padding bits)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
            tempHidReportDescriptor[hidReportDescriptorSize++] = buttonPaddingBits;

            // INPUT (Const,Var,Abs)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;

        } // Padding Bits Needed

    } // Buttons

    if (configuration.getTotalSpecialButtonCount() > 0)
    {
        // LOGICAL_MINIMUM (0)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        // LOGICAL_MAXIMUM (1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // REPORT_SIZE (1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        if (configuration.getDesktopSpecialButtonCount() > 0)
        {

            // USAGE_PAGE (Generic Desktop)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

            // REPORT_COUNT
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
            tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getDesktopSpecialButtonCount();
            if (configuration.getIncludeStart())
            {
                // USAGE (Start)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x3D;
            }

            if (configuration.getIncludeSelect())
            {
                // USAGE (Select)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x3E;
            }

            if (configuration.getIncludeMenu())
            {
                // USAGE (App Menu)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x86;
            }

            // INPUT (Data,Var,Abs)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
        }

        if (configuration.getConsumerSpecialButtonCount() > 0)
        {

            // USAGE_PAGE (Consumer Page)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0C;

            // REPORT_COUNT
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
            tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getConsumerSpecialButtonCount();

            if (configuration.getIncludeHome())
            {
                // USAGE (Home)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x23;
		        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
            }

            if (configuration.getIncludeBack())
            {
                // USAGE (Back)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x0A;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x24;
	        	tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
            }

            if (configuration.getIncludeVolumeInc())
            {
                // USAGE (Volume Increment)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0xE9;
            }

            if (configuration.getIncludeVolumeDec())
            {
                // USAGE (Volume Decrement)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0xEA;
            }

            if (configuration.getIncludeVolumeMute())
            {
                // USAGE (Mute)
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
                tempHidReportDescriptor[hidReportDescriptorSize++] = 0xE2;
            }

            // INPUT (Data,Var,Abs)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;
        }

        if (specialButtonPaddingBits > 0)
        {

            // REPORT_SIZE (1)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

            // REPORT_COUNT (# of padding bits)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
            tempHidReportDescriptor[hidReportDescriptorSize++] = specialButtonPaddingBits;

            // INPUT (Const,Var,Abs)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;

        } // Padding Bits Needed

    } // Special Buttons

    if (configuration.getAxisCount() > 0)
    {
        // USAGE_PAGE (Generic Desktop)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // USAGE (Pointer)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // LOGICAL_MINIMUM (-32767)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x16;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x80;

        // LOGICAL_MAXIMUM (+32767)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFF;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x7F;

        // REPORT_SIZE (16)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x10;

        // REPORT_COUNT (configuration.getAxisCount())
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
        tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getAxisCount();

        // COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        if (configuration.getIncludeXAxis())
        {
            // USAGE (X)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x30;
        }

        if (configuration.getIncludeYAxis())
        {
            // USAGE (Y)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x31;
        }

        if (configuration.getIncludeZAxis())
        {
            // USAGE (Z)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x32;
        }

        if (configuration.getIncludeRzAxis())
        {
            // USAGE (Rz)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x35;
        }

        if (configuration.getIncludeRxAxis())
        {
            // USAGE (Rx)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x33;
        }

        if (configuration.getIncludeRyAxis())
        {
            // USAGE (Ry)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x34;
        }

        if (configuration.getIncludeSlider1())
        {
            // USAGE (Slider)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x36;
        }

        if (configuration.getIncludeSlider2())
        {
            // USAGE (Slider)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x36;
        }

        // INPUT (Data,Var,Abs)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

        // END_COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

    } // X, Y, Z, Rx, Ry, and Rz Axis

    if (configuration.getSimulationCount() > 0)
    {

        // USAGE_PAGE (Simulation Controls)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

        // LOGICAL_MINIMUM (-32767)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x16;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x80;

        // LOGICAL_MAXIMUM (+32767)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x26;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xFF;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x7F;

        // REPORT_SIZE (16)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x10;

        // REPORT_COUNT (configuration.getSimulationCount())
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
        tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getSimulationCount();

        // COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        if (configuration.getIncludeRudder())
        {
            // USAGE (Rudder)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0xBA;
        }

        if (configuration.getIncludeThrottle())
        {
            // USAGE (Throttle)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0xBB;
        }

        if (configuration.getIncludeAccelerator())
        {
            // USAGE (Accelerator)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC4;
        }

        if (configuration.getIncludeBrake())
        {
            // USAGE (Brake)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC5;
        }

        if (configuration.getIncludeSteering())
        {
            // USAGE (Steering)
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0xC8;
        }

        // INPUT (Data,Var,Abs)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

        // END_COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

    } // Simulation Controls

    if (configuration.getHatSwitchCount() > 0)
    {

        // COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xA1;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        // USAGE_PAGE (Generic Desktop)
        tempHidReportDescriptor[hidReportDescriptorSize++] = USAGE_PAGE(1);
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // USAGE (Hat Switch)
        for (int currentHatIndex = 0; currentHatIndex < configuration.getHatSwitchCount(); currentHatIndex++)
        {
            tempHidReportDescriptor[hidReportDescriptorSize++] = USAGE(1);
            tempHidReportDescriptor[hidReportDescriptorSize++] = 0x39;
        }

        // Logical Min (1)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // Logical Max (8)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

        // Physical Min (0)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x35;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

        // Physical Max (315)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x46;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x3B;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

        // Unit (SI Rot : Ang Pos)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x12;

        // Report Size (8)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x08;

        // Report Count (4)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
        tempHidReportDescriptor[hidReportDescriptorSize++] = configuration.getHatSwitchCount();

        // Input (Data, Variable, Absolute)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0x42;

        // END_COLLECTION (Physical)
        tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;
    }

    // END_COLLECTION (Application)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

    xTaskCreate(this->taskServer, "server", 20000, (void *)this, 5, NULL);
}

void Gamepad::end(void)
{
}

void Gamepad::setAxes(int16_t x, int16_t y, int16_t z, int16_t rZ, int16_t rX, int16_t rY, int16_t slider1, int16_t slider2)
{
    if (x == -32768)
    {
        x = -32767;
    }
    if (y == -32768)
    {
        y = -32767;
    }
    if (z == -32768)
    {
        z = -32767;
    }
    if (rZ == -32768)
    {
        rZ = -32767;
    }
    if (rX == -32768)
    {
        rX = -32767;
    }
    if (rY == -32768)
    {
        rY = -32767;
    }
    if (slider1 == -32768)
    {
        slider1 = -32767;
    }
    if (slider2 == -32768)
    {
        slider2 = -32767;
    }

    _x = x;
    _y = y;
    _z = z;
    _rZ = rZ;
    _rX = rX;
    _rY = rY;
    _slider1 = slider1;
    _slider2 = slider2;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSimulationControls(int16_t rudder, int16_t throttle, int16_t accelerator, int16_t brake, int16_t steering)
{
    if (rudder == -32768)
    {
        rudder = -32767;
    }
    if (throttle == -32768)
    {
        throttle = -32767;
    }
    if (accelerator == -32768)
    {
        accelerator = -32767;
    }
    if (brake == -32768)
    {
        brake = -32767;
    }
    if (steering == -32768)
    {
        steering = -32767;
    }

    _rudder = rudder;
    _throttle = throttle;
    _accelerator = accelerator;
    _brake = brake;
    _steering = steering;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHats(signed char hat1, signed char hat2, signed char hat3, signed char hat4)
{
    _hat1 = hat1;
    _hat2 = hat2;
    _hat3 = hat3;
    _hat4 = hat4;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSliders(int16_t slider1, int16_t slider2)
{
    if (slider1 == -32768)
    {
        slider1 = -32767;
    }
    if (slider2 == -32768)
    {
        slider2 = -32767;
    }

    _slider1 = slider1;
    _slider2 = slider2;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::sendReport(void)
{
    if (this->isConnected())
    {
        uint8_t currentReportIndex = 0;

        uint8_t m[reportSize];

        memset(&m, 0, sizeof(m));
        memcpy(&m, &_buttons, sizeof(_buttons));

        currentReportIndex += numOfButtonBytes;

        if (configuration.getTotalSpecialButtonCount() > 0)
        {
            m[currentReportIndex++] = _specialButtons;
        }

        if (configuration.getIncludeXAxis())
        {
            m[currentReportIndex++] = _x;
            m[currentReportIndex++] = (_x >> 8);
        }
        if (configuration.getIncludeYAxis())
        {
            m[currentReportIndex++] = _y;
            m[currentReportIndex++] = (_y >> 8);
        }
        if (configuration.getIncludeZAxis())
        {
            m[currentReportIndex++] = _z;
            m[currentReportIndex++] = (_z >> 8);
        }
        if (configuration.getIncludeRzAxis())
        {
            m[currentReportIndex++] = _rZ;
            m[currentReportIndex++] = (_rZ >> 8);
        }
        if (configuration.getIncludeRxAxis())
        {
            m[currentReportIndex++] = _rX;
            m[currentReportIndex++] = (_rX >> 8);
        }
        if (configuration.getIncludeRyAxis())
        {
            m[currentReportIndex++] = _rY;
            m[currentReportIndex++] = (_rY >> 8);
        }

        if (configuration.getIncludeSlider1())
        {
            m[currentReportIndex++] = _slider1;
            m[currentReportIndex++] = (_slider1 >> 8);
        }
        if (configuration.getIncludeSlider2())
        {
            m[currentReportIndex++] = _slider2;
            m[currentReportIndex++] = (_slider2 >> 8);
        }

        if (configuration.getIncludeRudder())
        {
            m[currentReportIndex++] = _rudder;
            m[currentReportIndex++] = (_rudder >> 8);
        }
        if (configuration.getIncludeThrottle())
        {
            m[currentReportIndex++] = _throttle;
            m[currentReportIndex++] = (_throttle >> 8);
        }
        if (configuration.getIncludeAccelerator())
        {
            m[currentReportIndex++] = _accelerator;
            m[currentReportIndex++] = (_accelerator >> 8);
        }
        if (configuration.getIncludeBrake())
        {
            m[currentReportIndex++] = _brake;
            m[currentReportIndex++] = (_brake >> 8);
        }
        if (configuration.getIncludeSteering())
        {
            m[currentReportIndex++] = _steering;
            m[currentReportIndex++] = (_steering >> 8);
        }

        if (configuration.getHatSwitchCount() > 0)
        {
            signed char hats[4];

            hats[0] = _hat1;
            hats[1] = _hat2;
            hats[2] = _hat3;
            hats[3] = _hat4;

            for (int currentHatIndex = configuration.getHatSwitchCount() - 1; currentHatIndex >= 0; currentHatIndex--)
            {
                m[currentReportIndex++] = hats[currentHatIndex];
            }
        }

        this->inputGamepad->setValue(m, sizeof(m));
        this->inputGamepad->notify();
    }
}

void Gamepad::press(uint8_t b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    uint8_t result = _buttons[index] | bitmask;

    if (result != _buttons[index])
    {
        _buttons[index] = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::release(uint8_t b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    uint64_t result = _buttons[index] & ~bitmask;

    if (result != _buttons[index])
    {
        _buttons[index] = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

uint8_t Gamepad::specialButtonBitPosition(uint8_t b)
{
    if (b >= POSSIBLESPECIALBUTTONS)
        throw std::invalid_argument("Index out of range");
    uint8_t bit = 0;
    for (int i = 0; i < b; i++)
    {
        if (configuration.getWhichSpecialButtons()[i])
            bit++;
    }
    return bit;
}

void Gamepad::pressSpecialButton(uint8_t b)
{
    uint8_t button = specialButtonBitPosition(b);
    uint8_t bit = button % 8;
    uint8_t bitmask = (1 << bit);

    uint64_t result = _specialButtons | bitmask;

    if (result != _specialButtons)
    {
        _specialButtons = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::releaseSpecialButton(uint8_t b)
{
    uint8_t button = specialButtonBitPosition(b);
    uint8_t bit = button % 8;
    uint8_t bitmask = (1 << bit);

    uint64_t result = _specialButtons & ~bitmask;

    if (result != _specialButtons)
    {
        _specialButtons = result;
    }

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::pressStart()
{
    pressSpecialButton(START_BUTTON);
}

void Gamepad::releaseStart()
{
    releaseSpecialButton(START_BUTTON);
}

void Gamepad::pressSelect()
{
    pressSpecialButton(SELECT_BUTTON);
}

void Gamepad::releaseSelect()
{
    releaseSpecialButton(SELECT_BUTTON);
}

void Gamepad::pressMenu()
{
    pressSpecialButton(MENU_BUTTON);
}

void Gamepad::releaseMenu()
{
    releaseSpecialButton(MENU_BUTTON);
}

void Gamepad::pressHome()
{
    pressSpecialButton(HOME_BUTTON);
}

void Gamepad::releaseHome()
{
    releaseSpecialButton(HOME_BUTTON);
}

void Gamepad::pressBack()
{
    pressSpecialButton(BACK_BUTTON);
}

void Gamepad::releaseBack()
{
    releaseSpecialButton(BACK_BUTTON);
}

void Gamepad::pressVolumeInc()
{
    pressSpecialButton(VOLUME_INC_BUTTON);
}

void Gamepad::releaseVolumeInc()
{
    releaseSpecialButton(VOLUME_INC_BUTTON);
}

void Gamepad::pressVolumeDec()
{
    pressSpecialButton(VOLUME_DEC_BUTTON);
}

void Gamepad::releaseVolumeDec()
{
    releaseSpecialButton(VOLUME_DEC_BUTTON);
}

void Gamepad::pressVolumeMute()
{
    pressSpecialButton(VOLUME_MUTE_BUTTON);
}

void Gamepad::releaseVolumeMute()
{
    releaseSpecialButton(VOLUME_MUTE_BUTTON);
}

void Gamepad::setLeftThumb(int16_t x, int16_t y)
{
    if (x == -32768)
    {
        x = -32767;
    }
    if (y == -32768)
    {
        y = -32767;
    }

    _x = x;
    _y = y;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}
void Gamepad::setRightThumb(int16_t z, int16_t rZ)
{
    if (z == -32768)
    {
        z = -32767;
    }
    if (rZ == -32768)
    {
        rZ = -32767;
    }

    _z = z;
    _rZ = rZ;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setLeftTrigger(int16_t rX)
{
    if (rX == -32768)
    {
        rX = -32767;
    }

    _rX = rX;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setRightTrigger(int16_t rY)
{
    if (rY == -32768)
    {
        rY = -32767;
    }

    _rY = rY;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setTriggers(int16_t rX, int16_t rY)
{
    if (rX == -32768)
    {
        rX = -32767;
    }
    if (rY == -32768)
    {
        rY = -32767;
    }

    _rX = rX;
    _rY = rY;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHat(signed char hat)
{
    _hat1 = hat;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHat1(signed char hat1)
{
    _hat1 = hat1;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHat2(signed char hat2)
{
    _hat2 = hat2;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHat3(signed char hat3)
{
    _hat3 = hat3;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setHat4(signed char hat4)
{
    _hat4 = hat4;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setX(int16_t x)
{
    if (x == -32768)
    {
        x = -32767;
    }

    _x = x;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setY(int16_t y)
{
    if (y == -32768)
    {
        y = -32767;
    }

    _y = y;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setZ(int16_t z)
{
    if (z == -32768)
    {
        z = -32767;
    }

    _z = z;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setRZ(int16_t rZ)
{
    if (rZ == -32768)
    {
        rZ = -32767;
    }

    _rZ = rZ;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setRX(int16_t rX)
{
    if (rX == -32768)
    {
        rX = -32767;
    }

    _rX = rX;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setRY(int16_t rY)
{
    if (rY == -32768)
    {
        rY = -32767;
    }

    _rY = rY;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSlider(int16_t slider)
{
    if (slider == -32768)
    {
        slider = -32767;
    }

    _slider1 = slider;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSlider1(int16_t slider1)
{
    if (slider1 == -32768)
    {
        slider1 = -32767;
    }

    _slider1 = slider1;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSlider2(int16_t slider2)
{
    if (slider2 == -32768)
    {
        slider2 = -32767;
    }

    _slider2 = slider2;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setRudder(int16_t rudder)
{
    if (rudder == -32768)
    {
        rudder = -32767;
    }

    _rudder = rudder;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setThrottle(int16_t throttle)
{
    if (throttle == -32768)
    {
        throttle = -32767;
    }

    _throttle = throttle;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setAccelerator(int16_t accelerator)
{
    if (accelerator == -32768)
    {
        accelerator = -32767;
    }

    _accelerator = accelerator;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setBrake(int16_t brake)
{
    if (brake == -32768)
    {
        brake = -32767;
    }

    _brake = brake;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

void Gamepad::setSteering(int16_t steering)
{
    if (steering == -32768)
    {
        steering = -32767;
    }

    _steering = steering;

    if (configuration.getAutoReport())
    {
        sendReport();
    }
}

bool Gamepad::isPressed(uint8_t b)
{
    uint8_t index = (b - 1) / 8;
    uint8_t bit = (b - 1) % 8;
    uint8_t bitmask = (1 << bit);

    if ((bitmask & _buttons[index]) > 0)
        return true;
    return false;
}

bool Gamepad::isConnected(void)
{
    return this->connectionStatus->connected;
}

void Gamepad::setBatteryLevel(uint8_t level)
{
    this->batteryLevel = level;
    if (hid != 0)
        this->hid->setBatteryLevel(this->batteryLevel);
}

void Gamepad::taskServer(void *pvParameter)
{
    Gamepad *GamepadInstance = (Gamepad *)pvParameter; // static_cast<Gamepad *>(pvParameter);
    NimBLEDevice::init(GamepadInstance->deviceName);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(GamepadInstance->connectionStatus);

    GamepadInstance->hid = new NimBLEHIDDevice(pServer);
    GamepadInstance->inputGamepad = GamepadInstance->hid->inputReport(GamepadInstance->configuration.getHidReportId()); // <-- input REPORTID from report map
    GamepadInstance->connectionStatus->inputGamepad = GamepadInstance->inputGamepad;

    GamepadInstance->hid->manufacturer()->setValue(GamepadInstance->deviceManufacturer);

    GamepadInstance->hid->pnp(0x01, vid, pid, 0x0110);
    GamepadInstance->hid->hidInfo(0x00, 0x01);

    NimBLESecurity *pSecurity = new NimBLESecurity();

    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    uint8_t *customHidReportDescriptor = new uint8_t[hidReportDescriptorSize];
    memcpy(customHidReportDescriptor, tempHidReportDescriptor, hidReportDescriptorSize);

    for (int i = 0; i < hidReportDescriptorSize; i++)
        Serial.printf("%02x", customHidReportDescriptor[i]);

    GamepadInstance->hid->reportMap((uint8_t *)customHidReportDescriptor, hidReportDescriptorSize);
    GamepadInstance->hid->startServices();

    GamepadInstance->onStarted(pServer);

    NimBLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_GAMEPAD);
    pAdvertising->addServiceUUID(GamepadInstance->hid->hidService()->getUUID());
    pAdvertising->start();
    GamepadInstance->hid->setBatteryLevel(GamepadInstance->batteryLevel);

    ESP_LOGD(LOG_TAG, "Advertising started!");
    vTaskDelay(portMAX_DELAY); // delay(portMAX_DELAY);
}
