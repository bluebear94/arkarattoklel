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
    UserAudit::create(user.id(), 0, USER_CREATE, QString(), QString());
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
        tflash(error);
        redirect(urla("settings"));
        return;
    }
    const char* msg;
    User user = User::get(loginID());
    User user2 = User::authenticate(user.username(), password, &msg);
    if (user2.isNull()) {
        QString error = msg;
        tflash(error);
        redirect(urla("settings"));
        return;
    }
    bool stat = user.changePassword(newpass);
    if (!stat) {
        QString error = "Failed to change password for some reason.";
        tflash(error);
        redirect(urla("settings"));
        return;
    }
    user.update();
    UserAudit::create(user.id(), user.id(), USER_PASSWORD, "", "");
    QString notice = "Changed password.";
    tflash(notice);
    redirect(urla("settings"));
}

void AccountController::edit(const QString& id) {
    auto appConfig = Tf::conf("application.ini");
    int minAuthPass = 10;
    if (appConfig.contains("MinLevelPassword")) {
        bool success;
        int v = appConfig.value("MinLevelPass").toInt(&success);
        if (success) minAuthPass = v;
    }
    CHECK_LOGIN;
    User agent = User::get(loginID());
    User patient = User::get(id.toInt());
    if (agent.regist() <= 1 || agent.regist() <= patient.regist()) {
        QString message = "You don't have permission to edit this account.";
        tflash(message);
        redirect(urla("list"));
        return;
    }
    bool canChangePassword = agent.regist() >= minAuthPass;
    QString error = "Could not edit profile: ";
    switch (httpRequest().method()) {
    case Tf::Get: {
        texport(canChangePassword);
        texport(patient);
        render();
        break;
    }
    case Tf::Post: {
        auto agentPass = httpRequest().formItemValue("oldpassword");
        auto patientPass = httpRequest().formItemValue("newpassword");
        auto regist = httpRequest().formItemValue("regist");
        const char* msg;
        User agentAuth = User::authenticate(
            agent.username(), agentPass, &msg);
        if (agentAuth.isNull()) {
            error.append("Authentication failed: ");
            error.append(msg);
            tflash(error);
            redirect(urla("edit", patient.id()));
            return;
        }
        bool ok;
        int oldLevel = patient.regist();
        int newLevel = regist.toInt(&ok);
        if (!ok) {
            error.append(regist);
            error.append(" is not an int");
            tflash(error);
            redirect(urla("edit", patient.id()));
            return;
        }
        if (newLevel >= agent.regist()) {
            error.append("New level must be below yours (");
            error.append(QString::number(agent.regist()));
            error.append(")");
            tflash(error);
            redirect(urla("edit", patient.id()));
            return;
        }
        patient.setRegist(newLevel);
        if (oldLevel != newLevel) {
            UserAudit::create(
                patient.id(), agent.id(),
                USER_CHANGE_REGIST, QString::number(oldLevel), regist);
        }
        if (canChangePassword) {
            if (!patientPass.isEmpty()) {
                patient.changePassword(patientPass);
                UserAudit::create(patient.id(), agent.id(), USER_PASSWORD, "", "");
            }
        }
        patient.update();
        QString notice = "Account updated.";
        tflash(notice);
        redirect(urla("edit", patient.id()));
        break;
    }
    default:
        renderErrorResponse(Tf::NotFound);
        break;
    }
}

// Don't remove below this line
T_DEFINE_CONTROLLER(AccountController)
