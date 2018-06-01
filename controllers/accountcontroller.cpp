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

void AccountController::settings() {
    if (!isUserLoggedIn()) {
        redirect(urla("form"));
        return;
    }
    render();
}

void AccountController::changeSettings() {
    if (!isUserLoggedIn()) {
        redirect(urla("form"));
        return;
    }
    QString password = httpRequest().formItemValue("oldpassword");
    QString newpass = httpRequest().formItemValue("newpassword");
    if (password.isEmpty() || newpass.isEmpty()) {
        QString error = "Password cannot be empty.";
        texport(error);
        render("settings");
        return;
    }
    const char* msg;
    User user = User::get(loginID());
    User user2 = User::authenticate(user.username(), password, &msg);
    if (user2.isNull()) {
        QString error = msg;
        texport(error);
        render("settings");
        return;
    }
    bool stat = user.changePassword(newpass);
    if (!stat) {
        QString error = "Failed to change password for some reason.";
        texport(error);
        render("settings");
        return;
    }
    user.update();
    QString notice = "Changed password.";
    texport(notice);
    render("settings");
}

// Don't remove below this line
T_DEFINE_CONTROLLER(AccountController)
