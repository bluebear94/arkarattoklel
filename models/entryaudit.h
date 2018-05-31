#ifndef ENTRYAUDIT_H
#define ENTRYAUDIT_H

#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QSharedDataPointer>
#include <TGlobal>
#include <TAbstractModel>

class TModelObject;
class EntryAuditObject;
class QJsonArray;

enum EntryAction {
    ENTRY_CREATE = 1,
    ENTRY_EDIT,
    ENTRY_REMOVE,
    ENTRY_RENAME,
};

class T_MODEL_EXPORT EntryAudit : public TAbstractModel
{
public:
    EntryAudit();
    EntryAudit(const EntryAudit &other);
    EntryAudit(const EntryAuditObject &object);
    ~EntryAudit();

    int id() const;
    QString createdAt() const;
    int entryId() const;
    void setEntryId(int entryId);
    int doneBy() const;
    void setDoneBy(int doneBy);
    int actionId() const;
    void setActionId(int actionId);
    QString oldtext() const;
    void setOldtext(const QString &oldtext);
    QString newtext() const;
    void setNewtext(const QString &newtext);
    EntryAudit &operator=(const EntryAudit &other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool save()   override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static EntryAudit create(int entryId, int doneBy, int actionId, const QString &oldtext, const QString &newtext);
    static EntryAudit create(const QVariantMap &values);
    static EntryAudit get(int id);
    static int count();
    static QList<EntryAudit> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<EntryAuditObject> d;

    TModelObject *modelData() override;
    const TModelObject *modelData() const override;
    friend QDataStream &operator<<(QDataStream &ds, const EntryAudit &model);
    friend QDataStream &operator>>(QDataStream &ds, EntryAudit &model);
};

Q_DECLARE_METATYPE(EntryAudit)
Q_DECLARE_METATYPE(QList<EntryAudit>)

#endif // ENTRYAUDIT_H
