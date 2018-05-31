#include <TreeFrogModel>
#include "useraudit.h"
#include "userauditobject.h"

UserAudit::UserAudit()
    : TAbstractModel(), d(new UserAuditObject())
{
    d->id = 0;
    d->user = 0;
    d->done_by = 0;
    d->action_id = 0;
}

UserAudit::UserAudit(const UserAudit &other)
    : TAbstractModel(), d(new UserAuditObject(*other.d))
{ }

UserAudit::UserAudit(const UserAuditObject &object)
    : TAbstractModel(), d(new UserAuditObject(object))
{ }

UserAudit::~UserAudit()
{
    // If the reference count becomes 0,
    // the shared data object 'UserAuditObject' is deleted.
}

int UserAudit::id() const
{
    return d->id;
}

QString UserAudit::createdAt() const
{
    return d->created_at;
}

int UserAudit::user() const
{
    return d->user;
}

void UserAudit::setUser(int user)
{
    d->user = user;
}

int UserAudit::doneBy() const
{
    return d->done_by;
}

void UserAudit::setDoneBy(int doneBy)
{
    d->done_by = doneBy;
}

int UserAudit::actionId() const
{
    return d->action_id;
}

void UserAudit::setActionId(int actionId)
{
    d->action_id = actionId;
}

QString UserAudit::oldtext() const
{
    return d->oldtext;
}

void UserAudit::setOldtext(const QString &oldtext)
{
    d->oldtext = oldtext;
}

QString UserAudit::newtext() const
{
    return d->newtext;
}

void UserAudit::setNewtext(const QString &newtext)
{
    d->newtext = newtext;
}

UserAudit &UserAudit::operator=(const UserAudit &other)
{
    d = other.d;  // increments the reference count of the data
    return *this;
}

UserAudit UserAudit::create(int user, int doneBy, int actionId, const QString &oldtext, const QString &newtext)
{
    UserAuditObject obj;
    obj.user = user;
    obj.done_by = doneBy;
    obj.action_id = actionId;
    obj.oldtext = oldtext;
    obj.newtext = newtext;
    if (!obj.create()) {
        return UserAudit();
    }
    return UserAudit(obj);
}

UserAudit UserAudit::create(const QVariantMap &values)
{
    UserAudit model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

UserAudit UserAudit::get(int id)
{
    TSqlORMapper<UserAuditObject> mapper;
    return UserAudit(mapper.findByPrimaryKey(id));
}

int UserAudit::count()
{
    TSqlORMapper<UserAuditObject> mapper;
    return mapper.findCount();
}

QList<UserAudit> UserAudit::getAll()
{
    return tfGetModelListByCriteria<UserAudit, UserAuditObject>(TCriteria());
}

QJsonArray UserAudit::getAllJson()
{
    QJsonArray array;
    TSqlORMapper<UserAuditObject> mapper;

    if (mapper.find() > 0) {
        for (TSqlORMapperIterator<UserAuditObject> i(mapper); i.hasNext(); ) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(UserAudit(i.next()).toVariantMap())));
        }
    }
    return array;
}

TModelObject *UserAudit::modelData()
{
    return d.data();
}

const TModelObject *UserAudit::modelData() const
{
    return d.data();
}

QDataStream &operator<<(QDataStream &ds, const UserAudit &model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, UserAudit &model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(UserAudit)
