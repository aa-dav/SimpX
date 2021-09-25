#include "simple_lib.inc"

start		call initGraphicMode	; инициализируем графический режим

		r0 <- bitmapBase
		r1 <- bitmapSize
		r2 <- 0
.loop1		[ r0 ] <- r2
		r0 <- r0 + 1
		r2 = r2 + 1
		r1 <= r1 - 1
		jnz .loop1

.repeat		r0 <- bitmapBase
		r1 <- bitmapSize
		r2 <- 0
.loop		[ r0 ] = [ r0 ] +s r2
		r0 <- r0 + 1
		r2 = r2 + $1
		void = r2 - 25
		jnz .next
		r2 = r2 - r2
.next		r1 <= r1 - 1
		jnz .loop
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		psw = psw | CPU_HALT
		pc <- .repeat
		
		
.forever		psw = psw | CPU_HALT	; пауза
		pc <- .forever		; зацикливаемся
		