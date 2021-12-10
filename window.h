#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include <QPushButton>
#include <QLineEdit>

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private:
    QLineEdit *calculatorScreen;

    QPushButton* numberButtons [10] = {};

    QPushButton *buttonPoint;
    QPushButton *buttonDelete;

    QPushButton *buttonSum;
    QPushButton *buttonSub;
    QPushButton *buttonDivision;
    QPushButton *buttonMultiplication;

    QPushButton *buttonEqual;
    QPushButton *buttonClear;
    QPushButton *buttonParenthesesOpen;
    QPushButton *buttonParenthesesClose;

private slots:
    void buttonPress();
};

#endif // WINDOW_H
