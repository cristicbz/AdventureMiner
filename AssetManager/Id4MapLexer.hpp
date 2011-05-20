#include "Util/Lexer.hpp"

using namespace cge;
namespace cge {
	class Id4MapLexer: public cge::Lexer {
	public:
		enum { T_WORD, T_STRING, T_INT, T_DOUBLE, T_SYMBOL };

		Id4MapLexer();

		static bool isString( const LexToken &t ) { return t.hasType(T_STRING); }
		static bool isWord( const LexToken &t )   { return t.hasType(T_WORD); }
		static bool isDouble( const LexToken &t ) { return t.hasType(T_DOUBLE); }
		static bool isLParen( const LexToken &t ) { return t.getString()[0] == '('; }
		static bool isRParen( const LexToken &t ) { return t.getString()[0] == ')'; }
		static bool isLBrace( const LexToken &t ) { return t.getString()[0] == '{'; }
		static bool isRBrace( const LexToken &t ) { return t.getString()[0] == '}'; }

	private:
		enum {
			S_START, S_WORD, S_SYMBOL,
			S_MINUS, S_INT, S_DECIMAL, S_EXPONENT, S_DOT, S_E, S_E_MINUS,
			S_STRING, S_ESCAPE
		};
	};
}