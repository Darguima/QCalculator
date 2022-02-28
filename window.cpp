#include "window.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>

QString removeLastChar(QString string) {
    int charCount = string.length();

    return string.left(charCount - 1);
}

Window::Window(QWidget *parent)
    : QWidget{parent}
{
    QIcon eraserIcon(QPixmap(":/images/eraser"));

    setFixedSize(525, 500);
    setWindowTitle("QCalculator");

    calculatorScreen = new QLineEdit(this);
    calculatorScreen->setGeometry(25, 25, 475, 50);

    for (int buttonNum = 1; buttonNum <= 9; ++buttonNum) {
        numberButtons[buttonNum] = new QPushButton(this);
        numberButtons[buttonNum]->setGeometry(
            25 + (100) * ((buttonNum - 1) % 3),
            100 + (100) * ((buttonNum - 1) / 3),
            75, 75
            );\
        numberButtons[buttonNum]->setText(QString::number(buttonNum));
        connect(numberButtons[buttonNum], SIGNAL(clicked()), this, SLOT(buttonPress()));
    }

    numberButtons[0] = new QPushButton(this);
    numberButtons[0]->setGeometry(125, 400, 75, 75);
    numberButtons[0]->setText("0");
    connect(numberButtons[0], SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonPoint = new QPushButton(this);
    buttonPoint->setGeometry(25, 400, 75, 75);
    buttonPoint->setText(".");
    buttonPoint->setStyleSheet("background-color: #51575e");
    connect(buttonPoint, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonDelete = new QPushButton(this);
    buttonDelete->setGeometry(225, 400, 75, 75);
    buttonDelete->setIcon(eraserIcon);
    buttonDelete->setStyleSheet("background-color: red;");
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonSum = new QPushButton(this);
    buttonSum->setGeometry(325, 100, 75, 75);
    buttonSum->setText("+");
    buttonSum->setStyleSheet("background-color: #51575e");
    connect(buttonSum, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonSub = new QPushButton(this);
    buttonSub->setGeometry(325, 200, 75, 75);
    buttonSub->setText("-");
    buttonSub->setStyleSheet("background-color: #51575e");
    connect(buttonSub, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonMultiplication = new QPushButton(this);
    buttonMultiplication->setGeometry(325, 300, 75, 75);
    buttonMultiplication->setText("x");
    buttonMultiplication->setStyleSheet("background-color: #51575e");
    connect(buttonMultiplication, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonDivision = new QPushButton(this);
    buttonDivision->setGeometry(325, 400, 75, 75);
    buttonDivision->setText("/");
    buttonDivision->setStyleSheet("background-color: #51575e");
    connect(buttonDivision, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonEqual = new QPushButton(this);
    buttonEqual->setGeometry(425, 100, 75, 75);
    buttonEqual->setText("=");
    buttonEqual->setStyleSheet("background-color: green;");
    connect(buttonEqual, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonClear = new QPushButton(this);
    buttonClear->setGeometry(425, 200, 75, 75);
    buttonClear->setText("AC");
    buttonClear->setStyleSheet("background-color: red;");
    connect(buttonClear, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonParenthesesOpen = new QPushButton(this);
    buttonParenthesesOpen->setGeometry(425, 300, 75, 75);
    buttonParenthesesOpen->setText("(");
    buttonParenthesesOpen->setStyleSheet("background-color: #51575e;");
    connect(buttonParenthesesOpen, SIGNAL(clicked()), this, SLOT(buttonPress()));

    buttonParenthesesClose = new QPushButton(this);
    buttonParenthesesClose->setGeometry(425, 400, 75, 75);
    buttonParenthesesClose->setText(")");
    buttonParenthesesClose->setStyleSheet("background-color: #51575e;");
    connect(buttonParenthesesClose, SIGNAL(clicked()), this, SLOT(buttonPress()));

    }

double calculate(const Term& term)
{
    if (term.m_type == Term::Type::Operation)
    {
        auto v1 = calculate(*term.m_operation.m_term1);
        auto v2 = calculate(*term.m_operation.m_term2);
        switch (term.m_operation.m_type)
        {
        case Operation::Type::Addition:
            return v1 + v2;
        case Operation::Type::Subtraction:
            return v1 - v2;
        case Operation::Type::Multiplication:
            return v1 * v2;
        case Operation::Type::Division:
            return v1 / v2;
        }
    }
    return term.m_number.m_string.toDouble() * (term.m_number.m_negative ? -1 : 1);
}

void fprint_term(FILE* file, const Term& term, const char* const parent_node, const int term_num)
{
    const auto index = &term;
    char node[256];

    if (term.m_type == Term::Type::Operation)
    {
        snprintf(node, 256, "op_%d_%u", term_num, index);
        fprintf(file, "\t%s [label=%s]\n", node, term.m_operation.to_string());
    }
    else
    {
        snprintf(node, 256, "num_%d_%u", term_num, index);
        fprintf(file, "\t%s [label=%s]\n", node, term.m_number.m_string.toStdString().c_str());
    }

    if (parent_node)
        fprintf(file, "\t%s -> %s\n", parent_node, node);

    if (term.m_type == Term::Type::Operation)
    {
        fprint_term(file, *term.m_operation.m_term1, node, 1);
        fprint_term(file, *term.m_operation.m_term2, node, 2);
    }
}

void Window::buttonPress(){
    QString calcChar = qobject_cast<QPushButton*>(sender())->text();

    if (calcChar == "")
    {
        calculatorScreen->setText(removeLastChar(calculatorScreen->text()));
    }

    else if (calcChar == "AC")
    {
        calculatorScreen->setText("");
    }

    else if (calcChar == "=")
    {
        std::cout << std::endl;
        Lexer lexer{calculatorScreen->text()};
        const auto tokens = lexer.lex();
        for (const auto& token : tokens)
        {
            std::cout << token.to_string() << std::endl;
        }
        Parser parser{tokens};
        const auto term = parser.parse();
        auto file = fopen("tree.dot", "w");
        if (file)
        {
            fprintf(file, "digraph {\n");
            fprint_term(file, *term, 0, 1);
            fprintf(file, "}\n");
            fclose(file);
        }
        calculatorScreen->setText(QString::number(calculate(*term)));
    }

    else
    {
        calculatorScreen->setText(calculatorScreen->text() + calcChar);
    }
}
