#include "simpx.inc"

            org $8000
#include "font-00.asm"  ; загружаем битмап шрифта в $8000...

            org $0000
            mode new    ; "математический" режим ассемблера

            psw <- $0000 ; выключим прерывания
            sp <- $8000 ; установим указатель на стек
            [ vidBitmapPage ]   <- 4 ; настраиваем битмап на $8000-BFFF
            [ vidCharmapPage ]  <- 6 ; настраиваем страницу чармапа на $C000
            [ vidCharmapAddr ]  <- 0 ; и адрес в странице на $C000-C3FF
            call simpleInit
            pc <- start ; переходим на начало программы

; palette0 - простая палитра на 16 первых цветов по типу спектрумовской
; перебирает все комбинации вкл/выкл каналов RGB в первых 8 слотах
; и точно то же делает в последних 8 слотах, но с повышенной яркостью                
palette0    ; первые 16 слотов палитры, формат RRRRRGGGGGBBBBB
            ; slot 0
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
            ; slot 1
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
            dw 000001111111111b ; !

; loadPalette - процедура по заполнению слотов палитры
; in:   r0 - адрес палитры в памяти
;       r1 - начальный слот палитры
;       r2 - количество слотов палитры к заполнению
loadPalette 
            [ sp ] <- r0
            [ sp ] <- r1
            [ sp ] <- r2
.loop       [ vidPalPtr ] <- r1
            r1 <- r1 + 1
            [ vidPalData ] <- [ r0 ]
            r0 <- r0 + 1
            r2 <= r2 - 1
            jnz .loop
            r2 <- [ sp ]
            r1 <- [ sp ]
            r0 <- [ sp ]
            ret  

; simpleInit - инициализация простой тестовой среды выполнения
simpleInit
            [ sp ] <- r0
            [ sp ] <- r1
            [ sp ] <- r2
            ; загрузим простую палитру
            r0 <- palette0
            r1 <- 0                
            r2 <- 32
            call loadPalette
            ; инициализируем текстовую раскладку видеоданных
            call initTextMode
            r2 <- [ sp ]
            r1 <- [ sp ]
            r0 <- [ sp ]
            ret
            
textPos     dw 0
textEnd     dw ( 24 * 32 )
textAttrs   dw 0
textBase    = $C000

; initTextMode - инициализируем "текстовый режим"
; на самом деле в SimpX это не более чем упорядоченная определенным образом 
; раскладка видеоданных (чармап заполнен не линейно как в графическом режиме, а каждый
; его элемент и есть символ и его атрибуты цветности)
initTextMode
            [ sp ] <- r0
            [ sp ] <- r1
            r0 <- 0
            [ vidScrollX ] <- r0
            [ vidScrollY ] <- r0
            [ textAttrs ] <- r0
            [ textPos ] <- r0
            [ textEnd ] <- ( 24 * 32 )
            r0 <- $C000
            r1 <- 1024
.loop       [ r0 ] <- 0
            r0 <- r0 + 1
            r1 <= r1 - 1
            jnz .loop
            r1 <- [ sp ]
            r0 <- [ sp ]
            ret

; printChar - печать символа в r0 (ascii) в текстовом режиме
; in: r0 - символ для печати
printChar
            [ sp ] <- r0
            [ sp ] <- r1
            [ sp ] <- r2
            r1 <- [ textPos ]
            void = r0 - 13
            jnz .not_cr
            ; carriage return
            r2 = textBase + r1
.cr_loop    [ r2 ] <- [ textAttrs ]
            r2 <- r2 + 1
            r1 <- r1 + 1
            void = r1 & 11111b
            jnz .cr_loop
            pc <- .check_ovfl
.not_cr     r0 = r0 +s -32 ; ascii fix
            r0 = r0 | [ textAttrs ] ; apply attrs
            r2 = textBase + r1
            [ r2 ] <- r0
            r1 <- r1 + 1
.check_ovfl r1 = r1 & $3FF
            [ textPos ] <- r1
            r2 <- [ textEnd ]
            void = r2 - r1
            jnz .end
            ; overflow
            [ vidScrollY ] = [ vidScrollY ] +s 8
            r0 <- 32
            r1 = textBase + r2
.clear_loop [ r1 ] <- 0
            r1 <- r1 + 1
            r2 <- r2 + 1
            r0 <= r0 - 1
            jnz .clear_loop
            r2 = r2 & $3FF
            [ textEnd ] <- r2
.end        r2 <- [ sp ]
            r1 <- [ sp ]
            r0 <- [ sp ]
            ret

hexNumbers  dw "0123456789ABCDEF"                        

; printHex - печать числа в r0 как hex            
; in: r0 - число для печати
printHex                
            [ sp ] <- r0
            [ sp ] <- r1
            r1 <- r0

            r1 <- r1 >> 12
            r0 = r1 & $000F
            r0 = hexNumbers + r0
            r0 <- [ r0 ]
            call printChar

            r1 <- r1 >> 12
            r0 = r1 & $000F
            r0 = hexNumbers + r0
            r0 <- [ r0 ]
            call printChar

            r1 <- r1 >> 12
            r0 = r1 & $000F
            r0 = hexNumbers + r0
            r0 <- [ r0 ]
            call printChar

            r1 <- r1 >> 12
            r0 = r1 & $000F
            r0 = hexNumbers + r0
            r0 <- [ r0 ]
            call printChar
                                                
            r1 <- [ sp ]
            r0 <- [ sp ]
            ret

; zstrPrint - напечатать строку терминирующуюся нулём
; in: r0 - указатель на строку                        
zstrPrint   [ sp ] <- r0
            [ sp ] <- r1
            r1 <- r0
.loop       r0 <= [ r1 ]
            jz .end
            call printChar
            r1 <- r1 + 1
            pc <- .loop
.end        r1 <- [ sp ]
            r0 <- [ sp ]
            ret           


