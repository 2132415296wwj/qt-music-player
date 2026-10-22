#ifndef LOGINREGSITDIALOG_H
#define LOGINREGSITDIALOG_H

#include <QDialog>
#include<QString>
#include<QMessageBox>
#include"dbmanager.h"
#include"playermainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class loginRegsitDialog; }
QT_END_NAMESPACE
class playerMainWindow;

class loginRegsitDialog : public QDialog
{
    Q_OBJECT

public:
    loginRegsitDialog(QWidget *parent = nullptr);
    ~loginRegsitDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::loginRegsitDialog *ui;
    playerMainWindow* pw;//声明一个跳转播放器的指针
    UserData m_user;
};
#endif // LOGINREGSITDIALOG_H
