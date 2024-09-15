#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void digitNumbers();

    void on_dot_btn_clicked();
    void digitOperations();

    void on_pushButton_6_clicked();
    double evaluateExpression(const QString& expression);

    void on_clear_btn_clicked();

    void on_del_btn_clicked();

    void on_enter_btn_clicked();

    void on_sqrt_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
