#ifndef ENTERCHAR_H
#define ENTERCHAR_H

#include <QWidget>

namespace Ui {
class EnterChar;
}

class EnterChar : public QWidget
{
    Q_OBJECT

public:
    explicit EnterChar(QWidget *parent = nullptr);
    ~EnterChar();

private:
    Ui::EnterChar *ui;

signals:
    void returnChar(char a, int N);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
public slots:
    void getN(int N);
};

#endif // ENTERCHAR_H
