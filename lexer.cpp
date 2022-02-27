#include "lexer.h"

std::vector<Token> Lexer::lex()
{
    std::vector<Token> tokens;

    qsizetype index = 0;
    qsizetype token_start_index = 0;
    auto token_type = Token::Type::None;

    auto commit_token = [&](){
        if (token_type != Token::Type::None)
        {
            Token token(token_type, m_input.mid(token_start_index, index - token_start_index));
            tokens.push_back(token);
            token_type = Token::Type::None;
        }
    };

    auto start_token = [&](Token::Type type){
        if (type != token_type)
        {
            commit_token();
            token_start_index = index;
            token_type = type;
        }
    };

    while (index < m_input.length())
    {
        if (m_input[index].isSpace())
        {
            index++;
        }
        else if (m_input[index].isNumber() || m_input[index] == '.')
        {
            if (token_type == Token::Type::Minus)
            {
                token_type = Token::Type::NegativeNumber;
                token_start_index++;
            }
            if (token_type == Token::Type::NegativeNumber)
                start_token(Token::Type::NegativeNumber);
            else
                start_token(Token::Type::Number);
            index++;
        }
        else {
            start_token(Token::Type::Unknown);
            bool found_token = false;
            for (const auto& e : m_tokens)
            {
                if (m_input.mid(index, e.first.size()) == e.first)
                {
                    token_type = e.second;
                    index += e.first.size();
                    found_token = true;
                    // commit only if token is not a negation minus
                    if (!(tokens.size() == 0 || tokens.back().is_operator()) || token_type != Token::Type::Minus)
                        commit_token();
                    break;
                }
            }
            if (!found_token) {
                if (m_input[index].isLetter())
                {
                    start_token(Token::Type::String);
                }
                index++;
            }
        }
    }
    commit_token();

    return tokens;
}
