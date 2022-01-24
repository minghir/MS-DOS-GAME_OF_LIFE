#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "scancode.h"

unsigned char   KeyboardGetKey(unsigned int scanCode);
void            KeyboardClearKeys();
void            KeyboardInstallDriver();
void            KeyboardUninstallDriver();
void            KeyboardDumpScancodeLog();

#endif // KEYBOARD_H_INCLUDED