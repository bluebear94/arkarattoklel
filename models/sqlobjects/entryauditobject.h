#ifndef ENTRYAUDITOBJECT_H
#define ENTRYAUDITOBJECT_H

#include <TSqlObject>
#include <QSharedData>


class T_MODEL_EXPORT EntryAuditObject : public TSqlObject, public QSharedData
{
public:
    int id {0};
    QString created_at;
    int entry_id {0};
    int done_by {0};
    int action_id {0};
    QString oldtext;
    QString newtext;

    enum PropertyIndex {
        Id = 0,
        CreatedAt,
        EntryId,
        DoneBy,
        ActionId,
        Oldtext,
        Newtext,
    };

    int primaryKeyIndex() const override { return Id; }
    int autoValueIndex() const override { return Id; }
    QString tableName() const override { return QLatin1String("EntryAudit"); }

private:    /*** Don't modify below this line ***/
    Q_OBJECT
    Q_PROPERTY(int id READ getid WRITE setid)
    T_DEFINE_PROPERTY(int, id)
    Q_PROPERTY(QString created_at READ getcreated_at WRITE setcreated_at)
    T_DEFINE_PROPERTY(QString, created_at)
    Q_PROPERTY(int entry_id READ getentry_id WRITE setentry_id)
    T_DEFINE_PROPERTY(int, entry_id)
    Q_PROPERTY(int done_by READ getdone_by WRITE setdone_by)
    T_DEFINE_PROPERTY(int, done_by)
    Q_PROPERTY(int action_id READ getaction_id WRITE setaction_id)
    T_DEFINE_PROPERTY(int, action_id)
    Q_PROPERTY(QString oldtext READ getoldtext WRITE setoldtext)
    T_DEFINE_PROPERTY(QString, oldtext)
    Q_PROPERTY(QString newtext READ getnewtext WRITE setnewtext)
    T_DEFINE_PROPERTY(QString, newtext)
};

#endif // ENTRYAUDITOBJECT_H
