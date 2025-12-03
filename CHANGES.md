# Changes Summary - December 3, 2025

## Improvements Made

### 1. Enhanced `--help` Parameter
- **Before**: Simple one-line help message
- **After**: Comprehensive help display including:
  - Clear usage syntax
  - Detailed option descriptions
  - Multiple examples
  - List of supported algorithms
  - Navigation instructions
  - System requirements

**Command**: `sortvis --help` or `sortvis -h`

### 2. Improved Error Handling

#### Command-Line Arguments
- ✅ **Missing speed value**: Detects when `--speed` is provided without a value
- ✅ **Invalid speed value**: Validates that speed is a valid integer
- ✅ **Negative speed**: Prevents negative speed values
- ✅ **Unknown options**: Reports unrecognized command-line arguments
- ✅ **Helpful error messages**: All errors include suggestions to use `--help`

#### Terminal Initialization
- ✅ **Console handle validation**: Checks if console handle is valid
- ✅ **Console mode detection**: Verifies console mode can be retrieved
- ✅ **VT support verification**: Confirms Virtual Terminal processing can be enabled
- ✅ **Detailed error messages**: Explains Windows version requirements
- ✅ **Locale handling**: Warns if locale setting fails on Unix systems

### 3. Fixed Win32 Terminal Compatibility

#### New `getch_arrow()` Function
Created a robust cross-platform function for arrow key detection:

**Windows (`_WIN32`)**:
- Properly handles extended key prefix (0 or 224)
- Directly maps scan codes: 72=Up, 80=Down, 75=Left, 77=Right
- Returns normalized characters: 'U', 'D', 'L', 'R'

**Unix/Linux/macOS**:
- Handles ESC sequences correctly
- Parses ANSI escape codes `ESC[A/B/C/D`
- Returns same normalized characters as Windows

#### Fixed Arrow Key Handling
All three menu systems now use `getch_arrow()`:
1. **Main menu** (app_exec)
2. **Generate samples menu** (sample_generate)
3. **Color gradient selection** (select_gradient)

**Before**: Inconsistent behavior between Windows and Unix
**After**: Identical arrow key behavior on all platforms

### 4. Code Quality Improvements

#### Error Messages
- Changed from simple `die()` calls to descriptive `fprintf(stderr, ...)` messages
- Added context-specific guidance for users
- Included examples in error messages

#### Parameter Parsing
- Fixed argument consumption to properly skip processed arguments
- Added bounds checking for array access (`i + 1 < argc`)
- Improved validation with multiple error conditions

#### Input Validation
- Added `sample_validate()` function to check data integrity
- Validates sample values are within range [1, SAMPLE_SIZE]
- Auto-corrects out-of-range values with warnings

#### Logic Fix
- Fixed condition bug: `if (choice < 'A' && choice > 'M')` → `if (choice < 'A' || choice > 'Q')`
- Properly validates the full range of menu options (A-Q)

### 5. Documentation Updates

#### README.md
- Added usage examples
- Documented command-line options
- Listed system requirements clearly
- Specified minimum Windows version (Build 10586)

#### sortvis.c Comments
- Updated platform support information
- Added Windows 10/11 testing confirmation
- Documented new features in update history
- Clarified Windows version requirements

## Testing Results

All tests passed successfully:

```bash
✅ sortvis --help          # Displays comprehensive help
✅ sortvis --version       # Shows version 5.0.0
✅ sortvis -s 50          # Runs with custom speed
✅ sortvis --speed         # Error: requires value
✅ sortvis --speed abc     # Error: invalid value
✅ sortvis --speed -10     # Error: negative value
✅ sortvis --invalid       # Error: unknown option
```

## Files Modified

1. **helpers.h** - Added `getch_arrow()` function with error handling
2. **sortvis.h** - Enhanced help, improved parameter parsing, fixed menu navigation
3. **sortvis.c** - Updated comments and documentation
4. **vt.h** - Added comprehensive VT initialization error handling
5. **algs.h** - Fixed arrow key handling, added sample validation
6. **README.md** - Updated with usage examples and requirements

## Compatibility

### Supported Platforms
- ✅ Windows 10 Build 10586 or later
- ✅ Windows 11
- ✅ macOS with ANSI terminal
- ✅ Linux with ANSI terminal

### Unsupported Platforms
- ❌ Windows 7 (no VT terminal support)
- ❌ Windows 8/8.1 (no VT terminal support)
- ❌ Very old terminals without ANSI support

## Build Instructions

```bash
# Windows (MinGW)
mingw32-make

# macOS/Linux
make

# Clean build
make clean  # or mingw32-make clean
```

## Future Recommendations

1. Consider adding a `--no-color` option for terminals without color support
2. Add `--list-algorithms` to show detailed algorithm information
3. Consider adding a `--benchmark` mode for performance testing
4. Add configuration file support (`.sortvisrc`)
5. Consider adding `--size` parameter to customize sample size
