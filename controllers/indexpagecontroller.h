#ifndef INDEXPAGECONTROLLER_H
#define INDEXPAGECONTROLLER_H

#include "applicationcontroller.h"


class T_CONTROLLER_EXPORT IndexPageController : public ApplicationController
{
    Q_OBJECT
public:
    IndexPageController() : ApplicationController() {}

public slots:
    void index();
};

#endif // INDEXPAGECONTROLLER_H
