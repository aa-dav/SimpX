# SimpX

SimpX is (virtual) 16-bit PC based on (virtual) Simpleton 4 CPU: 

[Simpleton 4 short description (english)](README_simpleton_en.md)

[Simpleton 4 short description (russian)](README_simpleton_ru.md)

Online emulator+assembler: http://alxhost.tk/SimpX/SimpX.html
(select 'test0.asm' in list and click menu 'Compile and run')

## Memory

64Kw (kilowords, or 128Kb) of address space are divided by 8 banks of 8192 words each. 
Memory Managment Unit (MMU) maps 8Kw pages to CPU address space banks.
Last 8 words of CPU address space works as input-output ports of these mappings:
```
; MMU IO ports
mmuPage0        = $FFF8 ; page in bank 0000-1FFF
mmuPage1        = $FFF9 ; page in bank 2000-3FFF
mmuPage2        = $FFFA ; page in bank 4000-5FFF
mmuPage3        = $FFFB ; page in bank 6000-7FFF
mmuPage4        = $FFFC ; page in bank 8000-9FFF
mmuPage5        = $FFFD ; page in bank A000-BFFF
mmuPage6        = $FFFE ; page in bank C000-DFFF
mmuPage7        = $FFFF ; page in bank E000-FFFF
```
Writes to port mmuPageX switches page in corresponging bank.
That is SimpX with this MMU could address 65536 pages of 8192 words each, that is - 1Gb of RAM/ROM.
Current (virtual) model supports 64 pages / 524288 word / 1Mb.
At startup of PC mmuPageX are initialized to 0,1,2...6,7.

## Video

(...to be done...)

Every frame consists of 312 lines: 192 active (VDraw) and 120 inactive lines (borders and VBlank).
Each line consists of 256 active pixels (HDraw) and 192 inactive pixels (borders and HBlank) - 448 in sum.
Video chip accesses main memory at maximum speed of two reads/writes per pixel, that is ~14MHz.
14MHz is also CPU frequency, so memory must work at 28MHz.

## Interrupts

Video system generates interrupt signal at the end of the VDraw (entering VBlank). CPU enters interrupt handler if IRQ_ENABLE flag in psw is set.
If HALT flag is set CPU stops executing commands until interrupt signal arrives. Interrupt signal resets HALT flag in psw despite of IRQ_ENABLE state.
```
; PSW flags
IRQ_ENABLED     = $8000
CPU_HALT        = $4000
CPU_DEBUG       = $2000
CPU_DEBUG_HALT  = $6000
```
So, 
```
psw = psw | FLAG_HALT
```
will stop CPU until interrupt signal (HBlank in SimpX).
CPU_DEBUG and CPU_DEBUG_HALT are used for naive debugging right now and will be changed in future.
