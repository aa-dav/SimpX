#include "simpx.h"

namespace Simpleton
{

void Simpleton::SimpXMMU::reset()
{
    for ( int i = 0; i < 8; i++ )
        pages[ i ] = i;
    for ( int i = 0; i < 256; i++ )
    {
        pal16[ i ] = 0;
        pal32[ i ] = 0;
    }
    for ( unsigned int i = 0; i < pagesCount * pageSize; i++ )
        mem[ i ] = 0;
}

mWord Simpleton::SimpXMMU::read(mWord addr)
{
    if ( addr >= portStart ) // IO ports
    {
        if ( addr >= mmuPage0 )
        {
            return pages[ addr - mmuPage0 ];
        }
        else if ( addr == vidPalData )
        {
            return pal16[ mem[ vidPalPtr ] & 0xFF ];;
        }
        {
            return mem[ addr ]; // direct read
        }
    }
    else
    {
        return getMem( addr );
    }
}

void Simpleton::SimpXMMU::write(mWord addr, mWord data)
{
    if ( addr >= portStart ) // IO ports
    {
        if ( addr >= mmuPage0 )
        {
            pages[ addr - mmuPage0 ] = data % pagesCount;
        }
        else if ( addr == vidPalData )
        {
            pal16[ mem[ vidPalPtr ] & 0xFF ] = data;
            pal32[ mem[ vidPalPtr ] & 0xFF ] = ((data << 9) & 0xF80000) | ((data << 6) & 0xF800) | ((data << 3) & 0xF8);
        }
        else
            mem[ addr ] = data; // direct write
    }
    else
    {
        getMem( addr ) = data;
    }
}

mWord *Simpleton::SimpXMMU::getPtr(uint32_t addr)
{
    return &mem[ addr ];
}

void SimpX::stepFrame( mWord *buf16, uint32_t *buf32 )
{
    const mWord       *pal16 = mmu.getPalPtr16();
    const uint32_t    *pal32 = mmu.getPalPtr32();
    bool need_draw = (pal16 != nullptr) || (pal32 != nullptr);
    int page = (mmu.read( mmu.vidBitmapPage ) % mmu.getPagesCount()) & 0xFFFE;
    mWord *bitmap = mmu.getPtr( mmu.pageSize * page );
    page = mmu.read( mmu.vidCharmapPage ) % mmu.getPagesCount();
    mWord *charmap = mmu.getPtr( mmu.pageSize * page + (mmu.read( mmu.vidCharmapAddr ) & 0001110000000000 ) );

    // Process VBlank
    for ( int scan_line = 192; scan_line < 312; scan_line++ )
    {
        while ( cpu.getClocks() - clocks < 2 * 448 )
            cpu.step();
        clocks += 2 * 448; // skip line
    }

    int scroll_x = mmu.read( mmu.vidScrollX ) & 0xFF;
    int scroll_y = mmu.read( mmu.vidScrollY ) & 0xFF;

    int pix_addr = (scroll_y << 8) + (scroll_x);
    // ADDR MASKS:
    // yyyyyyyy xxxxxxxx point(x,y)
    // lllllhhh cccccwww char(l,c) char_low(w)
    // pppppppp ppppppss ptr(p) subpix(s)

    int scan_line = 0;
    int scan_col = 0;
    while ( scan_line < 192 )
    {
        while ( cpu.getClocks() - clocks < 2 )
            cpu.step();
        if ( scan_col < 256 )
        {
            if ( need_draw )
            {
                int char_idx = ((pix_addr & 0xF800) >> 6) | ((pix_addr & 0xF8) >> 3);
                int char_dat = charmap[ char_idx ];
                int pal_high = (char_dat & 0xF000) >> 8;
                int pix_idx = (((char_dat << 6) & 0xF800) | ((char_dat << 3) & 0xF8) | (pix_addr & 0b11100000111)) >> 2;
                int pix_line = bitmap[ pix_idx ];
                int pix_subnum = 12 - ((pix_addr & 0b11) << 2);
                int offs = pal_high | (( pix_line >> pix_subnum ) & 0x0F);
                if ( buf16 )
                    *buf16++ = pal16[ offs ];
                if ( buf32 )
                    *buf32++ = pal32[ offs ];
            }
            pix_addr++;
            scan_col++;
        }
        else
        {
            scan_col++;
            if ( scan_col >= 448 )
            {
                scan_col = 0;
                scan_line++;
            }
        }
        clocks += 2;
    }
    cpu.triggerIRQ();
}

int qtKeyToUSB(int key)
{
    int res = 0;
    switch ( key )
    {
    // Modifiers
    case Qt::Key_Shift:
        res = Key_SHIFT; break;
    case Qt::Key_Control:
        res = Key_CTRL; break;
    case Qt::Key_Alt:
        res = Key_ALT; break;
    // Letters:
    case Qt::Key_A:
        res = Key_A; break;
    case Qt::Key_B:
        res = Key_B; break;
    case Qt::Key_C:
        res = Key_C; break;
    case Qt::Key_D:
        res = Key_D; break;
    case Qt::Key_E:
        res = Key_E; break;
    case Qt::Key_F:
        res = Key_F; break;
    case Qt::Key_G:
        res = Key_G; break;
    case Qt::Key_H:
        res = Key_H; break;
    case Qt::Key_I:
        res = Key_I; break;
    case Qt::Key_J:
        res = Key_J; break;
    case Qt::Key_K:
        res = Key_K; break;
    case Qt::Key_L:
        res = Key_L; break;
    case Qt::Key_M:
        res = Key_M; break;
    case Qt::Key_N:
        res = Key_N; break;
    case Qt::Key_O:
        res = Key_O; break;
    case Qt::Key_P:
        res = Key_P; break;
    case Qt::Key_Q:
        res = Key_Q; break;
    case Qt::Key_R:
        res = Key_R; break;
    case Qt::Key_S:
        res = Key_S; break;
    case Qt::Key_T:
        res = Key_T; break;
    case Qt::Key_U:
        res = Key_U; break;
    case Qt::Key_V:
        res = Key_V; break;
    case Qt::Key_W:
        res = Key_W; break;
    case Qt::Key_X:
        res = Key_X; break;
    case Qt::Key_Y:
        res = Key_Y; break;
    case Qt::Key_Z:
        res = Key_Z; break;
    // Digits
    case '!':
    case Qt::Key_1:
        res = Key_1; break;
    case '@':
    case Qt::Key_2:
        res = Key_2; break;
    case '#':
    case Qt::Key_3:
        res = Key_3; break;
    case '$':
    case Qt::Key_4:
        res = Key_4; break;
    case '%':
    case Qt::Key_5:
        res = Key_5; break;
    case '^':
    case Qt::Key_6:
        res = Key_6; break;
    case '&':
    case Qt::Key_7:
        res = Key_7; break;
    case '*':
    case Qt::Key_8:
        res = Key_8; break;
    case '(':
    case Qt::Key_9:
        res = Key_9; break;
    case ')':
    case Qt::Key_0:
        res = Key_0; break;
    // Primary control keys:
    case Qt::Key_Enter:
        res = Key_ENTER; break;
    case Qt::Key_Escape:
        res = Key_ESC; break;
    case Qt::Key_Backspace:
        res = Key_BACKSPACE; break;
    case Qt::Key_Tab:
        res = Key_TAB; break;
    case Qt::Key_Space:
        res = Key_SPACE; break;
    case '-':
    case '_':
        res = Key_MINUS; break;
    case '=':
    case '+':
        res = Key_EQUAL; break;
    case '[':
    case '{':
        res = Key_LEFTBRACE; break;
    case ']':
    case '}':
        res = Key_RIGHTBRACE; break;
    case '\\':
    case '|':
        res = Key_BACKSLASH; break;
    //case ???:
    //    res = Key_HASHTILDE; break;
    case ';':
    case ':':
        res = Key_SEMICOLON; break;
    case '\'':
    case '\"':
        res = Key_APOSTROPHE; break;
    case '`':
    case '~':
        res = Key_GRAVE; break;
    case ',':
    case '<':
        res = Key_COMMA; break;
    case '.':
    case '>':
        res = Key_DOT; break;
    case '/':
    case '?':
        res = Key_SLASH; break;
    case Qt::Key_CapsLock:
        res = Key_CAPSLOCK; break;
    case Qt::Key_F1:
        res = Key_F1; break;
    case Qt::Key_F2:
        res = Key_F2; break;
    case Qt::Key_F3:
        res = Key_F3; break;
    case Qt::Key_F4:
        res = Key_F4; break;
    case Qt::Key_F5:
        res = Key_F5; break;
    case Qt::Key_F6:
        res = Key_F6; break;
    case Qt::Key_F7:
        res = Key_F7; break;
    case Qt::Key_F8:
        res = Key_F8; break;
    case Qt::Key_F9:
        res = Key_F9; break;
    case Qt::Key_F10:
        res = Key_F10; break;
    case Qt::Key_F11:
        res = Key_F11; break;
    case Qt::Key_F12:
        res = Key_F12; break;
    default:
        break;
    }
    return res;
}

}
