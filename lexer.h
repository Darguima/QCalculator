#ifndef LEXER_H
#define LEXER_H

#include <QString>
#include <vector>
#include <QMap>

#define ENUMERATE_TOKEN_TYPE \
    TOKEN(None) \
    TOKEN(Unknown) \
    TOKEN(Number) \
    TOKEN(NegativeNumber) \
    TOKEN(String) \
    TOKEN(Plus) \
    TOKEN(Minus) \
    TOKEN(Multiply) \
    TOKEN(Divide) \
    TOKEN(Pow) \
    TOKEN(StartParen) \
    TOKEN(EndParen) \
    TOKEN(Comma)

class Token {
public:
    enum class Type {
#define TOKEN(x) x,
        ENUMERATE_TOKEN_TYPE
#undef TOKEN
    };

    const char* to_string() const
    {
        switch(m_type)
        {
#define TOKEN(x) \
        case Type::x: \
            return #x;

        ENUMERATE_TOKEN_TYPE
#undef TOKEN
        }
    }

    Token(Type type, QString view)
        : m_type(type),
          m_view(view)
    {
    }

    Token(Type type)
        : m_type(type),
          m_view("")
    {
    }

    bool operator==(const Token& right) const
    {
        return m_type == right.type();
    }

    bool is_operator() const {
        return m_type >= Type::Plus && m_type <= Type::Pow;
    }

    Type type() const { return m_type; }
    QString view() const { return m_view; }

private:
    Type m_type;
    QString m_view;
};

class Lexer
{
public:
    Lexer(QString input)
        : m_input(input) {}

    std::vector<Token> lex();

private:
    QString m_input;
    const std::unordered_map<QString, Token::Type> m_tokens {
        {"+", Token::Type::Plus},
        {"-", Token::Type::Minus},
        {"x", Token::Type::Multiply},
        {"/", Token::Type::Divide},
        {"^", Token::Type::Pow},
        {"(", Token::Type::StartParen},
        {")", Token::Type::EndParen},
        {",", Token::Type::Comma}
    };
};

#endif // LEXER_H
