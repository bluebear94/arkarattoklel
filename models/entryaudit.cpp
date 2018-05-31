#include <TreeFrogModel>
#include "entryaudit.h"
#include "entryauditobject.h"

EntryAudit::EntryAudit()
    : TAbstractModel(), d(new EntryAuditObject())
{
    d->id = 0;
    d->entry_id = 0;
    d->done_by = 0;
    d->action_id = 0;
}

EntryAudit::EntryAudit(const EntryAudit &other)
    : TAbstractModel(), d(new EntryAuditObject(*other.d))
{ }

EntryAudit::EntryAudit(const EntryAuditObject &object)
    : TAbstractModel(), d(new EntryAuditObject(object))
{ }

EntryAudit::~EntryAudit()
{
    // If the reference count becomes 0,
    // the shared data object 'EntryAuditObject' is deleted.
}

int EntryAudit::id() const
{
    return d->id;
}

QString EntryAudit::createdAt() const
{
    return d->created_at;
}

int EntryAudit::entryId() const
{
    return d->entry_id;
}

void EntryAudit::setEntryId(int entryId)
{
    d->entry_id = entryId;
}

int EntryAudit::doneBy() const
{
    return d->done_by;
}

void EntryAudit::setDoneBy(int doneBy)
{
    d->done_by = doneBy;
}

int EntryAudit::actionId() const
{
    return d->action_id;
}

void EntryAudit::setActionId(int actionId)
{
    d->action_id = actionId;
}

QString EntryAudit::oldtext() const
{
    return d->oldtext;
}

void EntryAudit::setOldtext(const QString &oldtext)
{
    d->oldtext = oldtext;
}

QString EntryAudit::newtext() const
{
    return d->newtext;
}

void EntryAudit::setNewtext(const QString &newtext)
{
    d->newtext = newtext;
}

EntryAudit &EntryAudit::operator=(const EntryAudit &other)
{
    d = other.d;  // increments the reference count of the data
    return *this;
}

EntryAudit EntryAudit::create(int entryId, int doneBy, int actionId, const QString &oldtext, const QString &newtext)
{
    EntryAuditObject obj;
    obj.entry_id = entryId;
    obj.done_by = doneBy;
    obj.action_id = actionId;
    obj.oldtext = oldtext;
    obj.newtext = newtext;
    if (!obj.create()) {
        return EntryAudit();
    }
    return EntryAudit(obj);
}

EntryAudit EntryAudit::create(const QVariantMap &values)
{
    EntryAudit model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

EntryAudit EntryAudit::get(int id)
{
    TSqlORMapper<EntryAuditObject> mapper;
    return EntryAudit(mapper.findByPrimaryKey(id));
}

int EntryAudit::count()
{
    TSqlORMapper<EntryAuditObject> mapper;
    return mapper.findCount();
}

QList<EntryAudit> EntryAudit::getAll()
{
    return tfGetModelListByCriteria<EntryAudit, EntryAuditObject>(TCriteria());
}

QJsonArray EntryAudit::getAllJson()
{
    QJsonArray array;
    TSqlORMapper<EntryAuditObject> mapper;

    if (mapper.find() > 0) {
        for (TSqlORMapperIterator<EntryAuditObject> i(mapper); i.hasNext(); ) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(EntryAudit(i.next()).toVariantMap())));
        }
    }
    return array;
}

TModelObject *EntryAudit::modelData()
{
    return d.data();
}

const TModelObject *EntryAudit::modelData() const
{
    return d.data();
}

QDataStream &operator<<(QDataStream &ds, const EntryAudit &model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, EntryAudit &model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(EntryAudit)
