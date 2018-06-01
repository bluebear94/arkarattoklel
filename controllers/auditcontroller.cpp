#include "auditcontroller.h"

#include <tsqlquery.h>

#include "entry.h"
#include "user.h"
#include "entryaudit.h"
#include "useraudit.h"
#include "applicationhelper.h"
#include "melpalthelper.h"

// Give entry even when there is no corresponding user
static const char* entryAuditQuery = R"*(
SELECT
    A.created_at, A.action_id,
    E.name, U.username,
    E.id AS eid, U.id AS uid,
    A.id AS aid,
    A.oldtext, A.newtext
    FROM EntryAudit A
    LEFT JOIN Entry E ON A.entry_id = E.id
    LEFT JOIN User U ON A.done_by = U.id
    ORDER BY A.created_at DESC;
)*";

void AuditController::entries() {
    TSqlQuery query;
    query.prepare(entryAuditQuery);
    query.exec();
    QVector<MelpaltHelper::EntryAuditRecord> records;
    while (query.next()) {
        records.push_back({
            MelpaltHelper::create(query.value(0).toString()),
            query.value(1).toInt(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toInt(),
            query.value(5).toInt(),
            query.value(6).toInt(),
            query.value(7).toString(),
            query.value(8).toString(),
        });
    }
    texport(records);
    render();
}

static const char* userAuditQuery = R"*(
SELECT
    A.created_at, A.action_id,
    U1.username AS receiver, U2.username AS sender,
    A.oldtext, A.newtext
    FROM UserAudit A
    LEFT JOIN User U1 ON A.user = U1.id
    LEFT JOIN User U2 ON A.done_by = U2.id
    ORDER BY A.created_at DESC;
)*";

void AuditController::users() {
    TSqlQuery query;
    query.prepare(userAuditQuery);
    query.exec();
    QVector<MelpaltHelper::UserAuditRecord> records;
    while (query.next()) {
        records.push_back({
            MelpaltHelper::create(query.value(0).toString()),
            query.value(1).toInt(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
        });
    }
    texport(records);
    render();
}

void AuditController::viewChange(const QString& id) {
    int rid = id.toInt();
    auto revision = EntryAudit::get(rid);
    if (revision.isNull()) {
        QString message = "Revision not found.";
        texport(message);
        render("entries");
    }
    texport(revision);
    auto user = User::get(revision.doneBy());
    texport(user);
    auto word = Entry::get(revision.entryId());
    texport(word);
    render();
}

// Don't remove below this line
T_DEFINE_CONTROLLER(AuditController)
