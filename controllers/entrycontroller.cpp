#include "entrycontroller.h"

#include <QRegExp>

#include <tsqlqueryormapper.h>
#include <tsqlqueryormapperiterator.h>

#include "entry.h"
#include "entryaudit.h"
#include "sqlobjects/entryobject.h"
#include "melpalthelper.h"

#define CHECK_LOGIN \
    if (!isUserLoggedIn()) { \
        QString message = "Please log in to edit the dictionary."; \
        tflash(message); \
        redirect(QUrl("/Account/form")); \
        return; \
    }

void EntryController::index()
{
    auto entryList = Entry::getAll();
    texport(entryList);
    render();
}

void EntryController::show(const QString &id)
{
    auto entry = Entry::get(id.toInt());
    texport(entry);
    render();
}

void EntryController::create()
{
    CHECK_LOGIN
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto entry = httpRequest().formItems("entry");
        auto model = Entry::create(entry);

        if (!model.isNull()) {
            EntryAudit::create(
                model.id(), loginID(), ENTRY_CREATE, "", model.body());
            QString notice = "Created successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            auto existing = Entry::get(entry["name"].toString());
            if (!existing.isNull()) {
                QString msg = "This word already exists; redirecting.";
                tflash(msg);
                redirect(urla("save", existing.id()));
            } else {
                QString error = "Failed to create.";
                texport(error);
                texport(entry);
                render();
            }
        }
        break; }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EntryController::save(const QString &id)
{
    CHECK_LOGIN
    switch (httpRequest().method()) {
    case Tf::Get: {
        auto model = Entry::get(id.toInt());
        if (!model.isNull()) {
            session().insert("entry_lockRevision", model.lockRevision());
            auto entry = model.toVariantMap();
            texport(entry);
            render();
        }
        break; }

    case Tf::Post: {
        QString error;
        int rev = session().value("entry_lockRevision").toInt();
        auto model = Entry::get(id.toInt(), rev);
        
        if (model.isNull()) {
            error = "Original data not found. It may have been updated/removed by another transaction.";
            tflash(error);
            redirect(urla("save", id));
            break;
        }
        QString oldBody = model.body();
        QString oldTitle = model.name();
        auto entry = httpRequest().formItems("entry");
        model.setProperties(entry);
        if (model.save()) {
            if (model.body() != oldBody)
                EntryAudit::create(
                    entry["id"].toInt(), loginID(), ENTRY_EDIT, oldBody, model.body());
            if (model.name() != oldTitle)
                EntryAudit::create(
                    entry["id"].toInt(), loginID(), ENTRY_RENAME, oldTitle, model.name());
            QString notice = "Updated successfully.";
            tflash(notice);
            redirect(urla("show", model.id()));
        } else {
            error = "Failed to update.";
            texport(error);
            texport(entry);
            render();
        }
        break; }

    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

void EntryController::remove(const QString &id)
{
    CHECK_LOGIN
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto entry = Entry::get(id.toInt());
    EntryAudit::create(entry.id(), loginID(), ENTRY_REMOVE, entry.body(), "");
    entry.remove();
    redirect(urla("index"));
}

void EntryController::search(const QString& name) {
    auto entry = Entry::get(name);
    if (!entry.isNull()) {
        redirect(urla("show", entry.id()));
    } else {
        texport(name);
        render("nosuchword");
    }
}

static const char* rsearchQuery = R"*(
SELECT
	E.id, E.name, rank,
	highlight(EntryFTS, 0, "«", "»")
	FROM EntryFTS I
	JOIN Entry E on I.rowid = E.id
	WHERE I.body MATCH ?
    ORDER BY rank DESC;
)*";

void EntryController::rsearch(const QString& name) {
    TSqlQuery query;
    query.setForwardOnly(true);
    query.prepare(rsearchQuery);
    query.addBind(QVariant(name));
    query.exec();
    QVector<MelpaltHelper::EntryRSearchRecord> entries;
    while (query.next()) {
        entries.push_back({
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toDouble(),
            query.value(3).toString(),
        });
    }
    texport(entries);
    render();
}

void EntryController::search() {
    QString val = httpRequest().queryItemValue("word");
    search(val);
}

void EntryController::rsearch() {
    QString val = httpRequest().queryItemValue("eword");
    rsearch(val);
}


// Don't remove below this line
T_DEFINE_CONTROLLER(EntryController)
