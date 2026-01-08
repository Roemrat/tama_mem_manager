# Tama Mem Manager

A Flipper Zero application for managing Tamagotchi SMA card (Smart Card) memory chips.

## Description

Tama Mem Manager is a Flipper Zero application designed to manage Tamagotchi SMA card (Smart Card) memory. The application provides a user-friendly interface for reading, writing, erasing, and managing memory data stored on Tamagotchi SMA cards using 25-series SPI flash memory chips.

## Features

- **Read Memory**: Read the entire contents of a memory chip and save it to a file
- **Write Memory**: Write saved memory dumps back to a chip
- **Erase Memory**: Erase the entire memory chip
- **Protect/Unprotect**: Manage write protection on memory chips
- **Tama Reset**: Reset Tamagotchi device memory with validation
- **File Management**: Save, load, and manage memory dump files (.bin format)
- **Chip Detection**: Automatic detection of supported memory chips
- **Verification**: Verify written data matches the source file
- **Chip Information**: Display detailed information about detected chips

## Requirements

- Flipper Zero device
- Flipper Zero firmware with FAP support
- Tamagotchi SMA card with 25-series SPI memory chip connected via GPIO pins
- Proper wiring between Flipper Zero and the SMA card

## Installation

1. Clone this repository or download the source code
2. Build the application using the Flipper Zero build system:
   ```bash
   ./fbt fap_tama_mem_manager
   ```
3. Copy the generated `.fap` file to your Flipper Zero's SD card in the `apps/GPIO/` directory
4. The application will appear in the GPIO menu on your Flipper Zero

## Wiring

Connect the Tamagotchi SMA card (with its SPI memory chip) to your Flipper Zero's GPIO pins. The application uses the external SPI bus. Refer to the wiring diagram in the application (Wiring scene) for pin connections.

Typical connections:
- **CS (Chip Select)**: GPIO pin PA4
- **MOSI**: SPI data out
- **MISO**: SPI data in
- **SCK**: SPI clock
- **VCC**: 3.3V
- **GND**: Ground

⚠️ **Warning**: Ensure proper voltage levels (3.3V) and correct pin connections to avoid damaging your Flipper Zero or Tamagotchi SMA card.

## Usage

### Main Menu

The application provides the following options:

1. **Files**: Browse and manage saved memory dump files
2. **Reset**: Reset Tamagotchi memory (with validation)
3. **Protect**: Enable/disable write protection on the chip
4. **Read**: Read memory chip contents and save to file
5. **Erase**: Erase the entire memory chip

### Reading Memory

1. Select "Read" from the main menu
2. The application will attempt to detect the SMA card memory chip
3. If detection succeeds, you'll be prompted to enter a filename
4. The memory contents from the SMA card will be read and saved as a `.bin` file
5. Files are saved in the app data directory

### Writing Memory

1. Select "Files" from the main menu
2. Choose a saved memory dump file
3. Select "Write" from the file menu
4. The application will detect the SMA card chip and verify compatibility
5. The file will be written to the SMA card memory chip
6. Verification will be performed automatically

### Tama Reset

The Reset feature allows you to reset Tamagotchi memory:
- Validates the chip contains valid Tamagotchi data
- Displays current theme information
- Resets memory to factory state

### Protection Management

Use the Protect feature to:
- Enable write protection (prevents accidental writes)
- Disable write protection (allows writes)

## Supported Chips

The application supports a wide range of 25-series SPI memory chips commonly used in Tamagotchi SMA cards. The chip database is automatically generated from the chiplist.xml file. Supported chips include various capacities and manufacturers.

## File Format

Memory dumps are saved in binary format (`.bin` files) with the following naming convention:
- Prefix: `SPIMem`
- Extension: `.bin`
- Files are stored in the app data directory

## Development

### Project Structure

```
tama_mem_manager/
├── lib/spi/          # SPI communication and chip handling
├── scenes/           # Application scenes/views
├── views/            # UI views (progress, detection)
├── images/           # Application icons and assets
├── tools/            # Development tools (chiplist converter)
└── tama_mem_app.c    # Main application entry point
```

### Building

The application uses the Flipper Zero build system. Ensure you have the Flipper Zero toolchain installed.

```bash
# Build the application
./fbt fap_tama_mem_manager

# Build with debug symbols
./fbt fap_tama_mem_manager DEBUG=1
```

### Chip List Conversion

To update the chip database:
```bash
cd tools
./chiplist_convert.py chiplist/chiplist.xml
mv tama_mem_chip_arr.c ../lib/spi/tama_mem_chip_arr.c
```

## Version

Current version: **1.4**

## Disclaimer

⚠️ **Use at your own risk**. This tool allows direct access to Tamagotchi SMA card memory chips. Incorrect usage may:
- Corrupt SMA card memory
- Permanently damage your Tamagotchi SMA card or device
- Void warranties

Always backup your SMA card memory before writing or erasing. The authors are not responsible for any damage or data loss.

## Acknowledgments

- Based on SPI memory chip handling libraries
- Chip database derived from UsbAsp-flash chiplist.xml