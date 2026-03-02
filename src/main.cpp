#include <windows.h>
#include "app_window.h"
#include <string>
#include <stdexcept>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    try {
        AppWindow app(hInstance);
        if (!app.create()) {
            MessageBox(nullptr,
                L"Failed to create application window.",
                L"Error", MB_ICONERROR);
            return 1;
        }
        return app.run();
    } catch (const std::exception& e) {
        std::string what = e.what();
        std::wstring wmsg(what.begin(), what.end());
        std::wstring msg = L"An unexpected error occurred:\n" + wmsg;
        MessageBox(nullptr, msg.c_str(), L"Error", MB_ICONERROR);
        return 1;
    }
}
