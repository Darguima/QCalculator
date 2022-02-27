#include "parser.h"

Term* Parser::parse()
{
    if (m_tokens.size() >= 2)
    {
        if (m_tokens.front().type() == Token::Type::StartParen && m_tokens.back().type() == Token::Type::EndParen && std::count(m_tokens.begin(), m_tokens.end(), Token(Token::Type::EndParen)) == 1)
        {
            m_tokens.erase(m_tokens.begin());
            m_tokens.erase(m_tokens.end());
        }
    }

    int block_count = 0;
    int token_index = m_tokens.size() - 1;
    Operation::Type operation_type;
    int precedence_index = -1;
    for (int j = m_tokens.size() - 1; j >= 0; j--)
    {
        const auto& current_token = m_tokens[j];

        if (current_token.type() == Token::Type::EndParen)
            block_count++;
        else if (current_token.type() == Token::Type::StartParen)
            block_count--;
        else if (current_token.is_operator() && block_count == 0)
        {
            for (size_t i = 0; i < m_precedences.size(); i++)
            {
                bool found = false;
                const auto& precedence = m_precedences[i];
                for (const auto& operation : precedence)
                {
                    if (operation.first == current_token.type() && ((int)i < precedence_index || precedence_index == -1))
                    {
                        found = true;
                        precedence_index = i;
                        token_index = j;
                        operation_type = operation.second;
                        break;
                    }
                }
                if (found) break;
            }
        }
    }

    const auto token = m_tokens[token_index];
    if (token.type() == Token::Type::Number)
    {
        return new Term(BigNum(token.view(), false));
    }
    else if (token.type() == Token::Type::NegativeNumber)
    {
        return new Term(BigNum(token.view(), true));
    }
    else if (token.is_operator())
    {
        const auto term1 = Parser(std::vector(m_tokens.begin(), m_tokens.begin() + token_index)).parse();
        const auto term2 = Parser(std::vector(m_tokens.begin() + token_index + 1, m_tokens.end())).parse();
        return new Term(Operation(operation_type, term1, term2));
    }

    return nullptr;
}
