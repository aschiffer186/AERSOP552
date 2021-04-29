#ifndef LEXER_SKELETON_HH
#define LEXER_SKELETON_HH 1

#include <string>
#include <vector>
#include <ostream>

namespace lexer
{
    enum class token_type {

    };

    typedef size_t index_t;

    struct token_t
    {
        index_t _M_line;
        index_t _M_start_col;
        index_t _M_end_col;
        token_type _M_type;
        std::string _M_val;
        token_t() = default;
        token_t(index_t line, index_t start_col, index_t end_col, token_type type, const std::string& val);
    };

    std::ostream& operator<<(std::ostream& os, const token_t& t); 

    class lexer
    {
        public: 
            explicit lexer(const std::string& text);

            token_t next_token();

            std::vector<token_t> tokenize(); 
        private:
            void advance();

            char next_character() const;

            char lookahead() const;

            token_t make_token(token_type type, const std::string& value);
        private:
            index_t _M_pos;
            index_t _M_line;
            index_t _M_col;
            std::string _M_text;
    };
}

#endif