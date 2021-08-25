#include "simple_lib.inc"

start
		r0 <- str0    
		call zstrPrint ; напечатаем строку str0

		[ textAttrs ] <- $1000 ; изменим палитру на №1
		r0 <- str1
		call zstrPrint
		[ textAttrs ] <- $0000 ; вернёмся к палитре №0
            
.loop		
		call readKey
		r0 <= r0
		jz .loop
		r0 = r0 +s ( keysToChars - 1 )
		r0 = [ r0 ] & $FF
		call printChar ; выведем символ

		psw = psw | CPU_HALT ; выждем паузу
		;psw = psw | CPU_HALT ; выждем паузу
		;psw = psw | CPU_HALT ; выждем паузу
		;psw = psw | CPU_HALT ; выждем паузу

		pc <- .loop     ; бесконечный цикл
            
forever		psw = psw | CPU_HALT ; переводим процессор в режим ожидания
		pc <- forever

str0		dw "Hello, world!" 13 0
str1		dw "Welcome to future!" 13 0

         
            
