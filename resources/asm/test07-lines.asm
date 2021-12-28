; *************************************************************
; * test06.asm - вывод паттерна из линий в графическом режиме *
; *************************************************************

#include "simple_lib.inc"

start		call initGraphicMode	; инициализируем графический режим

		; заполняем экранную область возрастающими числами
		r0 <- bitmapBase	; r0 - начало экрана
		r1 <- bitmapSize	; r1 - размер экрана
		r2 <- 0		; зануляем r2 (могло бы быть r2 = r2 - r2)
.loop1		[ r0 ] <- r2	; записываем текущий r2
		r0 <- r0 + 1	; инкремент r0
		r2 <- r2 + 1	; инкремент r2
		r1 <= r1 - 1	; декремент r1 с влияением на флаги
		jnz .loop1	; если не ноль - повторяем

		; 
.repeat		r0 <- bitmapBase	; r0 - начало экрана
		r1 <- bitmapSize	; r1 - размер экрана
		r2 <- 0		; зануляем r2
		; прибавляем к полоске пикселей 1x4 из битмапа r2
.loop		[ r0 ] = [ r0 ] + r2
		r0 <- r0 + 1	; инкрементируем r0
		r2 = r2 + $1	; увеличиваем r2 на 1
		void = r2 - 25	; проверяем не стал ли r2 равен 25
		jnz .next		; если нет, то идём на .next
		r2 = r2 - r2	; иначе обнуляем r2
.next		r1 <= r1 - 1	; декрементируем r1 с проверкой на 0
		jnz .loop		; если не 0, то идём на новую итерацию
		; ждём четыре кадра
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		pc <- .repeat	; повторяем процедуру с самого начала
		

		