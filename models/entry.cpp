#include <TreeFrogModel>
#include "entry.h"
#include "entryobject.h"

Entry::Entry()
    : TAbstractModel(), d(new EntryObject())
{
    d->id = 0;
    d->lock_revision = 0;
}

Entry::Entry(const Entry &other)
    : TAbstractModel(), d(new EntryObject(*other.d))
{ }

Entry::Entry(const EntryObject &object)
    : TAbstractModel(), d(new EntryObject(object))
{ }

Entry::~Entry()
{
    // If the reference count becomes 0,
    // the shared data object 'EntryObject' is deleted.
}

int Entry::id() const
{
    return d->id;
}

QString Entry::name() const
{
    return d->name;
}

void Entry::setName(const QString &name)
{
    d->name = name;
}

QString Entry::body() const
{
    return d->body;
}

void Entry::setBody(const QString &body)
{
    d->body = body;
}

QString Entry::createdAt() const
{
    return d->created_at;
}

QString Entry::updatedAt() const
{
    return d->updated_at;
}

int Entry::lockRevision() const
{
    return d->lock_revision;
}

Entry &Entry::operator=(const Entry &other)
{
    d = other.d;  // increments the reference count of the data
    return *this;
}

Entry Entry::create(const QString &name, const QString &body)
{
    EntryObject obj;
    obj.name = name;
    obj.body = body;
    if (!obj.create()) {
        return Entry();
    }
    return Entry(obj);
}

Entry Entry::create(const QVariantMap &values)
{
    Entry model;
    model.setProperties(values);
    if (!model.d->create()) {
        model.d->clear();
    }
    return model;
}

Entry Entry::get(int id)
{
    TSqlORMapper<EntryObject> mapper;
    return Entry(mapper.findByPrimaryKey(id));
}

Entry Entry::get(int id, int lockRevision)
{
    TSqlORMapper<EntryObject> mapper;
    TCriteria cri;
    cri.add(EntryObject::Id, id);
    cri.add(EntryObject::LockRevision, lockRevision);
    return Entry(mapper.findFirst(cri));
}

Entry Entry::get(const QString& name) {
    TSqlORMapper<EntryObject> mapper;
    TCriteria cri;
    cri.add(EntryObject::Name, name);
    return Entry(mapper.findFirst(cri));
}

int Entry::count()
{
    TSqlORMapper<EntryObject> mapper;
    return mapper.findCount();
}

QList<Entry> Entry::getAll()
{
    return tfGetModelListByCriteria<Entry, EntryObject>(TCriteria());
}

QJsonArray Entry::getAllJson()
{
    QJsonArray array;
    TSqlORMapper<EntryObject> mapper;

    if (mapper.find() > 0) {
        for (TSqlORMapperIterator<EntryObject> i(mapper); i.hasNext(); ) {
            array.append(QJsonValue(QJsonObject::fromVariantMap(Entry(i.next()).toVariantMap())));
        }
    }
    return array;
}

TModelObject *Entry::modelData()
{
    return d.data();
}

const TModelObject *Entry::modelData() const
{
    return d.data();
}

QDataStream &operator<<(QDataStream &ds, const Entry &model)
{
    auto varmap = model.toVariantMap();
    ds << varmap;
    return ds;
}

QDataStream &operator>>(QDataStream &ds, Entry &model)
{
    QVariantMap varmap;
    ds >> varmap;
    model.setProperties(varmap);
    return ds;
}

// Don't remove below this line
T_REGISTER_STREAM_OPERATORS(Entry)
