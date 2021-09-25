; ************************************************************
; * test03.asm - вывод таблицы умножения (функция умножения) *
; ************************************************************


#include "simple_lib.inc"

col		dw 0	; текущая колонка
row		dw 0	; текущая строка

start		; напечатаем таблицу умножения
		
		[ row ] <- 1	; инициализируем строку
loop_row		[ col ] <- 1	; инициализируем колонку
loop_col		r0 <- [ row ]	; загрузим строку и колонку в r0 и r1
		r1 <- [ col ]				
		call r0_mul_r1	; и перемножим их
		r1 <- r0		; сохраним r0 в r1
		void = r0 - 10	; сравним итоговое число с десяткой
		jnc skip_space	; если не возникло заёма - значит идём на печать числа		
		r0 <- ' '		; иначе для выраванивания надо напечатать пробел
		call printChar		
skip_space	r0 <- r1		; восстановим результат умножения из r1
		call printNum	; напечатаем результат
		r0 <- ' '		; печатаем пробел для выравнивания
		call printChar			
		[ col ] <- [ col ] + 1	; переходим к следующей колонке
		void = [ col ] - 11		; проверяем не получилось ли 11
		jnz loop_col		; если нет - повторяем цикл по колонке		
		call printCr		; напечатаем перевод строки
		[ row ] <- [ row ] + 1	; переходим к следующей строке
		void = [ row ] - 11		; проверяем не получилось ли 11
		jnz loop_row		; если нет - повторяем цикл по строке

		; иначе зацикливаемся в энергосберегающем режиме
		; (энергосберегаемость тут необязательна)		
forever		psw = psw | CPU_HALT	; переводим процессор в режим ожидания прерывания
		pc <- forever		; зацикливаемся навечно