#include "entrycontroller.h"
#include "entry.h"


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
    switch (httpRequest().method()) {
    case Tf::Get:
        render();
        break;

    case Tf::Post: {
        auto entry = httpRequest().formItems("entry");
        auto model = Entry::create(entry);

        if (!model.isNull()) {
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

        auto entry = httpRequest().formItems("entry");
        model.setProperties(entry);
        if (model.save()) {
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
    if (httpRequest().method() != Tf::Post) {
        renderErrorResponse(Tf::NotFound);
        return;
    }

    auto entry = Entry::get(id.toInt());
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

void EntryController::search() {
    QString val = httpRequest().queryItemValue("word");
    search(val);
}


// Don't remove below this line
T_DEFINE_CONTROLLER(EntryController)
