#ifndef ENTRY_H
#define ENTRY_H

#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QSharedDataPointer>
#include <TGlobal>
#include <TAbstractModel>

class TModelObject;
class EntryObject;
class QJsonArray;


class T_MODEL_EXPORT Entry : public TAbstractModel
{
public:
    Entry();
    Entry(const Entry &other);
    Entry(const EntryObject &object);
    ~Entry();

    int id() const;
    QString name() const;
    void setName(const QString &name);
    QString body() const;
    void setBody(const QString &body);
    QString createdAt() const;
    QString updatedAt() const;
    int lockRevision() const;
    Entry &operator=(const Entry &other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool save()   override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static Entry create(const QString &name, const QString &body);
    static Entry create(const QVariantMap &values);
    static Entry get(int id);
    static Entry get(int id, int lockRevision);
    static Entry get(const QString& name);
    static int count();
    static QList<Entry> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<EntryObject> d;

    TModelObject *modelData() override;
    const TModelObject *modelData() const override;
    friend QDataStream &operator<<(QDataStream &ds, const Entry &model);
    friend QDataStream &operator>>(QDataStream &ds, Entry &model);
};

Q_DECLARE_METATYPE(Entry)
Q_DECLARE_METATYPE(QList<Entry>)

#endif // ENTRY_H
