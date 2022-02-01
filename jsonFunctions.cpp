#include <iostream>
#include <fstream>
#include <string>
#include <json/value.h>
#include <json/json.h>
#include "jsonFunctions.h"
#include "TakeOrder.h"
#pragma warning(disable : 4996)

using namespace std;

string genRandomID(const int len) {
    bool isConfirmed = false;
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    srand(time(NULL));
    do {
        tmp_s.reserve(len);
        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        if (readFromFile(historyPATH)["History"].size() != 0) {
            for (int i = 0; i < readFromFile(historyPATH)["History"].size(); i++) {
                if (readFromFile(historyPATH)["History"][i]["ID"].asString() == tmp_s) {
                    tmp_s.clear();
                    break;
                }
                if (i + 1 == readFromFile(historyPATH)["History"].size()) isConfirmed = true;
            }
        }
        else {
            isConfirmed = true;
        }
    } while (!isConfirmed);
    return tmp_s;
}

string getCurrentDate() {
    string date;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    date += to_string(1900 + ltm->tm_year);
    date += "-";
    if (1 + ltm->tm_mon < 10) date += "0";
    date += to_string(1 + ltm->tm_mon);
    date += "-";
    if (ltm->tm_mday < 10) date += "0";
    date += to_string(ltm->tm_mday);
    return date;
}

Json::Value readFromFile(string PATH) {
    ifstream file(PATH);
    if (!file.is_open()) {
        ofstream newfile(PATH);
        Json::Value root;
        if (PATH == setupPATH) {
            newfile << "{" << endl << '"' << "restaurant_name" << '"' << ':' << '"' << "Standard name" << '"' << ','
                << endl << '"' << "menu_header" << '"' << ':' << '"' << "  MENU  " << '"' << ',' << endl
                << '"' << "menu_width" << '"' << ':' << ' ' << 40 << endl << '}';
        }
        else if (PATH == menuPATH) {
            newfile << "{" << endl << '"' << "Types" << '"' << ':' << ' ' << '[' << endl << ']' << ',' << endl
                << '"' << "Food" << '"' << ':' << ' ' << '[' << endl << '{' << endl << '"' << "type" << '"' << ':' << '"' << '"' << ',' << endl
                << '"' << "name" << '"' << ':' << '"' << '"' << ',' << endl << '"' << "price" << '"' << ':' << 0 << endl <<
                '}' << endl << ']' << endl << '}';
        }
        else if (PATH == historyPATH) {
            newfile << "{" << endl << '"' << "History" << '"' << ':' << '[' << endl << ']' << endl << '}';
        }
        else {
            cout << "Nie ma takiej sciezki do pliku!";
            return EXIT_FAILURE;
        }
        newfile.close();
        file.open(PATH);
    }
    Json::Reader reader;
    Json::Value readedfile;
    reader.parse(file, readedfile);
    file.close();
    return readedfile;
}

void writeToHistoryFile(int orderedFood[], int MaxOrders, int FoodCounter) {
    readFromFile(historyPATH);
    ifstream file(historyPATH);
    string filetext;
    stringstream ss;
    ss << file.rdbuf();
    filetext = ss.str();
    file.close();
    for (int i = 0; i < 3; i++) filetext.pop_back();
    Json::Value root;
    root["Date"] = getCurrentDate();
    root["ID"] = genRandomID(10);
    float Sum = 0;
    int FoodNumber = 1;
    int count = 0;
    int index = 0;
    for (int i = 0; i < MaxOrders; i++) {
        if (orderedFood[i] == FoodNumber) count++;
        if (i + 1 == MaxOrders && count > 0) {
            root["Food"][index] = readFromFile(menuPATH)["Food"][FoodNumber - 1]["name"].asString();
            root["Price"][index] = readFromFile(menuPATH)["Food"][FoodNumber - 1]["price"].asFloat();
            root["N"][index] = count;
            root["P * N"][index] = count * (readFromFile(menuPATH)["Food"][FoodNumber - 1]["price"].asFloat());
            Sum += count * readFromFile(menuPATH)["Food"][FoodNumber - 1]["price"].asFloat();
            index++;
        }
        if (i + 1 == MaxOrders && FoodNumber != FoodCounter) {
            FoodNumber++;
            count = 0;
            // żeby i == 0;
            i = -1;
        }
    }
    root["Sum"] = Sum;
    Json::StreamWriterBuilder builder;
    std::string json_file = Json::writeString(builder, root);
    if (readFromFile(historyPATH)["History"].size() != 0) filetext += ",";
    filetext += "\n";
    filetext += json_file;
    filetext += "\n";
    filetext += "]";
    filetext += "\n";
    filetext += "}";
    ofstream offile(historyPATH);
    offile << filetext;
    offile.close();
}