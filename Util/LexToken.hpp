#pragma once

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace cge {
	class LexToken {
	public:
		typedef unsigned int Type;
		typedef char         Char;
		typedef std::string  String;

		LexToken() { t_ = -1; }
		LexToken( Type type, String str ): t_(type), s_(str) {}
		LexToken( const LexToken& tok ): t_(tok.t_), s_(tok.s_) {}

		Type   getType() const { return t_; }
		bool   hasType( Type t ) const { return t_ == t; }

		const String& getString() const { return s_; }

		template<class T>
		boost::optional<T> get() {
			boost::optional<T> value;
			try { value = boost::lexical_cast<T>(s_); } catch( boost::bad_lexical_cast& ) {}
			return value;
		}

	private:
		friend class Lexer;

		Type   t_;
		String s_;
	};

	typedef LexToken::Type LexTokenType;
}