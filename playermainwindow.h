#ifndef PLAYERMAINWINDOW_H
#define PLAYERMAINWINDOW_H

#include <QMainWindow>
#include<QFileDialog>
#include<QDebug>
#include<QMessageBox>
#include<QString>
#include<QStringList>
#include<QDir>

#include<QMediaPlayer>//播放器
#include<QMediaPlaylist>//播放列表
#include<QUrl>
#include<QIcon>
#include<QChar>
#include<QListWidgetItem>
#include<iostream>
#include<set>
#include"dbmanager.h"
#include"loginregsitdialog.h"
using namespace std;
namespace Ui {
class playerMainWindow;
}

class playerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit playerMainWindow(QWidget *parent = nullptr);
    ~playerMainWindow();
    void setUser(const UserData u);

private slots:
    void on_pushButton_choseList_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_pre_clicked();

    void on_pushButton_next_clicked();

    void dealPOstitionChangedSlot(qint64 postion);
    void on_horizontalSlider_play_sliderMoved(int position);

    void on_comboBox_currentIndexChanged(int index);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_choseUser_clicked();

    void on_lineEdit_serach_textChanged(const QString &arg1);

    void on_verticalSlider_vol_sliderMoved(int position);

private:
    Ui::playerMainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
    QStringList names;
    set<QString> historyMusicNames;
    UserData m_user;
    UserHistory h;
private:
    signals:
    void toLogSignal();
};

#endif // PLAYERMAINWINDOW_H
