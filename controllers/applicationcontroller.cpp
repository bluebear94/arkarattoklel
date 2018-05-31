#include "applicationcontroller.h"

#include "user.h"

ApplicationController::ApplicationController()
    : TActionController()
{ }

ApplicationController::~ApplicationController()
{ }

void ApplicationController::index()
{
    renderText("hello");
}

void ApplicationController::staticInitialize()
{ }

void ApplicationController::staticRelease()
{ }

bool ApplicationController::preFilter() {
    QString username = identityKeyOfLoginUser();
    texport(username);
    User user = User::getByIdentityKey(username);
    texport(user);
    userid = user.id();
    return true;
}

// Don't remove below this line
T_DEFINE_CONTROLLER(ApplicationController)
