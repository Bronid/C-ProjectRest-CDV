#include <iostream>
#include <fstream>
#include <string>
#include <json/value.h>
#include <json/json.h>
#include "TakeOrder.h"

using namespace std;
string setupPATH = "data/setup.json";
string STANDARDNAME = "Rest of group 2";
// Start of application
void AppStart() {
    ifstream setupfile(setupPATH);
    if (!setupfile.is_open()) {
        ofstream newsetupfile(setupPATH);
        newsetupfile << "{" << '"' << "restaurant_name" << '"' << ":" << '"' << STANDARDNAME << '"' << "}";
        newsetupfile.close();
        setupfile.open(setupPATH);
    }
    Json::Reader reader;
    Json::Value root;
    bool parsingSuccessful = reader.parse(setupfile, root);
    if (!parsingSuccessful)
    {
        cout << endl << "File not found";
    }
    Json::Value restname = root["restaurant_name"];
    cout << "Start aplikacji." << endl;
    cout << "Witam w " << restname.asString() << endl;
    setupfile.close();
}


int main() {
    AppStart();
    int keyPressed;

    do {
        cout << endl << "Opcje: " << endl << "[0] Zamkniecie programu" << endl
            << "[1] Zlozenie zamowienia" << endl << "[2] Historia zamówień" << endl
            << "[3] Historia zamówień dla danego dnia" << endl << endl
            << "Prosze podac opcje: ";

        cin >> keyPressed;

        switch (keyPressed) {
        case 0:
            break;
        case 1:
            takeOrder();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            cout << endl << "Nie wybrano poprawnej opcji." << endl;
            break;
        }

    } while (keyPressed != 0);

    cout << "Podano 0 - program konczy dzialanie";
    return 0;
}

