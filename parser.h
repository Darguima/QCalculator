#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define ENUMERATE_OPERATION_TYPE \
    TOKEN(Addition) \
    TOKEN(Subtraction) \
    TOKEN(Multiplication) \
    TOKEN(Division)

struct Term;

struct Operation {
    enum class Type {
#define TOKEN(x) x,
        ENUMERATE_OPERATION_TYPE
#undef TOKEN
    };

    const char* to_string() const
    {
        switch(m_type)
        {
#define TOKEN(x) \
        case Type::x: \
            return #x;

        ENUMERATE_OPERATION_TYPE
#undef TOKEN
        }
    }

    Operation(Type type, Term* term1, Term* term2)
        : m_type(type), m_term1(term1), m_term2(term2)
    {}

    Type m_type;
    Term* m_term1;
    Term* m_term2;
};

struct BigNum
{
    BigNum(QString string, bool negative)
        : m_string(string), m_negative(negative) {}

    QString m_string;
    bool m_negative;
};

struct Term
{
    enum class Type {
        Number, Operation
    };

    Term(BigNum number)
        : m_type(Type::Number),
          m_number(number) {}

    Term(Operation operation)
        : m_type(Type::Operation),
          m_operation(operation) {}

    Type m_type;

    union {
        BigNum m_number;
        Operation m_operation;
    };
};

class Parser
{
public:
    Parser(std::vector<Token> tokens)
        : m_tokens(tokens) {}
    Term* parse();

private:
    std::vector<Token> m_tokens;
    const std::vector<std::unordered_map<Token::Type, Operation::Type>> m_precedences {
        {
            {Token::Type::Plus, Operation::Type::Addition},
            {Token::Type::Minus, Operation::Type::Subtraction},
        },
        {
            {Token::Type::Multiply, Operation::Type::Multiplication},
            {Token::Type::Divide, Operation::Type::Division}
        }
    };
};

#endif // PARSER_H
