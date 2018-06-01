#ifndef USERAUDIT_H
#define USERAUDIT_H

#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QSharedDataPointer>
#include <TGlobal>
#include <TAbstractModel>

class TModelObject;
class UserAuditObject;
class QJsonArray;

enum UserAction {
    USER_CREATE = 1,
    USER_REMOVE,
    USER_CHANGE_REGIST,
    USER_PASSWORD,
};


class T_MODEL_EXPORT UserAudit : public TAbstractModel
{
public:
    UserAudit();
    UserAudit(const UserAudit &other);
    UserAudit(const UserAuditObject &object);
    ~UserAudit();

    int id() const;
    QString createdAt() const;
    int user() const;
    void setUser(int user);
    int doneBy() const;
    void setDoneBy(int doneBy);
    int actionId() const;
    void setActionId(int actionId);
    QString oldtext() const;
    void setOldtext(const QString &oldtext);
    QString newtext() const;
    void setNewtext(const QString &newtext);
    UserAudit &operator=(const UserAudit &other);

    bool create() override { return TAbstractModel::create(); }
    bool update() override { return TAbstractModel::update(); }
    bool save()   override { return TAbstractModel::save(); }
    bool remove() override { return TAbstractModel::remove(); }

    static UserAudit create(int user, int doneBy, int actionId, const QString &oldtext, const QString &newtext);
    static UserAudit create(const QVariantMap &values);
    static UserAudit get(int id);
    static int count();
    static QList<UserAudit> getAll();
    static QJsonArray getAllJson();

private:
    QSharedDataPointer<UserAuditObject> d;

    TModelObject *modelData() override;
    const TModelObject *modelData() const override;
    friend QDataStream &operator<<(QDataStream &ds, const UserAudit &model);
    friend QDataStream &operator>>(QDataStream &ds, UserAudit &model);
};

Q_DECLARE_METATYPE(UserAudit)
Q_DECLARE_METATYPE(QList<UserAudit>)

#endif // USERAUDIT_H
