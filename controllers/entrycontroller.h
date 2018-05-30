#ifndef ENTRYCONTROLLER_H
#define ENTRYCONTROLLER_H

#include "applicationcontroller.h"


class T_CONTROLLER_EXPORT EntryController : public ApplicationController
{
    Q_OBJECT
public:
    EntryController() : ApplicationController() {}

public slots:
    void index();
    void show(const QString& id);
    void create();
    void save(const QString& id);
    void remove(const QString& id);
    void search(const QString& name);
    void search();
};

#endif // ENTRYCONTROLLER_H
