#ifndef AUDITCONTROLLER_H
#define AUDITCONTROLLER_H

#include "applicationcontroller.h"

class T_CONTROLLER_EXPORT AuditController : public ApplicationController
{
    Q_OBJECT
public:
    AuditController() : ApplicationController() { }

public slots:
    void entries();
    void users();
    void viewChange(const QString& id);
};

#endif // AUDITCONTROLLER_H
