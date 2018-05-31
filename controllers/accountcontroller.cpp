#include "accountcontroller.h"

#include "user.h"
#include "useraudit.h"

void AccountController::form() {
    userLogout();
    render();
}

void AccountController::regform() {
    userLogout();
    render();
}

void AccountController::login() {
    QString username = httpRequest().formItemValue("username");
    QString password = httpRequest().formItemValue("password");
    const char* msg;
    User user = User::authenticate(username, password, &msg);
    if (!user.isNull()) {
        // success
        userLogin(&user);
        redirect(QUrl("/"));
    } else {
        QString message(msg);
        texport(message);
        render("form");
    }
}

void AccountController::regist() {
    QString username = httpRequest().formItemValue("username");
    QString password = httpRequest().formItemValue("password");
    QString confirm = httpRequest().formItemValue("confirm");
    if (QString::compare(
            confirm, "Seren Arbazard", Qt::CaseInsensitive) != 0) {
        QString message = "<hacm>tee, tee, tee!</hacm>";
        texport(message);
        render("regform");
        return;
    }
    if (username.isEmpty() || password.isEmpty()) {
        QString message = "Username and password cannot be empty";
        texport(message);
        render("regform");
        return;
    }
    User user = User::create(username, password, 0);
    if (user.isNull()) {
        QString message = "Cannot create account. Is the username taken?";
        texport(message);
        render("regform");
        return;
    }
    QVariantMap values;
    values["user"] = user.id();
    values["action"] = USER_CREATE;
    UserAudit::create(values);
    render("regok");
}

void AccountController::logout() {
    userLogout();
    redirect(QUrl("/"));
}

void AccountController::list() {
    QList<User> userList = User::getAll();
    texport(userList);
    render();
}

// Don't remove below this line
T_DEFINE_CONTROLLER(AccountController)
