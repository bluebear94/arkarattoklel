#ifndef USEROBJECT_H
#define USEROBJECT_H

#include <TSqlObject>
#include <QSharedData>


class T_MODEL_EXPORT UserObject : public TSqlObject, public QSharedData
{
public:
    int id {0};
    QString username;
    QByteArray passhash;
    QByteArray salt;
    int regist {0};

    enum PropertyIndex {
        Id = 0,
        Username,
        Passhash,
        Salt,
        Regist,
    };

    int primaryKeyIndex() const override { return Id; }
    int autoValueIndex() const override { return Id; }
    QString tableName() const override { return QLatin1String("User"); }

private:    /*** Don't modify below this line ***/
    Q_OBJECT
    Q_PROPERTY(int id READ getid WRITE setid)
    T_DEFINE_PROPERTY(int, id)
    Q_PROPERTY(QString username READ getusername WRITE setusername)
    T_DEFINE_PROPERTY(QString, username)
    Q_PROPERTY(QByteArray passhash READ getpasshash WRITE setpasshash)
    T_DEFINE_PROPERTY(QByteArray, passhash)
    Q_PROPERTY(QByteArray salt READ getsalt WRITE setsalt)
    T_DEFINE_PROPERTY(QByteArray, salt)
    Q_PROPERTY(int regist READ getregist WRITE setregist)
    T_DEFINE_PROPERTY(int, regist)
};

#endif // USEROBJECT_H
