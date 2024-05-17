#include "Class.h"

static unique_ptr<string> MakeUUID() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9999);

    char id[16];
    snprintf(id, sizeof(id), "%04d-%04d-%04d", dis(gen), dis(gen), dis(gen));
    return make_unique<string>(id);
}
//
// Класс SecurityLog
//
SecurityLog::SecurityLog() {
    this->breaches = make_unique<vector<unique_ptr<BreachInfo>>>();
    cout << "Создан объект класса SecurityLog\n";
}
unique_ptr<vector<unique_ptr<BreachInfo>>> SecurityLog::GetBreaches(unique_ptr<string> breachType) {
    bool isCopyAll = false;
    if (breachType == nullptr)
        isCopyAll = true;
    auto breachesCopy = make_unique<vector<unique_ptr<BreachInfo>>>();
    for (const auto& breach : *breaches) {
        if ((*breach->breachType == *breachType) || isCopyAll) {
            auto info = make_unique<BreachInfo>();
            info->breachID = make_unique<string>(*breach->breachID);
            info->breachType = make_unique<string>(*breach->breachType);
            info->breachDate = make_unique<tm>(*breach->breachDate);
            breachesCopy->push_back(move(info));
        }
    }
    return breachesCopy;
}
void SecurityLog::AddBreach(unique_ptr<string> breachType, unique_ptr<tm> breachDate) {
    unique_ptr<BreachInfo> info = make_unique<BreachInfo>(
        move(MakeUUID()), move(breachType), move(breachDate)
    );
    this->breaches->push_back(move(info));
}
void SecurityLog::RemoveBreach(unique_ptr<string> breachID) {
    for (auto item = breaches->begin(); item != breaches->end(); ) {
        if (*(*item)->breachID == *breachID) {
            item = breaches->erase(item);
        }
        else {
            ++item;
        }
    }
}
//
// Класс Application
//
Application::Application(unique_ptr<string> appName, unique_ptr<SecurityLog> appLog) {
    if (appName == nullptr) 
        appName = make_unique<string>("ERROR-NAME");
    this->appInfo = make_unique<AppInfo>(move(MakeUUID()), move(appName));
    if (appLog == nullptr) 
        appLog = make_unique<SecurityLog>();
    this->appLog = move(appLog);
    cout << "Создан объект класса Application\n";
}
unique_ptr<AppInfo> Application::GetAppInfo() {
    auto info = make_unique<AppInfo>();
    info->appID = make_unique<string>(*this->appInfo->appID);
    info->appName = make_unique<string>(*this->appInfo->appName);
    return info;
}
unique_ptr<vector<unique_ptr<BreachInfo>>> Application::GetAppBreaches(unique_ptr<string> breachType) {
    return this->appLog->GetBreaches(move(breachType));
}
void Application::AddAppBreach(unique_ptr<string> breachType, unique_ptr<tm> breachDate) {
    this->appLog->AddBreach(move(breachType), move(breachDate));
}
void Application::RemoveAppBreach(unique_ptr<string> breachID) {
    this->appLog->RemoveBreach(move(breachID));
}
//
// Класс User
//
User::User(unique_ptr<string> userName) {
    if (userName == nullptr)
        userName = make_unique<string>("ERROR-NAME");
    this->userInfo = make_unique<UserInfo>(move(MakeUUID()), move(userName));
    this->userApps = make_unique<vector<unique_ptr<Application>>>();
    cout << "Создан объект класса User\n";
}
unique_ptr<UserInfo> User::GetUserInfo() {
    auto info = make_unique<UserInfo>();
    info->userID = make_unique<string>(*this->userInfo->userID);
    info->userName = make_unique<string>(*this->userInfo->userName);
    cout << "Возвращена информация о пользователе\n";
    return info;
}
unique_ptr<vector<unique_ptr<AppInfo>>> User::GetUserAppsInfo() {
    auto info = make_unique<vector<unique_ptr<AppInfo>>>();
    for (auto item = this->userApps->begin(); item != this->userApps->end(); ) {
        info->push_back(move((*item)->GetAppInfo()));
        ++item;
    }
    cout << "Возвращена информация о приложениях пользователя\n";
    return info;
}
void User::AddApp(unique_ptr<Application> app) {
    this->userApps->push_back(move(app));
    cout << "Добавлено приложение\n";
}
void User::RemoveApp(unique_ptr<string> appID) {
    for (auto item = this->userApps->begin(); item != this->userApps->end(); ) {
        if (*(*item)->GetAppInfo()->appID == *appID) {
            item = userApps->erase(item);
            cout << "Удалено приложение\n";
            return;
        }
        else {
            ++item;
        }
    }
    cout << "Не найдено приложение с заданным ID\n";
}
void User::AddAppBreach(unique_ptr<string> appID, unique_ptr<string> breachType, unique_ptr<tm> breachDate) {
    for (auto item = this->userApps->begin(); item != this->userApps->end(); ) {
        if (*(*item)->GetAppInfo()->appID == *appID) {
            (*item)->AddAppBreach(move(breachType), move(breachDate));
            break;
        }
        else {
            ++item;
        }
    }
    cout << "Добавлено нарушение программы\n";
}
void User::RemoveAppBreach(unique_ptr<string> appID, unique_ptr<string> breachID) {
    for (auto item = this->userApps->begin(); item != this->userApps->end(); ) {
        if (*(*item)->GetAppInfo()->appID == *appID) {
            (*item)->RemoveAppBreach(move(breachID));
            cout << "Удалено нарушение программы\n";
            return;
        }
        else {
            ++item;
        }
    }
    cout << "Не найдено приложение с заданным ID\n";
}
unique_ptr<vector<unique_ptr<BreachInfo>>> User::GetAppBreaches(unique_ptr<string> appID, unique_ptr<string> breachType) {
    for (auto item = this->userApps->begin(); item != this->userApps->end(); ) {
        if (*(*item)->GetAppInfo()->appID == *appID) {
            auto info = (*item)->GetAppBreaches(move(breachType));
            cout << "Возвращена информация о нарушениях выбранного приложения\n";
            return info;
        }
        else {
            ++item;
        }
    }
    cout << "Не найденно приложение с указанным ID\n";
    return nullptr;
}