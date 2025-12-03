# sortvis
Sort Algorithm Visualizations

## Changes
Please see [sortvis.c](sortvis.c)

## Usage
```
sortvis [OPTIONS]

OPTIONS:
  -v, --version        Display program version information
  -h, --help           Display this help message
  -s, --speed <value>  Set animation speed in milliseconds (default: 60)
```

## Examples
```bash
sortvis              # Run with default settings
sortvis -s 100       # Run with slower animation
sortvis --speed 30   # Run with faster animation
sortvis --help       # Display detailed help
```

## Screenshots
![Screenshot on Mac](screens/screen-01-mac.png "Main menu")
![Screenshot on Mac](screens/screen-02-mac.png "Sort algorithm in action")
![Screenshot on Mac](screens/screen-03-mac.png "Color themes")

## Notes
This project was compiled and successfully tested on:
* macOS Tahoe
* Windows 10/11

**Requirements:**
* Windows 10 Build 10586 or later (for VT/ANSI support)
* macOS or Linux with ANSI terminal support

**Not compatible with:**
* Windows 7, 8, or 8.1 (no VT terminal support)
