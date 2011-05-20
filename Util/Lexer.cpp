#include "Lexer.hpp"

#include <boost/foreach.hpp>
#include <cassert>

#include <iostream>

#define foreach BOOST_FOREACH
#define max(a,b) ((a)>(b)?(a):(b))

namespace cge {
	Lexer::Lexer() {
		nStates_  = 1;
		nClasses_ = 0;
		state_    = S_NOT_COMPILED;

		classes_.reserve(4);
	}
	
	Lexer::~Lexer() {
	
	}

	bool Lexer::readNext( LexToken &tk, std::istream &is ) {
		bool reading = true;
		char ch;
		tk.t_ = T_UNDEFINED;
		tk.s_.clear();

		while( reading ) {
			ch = is.get();
			/*std::cout << "char: " << ch;
			std::cout << " tok: " << tk.t_;
			std::cout << " state: " << state_;
			std::cout << " str: " << tk.s_;
			std::cout << std::endl;*/
			
			
			if( is.bad() || is.eof() ) {
				ch = I_EOF;
				reading = false;
			}

			const Action& a = getAction_(state_,ch);
			switch( a.charAction ) {
				case A_ADD:
					tk.s_ += ch;
					break;
				
				case A_PUTBACK:
					if(ch != I_EOF )
						is.putback(ch);
					break;
				
				case A_IGNORE:
					break;
				
				case A_ERROR:
					tk.t_ = T_UNDEFINED;
					tk.s_.clear();
					reset();
					throw IncorrectTextException();
					break;
			}

			switch( a.bufferAction ) {
				case A_UNCHANGED: break;
				case A_CLEAR: tk.s_.clear(); break;
				case A_RETURN: reading = false; break;
			}

			if( a.newTokenType != T_UNCHANGED )
				tk.t_ = a.newTokenType;

			state_ = a.toState;
		}

		if( ch == I_EOF )
			return false;
		else
			return true;
	}
	
	Lexer::Input Lexer::createClass( const String& chars ) {
		classes_.push_back(chars);
		return CHAR_COUNT + nClasses_++;
	}

	bool Lexer::isValidInput( Input input ) {
		if( input < CHAR_COUNT )
			return true;
		
		input -= CHAR_COUNT;
		if( input < nClasses_ )
			return true;

		return false;
	}

	void Lexer::addRule( State inState, Input input, Action action ) {
		if( isCompiled() )
			throw AlreadyCompiledException();

		if( inState == S_UNCHANGED || !isValidInput(input) )
			throw InvalidRuleException();

		if( inState >= nStates_ )
			nStates_ = inState + 1;

		if( action.toState == S_UNCHANGED )
			action.toState = inState;
		else if( action.toState >= nStates_ )
			nStates_ = action.toState + 1;

		Rule r = { inState, input, action };
		rules_.push_back(r);
	}

	Lexer::Action& Lexer::getAction_( State s, Char ch ) {
		return table_.at(s*CHAR_COUNT + ch);
	}

	void Lexer::createTable_() {
		table_.clear();
		table_.resize( CHAR_COUNT*nStates_, Action(A_ERROR) );
	}

	bool Lexer::isClass( Input input ) {
		return input >= CHAR_COUNT && input < CHAR_COUNT+nClasses_;
	}

	Lexer::String& Lexer::getClass_( Input inp ) {
		return classes_.at( inp - CHAR_COUNT );
	}

	void Lexer::reset() {
		state_ = S_INITIAL;
	}

	void Lexer::compile() {
		if( isCompiled() )
			throw AlreadyCompiledException();

		createTable_();

		foreach(Rule &r, rules_) {
			assert( r.inState != S_UNCHANGED );
			assert( isValidInput(r.input) );
			assert( r.action.toState != S_UNCHANGED );
			
			if( isClass(r.input) ) {
				String& cl = getClass_(r.input);
				foreach( char ch, cl )
					getAction_(r.inState, ch) = r.action;
			} else if( r.input == I_DEFAULT )
				for(Char i=0; i<CHAR_COUNT; ++i)
					getAction_(r.inState, i) = r.action;
			else
				getAction_(r.inState, r.input) = r.action;
		}

		rules_.swap( std::vector<Rule>() );
		classes_.swap( std::vector<String>() );

		reset();
	}

	Lexer::Action::Action( CharAction ca, State ts, TokenType ntt, BufferAction ba ) {
		charAction   = ca;
		newTokenType = ntt;
		toState      = ts;
		bufferAction = ba;
	}
	
	Lexer::Action::Action( const Action &a ) {
		charAction   = a.charAction;
		newTokenType = a.newTokenType;
		toState      = a.toState;
		bufferAction = a.bufferAction;
	}

	Lexer::Action& Lexer::Action::operator=( const Action &a ) {
		charAction   = a.charAction;
		newTokenType = a.newTokenType;
		toState      = a.toState;
		bufferAction = a.bufferAction;
		return *this;
	}
	

	const Lexer::String& Lexer::getWhiteSpace() {
		static const std::string chars(" \t\n");
		return chars;
	}

	const Lexer::String& Lexer::getWhiteSpaceEof() {
		static const std::string chars(getWhiteSpace() + I_EOF);
		return chars;
	}
	
	const Lexer::String& Lexer::getDigits() {
		static const std::string chars("0123456789");
		return chars;
	}
	
	const Lexer::String& Lexer::getLetters() {
		static const std::string chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
		return chars;
	}
	
	const Lexer::String& Lexer::getAlphanumerics() {
		static const std::string chars(getDigits() + getLetters() + '_');
		return chars;
	}
	
	const Lexer::String& Lexer::getSymbols() {
		static const std::string chars("~!@#$%^&*()-+{}[]:\"\\<>?,./");
		return chars;
	}
}