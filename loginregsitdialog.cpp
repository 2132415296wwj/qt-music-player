#include "loginregsitdialog.h"
#include "ui_loginregsitdialog.h"

loginRegsitDialog::loginRegsitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginRegsitDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("请输入用户名");
    ui->lineEdit->setText("");  // 确保实际内容是空的    
    this->setWindowTitle("登录界面");
}

loginRegsitDialog::~loginRegsitDialog()
{
    delete ui;
}


void loginRegsitDialog::on_pushButton_clicked()//注册功能，将用户名和密码插入数据库
{
    QString user_name=ui->lineEdit->text();
    QString passwd=ui->lineEdit_2->text();//获取输入的用户名和密码
    if(user_name.isEmpty()||passwd.isEmpty())
    {
       QMessageBox::information(this,"提示","用户名和密码不能为空");
       return ;
    }
    //向数据库中插入这条数据
    DbManager* dm=DbManager::getInstance();
    struct UserData user;
    user.name=user_name;
    user.password=passwd;
    if(dm->insertUser(user))
    {
        QMessageBox::information(this,"提示","注册成功");
    }
    else
    {
        QMessageBox::information(this,"提示","注册失败");
    }
    //清空行内容
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void loginRegsitDialog::on_pushButton_2_clicked()//登录逻辑
{

    QString user_name=ui->lineEdit->text();
    QString passwd=ui->lineEdit_2->text();//获取输入的用户名和密码
    if(user_name.isEmpty()||passwd.isEmpty())
    {
       QMessageBox::information(this,"提示","用户名和密码不能为空");
       return ;
    }
    DbManager* dm=DbManager::getInstance();
    struct UserData user;
    user.name=user_name;
    user.password=passwd;
    struct UserHistory his;
    his.userid=user.id;
    his.username=user.name;
    if(dm->selectUser(user))//登陆成功
    {
        //此时应该跳转到播放器窗口
        pw=new playerMainWindow;//实例化对象
        pw->setUser(user);//先给player设置user数据
        pw->show();
        pw->setAttribute(Qt::WA_DeleteOnClose);
        //链接切换用户的信号与槽
        connect(pw,&playerMainWindow::toLogSignal,this,&loginRegsitDialog::show);
        this->hide();
    }
    else
        QMessageBox::information(this,"提示","用户名密码有误");
}
