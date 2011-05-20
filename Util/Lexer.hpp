#pragma once

#include "LexToken.hpp"

#include <vector>
namespace cge {	
	class Lexer {
	public:
		typedef unsigned         State;
		typedef unsigned         Input;
		typedef LexTokenType     TokenType;
		typedef LexToken::String String;
		typedef LexToken::Char   Char;

		Lexer();
		virtual ~Lexer();

		void reset();
		bool readNext( LexToken &tk, std::istream &is );

		bool isCompiled() const { return state_ != S_NOT_COMPILED; }

		struct AlreadyCompiledException {};
		struct NotCompiledException     {};
		struct InvalidRuleException     {};
		struct IncorrectTextException   {};

	protected:
		static const TokenType T_UNCHANGED    = -1;
		static const TokenType T_UNDEFINED    = -1;
		static const State     S_UNCHANGED    = -1;
		static const State     S_NOT_COMPILED = -1;
		static const State     S_INITIAL      = 0;
		static const unsigned  CHAR_COUNT     = 128;
		static const Char      I_EOF          = 26;
		static const Char      I_DEFAULT      = 0;

		enum CharAction { A_ADD, A_IGNORE, A_PUTBACK, A_ERROR };
		enum BufferAction { A_UNCHANGED, A_CLEAR, A_RETURN };

		struct Action {
			Action( CharAction   charAction,
					State        toState      = S_UNCHANGED,
					TokenType    newTokenType = T_UNCHANGED,
					BufferAction bufferAction = A_UNCHANGED );

			Action( const Action &rhs );
			Action& operator=( const Action &rhs );

			CharAction   charAction;
			TokenType    newTokenType;
			State        toState;
			BufferAction bufferAction;
		};

		static const String& getWhiteSpace();
		static const String& getWhiteSpaceEof();
		static const String& getDigits();
		static const String& getLetters();
		static const String& getAlphanumerics();
		static const String& getSymbols();

		Input createClass( const String& chars );

		bool  isValidInput( Input input );
		bool  isClass( Input input );

		void  addRule( State inState, Input input, Action action );

		void  compile();

	private:
		struct Rule { State inState; Input input; Action action; };

		void    createTable_();
		Action& getAction_( State s, Char ch );
		String& getClass_( Input inp );

		std::vector< Action > table_;
		std::vector< Rule >   rules_;
		std::vector< String > classes_;

		unsigned nStates_;
		unsigned nClasses_;
	
		State    state_;
	};
}