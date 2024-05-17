#include <iostream>
#include <locale>
#include "Class.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    auto type_1 = make_unique<string>("Вылет");
    auto time_1 = make_unique<tm>();
    time_1->tm_mday = 15;
    auto type_2 = make_unique<string>("Черный экран");
    auto time_2 = make_unique<tm>();
    time_2->tm_mday = 9;
    auto type_3 = make_unique<string>("Вылет");
    auto time_3 = make_unique<tm>();
    time_3->tm_mday = 26;

    auto log_1 = make_unique<SecurityLog>();
    auto name_1 = make_unique<string>("Angry Pigs");
    auto app_1 = make_unique<Application>(move(name_1), move(log_1));
    
    auto username = make_unique<string>("Александр");
    auto user = make_unique<User>(move(username));

    user->AddApp(move(app_1));

    auto user_apps = user->GetUserAppsInfo();
    user->AddAppBreach(make_unique<string>(*(*user_apps)[0]->appID), move(type_1), move(time_1));
    user->AddAppBreach(make_unique<string>(*(*user_apps)[0]->appID), move(type_2), move(time_2));
    user->AddAppBreach(make_unique<string>(*(*user_apps)[0]->appID), move(type_3), move(time_3));

    auto br_type = make_unique<string>("Вылет");
    auto breaches = user->GetAppBreaches(make_unique<string>(*(*user_apps)[0]->appID), move(br_type));
    
    for (auto item = breaches->begin(); item != breaches->end(); item++) {
        cout << *(*item)->breachID << " -- " << *(*item)->breachType << " -- " << (*item)->breachDate->tm_mday << "\n";
    }
}