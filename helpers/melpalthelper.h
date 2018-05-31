#ifndef MELPALTHELPER_H
#define MELPALTHELPER_H

#include <QDateTime>
#include <QString>
#include <TGlobal>
#include "applicationhelper.h"

class T_HELPER_EXPORT MelpaltHelper : public ApplicationHelper {
public:
    MelpaltHelper();
    struct MelTimestamp {
        int salt, xelt, sel;
        int miv, fei, jin;
        QString toString() const;
    };
    static MelTimestamp create(const QDateTime& lipalt);
    static MelTimestamp create(const QString& l);
    static QString ltm(const QString& l);
    struct EntryAuditRecord {
        MelpaltHelper::MelTimestamp createdAt;
        int actionID;
        QString entryName, userName;
        int entryID, userID;
        int revisionID;
        QString oldtext, newtext;
    };
    static bool isVowel(QChar c);
};

Q_DECLARE_METATYPE(MelpaltHelper::EntryAuditRecord)

#endif // MELPALTHELPER_H
