#include <iostream>
#include "alxvm.h"
#include "alxvm_tokens.h"
#include "alxvm_templates.h"

int sub( int x, int y )
{
	return x - y;
};

int main(int argc, const char *agrv[])
{
	AlxVM::FileTokenizer tokenizer( "test.avil" );	// будем разбирать на токены файл test.avil
	AlxVM::Runtime runtime( 1024 );		// параметр - размер стека

	AlxVM::bindNativeFunction( runtime, "sub", sub );

	runtime.compile(tokenizer);		// компилируем из токенайзера
	//runtime.dump();				// выведем отладочную информацию

	AlxVM::TypedFunction<int(int, int)> funcCallNative2( runtime, "testModule", "callNative2" );
	for ( int i = 1; i <= 10; i ++ )
		std::cout << "callNative(" << i << "): " << funcCallNative2(100, i) << "\n";

	return 0;
}