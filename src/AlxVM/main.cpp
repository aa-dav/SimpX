#include <iostream>
#include "alxvm.h"
#include "alxvm_tokens.h"

int main(int argc, const char *agrv[])
{
	AlxVM::FileTokenizer tokenizer( "test.avil" );
	AlxVM::Runtime runtime( 1024 );		// параметр - размер стека в байтах
	runtime.compile(tokenizer);
	runtime.dump();
	std::shared_ptr<AlxVM::Function> funcMax = runtime.getFunction("test", "max");	// модуль, имя функции
	runtime.startParams(4);			// параметр - позиция в стеке начиная с которой будут размещены параметры
	runtime.addParam<int32_t>( 40 );
	runtime.addParam<int32_t>( -10 );
	runtime.execFunction(funcMax, 0);	// второй параметр - позиция в стеке где будет сохранён результат
	std::cout << "Result is: " << runtime.stackAs<int32_t>(0) << "\n";
}