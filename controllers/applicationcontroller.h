#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <TActionController>
#include "applicationhelper.h"


class T_CONTROLLER_EXPORT ApplicationController : public TActionController
{
    Q_OBJECT
public:
    ApplicationController();
    virtual ~ApplicationController();

public slots:
    void index();
    void staticInitialize();
    void staticRelease();

protected:
    virtual bool preFilter();
    int loginID() { return userid; }
    int userid;
};

#endif // APPLICATIONCONTROLLER_H
