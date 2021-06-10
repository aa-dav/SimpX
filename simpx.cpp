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
    mWord *bitmap = mmu.getPtr( mmu.pageSize * mmu.read( mmu.vidBitmapPage ) );
    mWord *charmap = mmu.getPtr( mmu.pageSize * mmu.read( mmu.vidCharmapPage ) + mmu.read( mmu.vidCharmapAddr ) );

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
    for ( int scan_line = 192; scan_line < 312; scan_line++ )
    {
        while ( cpu.getClocks() - clocks < 2 * 448 )
            cpu.step();
        clocks += 2 * 448; // skip line
    }
}

}
