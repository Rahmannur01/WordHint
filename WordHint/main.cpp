#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <Windows.h>

MainWindow *window;
HHOOK hook = nullptr;

QString vkCodeToString(DWORD vkCode)
{
    BYTE keyboardState[256];
    if (!GetKeyboardState(keyboardState))
        return "";

    WCHAR buffer[5] = {0};

    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);

    int result = ToUnicode(
        vkCode,
        scanCode,
        keyboardState,
        buffer,
        4,
        0
        );

    if (result > 0)
        return QString::fromWCharArray(buffer, result);

    return "";
}

LRESULT CALLBACK hookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            QString text;

            bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
            bool altPressed  = (GetAsyncKeyState(VK_MENU) & 0x8000);

            if(kb->vkCode == VK_BACK) text = "[backspace]";
            else if(!ctrlPressed && altPressed) text = "[alt_control]";
            else if(kb->vkCode == VK_LCONTROL) text = "[control]";
            else text = vkCodeToString(kb->vkCode);

            if(!text.isEmpty() && window != nullptr){
                QMetaObject::invokeMethod(
                    window,
                    "inputTest",
                    Qt::QueuedConnection,
                    Q_ARG(QString, text)
                    );
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void install()
{
    hook = SetWindowsHookEx(
        WH_KEYBOARD_LL,      // тип хука
        hookProc,            // УКАЗАТЕЛЬ на функцию
        GetModuleHandle(nullptr),
        0
        );

    if (!hook) {
        qDebug() << "Failed to install keyboard hook!";
    }
}

void uninstall()
{
    if (hook) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    window = &w;

    install();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){
        uninstall();
    });

    window->show();

    return app.exec();
}
