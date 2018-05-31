#ifndef ACCOUNTCONTROLLER_H
#define ACCOUNTCONTROLLER_H

#include "applicationcontroller.h"


class T_CONTROLLER_EXPORT AccountController : public ApplicationController
{
    Q_OBJECT
public:
    AccountController() : ApplicationController() { }

public slots:
    void form();
    void login();
    void logout();
    void regform();
    void regist();
    void list();
};

#endif // ACCOUNTCONTROLLER_H
