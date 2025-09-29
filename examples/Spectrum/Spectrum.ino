/*
  RadiaCode Spectrum Example
  
  This example demonstrates how to read and visualize spectrum data from a RadiaCode device.
*/

#include <RadiaCode.h>

// Bluetooth MAC address
const char* bluetoothMac = "52:43:06:70:24:67"; // Replace with your device's MAC address

// Create RadiaCode instance
RadiaCode* radiacode = nullptr;

// Variables for storing measurements
float countRate = 0.0f;
float doseRate = 0.0f;
String serialNum = "";

// Spectrum variables
float a0 = 0.0f, a1 = 0.0f, a2 = 0.0f;
uint32_t maxCount = 0;
uint32_t acquisitionStartTime = 0;
bool spectrumPrinted = false;

void setup(void)
{
    // Initialize serial
    Serial.begin(115200);
    while (!Serial && millis() < 5000);

    Serial.println("RadiaCode Spectrum Example");
    Serial.println("---------------------------");

    // Connect to RadiaCode device
    try
    {
        Serial.println("Connecting via Bluetooth...");
        radiacode = new RadiaCode(bluetoothMac);

        if (radiacode != nullptr)
        {
            // Get device information
            serialNum = radiacode->serialNumber();
            if (serialNum.length() > 0)
            {
                Serial.print("Connected to RadiaCode device: ");
                Serial.println(serialNum);

                // Get energy calibration coefficients
                std::vector<float> calib = radiacode->energyCalib();
                if (calib.size() >= 3)
                {
                    a0 = calib[0];
                    a1 = calib[1];
                    a2 = calib[2];

                    Serial.printf("Energy calibration coefficients: a0= %f, a1= %f, a2= %f\n", a0, a1, a2);
                }

                Serial.print("Detected spectrum format version: ");
                Serial.println(radiacode->getSpectrumFormatVersion());

                Serial.println("Resetting spectrum data...");
                radiacode->spectrumReset();
                acquisitionStartTime = millis();

                Serial.println("---------------------------");
                Serial.println("Starting spectrum acquisition. Wait at least 30 seconds, then press any key to stop.");
            }
            else
            {
                Serial.println("Failed to get device serial number - connection failed!");
                delete radiacode;
                radiacode = nullptr;
            }
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
        delay(5000);
        return;
    }

    // Check if a key was pressed
    if (Serial.available() && !spectrumPrinted)
    {
        // Clear the entire input buffer
        while (Serial.available())
        {
            Serial.read();
        }

        uint32_t acquisitionTime = (millis() - acquisitionStartTime) / 1000;
        Serial.print("Acquisition time: ");
        Serial.print(acquisitionTime);
        Serial.println(" seconds");

        if (acquisitionTime < 10)
        {
            Serial.println("WARNING: Very short acquisition time. Spectrum data may be unreliable.");
        }

        printSpectrum();
        spectrumPrinted = true;
        Serial.println("\nContinuing live measurements. Press any key to print spectrum again.");
        return;
    }

    // If a key was pressed after spectrum was printed, reset and print spectrum again
    if (Serial.available() && spectrumPrinted)
    {
        // Clear the entire input buffer
        while (Serial.available())
        {
            Serial.read();
        }

        uint32_t acquisitionTime = (millis() - acquisitionStartTime) / 1000;
        Serial.print("Acquisition time: ");
        Serial.print(acquisitionTime);
        Serial.println(" seconds");

        printSpectrum();
        Serial.println("\nContinuing live measurements. Press any key to print spectrum again.");
        return;
    }

    // Read data buffer to get current measurements
    try
    {
        std::vector<DataItem*> data = radiacode->dataBuf();

        // Process data
        for (DataItem* item : data)
        {
            // Check for real-time data using the type field instead of dynamic_cast
            if (item->type == TYPE_REAL_TIME_DATA)
            {
                RealTimeData* rtData = static_cast<RealTimeData*>(item);
                countRate = rtData->count_rate;
                doseRate = rtData->dose_rate;
            }
        }

        // Clean up data objects
        for (DataItem* item : data)
        {
            delete item;
        }
        data.clear();
    }
    catch (...)
    {
        Serial.println("Error reading data from device");
    }

    // Display current measurements with acquisition time
    uint32_t acquisitionTime = (millis() - acquisitionStartTime) / 1000;
    Serial.print("Time: ");
    Serial.print(acquisitionTime);
    Serial.print("s, Count rate: ");
    Serial.print(countRate);
    Serial.print(" CPS, Dose rate: ");
    Serial.print(doseRate * 10000.0f);
    Serial.println(" µSv/h");

    // Wait before next reading
    delay(1000);
}

void printSpectrum(void)
{
    Serial.println("\n\n---------------------------");
    Serial.println("Spectrum Acquisition Complete");
    Serial.println("---------------------------");

    try
    {
        // Get spectrum
        //Spectrum spectrum = radiacode->spectrumAccum();
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

        // Print table header
        Serial.println("\nChannel\tEnergy (keV)\tCounts\tGraph");
        Serial.println("--------------------------------------");

        // Print spectrum data (every 10th channel to keep output manageable)
        for (int i = 0; i < channelCount; i += 10)
        {
            // Use the existing spectrumChannelToEnergy function from the library
            float energy = spectrumChannelToEnergy(i, a0, a1, a2);
            uint32_t counts = spectrum.at(i);

            Serial.print(i);
            Serial.print("\t");
            Serial.print(energy, 2);
            Serial.print("\t\t");
            Serial.print(counts);
            Serial.print("\t");

            // Print simple ASCII graph
            int barLength = (maxCount > 0) ? (counts * 250) / maxCount : 0;
            for (int j = 0; j < barLength; j++)
            {
                Serial.print("#");
            }
            Serial.println();
        }
    }
    catch (...)
    {
        Serial.println("Error: Failed to read spectrum data from device");
        Serial.println("Please check device connection and try again");
    }
}
