#include "dbmanager.h"
DbManager * DbManager::instance=new DbManager;//饿汉模式
DbManager *DbManager::getInstance()
{
    return instance;
}

void DbManager::createTable()
{
    //创建用户信息表
    QString sql="create table if not exists user_info"
                "(id integer primary key autoincrement,"//id主键自增
                "name varchar(20),"
                "password varchar(20))";
                QSqlQuery query;
                bool ok=query.exec(sql);
                if(ok)
                    qDebug()<<"创建用户表成功";
                else
                    qDebug()<<"创建用户表失败";
  //创建历史记录表
    sql="create table if not exists history_info"
            "(userId integer,"
            "userName varchar(20),"
            "musicName varchar(50))";
    ok=query.exec(sql);
    if(ok)
        qDebug()<<"创建历史记录表成功";
    else
        qDebug()<<"创建历史记录表失败";
}

DbManager::DbManager()
{
    //添加驱动
    db=QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName("music.db");
    //打开数据库
    db.open();
    //创建表
    createTable();
}

DbManager::DbManager(const DbManager &)
{

}

bool DbManager::insertUser(UserData &user)   //注册时插入表
{
    //逻辑：先查重，没有重复用户名再插入
    QString sql="SELECT COUNT(*) FROM user_info WHERE name=? ;";
    QSqlQuery query(db);
    query.prepare(sql);//准备
    query.bindValue(0,user.name);
    query.exec();
    int count =-1;
    if(query.next())
    {
        count=query.value(0).toInt();
        if(count>0)
        {
            qDebug()<<count<<"存在该用户名";
            return  false;
        }
        //没有重复的，可以插入了
        sql="INSERT INTO user_info(name,password)VALUES(?,?)";
        query.prepare(sql);
        query.bindValue(0,user.name);
        query.bindValue(1,user.password);
        bool ok=query.exec();
        if(ok)
        {
            qDebug()<<"插入成功";
            return  true;
        }
        else
        {
            qDebug()<<"插入失败";
            return false;
        }
    }
    else /*if(!query.next())*///当query.next执行出错的时候
        return false;

}

bool DbManager::selectUser(UserData &user)
{
    QString sql="SELECT id FROM user_info WHERE name=? and password=?;";
    QSqlQuery query(db);
    query.prepare(sql);//准备
    query.bindValue(0,user.name);
    query.bindValue(1,user.password);
    query.exec();
    int count =-1;//初始值-1防止意外
    if(query.next())
    {
        count=query.value(0).toInt();
        if(count>0)
        {
            qDebug()<<count<<"登陆成功";
            user.id = query.value(0).toInt();
            return  true;
        }
        else//用户名不存在
        {
            return false;
        }
    }
    else //当query.next执行出错的时候
        return false;

}

//插入history_info表的函数
bool DbManager::insertHisoryInfo(const UserHistory &history)
{
    QString sql="insert into history_info values(?,?,?);";
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0,history.userid);
    query.bindValue(1,history.username);
    query.bindValue(2,history.musicName);
    bool ok =query.exec();
    if(ok)
    {
        qDebug()<<"历史记录表插入成功";
        return true;
    }
    else
    {
        qDebug()<<"历史记录表插入失败";
        return false;
    }

}

bool DbManager::selectHistoryInfo(UserHistory &histry, set<QString> &infos)
{
    QString sql="select musicName from history_info where userId=? ;";
    QSqlQuery  query(db);
    query.prepare(sql);
    query.bindValue(0,histry.userid);
    query.exec();
    while(query.next())
    {
        QString music_name=query.value(0).toString();
        infos.insert(music_name);

    }
    return false;
}
