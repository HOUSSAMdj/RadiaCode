/*
  RadiaCode API Test
  
  This example tests all APIs of a RadiaCode device over Bluetooth.
*/

#include <RadiaCode.h>

// Bluetooth MAC address
const char* bluetoothMac = "52:43:06:70:24:67"; // Replace with your device's MAC address

// Create RadiaCode instance
RadiaCode* radiacode = nullptr;

void test_BLEconnection(void)
{
    Serial.println("------------------------");
    Serial.println("Testing BLE connection...");
    Serial.println("Connecting via Bluetooth...");
    Serial.println("Setting internally the local time and resetting the timestamp ...");
    radiacode = new RadiaCode(bluetoothMac);
}

void test_serialNumber(void)
{
    String serialNum;

    serialNum = radiacode->serialNumber();

    Serial.println("------------------------");
    Serial.println("Testing serialNumber() method...");
    Serial.print("Connected to RadiaCode device: ");
    Serial.println(serialNum);
}

void test_fwVersion(void)
{
    int boot_major, boot_minor, target_major, target_minor;
    String boot_date, target_date;

    Serial.println("------------------------");
    Serial.println("Testing fwVersion() method...");

    auto version = radiacode->fwVersion();
    boot_major = std::get<0>(version);
    boot_minor = std::get<1>(version);
    boot_date = std::get<2>(version);
    target_major = std::get<3>(version);
    target_minor = std::get<4>(version);
    target_date = std::get<5>(version);

    Serial.print("Bootloader version: ");
    Serial.print(boot_major);
    Serial.print(".");
    Serial.printf("%02d", boot_minor);
    Serial.print(" (");
    Serial.print(boot_date);
    Serial.println(")");

    Serial.print("Firmware version: ");
    Serial.print(target_major);
    Serial.print(".");
    Serial.printf("%02d", target_minor);
    Serial.print(" (");
    Serial.print(target_date);
    Serial.println(")");
}

void test_deviceStatus(void)
{
    uint32_t deviceStatus;

    Serial.println("------------------------");
    Serial.println("Testing deviceStatus() method...");
    deviceStatus = radiacode->deviceStatus();
    Serial.print("Device status flags: ");
    Serial.println(deviceStatus);
}

void test_energyCalib(void)
{
#if 0
    std::vector<float> calib;
    calib = radiacode->energyCalib();
#else
    auto calib = radiacode->energyCalib();
#endif

    Serial.println("------------------------");
    Serial.println("Testing energyCalib() method...");
    if (calib.size() >= 3)
    {
        Serial.println("Energy calibration coefficients:");
        Serial.printf("a0: %.6f a1: %.6f a2: %.6f\n", calib[0], calib[1], calib[2]);
    }
}

void test_setEnergyCalib(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setEnergyCalib() method...");
    radiacode->setEnergyCalib(1.352235f, 2.381880f, 0.000348f);
}

void test_dataBuf(void)
{
    static float countRate = 0.0f;
    static float doseRate = 0.0f;
    static float temperature = 0.0f;
    static float batteryLevel = 0.0f;

    int realTimeCount = 0, rareDataCount = 0, rawDataCount = 0, otherCount = 0;
#if 0
    std::vector<DataItem*> data;
    data = radiacode->dataBuf();
#else
    auto data = radiacode->dataBuf();
#endif

    Serial.println("------------------------");
    Serial.println("Testing dataBuf() method...");
    Serial.print("Data buffer size: ");
    Serial.println(data.size());

    // Process data
    for (size_t i = 0; i < data.size(); i++)
    {
        DataItem* item = data[i];
        if (item != nullptr)
        {
            switch (item->type)
            {
                case TYPE_REAL_TIME_DATA:
                {
                    RealTimeData* rtData = (RealTimeData*)item;
                    if (rtData != nullptr)
                    {
                        countRate = rtData->count_rate;
                        doseRate = rtData->dose_rate;
                        realTimeCount++;
                    }
                    break;
                }
                case TYPE_RAW_DATA:
                {
                    RawData* rawData = (RawData*)item;
                    if (rawData != nullptr)
                    {
                        // Use dose rate from raw data if real-time data doesn't have it
                        if (doseRate == 0.0f && rawData->dose_rate > 0.0f)
                        {
                            doseRate = rawData->dose_rate;
                        }
                        rawDataCount++;
                    }
                    break;
                }
                case TYPE_RARE_DATA:
                {
                    RareData* rareData = (RareData*)item;
                    if (rareData != nullptr)
                    {
                        temperature = rareData->temperature;
                        batteryLevel = rareData->charge_level;
                        rareDataCount++;
                    }
                    break;
                }
                case TYPE_DOSE_RATE_DB:
                default:
                    otherCount++;
                    break;
            }
        }
    }

    // Clean up data objects
    for (size_t i = 0; i < data.size(); i++)
    {
        if (data[i] != nullptr)
        {
            delete data[i];
            data[i] = nullptr;
        }
    }
    data.clear();

    Serial.print("Count rate: ");
    Serial.print(countRate);
    Serial.println(" CPS");
    
    Serial.print("Dose rate: ");
    Serial.print(doseRate * 10000.0f);
    Serial.println(" µSv/h");
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.print("Battery level: ");
    Serial.print(batteryLevel);
    Serial.println("%");
}

void test_getTemperature(void)
{
    float temperature;
    
    Serial.println("------------------------");
    Serial.println("Testing getTemperature() method...");
    temperature = radiacode->getTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
}

void test_fwSignature(void)
{
    String signature;

    Serial.println("------------------------");
    Serial.println("Testing fwSignature() method...");
    signature = radiacode->fwSignature();
    Serial.println(signature);
}

void test_hwSerialNumber(void)
{
    String SerialNumber;

    Serial.println("------------------------");
    Serial.println("Testing hwSerialNumber() method...");
    Serial.print("Serial Number: ");
    SerialNumber = radiacode->hwSerialNumber();
    Serial.println(SerialNumber);
}

void test_textMessage(void)
{
    String textMessage;

    Serial.println("------------------------");
    Serial.println("Testing textMessage() method...");
    Serial.print("Text Message: ");
    textMessage = radiacode->textMessage();
    Serial.println(textMessage);
}

void test_commands(void)
{
    String commands;

    Serial.println("------------------------");
    Serial.println("Testing commands() method...");
    Serial.print("SFR file content: ");
    commands = radiacode->commands();
    Serial.println(commands);
}

void test_getSpectrumFormatVersion(void)
{
    uint8_t version = radiacode->getSpectrumFormatVersion();
    Serial.println("------------------------");
    Serial.println("Testing getSpectrumFormatVersion() method...");
    Serial.print("Detected spectrum format version: ");
    Serial.println(version);
}

void test_spectrumReset(void)
{
    Serial.println("------------------------");
    Serial.println("Testing spectrumReset() method...");
    radiacode->spectrumReset();
    Serial.println("Spectrum data reset.");
}

void test_spectrum(void)
{
    Serial.println("------------------------");
    Serial.println("Testing spectrum() method...");

    Spectrum spectrum = radiacode->spectrum();

    int channelCount = spectrum.size();
    if (channelCount == 0)
    {
        Serial.println("Error: No spectrum data available");
        return;
    }

    Serial.print("Duration: ");
    Serial.print(spectrum.duration_sec);
    Serial.println(" seconds");

    Serial.print("Channels: ");
    Serial.println(channelCount);

    // Find maximum count for scaling
    uint32_t maxCount = 0;
    for (int i = 0; i < channelCount; i++)
    {
        uint32_t count = spectrum.at(i);
        if (count > maxCount)
        {
            maxCount = count;
        }
    }

    Serial.print("Maximum count: ");
    Serial.println(maxCount);
}

void test_spectrumAccum(void)
{
    Serial.println("------------------------");
    Serial.println("Testing spectrumAccum() method...");

    Spectrum spectrum = radiacode->spectrumAccum();

    int channelCount = spectrum.size();
    if (channelCount == 0)
    {
        Serial.println("Error: No spectrum data available");
        return;
    }

    Serial.print("Duration: ");
    Serial.print(spectrum.duration_sec);
    Serial.println(" seconds");

    Serial.print("Channels: ");
    Serial.println(channelCount);

    // Find maximum count for scaling
    uint32_t maxCount = 0;
    for (int i = 0; i < channelCount; i++)
    {
        uint32_t count = spectrum.at(i);
        if (count > maxCount)
        {
            maxCount = count;
        }
    }

    Serial.print("Maximum count: ");
    Serial.println(maxCount);
}

void test_doseReset(void)
{
    Serial.println("------------------------");
    Serial.println("Testing doseReset() method...");
    radiacode->doseReset();
    Serial.println("Dose reset.");
}

void test_setLanguage(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setLanguage() method...");
    radiacode->setLanguage("en");
    Serial.println("Language set to English.");
}

void test_setDeviceOn(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setDeviceOn() method...");
    radiacode->setDeviceOn(false);
    Serial.println("Device turned off.");
}

void test_setSoundOn(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setSoundOn() method...");
    // Sound On/Off (main switch)
    radiacode->setSoundOn(true);
    Serial.println("Sound enabled.");
}

void test_setVibroOn(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setVibroOn() method...");
    // Vibro On/Off (main switch)
    radiacode->setVibroOn(true);
    Serial.println("Vibration enabled.");
}

void test_setSoundCtrl(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setSoundCtrl() method...");

    radiacode->setSoundCtrl((CTRL)(CTRL::BUTTONS));
    //radiacode->setSoundCtrl((CTRL)(CTRL::BUTTONS | CTRL::CLICKS));
    Serial.println("Sound control flags set.");
}

void test_setVibroCtrl(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setVibroCtrl() method...");

    radiacode->setVibroCtrl((CTRL)(CTRL::BUTTONS));
    Serial.println("Vibration control flags set.");
}

void test_setDisplayOffTime(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setDisplayOffTime() method...");

    radiacode->setDisplayOffTime(10);
    Serial.println("Display off time set to 10 seconds.");
}

void test_setDisplayBrightness(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setDisplayBrightness() method...");

    radiacode->setDisplayBrightness(5);
    Serial.println("Display brightness set to 5.");
}

void test_setDisplayDirection(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setDisplayDirection() method...");

    radiacode->setDisplayDirection(DisplayDirection::AUTO);
    Serial.println("Display direction set to AUTO.");
}

void test_getAlarmLimits(void)
{
    Serial.println("------------------------");
    Serial.println("Testing getAlarmLimits() method...");

#if 0
    AlarmLimits limits;
    limits = radiacode->getAlarmLimits();
#else
    auto limits = radiacode->getAlarmLimits();
#endif

    Serial.print("l1_count_rate: ");
    Serial.println(limits.l1_count_rate);
    Serial.print("l2_count_rate: ");
    Serial.println(limits.l2_count_rate);
    Serial.print("count_unit: ");
    Serial.println(limits.count_unit);
    Serial.print("l1_dose_rate: ");
    Serial.println(limits.l1_dose_rate);
    Serial.print("l2_dose_rate: ");
    Serial.println(limits.l2_dose_rate);
    Serial.print("l1_dose: ");
    Serial.println(limits.l1_dose);
    Serial.print("l2_dose: ");
    Serial.println(limits.l2_dose);
    Serial.print("dose_unit: ");
    Serial.println(limits.dose_unit);
}

void test_setAlarmLimits(void)
{
    bool result;

    Serial.println("------------------------");
    Serial.println("Testing setAlarmLimits() method...");

    result = radiacode->setAlarmLimits(20.0f, 60.0f, 0.4f, 1.2f, 9.99f, 9.99f, true, false);
    if (!result)
    {
        Serial.println("Failed to set alarm limits!");
        return;
    }
    Serial.println("Alarm limits set.");
}

void test_setLightOn(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setLightOn() method...");

    radiacode->setLightOn(true);
    Serial.println("Light enabled.");
}

void test_setDeviceCtrl(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setDeviceCtrl() method...");

    radiacode->setDeviceCtrl((DEV_CTRL)(DEV_CTRL::PWR | DEV_CTRL::LIGHT | DEV_CTRL::SOUND));
    Serial.println("Power, Light, Sound enabled, Vibration disabled.");
}

void test_setAlarmSignalMode(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setAlarmSignalMode() method...");

    // Set alarm signal mode to once
    radiacode->setAlarmSignalMode(AlarmSignalMode::ONCE);
    Serial.println("Alarm signal mode set to once.");
}

void test_setMeasurementUnit(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setMeasurementUnit() method...");

    radiacode->setMeasurementUnit(MeasurementUnits::SIEVERT);
    Serial.println("Measurement unit set to Sievert.");
}

void test_setCountRateUnit(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setCountRateUnit() method...");

    radiacode->setCountRateUnit(CountRateUnits::CPS);
    Serial.println("Count rate unit set to CPS.");
}

void test_setTemperatureUnit(void)
{
    Serial.println("------------------------");
    Serial.println("Testing setTemperatureUnit() method...");

    radiacode->setTemperatureUnit(TemperatureUnits::CELSIUS);
    Serial.println("Temperature unit set to Celsius.");
}

void test_displayControl(void)
{
    Serial.println("------------------------");
    Serial.println("Testing displayControl() method...");

    radiacode->setDisplayCtrl(DISPLAY_CTRL::BACKLT_ON_AUTO);
    Serial.println("Display control set to BACKLT_ON_AUTO.");
}

void setup(void)
{
    // Initialize serial
    Serial.begin(115200);
    while (!Serial && millis() < 5000);

    Serial.printf("RadiaCode API Test Project, Driver Version: %s\n", getDriverVersion());

    // Connect to RadiaCode device
    try
    {
        test_BLEconnection();

        if (radiacode != nullptr)
        {
            test_serialNumber();

            test_fwVersion();

            //test_setEnergyCalib();

            test_energyCalib();

            test_deviceStatus();

            test_dataBuf();

            test_fwSignature();

            test_hwSerialNumber();

            test_textMessage();

            test_commands();

            test_getSpectrumFormatVersion();

            test_spectrumReset();

            test_doseReset();

            //test_setLanguage();

            //test_setDeviceOn();

            //test_setSoundOn();

            //test_setVibroOn();

            //test_setLightOn();

            //test_setSoundCtrl();

            //test_setVibroCtrl();

            //test_setDeviceCtrl();

            //test_setDisplayOffTime();

            //test_setDisplayBrightness();

            //test_setDisplayDirection();

            //test_setAlarmSignalMode();

            //test_setAlarmLimits();

            //test_setMeasurementUnit();

            //test_setCountRateUnit();

            //test_setTemperatureUnit();

            //test_displayControl();

            test_getAlarmLimits();
        }
        else
        {
            Serial.println("Failed to create RadiaCode instance!");
        }
    }
    catch (...)
    {
        Serial.println("Exception during RadiaCode initialization!");
        if (radiacode != nullptr)
        {
            delete radiacode;
            radiacode = nullptr;
        }
    }
}

void loop(void)
{
    if (radiacode == nullptr)
    {
        Serial.println("No device connected.");
    }
    else
    {
        try
        {
            test_dataBuf();
             
            test_getTemperature();

            test_spectrum();

            //test_spectrumAccum();
        }
        catch (...)
        {
            Serial.println("Error reading data from device - restarting...");
            delay(1000);
            ESP.restart();
        }
    }

    // Wait 2 seconds before next cycle
    delay(2000);
}
