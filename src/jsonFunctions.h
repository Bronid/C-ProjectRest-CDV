#ifndef JSONFUNCTIONS_H
#define JSONFUNCTIONS_H

#include <json/value.h>
#include <json/json.h>
#include <string>

using namespace std;

const string menuPATH = "data/menuSettings.json";
const string setupPATH = "data/setup.json";
const string historyPATH = "data/historyList.json";

Json::Value readFromFile(string PATH);
void writeToHistoryFile(int orderedFood[], int MaxOrders, int FoodCounter);

#endif
