#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStack>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QPushButton*> buttons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    QList<QPushButton*> operationsButtons = {
        ui-> plus_btn, ui -> minus_btn, ui -> div_btn, ui -> umn_btn, ui -> percent_btn

    };
    for (QPushButton *button : buttons){
        connect(button, SIGNAL(clicked()), this, SLOT(digitNumbers()));
    }
    for (QPushButton *button : operationsButtons){
        connect(button, SIGNAL(clicked()), this, SLOT(digitOperations()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::digitNumbers()
{
    QPushButton *button = (QPushButton *)sender();
    double numbers;
    QString labelForScreen;
    QString currentScreenText;
    currentScreenText = ui -> screen -> text();
    if (currentScreenText.contains("+") || currentScreenText.contains("-") || currentScreenText.contains("*") || currentScreenText.contains("/")){
        labelForScreen = currentScreenText + button -> text();
    } else {
        numbers = (currentScreenText + button -> text()).toDouble();

        labelForScreen = QString::number(numbers, 'g', 10);
    }
    ui -> screen -> setText(labelForScreen);

}


void MainWindow::digitOperations()
{
    QPushButton *button = (QPushButton *)sender();
    double numbers;
    QString labelForScreen;

    if (button -> text() == "%"){
        labelForScreen = ui -> screen -> text() + "%";


    } else if (button -> text() == "+"){
        labelForScreen = ui -> screen -> text() + " + ";

    } else if (button -> text() == "-"){
        labelForScreen = ui -> screen -> text() + " - ";
    } else if (button -> text() == "*"){
        labelForScreen = ui -> screen -> text() + " * ";
    } else if (button -> text() == "/"){
        labelForScreen = ui -> screen -> text() + " / ";
    }

    ui -> screen -> setText(labelForScreen);

}


void MainWindow::on_dot_btn_clicked()
{
    QString currentScreenText;
    currentScreenText = ui -> screen -> text();
    QChar lastSymbol = currentScreenText.at(currentScreenText.length() - 1);
    if (!(lastSymbol == '.')){
        ui -> screen -> setText(currentScreenText + '.');
    }


}


void MainWindow::on_pushButton_6_clicked()
{
    QString operations;
    QString labelForScreen;
    double res;
    operations = ui -> screen -> text();
    ui -> screen -> setText(operations + " =");
    operations = operations.remove(' ');

    if (operations.contains("%")){
        operations.replace("%", "*0.01");
    }

    res = evaluateExpression(operations);
    labelForScreen = QString::number(res, 'g', 10);
    ui -> result -> setText(labelForScreen);

}

double MainWindow::evaluateExpression(const QString& expression) {
    QStack<double> numbers;
    QStack<QChar> operators;

    int i = 0;
    while (i < expression.length()) {
        if (expression[i].isDigit() || expression[i] == '.') {
            double number = 0.0;
            double fractionalPart = 0.0;
            bool isFractional = false;
            double fractionDivider = 10.0;

            while (i < expression.length() && (expression[i].isDigit() || expression[i] == '.')) {
                if (expression[i] == '.') {
                    isFractional = true;
                    ++i;
                    continue;
                }

                if (!isFractional) {
                    number = number * 10 + expression[i].digitValue();
                } else {
                    fractionalPart += expression[i].digitValue() / fractionDivider;
                    fractionDivider *= 10;
                }
                ++i;
            }

            number += fractionalPart;
            numbers.push(number);
        }
        else if (expression[i] == '-' && (i == 0 || !expression[i - 1].isDigit())) {
            ++i;
            double number = 0.0;
            double fractionalPart = 0.0;
            bool isFractional = false;
            double fractionDivider = 10.0;

            while (i < expression.length() && (expression[i].isDigit() || expression[i] == '.')) {
                if (expression[i] == '.') {
                    isFractional = true;
                    ++i;
                    continue;
                }

                if (!isFractional) {
                    number = number * 10 + expression[i].digitValue();
                } else {
                    fractionalPart += expression[i].digitValue() / fractionDivider;
                    fractionDivider *= 10;
                }
                ++i;
            }

            number += fractionalPart;
            numbers.push(-number);
        }
        else if (expression[i] == '+' || expression[i] == '-') {
            while (!operators.isEmpty() && (operators.top() == '*' || operators.top() == '/' || operators.top() == '+' || operators.top() == '-')) {
                double right = numbers.pop();
                double left = numbers.pop();
                QChar op = operators.pop();

                if (op == '+') {
                    numbers.push(left + right);
                } else if (op == '-') {
                    numbers.push(left - right);
                } else if (op == '*') {
                    numbers.push(left * right);
                } else if (op == '/') {
                    numbers.push(left / right);
                }
            }
            operators.push(expression[i]);
            ++i;
        }
        else if (expression[i] == '*' || expression[i] == '/') {
            while (!operators.isEmpty() && (operators.top() == '*' || operators.top() == '/')) {
                double right = numbers.pop();
                double left = numbers.pop();
                QChar op = operators.pop();

                if (op == '*') {
                    numbers.push(left * right);
                } else if (op == '/') {
                    numbers.push(left / right);
                }
            }
            operators.push(expression[i]);
            ++i;
        } else {
            ++i;
        }
    }

    while (!operators.isEmpty()) {
        double right = numbers.pop();
        double left = numbers.pop();
        QChar op = operators.pop();

        if (op == '+') {
            numbers.push(left + right);
        } else if (op == '-') {
            numbers.push(left - right);
        } else if (op == '*') {
            numbers.push(left * right);
        } else if (op == '/') {
            numbers.push(left / right);
        }
    }

    return numbers.isEmpty() ? 0 : numbers.pop();
}



void MainWindow::on_clear_btn_clicked()
{
    ui -> screen -> clear();
    ui -> result -> clear();

}


void MainWindow::on_del_btn_clicked()
{
    QString currentScreenData;
    currentScreenData = ui -> screen -> text();
    currentScreenData.chop(1);
    ui -> screen -> setText(currentScreenData);
}


void MainWindow::on_enter_btn_clicked()
{
    QString currentResult;
    currentResult = ui -> result -> text();
    ui -> screen -> setText(currentResult);
    ui -> result -> setText("");
}


void MainWindow::on_sqrt_btn_clicked()
{
    QString currentScreenText = ui -> screen -> text();
    double number;
    ui -> screen -> setText("√" + currentScreenText + " =");
    number = currentScreenText.toDouble();
    double result = std::sqrt(number);
    QString labelForScreen = QString::number(result, 'g', 10);
    ui -> result -> setText(labelForScreen);


}

