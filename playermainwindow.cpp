#include "playermainwindow.h"
#include "ui_playermainwindow.h"

playerMainWindow::playerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::playerMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("wwj播放器");
    player = new QMediaPlayer(this);
    playerlist = new QMediaPlaylist(this);
    player->setPlaylist(playerlist);//设置播放列表到播放器


    //链接同步进度条的信号与槽
    connect(player,&QMediaPlayer::positionChanged,this,&playerMainWindow::dealPOstitionChangedSlot);
    ui->verticalSlider_vol->setRange(0,100);
    player->setVolume(50);
}

playerMainWindow::~playerMainWindow()
{
    delete ui;
}

void playerMainWindow::setUser(const UserData u)
{
        m_user = u;
        // 读这个用户的历史记录并显示
        DbManager *dm = DbManager::getInstance();
        UserHistory h;
        h.userid = m_user.id;

        dm->selectHistoryInfo(h, historyMusicNames);//将数据库中的记录插入到容器set
        for(auto iter = historyMusicNames.begin(); iter != historyMusicNames.end(); iter++)
        {

            ui->listWidget_history->addItem(*iter);
        }
}


//选择目录打开
void playerMainWindow::on_pushButton_choseList_clicked()
{
    QString filename=QFileDialog::getExistingDirectory(this,"打开目录",".");
    if(!filename.isEmpty())
    {
        qDebug()<<filename;
        QDir dir(filename,"*.mp3");//实例化目录对象 关联到用户选择的目录
        names = dir.entryList(QDir::Files, QDir::Name);//读取目录下所有的文件名
        ui->listWidget->addItems(names);//添加到列表框
        for(auto iter=names.begin();iter!=names.end();iter++)
        {
            QString path=filename+ "/" + *iter;//拼接路径
            playerlist->addMedia(QUrl::fromLocalFile(path)); //将音乐添加到播放器列表
        }
        playerlist->setCurrentIndex(0);//设置默认播放的下标
        ui->label_music_name->setText(names[0]);
        playerlist->setPlaybackMode(QMediaPlaylist::Loop); // 默认列表循环
    }
}

void playerMainWindow::on_pushButton_play_clicked()//点击按钮可以切换两种状态
{
    //根据当前状态切换播放/暂停
    if(player->state() == QMediaPlayer::PlayingState)//当前状态是播放，则点击按钮应该将图标设置成播放图标并暂停音乐
    {
        //正在播放 → 暂停
        player->pause();
        ui->pushButton_play->setIcon(QIcon(":/new/prefix1/play.png"));
    }
    else
    {
        //暂停或停止 → 播放
        player->play();
        ui->pushButton_play->setIcon(QIcon(":/new/prefix1/pause.png"));
    }
}

void playerMainWindow::on_pushButton_pre_clicked()
{
    playerlist->previous();
    player->play();
}

void playerMainWindow::on_pushButton_next_clicked()
{
    playerlist->next();
    player->play();
}


//播放自动调用槽函数
void playerMainWindow::dealPOstitionChangedSlot(qint64 postion)
{
    ui->horizontalSlider_play->setMaximum(player->duration());
    ui->horizontalSlider_play->setValue(postion);
    //获取当前播放的歌曲下标
    int index=playerlist->currentIndex();
    //根据当前下标找到歌曲名
    QString name=names.at(index);
    //设置当前的歌曲名
    ui->label_music_name->setText(name);
    //显示总时间
    int min=player->duration()/1000/60;
    int sec=player->duration()/1000%60;
    //当前时间
    int curMin=postion/1000/60;
    int curSec=postion/1000%60;
    QString str= QString("%1:%2/%3:%4").arg(curMin,2,10,QChar('0')).arg(curSec,2,10,QChar('0')).arg(min,2,10,QChar('0')).arg(sec,2,10,QChar('0'));
    ui->label_time->setText(str);

    if(!historyMusicNames.count(names.at(index)))  // 只在新歌时记录一次
    {
        historyMusicNames.insert(names.at(index));
        ui->listWidget_history->addItem(names.at(index));

        DbManager *db = DbManager::getInstance();   //  正确获取单例
        h.userid = m_user.id;                       //  赋值
        h.username = m_user.name;                   //
        h.musicName = names.at(index);              //
        db->insertHisoryInfo(h);                    //  插入
    }

}


//进度条滑动自动调用
void playerMainWindow::on_horizontalSlider_play_sliderMoved(int position)
{
    player->setPosition(position);
}
//选择播放模式
void playerMainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        playerlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 1:
        playerlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 2:
        playerlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
   }
}
//双击列表歌曲，自动调用的槽函数
void playerMainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //获取用户双击的歌名文本
    QString name=item->text();

    //根据歌名name在集合names中获取索引
    int index=names.indexOf(name);

    //设置播放音乐
    playerlist->setCurrentIndex(index);

}

//点击切换用户的 槽函数
void playerMainWindow::on_pushButton_choseUser_clicked()
{
    emit toLogSignal();
    this->hide();
}

//搜素的槽函数
void playerMainWindow::on_lineEdit_serach_textChanged(const QString &arg1)
{
    ui->listWidget->clear();
    if(ui->lineEdit_serach->text().isEmpty())
    {
        ui->listWidget->addItems(names);
    }
    else
    {
        for(auto iter=names.begin();iter!=names.end();iter++)
        {
            if(iter->contains(arg1,Qt::CaseInsensitive))
            {
                ui->listWidget->addItem(*iter);
            }
        }
        int num=ui->listWidget->count();
        QString msg= QString("搜索到%1条记录").arg(num);
        ui->statusbar->showMessage(msg,3000);
    }
}

//音量调节
void playerMainWindow::on_verticalSlider_vol_sliderMoved(int position)
{
    ui->verticalSlider_vol->setValue(position);
    player->setVolume(position);
}
