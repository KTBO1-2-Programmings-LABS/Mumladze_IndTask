#pragma once

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <iostream>

using namespace std;

static unique_ptr<string> MakeUUID();

struct BreachInfo {
	unique_ptr<string> breachID;
	unique_ptr<string> breachType;
	unique_ptr<tm> breachDate;
	BreachInfo() = default;
	BreachInfo(unique_ptr<string> id, unique_ptr<string> type, unique_ptr<tm> date)
		: breachID(move(id)), breachType(move(type)), breachDate(move(date)) {}
};

class SecurityLog {
private:
	unique_ptr<vector<unique_ptr<BreachInfo>>> breaches;
public:
	SecurityLog();
	unique_ptr<vector<unique_ptr<BreachInfo>>> GetBreaches(unique_ptr<string> breachType);
	void AddBreach(unique_ptr<string> breachType, unique_ptr<tm> breachDate);
	void RemoveBreach(unique_ptr<string> breachID);
};

struct AppInfo {
	unique_ptr<string> appID;
	unique_ptr<string> appName;
	AppInfo() = default;
	AppInfo(unique_ptr<string> id, unique_ptr<string> name)
		: appID(move(id)), appName(move(name)) {}
};

class Application {
private:
	unique_ptr<AppInfo> appInfo;
	unique_ptr<SecurityLog> appLog;
public:
	Application(unique_ptr<string> appName, unique_ptr<SecurityLog> appLog);
	unique_ptr<AppInfo> GetAppInfo();
	unique_ptr<vector<unique_ptr<BreachInfo>>> GetAppBreaches(unique_ptr<string> breachType);
	void AddAppBreach(unique_ptr<string> breachType, unique_ptr<tm> breachDate);
	void RemoveAppBreach(unique_ptr<string> breachID);
};

struct UserInfo {
	unique_ptr<string> userID;
	unique_ptr<string> userName;
	UserInfo() = default;
	UserInfo(unique_ptr<string> id, unique_ptr<string> name)
		: userID(move(id)), userName(move(name)) {}
};

class User {
private:
	unique_ptr<UserInfo> userInfo;
	unique_ptr<vector<unique_ptr<Application>>> userApps;
public:
	User(unique_ptr<string> userName);
	unique_ptr<UserInfo> GetUserInfo();
	unique_ptr<vector<unique_ptr<AppInfo>>> GetUserAppsInfo();
	void AddApp(unique_ptr<Application> app);
	void RemoveApp(unique_ptr<string> appID);
	void AddAppBreach(unique_ptr<string> appID, unique_ptr<string> breachType, unique_ptr<tm> breachDate);
	void RemoveAppBreach(unique_ptr<string> appID, unique_ptr<string> breachID);
	unique_ptr<vector<unique_ptr<BreachInfo>>> GetAppBreaches(unique_ptr<string> appID, unique_ptr<string> breachType);
};