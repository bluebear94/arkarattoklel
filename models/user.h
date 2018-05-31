#ifndef USER_H
#define USER_H

#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QSharedDataPointer>
#include <TGlobal>
#include <TAbstractUser>
#include <TAbstractModel>

class TModelObject;
class UserObject;
class QJsonArray;


class T_MODEL_EXPORT User : public TAbstractUser, public TAbstractModel
{
public:
    User();
    User(const User &other);
    User(const UserObject &object);
    ~User();

    int id() const;
    QString username() const;
    void setUsername(const QString &username);
    QByteArray passhash() const;
    void setPasshash(const QByteArray &passhash);
    QByteArray salt() const;
    void setSalt(const QByteArray& salt);
    int regist() const;
    void setRegist(int regist);
    QString identityKey() const { return username(); }
    User &operator=(const User &other);

    bool create() { return TAbstractModel::create(); }
    bool update() { return TAbstractModel::update(); }
    bool save()   { return TAbstractModel::save(); }
    bool remove() { return TAbstractModel::remove(); }

    static User authenticate(const QString &username, const QString &password, const char** message = nullptr);
    static User create(const QString &username, const QByteArray &passhash, const QByteArray& salt, int regist);
    static User create(const QString &username, const QString &password, int regist);
    static User create(const QVariantMap &values);
    static User get(int id);
    static int count();
    static QList<User> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<UserObject> d;

    TModelObject *modelData();
    const TModelObject *modelData() const;
    friend QDataStream &operator<<(QDataStream &ds, const User &model);
    friend QDataStream &operator>>(QDataStream &ds, User &model);
};

Q_DECLARE_METATYPE(User)
Q_DECLARE_METATYPE(QList<User>)

#endif // USER_H
