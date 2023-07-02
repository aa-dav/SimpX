#ifndef ALXVM_TEMPLATES
#define ALXVM_TEMPLATES

#include <memory>
#include <string>
#include <functional>
#include <type_traits>
//#include <iostream>

namespace AlxVM
{

template<class T>
struct SizeOf
{
	static const int value = sizeof( T );
};

template<>
struct SizeOf<void>
{
	static const int value = 0;
};

// ******************************************
// *           ArgumentsChecker             *
// ******************************************

template<typename... Args> struct ArgumentsChecker;

template<>
struct ArgumentsChecker<>
{
	static void check( std::shared_ptr< Function > function, int index ) {};
};

template<typename T, typename... Args>
struct ArgumentsChecker<T, Args...>
{
	static void check( std::shared_ptr< Function > function, int index )
	{
		if ( function->getParamSize( index ) != SizeOf<T>::value )
			throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + 
						": wrong param " + std::to_string( index + 1 ) + " size " +
						std::to_string( function->getParamSize( index ) ) + " instead of " + std::to_string( SizeOf<T>::value ) + "." );
		ArgumentsChecker< Args... >::check( function, index + 1 );
	}
};

template< typename... Args >
static void checkFunctionCompatibility( std::shared_ptr< Function > function, int sizeOfReturn )
{
	if ( function->getReturnSize() != sizeOfReturn )
		throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + ": wrong return size " +
					std::to_string( function->getReturnSize() ) + " instead of " + std::to_string( sizeOfReturn ) + "." );
	if ( function->getParamsCount() != sizeof...( Args ) )
		throw std::runtime_error( std::string( "Incompatible function " ) + function->getName() + ": wrong params count " +
					std::to_string( function->getParamsCount() ) + " instead of " + std::to_string( sizeof...( Args ) ) + "." );
	ArgumentsChecker< Args... >::check( function, 0 );
};

// ******************************************
// *             TypedFunction              *
// ******************************************

template< typename... Args > class TypedFunction;

template< class T, typename... Args >
class TypedFunction< T( Args... ) >
{
	std::shared_ptr< Function > function;
	Runtime &runtime;

public:
	TypedFunction( std::shared_ptr< Function > aFunction, Runtime &aRuntime ): function( aFunction ), runtime( aRuntime ) 
	{
		checkFunctionCompatibility< Args... >( function, SizeOf<T>::value );
	};
	TypedFunction( Runtime &aRuntime, const std::string &moduleName, const std::string &functionName ): runtime( aRuntime )
	{
		function = runtime.getFunction( moduleName, functionName );
		checkFunctionCompatibility< Args... >( function, SizeOf<T>::value );
	};
	T operator()( Args...args )
	{
		//std::cout << "T exec(" << sizeof...(Args) << ")\n";
		runtime.startParams( SizeOf<T>::value );
		runtime.addParams( args... );
		runtime.execFunction( function, 0 );
		if constexpr ( !std::is_void<T>::value )
		{
			return runtime.stackAs< T >( 0 );
		}
	}
};

// ******************************************
// *             NativeFunction             *
// ******************************************

template <typename... Args> class NativeFunction;

#define ALX_ARGS( ... ) __VA_ARGS__

#define ALX_FUNCTION_DEF( typenames, paramnames, arguments ) \
template < typenames > \
class NativeFunction< R( paramnames ) >: public NativeFunctionBase \
{ \
	std::function< R( paramnames ) > func; \
public: \
	NativeFunction( const std::function< R( paramnames ) > &aFunc ): func( aFunc ) {}; \
	void invoke( Runtime &runtime, StackPos params, StackPos rp ) override \
	{ \
		if constexpr ( std::is_void<R>::value ) \
			func( arguments ); \
		else \
			runtime.stackAs<R>( rp ) = func( arguments ); \
	} \
};


ALX_FUNCTION_DEF( ALX_ARGS( typename R ), ALX_ARGS(), ALX_ARGS() )

ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1 ), 
	ALX_ARGS( P1 ), 
	ALX_ARGS( 
		runtime.stackAs<P1>(params - sizeof(P1) ) 
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2 ), 
	ALX_ARGS( P1, P2 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P2))
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3 ), 
	ALX_ARGS( P1, P2, P3 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P3))
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3, typename P4 ), 
	ALX_ARGS( P1, P2, P3, P4 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P4) - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P4) - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P4) - sizeof(P3)),
		runtime.stackAs<P4>(params - sizeof(P4))
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3, typename P4, typename P5 ), 
	ALX_ARGS( P1, P2, P3, P4, P5 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P5) - sizeof(P4) - sizeof(P3)),
		runtime.stackAs<P4>(params - sizeof(P5) - sizeof(P4)),
		runtime.stackAs<P5>(params - sizeof(P5)) 
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 ), 
	ALX_ARGS( P1, P2, P3, P4, P5, P6 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3)),
		runtime.stackAs<P4>(params - sizeof(P6) - sizeof(P5) - sizeof(P4)),
		runtime.stackAs<P5>(params - sizeof(P6) - sizeof(P5)),
		runtime.stackAs<P6>(params - sizeof(P6)) 
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 ), 
	ALX_ARGS( P1, P2, P3, P4, P5, P6, P7 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3)),
		runtime.stackAs<P4>(params - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4)),
		runtime.stackAs<P5>(params - sizeof(P7) - sizeof(P6) - sizeof(P5)),
		runtime.stackAs<P6>(params - sizeof(P7) - sizeof(P6)),
		runtime.stackAs<P6>(params - sizeof(P7)) 
		) )
ALX_FUNCTION_DEF( 
	ALX_ARGS( typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 ), 
	ALX_ARGS( P1, P2, P3, P4, P5, P6, P7, P8 ), 
	ALX_ARGS(
		runtime.stackAs<P1>(params - sizeof(P8) - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2) - sizeof(P1)),
		runtime.stackAs<P2>(params - sizeof(P8) - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3) - sizeof(P2)),
		runtime.stackAs<P3>(params - sizeof(P8) - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4) - sizeof(P3)),
		runtime.stackAs<P4>(params - sizeof(P8) - sizeof(P7) - sizeof(P6) - sizeof(P5) - sizeof(P4)),
		runtime.stackAs<P5>(params - sizeof(P8) - sizeof(P7) - sizeof(P6) - sizeof(P5)),
		runtime.stackAs<P6>(params - sizeof(P8) - sizeof(P7) - sizeof(P6)),
		runtime.stackAs<P6>(params - sizeof(P8) - sizeof(P7)),
		runtime.stackAs<P6>(params - sizeof(P8))
		) )

template< typename R, typename... Args >
void bindNativeFunction( Runtime &runtime, const std::string &name, std::function< R( Args... ) > func )
{
	runtime.addNativeFunction( name, new NativeFunction< R( Args... ) >( func ) );
}

template< typename R, typename... Args >
void bindNativeFunction( Runtime &runtime, const std::string &name, R(*func)( Args... ) )
{
	runtime.addNativeFunction( name, new NativeFunction< R( Args... ) >( func ) );
}

};

#endif