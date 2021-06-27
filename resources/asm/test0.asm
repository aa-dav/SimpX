#include "simple_lib.inc"

start       
            r0 <- str0    
            call zstrPrint ; напечатаем строку str0
            
            [ textAttrs ] <- $1000 ; изменим палитру на №1
            r0 <- str1
            call zstrPrint
            [ textAttrs ] <- $0000 ; вернёмся к палитре №0
            
.loop
            [ portInput ] <- 1
            r0 <- [ portInput ]
            call printHex  ; выведем его на экран
            r0 <- 32       ; код пробела
            call printChar ; напечатаем пробел

            [ portInput ] <- 2
            r0 <- [ portInput ]
            call printHex  ; выведем его на экран
            r0 <- 32       ; код пробела
            call printChar ; напечатаем пробел

            [ portInput ] <- 4
            r0 <- [ portInput ]
            call printHex  ; выведем его на экран
            r0 <- 32       ; код пробела
            call printChar ; напечатаем пробел

            [ portInput ] <- 8
            r0 <- [ portInput ]
            call printHex  ; выведем его на экран
            r0 <- 32       ; код пробела
            call printChar ; напечатаем пробел

            [ portInput ] <- 256
            r0 <- [ portInput ]
            call printHex  ; выведем его на экран
            r0 <- 13
            call printChar ; перевод строки

            psw = psw | CPU_HALT ; выждем паузу
            ;psw = psw | CPU_HALT ; выждем паузу
            ;psw = psw | CPU_HALT ; выждем паузу
            ;psw = psw | CPU_HALT ; выждем паузу

            pc <- .loop     ; бесконечный цикл
            
forever     psw = psw | CPU_HALT ; переводим процессор в режим ожидания
            pc <- forever

str0        dw "Hello, world!" 13 0
str1        dw "Welcome to future!" 13 0

         
            
