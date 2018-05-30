#include "applicationcontroller.h"


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

bool ApplicationController::preFilter()
{
    return true;
}

// Don't remove below this line
T_DEFINE_CONTROLLER(ApplicationController)
