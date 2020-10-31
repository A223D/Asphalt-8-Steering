import ctypes
from ctypes import wintypes
import time
import serial

ser=serial.Serial("COM19", baudrate=9600,
                           bytesize=8) #CHANGE SERIAL NAME
print(ser.name)

user32 = ctypes.WinDLL('user32', use_last_error=True)


INPUT_KEYBOARD = 1


KEYEVENTF_EXTENDEDKEY = 0x0001
KEYEVENTF_KEYUP       = 0x0002
KEYEVENTF_UNICODE     = 0x0004
KEYEVENTF_SCANCODE    = 0x0008

MAPVK_VK_TO_VSC = 0

# msdn.microsoft.com/en-us/library/dd375731
VK_TAB  = 0x09
VK_MENU = 0x12

# C struct definitions

wintypes.ULONG_PTR = wintypes.WPARAM

class MOUSEINPUT(ctypes.Structure):
    _fields_ = (("dx",          wintypes.LONG),
                ("dy",          wintypes.LONG),
                ("mouseData",   wintypes.DWORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

class KEYBDINPUT(ctypes.Structure):
    _fields_ = (("wVk",         wintypes.WORD),
                ("wScan",       wintypes.WORD),
                ("dwFlags",     wintypes.DWORD),
                ("time",        wintypes.DWORD),
                ("dwExtraInfo", wintypes.ULONG_PTR))

    def __init__(self, *args, **kwds):
        super(KEYBDINPUT, self).__init__(*args, **kwds)
        # some programs use the scan code even if KEYEVENTF_SCANCODE
        # isn't set in dwFflags, so attempt to map the correct code.
        if not self.dwFlags & KEYEVENTF_UNICODE:
            self.wScan = user32.MapVirtualKeyExW(self.wVk,
                                                 MAPVK_VK_TO_VSC, 0)

class HARDWAREINPUT(ctypes.Structure):
    _fields_ = (("uMsg",    wintypes.DWORD),
                ("wParamL", wintypes.WORD),
                ("wParamH", wintypes.WORD))

class INPUT(ctypes.Structure):
    class _INPUT(ctypes.Union):
        _fields_ = (("ki", KEYBDINPUT),
                    ("mi", MOUSEINPUT),
                    ("hi", HARDWAREINPUT))
    _anonymous_ = ("_input",)
    _fields_ = (("type",   wintypes.DWORD),
                ("_input", _INPUT))

LPINPUT = ctypes.POINTER(INPUT)

def _check_count(result, func, args):
    if result == 0:
        raise ctypes.WinError(ctypes.get_last_error())
    return args

user32.SendInput.errcheck = _check_count
user32.SendInput.argtypes = (wintypes.UINT, # nInputs
                             LPINPUT,       # pInputs
                             ctypes.c_int)  # cbSize

# Functions

def PressKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,
              ki=KEYBDINPUT(wVk=hexKeyCode))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))

def ReleaseKey(hexKeyCode):
    x = INPUT(type=INPUT_KEYBOARD,
              ki=KEYBDINPUT(wVk=hexKeyCode,
                            dwFlags=KEYEVENTF_KEYUP))
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(x))

def AltTab():
    """Press Alt+Tab and hold Alt key for 2 seconds
    in order to see the overlay.
    """
    PressKey(VK_MENU)   # Alt
    PressKey(VK_TAB)    # Tab
    ReleaseKey(VK_TAB)  # Tab~
    time.sleep(2)
    ReleaseKey(VK_MENU) # Alt~

if __name__ == "__main__":

    while(True):
        if(ser.in_waiting>0):
            #print("I received something")
            msgstring=ser.readline().decode('Ascii')[0:-3]
            #print("Simple decoded " + msgstring)
            #print("Length is "+str(len(msgstring)))
            if(len(msgstring)<8):
                msgstring = "0"*(8-len(msgstring)) + msgstring
            #print("Ready String" + msgstring)

            if (msgstring[0]=="1"):
                PressKey(0x20)
                print("zeroth bit nitro(space)")
            else:
                ReleaseKey(0x20)

            if (msgstring[1]=="1"):
                PressKey(0x53)
                print("first bit brake(s)")
            else:
                ReleaseKey(0x53)

            if (msgstring[2]=="1"):
                PressKey(0x42)
                print("second bit powerup1(v)")
            else:
                ReleaseKey(0x42)

            if (msgstring[3]=="1"):
                PressKey(0x56)
                print("third bit powerup2(b)")
            else:
                ReleaseKey(0x56)

            if (msgstring[4]=="1"):
                PressKey(0x41)
                print("fourth bit left(a)")
            else:
                ReleaseKey(0x41)

            if (msgstring[5]=="1"):
                PressKey(0x44)
                print("fifth bit right(d)")
            else:
                ReleaseKey(0x44)
