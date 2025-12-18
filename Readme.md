# Libby Drainage District Warrant Calculator

A standalone desktop application for calculating interest and total payback amounts on municipal warrants.  
Designed for clarity, auditability, and ease of use in a local government or special district setting.

---

## Overview

This program calculates the total payback amount for a warrant using:

- Warrant principal amount
- Adjustable interest rate (default 6 percent)
- Start and end dates

The application is implemented in Python with a Tkinter graphical interface and can be compiled into a single executable for distribution to staff or board members.

---

## Features

- Graphical user interface (no command line required)
- Default interest rate set to 6 percent, adjustable when needed
- Automatic date formatting (`MM-DD-YY`)
- Input validation with clear error messages
- Deterministic interest calculations
- Main menu navigation
- No internet access required
- No third party runtime dependencies

---

## Requirements (Development)

To run or build the application from source, you need:

- Python 3.10 or newer
- Git
- pip (included with Python)

Tkinter is included with standard Python installations.

---

## Downloading the Source Code

Clone the repository using Git:

```bash
git clone https://github.com/<your-organization-or-username>/warrant-calculator.git
cd warrant-calculator
```

Alternatively, you may download the repository as a ZIP file from GitHub and extract it locally.

## Running the Program (From Source)

To run the application directly with Python:
```bash
python app.py
```

This will open the graphical interface.

## Compiling to a Standalone Executable (Windows)

The recommended method for distribution is to compile the program into a single executable file using PyInstaller.

## Install PyInstaller
```bash
pip install pyinstaller
```
## Build the Executable

From the project directory:
```bash
pyinstaller --onefile --windowed app.py
```
## Output

After the build completes, the executable will be located at:
```bash
dist/app.exe
```

This file is fully standalone and does not require Python to be installed on the target system.

## Usage Instructions

1. Launch the application

2. Select Compute New Warrant

3. Enter:

    * Warrant Amount

    * Interest Rate (default 6.00 percent)

    * Start Date (MM-DD-YY)

    * End Date (MM-DD-YY)

4. Click Calculate

5. Review:

    * Number of days

    * Interest amount

    * Total payback amount

6. Return to the main menu or exit

## Date Entry Behavior

* Dates are entered in MM-DD-YY format

* Hyphens are automatically inserted as digits are typed

* Manual hyphen entry is also supported

* Cursor movement behaves naturally during entry

## Calculation Method

Interest is calculated using simple interest:
```text
Interest = Principal × Rate × (Days / 365)
```

The total payback amount is:
```text
Total = Principal + Interest
```
## Audit and Governance Notes

* All calculations are deterministic

* No external services or APIs are used

* Interest rate is explicitly visible and adjustable

* Source code can be reviewed and archived for compliance purposes

* Compiled executables can be digitally signed if required

## License

#### MIT License

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
SOFTWARE


## Contact

For questions, updates, or maintenance, contact Charles Davis or the designated system administrator.