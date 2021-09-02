#include "simple_lib.inc"

start		; точка входа (!)
		r0 <- str0    
		call zstrPrint ; напечатаем строку str0

		[ textAttrs ] <- $1000 ; изменим палитру на №1
		r0 <- str1
		call zstrPrint
		[ textAttrs ] <- $0000 ; вернёмся к палитре №0
            
.loop		call inputChar
		void = r0 - '+'
		jz .printLine
		call printSpChar ; выведем символ
		pc <- .loop    ; бесконечный цикл
.printLine	r0 <- str1
		call zstrPrint
		pc <- .loop		
            
forever		psw = psw | CPU_HALT ; переводим процессор в режим ожидания
		pc <- forever

str0		dw "Hello, world!" 13 0
str1		dw "Welcome to future!" 13 0

         
            
