#include "lexer_skeleton.hh"

namespace lexer
{
    token_t::token_t(index_t line, index_t start_col, index_t end_col, token_type type, const std::string& val)
        : _M_line(line), _M_start_col(start_col), _M_end_col(end_col), _M_type(type), _M_val(val)
    {

    }

    lexer::lexer(const std::string& text)
        : _M_pos(0), _M_line(0), _M_col(0), _M_text(text)
    {

    }

    std::vector<token_t> lexer::tokenize()
    {
        std::vector<token_t> tokens;
        token_t tok;
        do
        {
            tok = next_token();
            tokens.push_back(tok);
        } while(_M_pos < _M_text.length());
        tokens.push_back(make_token(token_type::tl_EOF,"$"));
        return tokens;
    }

    token_t lexer::next_token() {

    }

    void lexer::advance()
    {
        if (_M_pos >= _M_text.length())
            return;
        do 
        {
            ++_M_pos;
            if (_M_text[_M_pos] == '\n')
            {
                ++_M_line; 
                _M_col = 0;
            }
            else 
            {
                ++_M_col;
            }
        } while(isspace(_M_text[_M_pos]));
    }

    char lexer::next_character() const
    {
        return (_M_pos >= _M_text.length()) ? 0 : _M_text[_M_pos];
    }

    char lexer::lookahead() const 
    {
        return (_M_pos + 1 >= _M_text.length()) ? 0 : _M_text[_M_pos + 1];
    }

    token_t lexer::make_token(token_type type, const std::string& value)
    {
        index_t start_col = _M_col - value.length();
        return token_t(_M_line, start_col, _M_col, type, value);
    }
} // namespace lexer
