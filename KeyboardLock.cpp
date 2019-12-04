//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <winuser.h>
#include "KeyboardLock.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


HHOOK hHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// By returning a non-zero value from the hook procedure, the
	// message does not get passed to the target window
	BOOL bControlKeyDown = 0;
	BOOL bAltKeyDown = 0;
	KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;

	switch (nCode) {
	case HC_ACTION:
		// Check to see if the CTRL key is pressed
		bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
		// Check to see if the ALT key is pressed
		bAltKeyDown = GetAsyncKeyState (VK_MENU) >> ((sizeof(SHORT) * 8) - 1);
		if ((pkbhs->vkCode == VK_ESCAPE) && bControlKeyDown)
			return 1;
		if ((pkbhs->vkCode == VK_TAB) && bAltKeyDown)
			return 1;
		if ((pkbhs->vkCode == VK_ESCAPE) && bAltKeyDown)
			return 1;
		if ((pkbhs->vkCode == VK_F4) && bAltKeyDown)
			return 1;
		if ((pkbhs->vkCode == VK_LWIN) || (pkbhs->vkCode == VK_RWIN))
			return 1;
		if ((pkbhs->vkCode == VK_DELETE) && bAltKeyDown && bControlKeyDown) {
			return 1;
		}
		break;
	default:
		break;
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void LockKeyboard()
{
//	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, HInstance, 0);
}
//---------------------------------------------------------------------------

void UnlockKeyboard()
{
//	UnhookWindowsHookEx(hHook);
}
//---------------------------------------------------------------------------

