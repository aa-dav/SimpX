                mode new    ; "математический" режим ассемблера

; Video IO ports
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


                sp <- $7FFF ; установим указатель на стек
                psw <- $8000 ; включим прерывания
                pc <- start ; переходим на начало

                org $0010   ; IRQ handler

                [ $8000 ] <- [ $8000 ] + 1
                psw <- [ sp ]
                pc <- [ sp ]

palette         ; первые 16 слотов палитры, формат RRRRRGGGGGBBBBB
                dw 000000000000000b
                dw 000000000010000b
                dw 000001000000000b
                dw 000001000010000b
                dw 100000000000000b
                dw 100000000010000b
                dw 100001000000000b
                dw 100001000010000b
                dw 011110111101111b
                dw 000000000011111b
                dw 000001111100000b
                dw 000001111111111b
                dw 111110000000000b
                dw 111110000011111b
                dw 111111111100000b
                dw 111111111111111b

start
                ; настроим видеобитмап на страницу 4
                ; (по умолч. она замаплена на $8000)
                [ vidBitmapPage ]   <- 4
                ; настроим карту символов на страницу 6
                ; (по умолч. она замаплена на $C000)
                [ vidCharmapPage ]  <- 6
                ; адрес карты символов внутри страницы
                ; (кратен 1024, не превышает 8192)
                [ vidCharmapAddr ]  <- 0

                ; вызовем процедуру заполнения палитры
                r0 <- palette
                r1 <- 0
                r2 <- 16
                call loadPalette

                ; зальём первый символ цветастым паттерном
                r0 <- $8000
                r1 <- 8
                r2 <- $F274
@loop3          [ r0 ] <- r2
                r0 <- r0 + 1
                [ r0 ] <- r2
                r0 = r0 +s (64 - 1)
                r1 <= r1 - 1
                jnz @loop3

                ; чтобы увидеть как инициализируется чармап
                r0 <- $C000
                r1 <- 1024
                r2 <- 0
@loop2          [ r0 ] <- r2
                r0 <- r0 + 1
                r2 <- r2 + 1
                r1 <= r1 - 1
                ; psw = psw | $4000 ; halt
                jnz @loop2

                ; зальём битмап увеличивающимися числами
                r0 <- $8000
                r1 <- 16384
                r2 <- $CCCC
@loop1          [ r0 ] <- r2
                r0 <- r0 + 1
                r2 <- r2 + 1
                r1 <= r1 - 1
                jnz @loop1


loop_scroll     psw = psw | $4000
                ; увеличим координаты скроллинга
                [ vidScrollX ] = [ vidScrollX ] +s 1
                [ vidScrollY ] = [ vidScrollY ] +s 1
                pc <- loop_scroll

; loadPalette - процедура по заполнению слотов палитры
; in:   r0 - адрес палитры в памяти
;       r1 - начальный слот палитры
;       r2 - количество слотов палитры
loadPalette     [ vidPalPtr ] <- r1
                r1 <- r1 + 1
                [ vidPalData ] <- [ r0 ]
                r0 <- r0 + 1
                r2 <= r2 - 1
                jnz loadPalette
                ret













