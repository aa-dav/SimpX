#ifndef SIMP_X_H
#define SIMP_X_H

#include "simpleton4.h"
#include <vector>
#include <QImage>

#define PPU_SOFT_RENDER 1
#define BUILD_WEBASSEMBLY 1

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

int qtKeyToUSB( int key );

class SimpXMMU : public MMU
{
private:
    unsigned int pagesCount;
    std::vector< mWord > mem;
    mWord pages[ 8 ];
    mWord pal16[ 256 ];
    uint32_t pal32[ 256 ];
    mWord input[ 16 ];

    // MMU interface
public:
    static const unsigned int bitsInPage = 3;
    static const unsigned int bitsInAddr = 16 - bitsInPage;
    static const unsigned int pageSize = 1 << bitsInAddr;
    static const unsigned int addrMask = pageSize - 1;
    static const unsigned int pageMask = (1 << bitsInPage) - 1;

    constexpr static mWord getPageNum( mWord addr )
    {
        return (addr >> bitsInAddr) & pageMask;
    }
    mWord &getMem( mWord addr )
    {
        size_t idx = (pages[ getPageNum( addr ) ] << bitsInAddr) | (addr & addrMask);
        if ( idx >= mem.size() )
        {
            return mem[ 0 ];
        }
        return mem[ idx ];
    }
    const mWord    *getPalPtr16() { return pal16; };
    const uint32_t *getPalPtr32() { return pal32; };

    enum IOPorts
    {
        portStart        = 0xFFE0,
        // Keyboard/Gamepads/Mouse input-output port
        portInput        = 0xFFE0,
        // Video IO ports
        vidFlags         = 0xFFF0,
        vidBitmapPage    = 0xFFF1,
        vidCharmapPage   = 0xFFF2,
        vidCharmapAddr   = 0xFFF3, // 000 XXX00 00000000
        vidScrollX       = 0xFFF4,
        vidScrollY       = 0xFFF5,
        vidPalPtr        = 0xFFF6,
        vidPalData       = 0xFFF7,
        // MMU IO ports
        mmuPage0         = 0xFFF8,
        mmuPage1         = 0xFFF9,
        mmuPage2         = 0xFFFA,
        mmuPage3         = 0xFFFB,
        mmuPage4         = 0xFFFC,
        mmuPage5         = 0xFFFD,
        mmuPage6         = 0xFFFE,
        mmuPage7         = 0xFFFF
    };

    SimpXMMU( int _pagesCount ): pagesCount( _pagesCount ), mem( pageSize * _pagesCount, 0 ) {};
    ~SimpXMMU() {};
    void reset() override;
    mWord read(mWord addr) override;
    void write(mWord addr, mWord data) override;
    mWord *getPtr(uint32_t addr) override;
    mWord getPagesCount() { return pagesCount; };
};

class SimpX
{
    SimpXMMU mmu;
    CPU cpu;
    uint64_t clocks;
public:
    SimpX( int _pages ): mmu( _pages ), cpu( mmu ) {}
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
