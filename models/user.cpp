#include <TreeFrogModel>

#include <random>

#include <argon2.h>

#include "user.h"
#include "userobject.h"

User::User()
    : TAbstractUser(), TAbstractModel(), d(new UserObject())
{
    d->id = 0;
    d->regist = 0;
}

User::User(const User &other)
    : TAbstractUser(), TAbstractModel(), d(new UserObject(*other.d))
{ }

User::User(const UserObject &object)
    : TAbstractUser(), TAbstractModel(), d(new UserObject(object))
{ }


User::~User()
{
    // If the reference count becomes 0,
    // the shared data object 'UserObject' is deleted.
}

int User::id() const
{
    return d->id;
}

QString User::username() const
{
    return d->username;
}

void User::setUsername(const QString &username)
{
    d->username = username;
}

QByteArray User::passhash() const
{
    return d->passhash;
}

void User::setPasshash(const QByteArray &passhash)
{
    d->passhash = passhash;
}

int User::regist() const
{
    return d->regist;
}

void User::setRegist(int regist)
{
    d->regist = regist;
}

QByteArray User::salt() const
{
    return d->salt;
}

void User::setSalt(const QByteArray& salt)
{
    d->salt = salt;
}

User &User::operator=(const User &other)
{
    d = other.d;  // increments the reference count of the data
    return *this;
}

static QByteArray makeHash(
        const QByteArray& salt, const QString& password,
        int& errc) {
    QByteArray passUTF8 = password.toUtf8();
    QByteArray hash(64, '\0');
    errc = argon2i_hash_raw(
        2, 1 << 16, 1,
        passUTF8.data(), passUTF8.size(),
        salt.data(), salt.size(),
        hash.data(), 64
        );
    return hash;
}

User User::authenticate(
    const QString& username, const QString& password,
    const char** message)
{
    const char* sink;
    const char*& m = (message == nullptr) ? sink : *message;
    if (username.isEmpty() || password.isEmpty()) {
        m = "Username or password not provided";
        return User();
    }

    TSqlORMapper<UserObject> mapper;
    UserObject obj = mapper.findFirst(TCriteria(UserObject::Username, username));
    if (obj.isNull()) {
        m = "Invalid username or password";
        obj.clear();
        return User(obj);
    }
    if (obj.regist < 1) {
        m = "Account not confirmed by administrator";
        obj.clear();
        return User(obj);
    }
    int errc;
    QByteArray hash = makeHash(obj.salt, password, errc);
    if (errc != ARGON2_OK) {
        m = "kozet fucked up; please yell at him";
        obj.clear();
    } else if (obj.passhash != hash) {
        m = "Invalid username or password";
        obj.clear();
    }
    return User(obj);
}

User User::create(const QString &username, const QString &password, int regist)
{
    int errc;
    QByteArray salt(16, '\0');
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 255);
    for (int i = 0; i < 16; ++i) {
        salt[i] = dist(rd);
    }
    QByteArray hash = makeHash(salt, password, errc);
    if (errc != ARGON2_OK) {
        tError("User::create: Hash returned error code %d", errc);
        return User();
    }
    return User::create(username, hash, salt, regist);
}

User User::create(const QString &username, const QByteArray &passhash, const QByteArray& salt, int regist)
{
    UserObject obj;
    obj.username = username;
    obj.passhash = passhash;
    obj.salt = salt;
    obj.regist = regist;
    if (!obj.create()) {
        return User();
    }
    return User(obj);
}

User User::create(const QVariantMap &values)
{
    User model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

User User::get(int id)
{
    TSqlORMapper<UserObject> mapper;
    return User(mapper.findByPrimaryKey(id));
}

int User::count()
{
    TSqlORMapper<UserObject> mapper;
    return mapper.findCount();
}

QList<User> User::getAll()
{
    return tfGetModelListByCriteria<User, UserObject>();
}

QJsonArray User::getAllJson()
{
    QJsonArray array;
    TSqlORMapper<UserObject> mapper;

    if (mapper.find() > 0) {
        for (TSqlORMapperIterator<UserObject> i(mapper); i.hasNext(); ) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(User(i.next()).toVariantMap())));
        }
    }
    return array;
}

TModelObject *User::modelData()
{
    return d.data();
}

const TModelObject *User::modelData() const
{
    return d.data();
}

QDataStream &operator<<(QDataStream &ds, const User &model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, User &model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(User)