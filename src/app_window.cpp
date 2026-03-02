#include "app_window.h"
#include "resource.h"
#include "calculator.h"
#include "date_utils.h"
#include "currency.h"
#include <string>
#include <stdexcept>

// ===========================
// Construction / Destruction
// ===========================

AppWindow::AppWindow(HINSTANCE hInstance) : hInstance_(hInstance) {}

AppWindow::~AppWindow() {
    if (hTitleFont_) DeleteObject(hTitleFont_);
    if (hSubtitleFont_) DeleteObject(hSubtitleFont_);
    if (hNormalFont_) DeleteObject(hNormalFont_);
    if (hResultFont_) DeleteObject(hResultFont_);
}

// ===========================
// Window Creation
// ===========================

bool AppWindow::create() {
    // Initialize Common Controls for modern themed appearance
    INITCOMMONCONTROLSEX icc = {sizeof(icc), ICC_STANDARD_CLASSES};
    InitCommonControlsEx(&icc);

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance_;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) return false;

    // Calculate window rect for desired client area
    RECT rc = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

    int windowW = rc.right - rc.left;
    int windowH = rc.bottom - rc.top;

    // Center on screen
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenW - windowW) / 2;
    int posY = (screenH - windowH) / 2;

    hWnd_ = CreateWindowEx(
        0, WINDOW_CLASS, WINDOW_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        posX, posY, windowW, windowH,
        nullptr, nullptr, hInstance_, this);

    if (!hWnd_) return false;

    // Create fonts
    hTitleFont_ = CreateFont(
        -24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

    hSubtitleFont_ = CreateFont(
        -20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

    hNormalFont_ = CreateFont(
        -15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

    hResultFont_ = CreateFont(
        -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");

    showMainMenu();

    ShowWindow(hWnd_, SW_SHOW);
    UpdateWindow(hWnd_);

    return true;
}

// ===========================
// Message Loop
// ===========================

int AppWindow::run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        // Handle Tab key navigation
        if (!IsDialogMessage(hWnd_, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return static_cast<int>(msg.wParam);
}

// ===========================
// Window Procedure
// ===========================

LRESULT CALLBACK AppWindow::windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    AppWindow* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<AppWindow*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hWnd_ = hWnd;
    } else {
        pThis = reinterpret_cast<AppWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT AppWindow::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTN_COMPUTE:
            showCalculator();
            return 0;
        case IDC_BTN_EXIT:
            DestroyWindow(hWnd_);
            return 0;
        case IDC_BTN_CALCULATE:
            calculate();
            return 0;
        case IDC_BTN_MAIN_MENU:
            showMainMenu();
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CTLCOLORSTATIC: {
        HDC hdc = reinterpret_cast<HDC>(wParam);
        SetBkMode(hdc, TRANSPARENT);
        return reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_WINDOW));
    }
    }

    return DefWindowProc(hWnd_, msg, wParam, lParam);
}

// ===========================
// Date Entry Subclass
// ===========================

LRESULT CALLBACK AppWindow::dateEditSubclassProc(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,
    UINT_PTR subclassId, DWORD_PTR refData)
{
    (void)subclassId;
    (void)refData;

    if (msg == WM_CHAR) {
        wchar_t ch = static_cast<wchar_t>(wParam);

        // Allow control characters (backspace, etc.)
        if (ch < L' ') return DefSubclassProc(hWnd, msg, wParam, lParam);

        // Block non-digit characters
        if (ch < L'0' || ch > L'9') return 0;

        // Get current text
        wchar_t buf[20] = {};
        GetWindowText(hWnd, buf, 20);
        std::wstring text(buf);

        // Extract digits only
        std::wstring digits;
        for (wchar_t c : text) {
            if (c >= L'0' && c <= L'9') digits += c;
        }

        // Get cursor position and count digits before cursor
        DWORD selStart = 0, selEnd = 0;
        SendMessage(hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&selStart), reinterpret_cast<LPARAM>(&selEnd));

        int digitsBefore = 0;
        for (DWORD i = 0; i < selStart && i < text.length(); ++i) {
            if (text[i] >= L'0' && text[i] <= L'9') ++digitsBefore;
        }

        // Insert the new digit at the correct position
        if (digits.length() >= 8) return 0;  // Max 8 digits (MMDDYYYY)
        digits.insert(digits.begin() + digitsBefore, ch);

        // Reformat with hyphens: MM-DD-YYYY
        std::wstring formatted;
        for (size_t i = 0; i < digits.length(); ++i) {
            if (i == 2 || i == 4) formatted += L'-';
            formatted += digits[i];
        }

        // Calculate new cursor position
        int newDigitPos = digitsBefore + 1;
        int newCursorPos = newDigitPos;
        if (newDigitPos > 2) ++newCursorPos;  // After first hyphen
        if (newDigitPos > 4) ++newCursorPos;  // After second hyphen

        SetWindowText(hWnd, formatted.c_str());
        SendMessage(hWnd, EM_SETSEL, newCursorPos, newCursorPos);

        return 0;
    }

    return DefSubclassProc(hWnd, msg, wParam, lParam);
}

// ===========================
// Screen Management
// ===========================

void AppWindow::clearScreen() {
    HWND child = GetWindow(hWnd_, GW_CHILD);
    while (child) {
        HWND next = GetWindow(child, GW_HWNDNEXT);
        RemoveWindowSubclass(child, dateEditSubclassProc, 0);
        DestroyWindow(child);
        child = next;
    }
    hWarrantEntry_ = nullptr;
    hInterestEntry_ = nullptr;
    hStartDateEntry_ = nullptr;
    hEndDateEntry_ = nullptr;
    hResultsLabel_ = nullptr;
}

// ===========================
// Main Menu
// ===========================

void AppWindow::showMainMenu() {
    clearScreen();

    // Title label
    HWND hTitle = CreateWindow(L"STATIC",
        L"Libby Drainage District\r\nWarrant Calculator",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        60, 40, 400, 60,
        hWnd_, reinterpret_cast<HMENU>(IDC_TITLE_LABEL), hInstance_, nullptr);
    setFont(hTitle, hTitleFont_);

    // "Compute New Warrant" button
    HWND hCompute = CreateWindow(L"BUTTON", L"Compute New Warrant",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        135, 140, 250, 35,
        hWnd_, reinterpret_cast<HMENU>(IDC_BTN_COMPUTE), hInstance_, nullptr);
    setFont(hCompute, hNormalFont_);

    // "Exit" button
    HWND hExit = CreateWindow(L"BUTTON", L"Exit",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        135, 195, 250, 35,
        hWnd_, reinterpret_cast<HMENU>(IDC_BTN_EXIT), hInstance_, nullptr);
    setFont(hExit, hNormalFont_);
}

// ===========================
// Calculator Screen
// ===========================

void AppWindow::showCalculator() {
    clearScreen();

    // Title
    HWND hCalcTitle = CreateWindow(L"STATIC", L"Warrant Calculation",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        60, 10, 400, 30,
        hWnd_, reinterpret_cast<HMENU>(IDC_CALC_TITLE), hInstance_, nullptr);
    setFont(hCalcTitle, hSubtitleFont_);

    int labelX = 80, editX = 285, y = 55, rowH = 35;
    int labelW = 195, editW = 155, ctrlH = 24;

    // Row 0: Warrant Amount
    HWND hLblWarrant = CreateWindow(L"STATIC", L"Warrant Amount ($):",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        labelX, y + 3, labelW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_LBL_WARRANT), hInstance_, nullptr);
    setFont(hLblWarrant, hNormalFont_);

    hWarrantEntry_ = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        editX, y, editW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_EDIT_WARRANT), hInstance_, nullptr);
    setFont(hWarrantEntry_, hNormalFont_);

    y += rowH;

    // Row 1: Interest Rate
    HWND hLblRate = CreateWindow(L"STATIC", L"Interest Rate (%):",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        labelX, y + 3, labelW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_LBL_RATE), hInstance_, nullptr);
    setFont(hLblRate, hNormalFont_);

    hInterestEntry_ = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", DEFAULT_RATE,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        editX, y, editW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_EDIT_RATE), hInstance_, nullptr);
    setFont(hInterestEntry_, hNormalFont_);

    y += rowH;

    // Row 2: Start Date
    HWND hLblStart = CreateWindow(L"STATIC", L"Start Date (MM-DD-YYYY):",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        labelX, y + 3, labelW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_LBL_START_DATE), hInstance_, nullptr);
    setFont(hLblStart, hNormalFont_);

    hStartDateEntry_ = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        editX, y, editW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_EDIT_START_DATE), hInstance_, nullptr);
    setFont(hStartDateEntry_, hNormalFont_);
    SetWindowSubclass(hStartDateEntry_, dateEditSubclassProc, 0, 0);

    y += rowH;

    // Row 3: End Date
    HWND hLblEnd = CreateWindow(L"STATIC", L"End Date (MM-DD-YYYY):",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        labelX, y + 3, labelW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_LBL_END_DATE), hInstance_, nullptr);
    setFont(hLblEnd, hNormalFont_);

    hEndDateEntry_ = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        editX, y, editW, ctrlH,
        hWnd_, reinterpret_cast<HMENU>(IDC_EDIT_END_DATE), hInstance_, nullptr);
    setFont(hEndDateEntry_, hNormalFont_);
    SetWindowSubclass(hEndDateEntry_, dateEditSubclassProc, 0, 0);

    y += rowH + 10;

    // Buttons row
    HWND hBtnCalc = CreateWindow(L"BUTTON", L"Calculate",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        140, y, 110, 30,
        hWnd_, reinterpret_cast<HMENU>(IDC_BTN_CALCULATE), hInstance_, nullptr);
    setFont(hBtnCalc, hNormalFont_);

    HWND hBtnMenu = CreateWindow(L"BUTTON", L"Main Menu",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        270, y, 110, 30,
        hWnd_, reinterpret_cast<HMENU>(IDC_BTN_MAIN_MENU), hInstance_, nullptr);
    setFont(hBtnMenu, hNormalFont_);

    y += 45;

    // Results label
    hResultsLabel_ = CreateWindow(L"STATIC", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        30, y, 460, 80,
        hWnd_, reinterpret_cast<HMENU>(IDC_LBL_RESULTS), hInstance_, nullptr);
    setFont(hResultsLabel_, hResultFont_);

    // Set focus to warrant entry
    SetFocus(hWarrantEntry_);
}

// ===========================
// Calculation Logic
// ===========================

void AppWindow::calculate() {
    std::wstring warrantStr = getControlText(hWarrantEntry_);
    std::wstring rateStr = getControlText(hInterestEntry_);
    std::wstring startStr = getControlText(hStartDateEntry_);
    std::wstring endStr = getControlText(hEndDateEntry_);

    // Validate warrant amount
    auto principalCents = Currency::parseDollars(warrantStr);
    if (!principalCents.has_value()) {
        showError(L"Please enter a valid warrant amount (e.g., 1234.56).");
        return;
    }
    if (*principalCents <= 0) {
        showError(L"Warrant amount must be greater than zero.");
        return;
    }

    // Validate interest rate
    double ratePercent;
    try {
        size_t pos = 0;
        ratePercent = std::stod(rateStr, &pos);
        if (pos != rateStr.length()) {
            showError(L"Please enter a valid interest rate (e.g., 6.00).");
            return;
        }
    } catch (...) {
        showError(L"Please enter a valid interest rate (e.g., 6.00).");
        return;
    }
    if (ratePercent <= 0.0 || ratePercent > 20.0) {
        showError(L"Interest rate must be greater than 0 and at most 20 percent.");
        return;
    }

    // Validate start date
    auto startDate = DateUtils::parseDate(startStr);
    if (!startDate.has_value()) {
        showError(L"Please enter a valid start date in MM-DD-YYYY format.");
        return;
    }

    // Validate end date
    auto endDate = DateUtils::parseDate(endStr);
    if (!endDate.has_value()) {
        showError(L"Please enter a valid end date in MM-DD-YYYY format.");
        return;
    }

    // Calculate days
    long long days = DateUtils::daysBetween(*startDate, *endDate);
    if (days < 0) {
        showError(L"End date must be after start date.");
        return;
    }

    // Compute interest
    try {
        auto result = InterestCalculator::compute(*principalCents, ratePercent, days);

        std::wstring resultText =
            L"Days: " + std::to_wstring(result.days) + L"\r\n" +
            L"Interest: " + Currency::formatCents(result.interestCents) + L"\r\n" +
            L"Total Payback Amount: " + Currency::formatCents(result.totalCents);

        SetWindowText(hResultsLabel_, resultText.c_str());
    } catch (const std::invalid_argument& e) {
        // Convert narrow string to wide string
        std::string what = e.what();
        std::wstring wmsg(what.begin(), what.end());
        showError(wmsg);
    }
}

void AppWindow::showError(const std::wstring& message) {
    MessageBox(hWnd_, message.c_str(), L"Input Error", MB_ICONERROR | MB_OK);
}

// ===========================
// Helpers
// ===========================

std::wstring AppWindow::getControlText(HWND hCtrl) {
    if (!hCtrl) return L"";
    int len = GetWindowTextLength(hCtrl);
    if (len == 0) return L"";
    std::wstring text(len, L'\0');
    GetWindowText(hCtrl, &text[0], len + 1);
    return text;
}

void AppWindow::setFont(HWND hCtrl, HFONT hFont) {
    SendMessage(hCtrl, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}
