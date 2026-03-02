# Libby Drainage District Warrant Calculator

A standalone Windows desktop application for calculating interest and total payback amounts on municipal warrants. Built in C++ with the native Win32 API for maximum reliability, minimal footprint, and zero external dependencies.

Designed for clarity, auditability, and ease of use in a local government or special district setting.

---

## Overview

This program calculates the total payback amount for a warrant using:

- Warrant principal amount (in dollars)
- Adjustable annual interest rate (default 6 percent)
- Start date and end date

The result includes the number of elapsed days, the computed interest amount, and the total payback (principal plus interest).

---

## Features

- **Native Windows GUI** — no command line, no browser, no runtime required
- **Single executable** — one `.exe` file, no DLLs or installers needed
- **Default interest rate** set to 6 percent, adjustable per calculation
- **Automatic date formatting** — hyphens are inserted as you type (`MM-DD-YYYY`)
- **Four-digit year format** — eliminates date ambiguity
- **Input validation** with clear, specific error messages
- **Leap year support** — day counts are always accurate
- **Deterministic calculations** — same inputs always produce the same output
- **Main menu navigation** — compute multiple warrants without restarting
- **No internet access required** — all calculations are performed locally
- **No third-party runtime dependencies** — runs on any modern Windows system

---

## System Requirements

### To run the compiled executable

- Windows 10 or later (64-bit)
- No additional software required

### To build from source

- **Visual Studio 2022** (Community, Professional, or Enterprise) or **Visual Studio 2022 Build Tools** with the "Desktop development with C++" workload
- **CMake 3.20+** (optional — a batch file build is also provided)
- **Git** (for cloning the repository)

---

## Downloading the Source Code

Clone the repository using Git:

```bash
git clone https://github.com/cdavisv/warrant-interest-calculator.git
cd warrant-interest-calculator
```

Alternatively, download the repository as a ZIP file from GitHub and extract it locally.

---

## Building the Executable

### Option 1: Using the build script (recommended)

Open a **Developer Command Prompt for VS 2022** (or any terminal where `cl.exe` is available) and run:

```bash
build.bat
```

The compiled executable will be located at:

```
build_out\WarrantCalculator.exe
```

### Option 2: Using CMake

From the project root directory:

```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The compiled executable will be located at:

```
build\Release\WarrantCalculator.exe
```

### Build output

The resulting `.exe` is fully standalone (approximately 200 KB). It statically links the C++ runtime, so it does **not** require the Visual C++ Redistributable to be installed on the target machine.

---

## Usage Instructions

### 1. Launch the application

Double-click `WarrantCalculator.exe`. The main menu will appear.

### 2. Select "Compute New Warrant"

This opens the calculator form.

### 3. Enter the calculation parameters

| Field | Description | Example |
|-------|-------------|---------|
| **Warrant Amount ($)** | The principal amount of the warrant in dollars | `10000` or `10000.00` |
| **Interest Rate (%)** | The annual interest rate as a percentage (default: 6.00) | `6.00` |
| **Start Date** | The date interest begins accruing, in `MM-DD-YYYY` format | `01-15-2025` |
| **End Date** | The date interest stops accruing, in `MM-DD-YYYY` format | `07-15-2025` |

### 4. Click "Calculate"

The results will appear below the form:

- **Days** — the number of calendar days between the start and end dates
- **Interest** — the computed interest amount in dollars
- **Total Payback Amount** — the principal plus the computed interest

### 5. Continue or exit

- Click **"Main Menu"** to return and compute another warrant
- Close the window to exit the application

---

## Date Entry Behavior

- Dates use the format `MM-DD-YYYY` (month-day-four digit year)
- Hyphens are automatically inserted as digits are typed
- Example: typing `01152025` produces `01-15-2025`
- Backspace and navigation keys work normally
- The four-digit year eliminates ambiguity (e.g., `2025` vs. `1925`)

---

## Calculation Method

Interest is calculated using the **simple interest** formula:

```
Interest = Principal × Rate × (Days ÷ 365)
```

The total payback amount is:

```
Total = Principal + Interest
```

Where:
- **Principal** is the warrant amount in dollars
- **Rate** is the annual interest rate as a decimal (e.g., 6% = 0.06)
- **Days** is the number of calendar days between the start and end dates

### Worked example

| Input | Value |
|-------|-------|
| Warrant Amount | $10,000.00 |
| Interest Rate | 6.00% |
| Start Date | 01-01-2025 |
| End Date | 07-01-2025 |

**Calculation:**
- Days between January 1 and July 1, 2025 = **181 days**
- Interest = $10,000.00 × 0.06 × (181 ÷ 365) = **$29.75**
- Total Payback = $10,000.00 + $29.75 = **$10,029.75**

### Precision and rounding

- The principal is tracked internally in whole cents to avoid floating-point rounding errors
- Interest is computed using double-precision arithmetic and rounded to the nearest cent
- Day counts use the C++ `<chrono>` calendar library, which correctly accounts for leap years and varying month lengths

---

## Input Validation

The application validates all inputs before performing a calculation:

| Condition | Error Message |
|-----------|---------------|
| Warrant amount is blank or not a number | "Please enter a valid warrant amount (e.g., 1234.56)." |
| Warrant amount is zero or negative | "Warrant amount must be greater than zero." |
| Interest rate is blank or not a number | "Please enter a valid interest rate (e.g., 6.00)." |
| Interest rate is 0 or below, or above 20 | "Interest rate must be greater than 0 and at most 20 percent." |
| Start date is blank or invalid | "Please enter a valid start date in MM-DD-YYYY format." |
| End date is blank or invalid | "Please enter a valid end date in MM-DD-YYYY format." |
| End date is before start date | "End date must be after start date." |

---

## Project Structure

```
warrant-interest-calculator/
├── src/
│   ├── main.cpp            Entry point (WinMain)
│   ├── app_window.h        GUI window class declaration
│   ├── app_window.cpp       GUI layout, controls, and message handling
│   ├── calculator.h         Interest calculation class declaration
│   ├── calculator.cpp       Interest calculation logic
│   ├── date_utils.h         Date parsing and day-counting declarations
│   ├── date_utils.cpp       Date parsing, validation, and leap year support
│   ├── currency.h           Currency formatting declarations
│   ├── currency.cpp         Dollar parsing and comma-formatted output
│   ├── resource.h           Win32 control identifiers
│   ├── app.rc               Windows resource file (version info, manifest)
│   └── app.manifest         Application manifest (visual styles, DPI)
├── CMakeLists.txt           CMake build configuration
├── build.bat                One-command build script
├── README.md                This file
└── License.md               MIT License
```

### Architecture

The application separates concerns across four modules:

- **`app_window`** — All GUI code (window creation, control layout, user interaction). Uses the Win32 API directly with no framework.
- **`calculator`** — Pure calculation logic with no GUI dependency. Takes a principal (in cents), interest rate, and day count; returns interest and total in cents.
- **`date_utils`** — Date parsing (`MM-DD-YYYY` format), calendar validation (including leap years), and accurate day counting using `std::chrono`.
- **`currency`** — Parses dollar strings into integer cents and formats cents back into `$X,XXX.XX` display strings.

This separation means the calculation engine can be tested, reused, or audited independently of the user interface.

---

## Audit and Governance Notes

- All calculations are deterministic — the same inputs always produce the same outputs
- No external services, APIs, or network calls are used
- The interest rate is explicitly visible and adjustable for each calculation
- Source code is open and can be reviewed, archived, and version-controlled for compliance
- The compiled executable can be digitally signed if required by your organization
- The calculation formula is the standard simple interest method documented above
- Internal currency arithmetic uses integer cents to prevent floating-point rounding issues

---

## Troubleshooting

**The application won't start**
- Ensure you are running on Windows 10 or later (64-bit)
- Try right-clicking the `.exe` and selecting "Run as administrator"
- Check that your antivirus is not quarantining the file

**Date is rejected as invalid**
- Ensure the date is in `MM-DD-YYYY` format with four-digit year
- Verify the date exists on the calendar (e.g., February 30 is not valid)
- Years must be between 1900 and 2100

**Interest shows $0.00**
- This occurs when the start and end dates are the same (0 days of interest)

**Build fails**
- Ensure Visual Studio 2022 Build Tools are installed with the "Desktop development with C++" workload
- Run `build.bat` from a Developer Command Prompt, not a regular terminal
- If using CMake, ensure CMake 3.20 or later is installed

---

## License

### MIT License

Copyright (c) 2025 Charles Davis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

## Contact

For questions, updates, or maintenance, contact Charles Davis or the designated system administrator.
