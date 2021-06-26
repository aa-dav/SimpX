; Keyboard/Gamepads/Mouse input-output port
portInput       = $FFE0
; Video IO ports
vidVCount       = $FFEE ; счётчик до прерывания HBlank
vidAddr         = $FFEF ; адрес следующей строки в HBlank
vidFlags        = $FFF0 ; флаги
vidBitmapPage   = $FFF1 ; страница битмапа
vidCharmapPage  = $FFF2 ; страница чармапа
vidCharmapAddr  = $FFF3 ; адрес чармапа внутри страницы (кратен 1024, не превышает 8192)
vidScrollX      = $FFF4 ; скроллинг по горизонтали
vidScrollY      = $FFF5 ; скроллинг по вертикали
vidPalPtr       = $FFF6 ; порт адреса слота палитры
vidPalData      = $FFF7 ; порт записи/чтения в/из палитры

; MMU IO ports
mmuPage0        = $FFF8 ; страница в банке 0000-1FFF
mmuPage1        = $FFF9 ; страница в банке 2000-3FFF
mmuPage2        = $FFFA ; страница в банке 4000-5FFF
mmuPage3        = $FFFB ; страница в банке 6000-7FFF
mmuPage4        = $FFFC ; страница в банке 8000-9FFF
mmuPage5        = $FFFD ; страница в банке A000-BFFF
mmuPage6        = $FFFE ; страница в банке C000-DFFF
mmuPage7        = $FFFF ; страница в банке E000-FFFF

; PSW flags
IRQ_ENABLED     = $8000
CPU_HALT        = $4000
CPU_DEBUG       = $2000
CPU_DEBUG_HALT  = $6000
