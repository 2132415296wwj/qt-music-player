#ifndef DBMANAGER_H
#define DBMANAGER_H
#include<QSqlDatabase>//数据库打开 关闭 驱动
#include<QSqlQuery>//数据库crud
#include<QDebug>
#include<iostream>
using namespace std;
#include<set>
struct UserData
{
    int id;
    QString name;
    QString password;
};
struct UserHistory
{
    int userid;
    QString username;
    QString musicName;
};
class DbManager
{
public:
    static DbManager *getInstance();
    void createTable();
    bool insertUser(UserData&);
    bool selectUser(UserData&);
    bool insertHisoryInfo(const UserHistory &history);
    bool selectHistoryInfo(UserHistory& histry,set<QString>&infos);
private:
    DbManager();
    DbManager(const DbManager&);
    static DbManager *instance;
    //数据库对象
    QSqlDatabase db;

};

#endif // DBMANAGER_H
