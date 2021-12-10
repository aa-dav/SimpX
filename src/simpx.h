#ifndef SIMP_X_H
#define SIMP_X_H

#include "simpleton4.h"
#include <vector>
#include <QImage>

#ifdef __EMSCRIPTEN__

#define SIMPX_SOFT_RENDER 1
#define SIMPX_NO_INI  1
#define SIMPX_FAKE_FS   1

#else

#define SIMPX_SOFT_RENDER 1
#define SIMPX_NO_INI  0
#define SIMPX_FAKE_FS   1

#endif

namespace Simpleton
{

enum KeyCodes
{

// Modifiers are not used right now...
// #define KEY_MOD_LCTRL  0x01
// #define KEY_MOD_LSHIFT 0x02
// #define KEY_MOD_LALT   0x04
// #define KEY_MOD_LMETA  0x08
// #define KEY_MOD_RCTRL  0x10
// #define KEY_MOD_RSHIFT 0x20
// #define KEY_MOD_RALT   0x40
// #define KEY_MOD_RMETA  0x80

    // Controls
    Key_NONE        = 0x00, // None
    Key_CTRL        = 0x01, // Ctrl
    Key_SHIFT       = 0x02, // Shift
    Key_ALT         = 0x03, // Alt
    // Letters
    Key_A           = 0x04, // Keyboard a and A
    Key_B           = 0x05, // Keyboard b and B
    Key_C           = 0x06, // Keyboard c and C
    Key_D           = 0x07, // Keyboard d and D
    Key_E           = 0x08, // Keyboard e and E
    Key_F           = 0x09, // Keyboard f and F
    Key_G           = 0x0a, // Keyboard g and G
    Key_H           = 0x0b, // Keyboard h and H
    Key_I           = 0x0c, // Keyboard i and I
    Key_J           = 0x0d, // Keyboard j and J
    Key_K           = 0x0e, // Keyboard k and K
    Key_L           = 0x0f, // Keyboard l and L
    Key_M           = 0x10, // Keyboard m and M
    Key_N           = 0x11, // Keyboard n and N
    Key_O           = 0x12, // Keyboard o and O
    Key_P           = 0x13, // Keyboard p and P
    Key_Q           = 0x14, // Keyboard q and Q
    Key_R           = 0x15, // Keyboard r and R
    Key_S           = 0x16, // Keyboard s and S
    Key_T           = 0x17, // Keyboard t and T
    Key_U           = 0x18, // Keyboard u and U
    Key_V           = 0x19, // Keyboard v and V
    Key_W           = 0x1a, // Keyboard w and W
    Key_X           = 0x1b, // Keyboard x and X
    Key_Y           = 0x1c, // Keyboard y and Y
    Key_Z           = 0x1d, // Keyboard z and Z
    // Numbers
    Key_1           = 0x1e, // Keyboard 1 and !
    Key_2           = 0x1f, // Keyboard 2 and @
    Key_3           = 0x20, // Keyboard 3 and #
    Key_4           = 0x21, // Keyboard 4 and $
    Key_5           = 0x22, // Keyboard 5 and %
    Key_6           = 0x23, // Keyboard 6 and ^
    Key_7           = 0x24, // Keyboard 7 and &
    Key_8           = 0x25, // Keyboard 8 and *
    Key_9           = 0x26, // Keyboard 9 and (
    Key_0           = 0x27, // Keyboard 0 and )
    // Additional
    Key_ENTER       = 0x28, // Keyboard Return (ENTER)
    Key_ESC         = 0x29, // Keyboard ESCAPE
    Key_BACKSPACE   = 0x2a, // Keyboard DELETE (Backspace)
    Key_TAB         = 0x2b, // Keyboard Tab
    Key_SPACE       = 0x2c, // Keyboard Spacebar
    Key_MINUS       = 0x2d, // Keyboard - and _
    Key_EQUAL       = 0x2e, // Keyboard = and +
    Key_LEFTBRACE   = 0x2f, // Keyboard [ and {
    Key_RIGHTBRACE  = 0x30, // Keyboard ] and }
    Key_BACKSLASH   = 0x31, // Keyboard \ and |
    Key_HASHTILDE   = 0x32, // Keyboard Non-US # and ~
    Key_SEMICOLON   = 0x33, // Keyboard ; and :
    Key_APOSTROPHE  = 0x34, // Keyboard ' and "
    Key_GRAVE       = 0x35, // Keyboard ` and ~
    Key_COMMA       = 0x36, // Keyboard , and <
    Key_DOT         = 0x37, // Keyboard . and >
    Key_SLASH       = 0x38, // Keyboard / and ?
    Key_CAPSLOCK    = 0x39, // Keyboard Caps Lock

    Key_F1          = 0x3a, // Keyboard F1
    Key_F2          = 0x3b, // Keyboard F2
    Key_F3          = 0x3c, // Keyboard F3
    Key_F4          = 0x3d, // Keyboard F4
    Key_F5          = 0x3e, // Keyboard F5
    Key_F6          = 0x3f, // Keyboard F6
    Key_F7          = 0x40, // Keyboard F7
    Key_F8          = 0x41, // Keyboard F8
    Key_F9          = 0x42, // Keyboard F9
    Key_F10         = 0x43, // Keyboard F10
    Key_F11         = 0x44, // Keyboard F11
    Key_F12         = 0x45, // Keyboard F12

    Key_SYSRQ       = 0x46, // Keyboard Print Screen
    Key_SCROLLLOCK  = 0x47, // Keyboard Scroll Lock
    Key_PAUSE       = 0x48, // Keyboard Pause
    Key_INSERT      = 0x49, // Keyboard Insert
    Key_HOME        = 0x4a, // Keyboard Home
    Key_PAGEUP      = 0x4b, // Keyboard Page Up
    Key_DELETE      = 0x4c, // Keyboard Delete Forward
    Key_END         = 0x4d, // Keyboard End
    Key_PAGEDOWN    = 0x4e, // Keyboard Page Down
    Key_RIGHT       = 0x4f, // Keyboard Right Arrow
    Key_LEFT        = 0x50, // Keyboard Left Arrow
    Key_DOWN        = 0x51, // Keyboard Down Arrow
    Key_UP          = 0x52, // Keyboard Up Arrow

    Key_NUMLOCK     = 0x53, // Keyboard Num Lock and Clear
    Key_KPSLASH     = 0x54, // Keypad /
    Key_KPASTERISK  = 0x55, // Keypad *
    Key_KPMINUS     = 0x56, // Keypad -
    Key_KPPLUS      = 0x57, // Keypad +
    Key_KPENTER     = 0x58, // Keypad ENTER
    Key_KP1         = 0x59, // Keypad 1 and End
    Key_KP2         = 0x5a, // Keypad 2 and Down Arrow
    Key_KP3         = 0x5b, // Keypad 3 and PageDn
    Key_KP4         = 0x5c, // Keypad 4 and Left Arrow
    Key_KP5         = 0x5d, // Keypad 5
    Key_KP6         = 0x5e, // Keypad 6 and Right Arrow
    Key_KP7         = 0x5f, // Keypad 7 and Home
    Key_KP8         = 0x60, // Keypad 8 and Up Arrow
    Key_KP9         = 0x61, // Keypad 9 and Page Up
    Key_KP0         = 0x62, // Keypad 0 and Insert
    Key_KPDOT       = 0x63 // Keypad . and Delete
};

enum GamePadKeys
{
    // key masks...
    GPK_LEFT        = 1 << 0,
    GPK_RIGHT       = 1 << 1,
    GPK_UP          = 1 << 2,
    GPK_DOWN        = 1 << 3,
    GPK_A           = 1 << 4,
    GPK_B           = 1 << 5,
    GPK_SELECT      = 1 << 8,
    GPK_START       = 1 << 9,
    GPK_X           = 1 << 6,
    GPK_Y           = 1 << 7,
    GPK_L1          = 1 << 10,
    GPK_R1          = 1 << 11,
    GPK_L2          = 1 << 12,
    GPK_R2          = 1 << 13,
    GPK_L3          = 1 << 14,
    GPK_R3          = 1 << 15
};

enum InputSlots
{
    InputKeys0      = 1 << 0,
    InputKeys1      = 1 << 1,
    InputKeys2      = 1 << 2,
    InputKeys3      = 1 << 3,
    InputKeys4      = 1 << 4,
    InputKeys5      = 1 << 5,
    InputKeys6      = 1 << 6,
    InputKeys7      = 1 << 7,
    InputGamePad1   = 1 << 8,
    InputGamePad2   = 1 << 9
};

enum IOPorts
{
    inputsCount     = 16,
    portsCount      = 32,
    portStart       = 0xFFE0,
    // Keyboard/Gamepads/Mouse input-output port
    portInput       = 0xFFE0,
    // Video IO ports
    vidVCount       = 0xFFEE, // счётчик до прерывания HBlank
    vidAddr         = 0xFFEF, // адрес следующей строки в HBlank
    vidFlags        = 0xFFF0, // глобальные флаги видеоадаптера
    vidBitmapPage   = 0xFFF1, // страница битмапа
    vidCharmapPage  = 0xFFF2, // страница чармапа
    vidCharmapAddr  = 0xFFF3, // внутристраничная часть (000 XXX00 00000000) адреса чармапа
    vidScrollX      = 0xFFF4,
    vidScrollY      = 0xFFF5,
    vidPalPtr       = 0xFFF6,
    vidPalData      = 0xFFF7,
    // MMU IO ports
    mmuPageBase     = portsCount - 8, // first slot of pageSels in ports[]
    mmuPage0        = 0xFFF8,
    mmuPage1        = 0xFFF9,
    mmuPage2        = 0xFFFA,
    mmuPage3        = 0xFFFB,
    mmuPage4        = 0xFFFC,
    mmuPage5        = 0xFFFD,
    mmuPage6        = 0xFFFE,
    mmuPage7        = 0xFFFF

};

int qtKeyToUSB( int key, int modif );

class SimpXMMU : public MMU
{
public:
    enum Params
    {
        // not really enum, but constexpr values for parameters...
        bitsInPage = 3,
        bitsInAddr = 16 - bitsInPage,
        pageSize = 1 << bitsInAddr,
        addrMask = pageSize - 1,
        pageMask = (1 << bitsInPage) - 1
    };

private:
    unsigned int pagesCount;
    std::vector< mWord > mem;
    mWord pal16[ 256 ];
    uint32_t pal32[ 256 ];
    mWord ports[ portsCount ];
    mWord inputs[ inputsCount ];

    // MMU interface
public:
    constexpr static mWord getPageNum( mWord addr )
    {
        return (addr >> bitsInAddr) & pageMask;
    }
    mWord &getMem( mWord addr )
    {
        size_t idx = (ports[ mmuPageBase + getPageNum( addr ) ] << bitsInAddr) | (addr & addrMask);
        if ( idx >= mem.size() )
        {
            return mem[ 0 ];
        }
        return mem[ idx ];
    }
    const mWord    *getPalPtr16() { return pal16; };
    const uint32_t *getPalPtr32() { return pal32; };

    explicit SimpXMMU( int _pagesCount ): pagesCount( _pagesCount ), mem( pageSize * _pagesCount, 0 )
    {
        memset( pal16, 0, sizeof( pal16 ) );
        memset( pal32, 0, sizeof( pal32 ) );
        memset( ports, 0, sizeof( ports ) );
        memset( inputs, 0, sizeof( inputs ) );
    };
    ~SimpXMMU() {};
    void reset() override;
    mWord read(mWord addr) override;
    void write(mWord addr, mWord data) override;
    mWord *getPtr(uint32_t addr) override;
    mWord getPagesCount() { return pagesCount; };
    void setInputBit( mWord adds, bool bit );
    void setInputWord( mWord idx, mWord data );
};

class SimpX
{
    SimpXMMU mmu;
    CPU cpu;
    uint64_t clocks = 0;
public:
    explicit SimpX( int _pages ): mmu( _pages ), cpu( mmu ) {}
    SimpXMMU &getMMU() { return mmu; }
    CPU &getCPU() { return cpu; }
    uint64_t getClocks() { return clocks; }
    void stepFrame( mWord *buf16, uint32_t *buf32 );;
    void reset()
    {
        mmu.reset();
        cpu.reset();
        clocks = 0;
    };
};

};  // namespace Simpleton

#endif // SIMPX_H
