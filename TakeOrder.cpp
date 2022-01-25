#include <iostream>
#include <fstream>
#include <cstdlib>
#include <json/value.h>
#include <json/json.h>

using namespace std;

string menuPATH = "data/menu_settings.json";
string testPATH = "data/billList.json";
const int maxOrders = 10;
int menuWidth = 40;
int FoodID = 1;
string menuHeader = " MENU ";
int orderedFood[maxOrders];
bool confirmRepeat = true;

void menuEmptySpace() {
    string emptyLine;
    emptyLine += "|";
    for (int i = 0; i < menuWidth - 1; i++) emptyLine += " ";
    emptyLine += "|";
    emptyLine += "\n";

    cout << emptyLine;
}

void menuTop() {
    string topLine;

    for (int i = 0; i < (menuWidth - menuHeader.length()) / 2; i++) {
        topLine += "-";
    }

    topLine += menuHeader;
    for (int i = 0; i < (menuWidth - menuHeader.length()) / 2; i++) {
        topLine += "-";
    }
    topLine += "\n";
    cout << topLine;
    menuEmptySpace();
}

void menuBottom() {
    string bottomLine;
    for (int i = 0; i < menuWidth; i++) bottomLine += "-";
    bottomLine += "\n";

    cout << bottomLine;
} 

void dotLine(int foodNameSize) {
    for (int n = 0; n < menuWidth - foodNameSize - 10; n++) {
        cout << ".";
    }
}

void createOrder() {
    cout << "Prosze podac numery id dan jakie maja zostac dodane do zamowienia," << endl
        << "wpisanie 0 spowoduje przejscie do nastepnego kroku." << endl;
    int keyPressed;
    int FoodCounter = 0;
    for (int i = 0; i < maxOrders; i++) {
        cout << endl << "Prosze podac numer id dania: ";
        cin >> keyPressed;
        if (keyPressed > FoodID || keyPressed < 0) {
            i--;
            cout << "Masz do wyboru tylko " << FoodID << " dan";
            continue;
        }
        if (keyPressed == 0) {
            break;
        }
        orderedFood[i] = keyPressed;
        FoodCounter++;
    }
    cout << endl << "Wybrano lacznie " << FoodCounter << " dan:" << endl;
}

void endofOrder() {
    ifstream setupfile(testPATH);
    if (!setupfile.is_open()) {
        ofstream newsetupfile(testPATH);
        newsetupfile << "{" << '"' << "Bills" << '"' << ":" << "[" << "]" << "}";
        newsetupfile.close();
        setupfile.open(testPATH);
    }
    Json::Reader reader;
    Json::Value root;
    Json::StyledStreamWriter writer;
    bool parsingSuccessful = reader.parse(setupfile, root);
    if (!parsingSuccessful)
    {
        cout << endl << "File not found";
    }
    setupfile.close();
}

void orderList() {
    ifstream menufile(menuPATH);
    if (!menufile.is_open()) {
        ofstream newsetupfile(menuPATH);
        newsetupfile << 'a'; //Zrobic to
        newsetupfile.close();
        menufile.open(menuPATH);
    }
    Json::Reader reader;
    Json::Value root;
    bool parsingSuccessful = reader.parse(menufile, root);
    if (!parsingSuccessful)
    {
        cout << endl << "File not found";
    }
    Json::Value food = root["Food"];
    float Sum = 0;
    int FoodNumber = 1;
    int count = 0;
    for (int f = 0; f < maxOrders; f++) {
        if (orderedFood[f] == FoodNumber) count++;
        if (f + 1 == maxOrders && count > 0) {
            cout << "- " << "[";
            if (FoodNumber < 10) cout << "0";
            cout << FoodNumber << "]" << food[FoodNumber - 1]["name"].asString();
            dotLine(food[FoodNumber - 1]["name"].asString().size());
            cout << " x" << count << " = " << count * food[FoodNumber - 1]["price"].asFloat() << endl;
            Sum += count * food[FoodNumber - 1]["price"].asFloat();
        }
        if (f + 1 == maxOrders && FoodNumber != FoodID) {
            FoodNumber++;
            count = 0;
            // żeby f == 0;
            f = -1;
        }
    }
    cout << endl << "Suma: " << Sum;
}

void editOrder() {
    int deleteorderedFood[maxOrders];
    int forDelete;
    do {
        cout << endl << "Co trzeba usunac?" << endl << "wpisanie 0 spowoduje przejscie do nastepnego kroku."
            << endl << "Podaj ID: ";
        cin >> forDelete;
        for (int i = 0; i < maxOrders; i++) {
            if (forDelete == 0) break;
            if (forDelete == orderedFood[i]) {
                orderedFood[i] = NULL;
                cout << endl << "ID " << forDelete << " Usunięto!" << endl;
                break;
            }
            if (i + 1 == maxOrders) {
                cout << endl << "ID " << forDelete << " nie byl dodany do listy zamowien" << endl;
            }
        }
    } while (forDelete != 0);
    orderList();
}

void confirmOrder() {
    bool Confirmation = true;
    while (Confirmation) {
        char keyPressed;
        cout << endl << endl << "Potwierdzic zamowienie[T/N]?: ";
        cin >> keyPressed;
        if (keyPressed == 'T' || keyPressed == 't') {
            Confirmation = false;
            confirmRepeat = false;
            endofOrder();
        }
        else if (keyPressed == 'N' || keyPressed == 'n') {
            Confirmation = false;
            editOrder();
        }
        else {
            cout << endl << "Wpisz T czyli TAK albo N czyli NIE";
        }
    }
}

void menuContent() {
    menuTop();
    ifstream menufile(menuPATH);
    if (!menufile.is_open()) {
        ofstream newsetupfile(menuPATH);
        newsetupfile << 'a'; //Zrobic to
        newsetupfile.close();
        menufile.open(menuPATH);
    }
    Json::Reader reader;
    Json::Value root;
    bool parsingSuccessful = reader.parse(menufile, root);
    if (!parsingSuccessful)
    {
        cout << endl << "File not found";
    }
    Json::Value types = root["Types"];
    Json::Value food = root["Food"];
    for (int i = 0; i < types.size(); i++) {
        cout << "| " << types[i].asString();
        for (int g = 0; g < menuWidth - (types[i].asString().size() + 2); g++) cout << " ";
        cout << "|" << endl;
        for (int f = 0; f < food.size(); f++) {
            if (food[f]["type"].asString() == types[i].asString()) {
                cout << "| " << FoodID << "." << food[f]["name"].asString();
                FoodID++;
                dotLine(food[f]["name"].asString().size());
                cout << food[f]["price"].asFloat() << " |" << endl;
            }
        }
        menuEmptySpace();
    }
    menufile.close();
    FoodID--;
    menuBottom();
    cout << endl;
}

void takeOrder() {
    menuContent();
    createOrder();
    orderList();
    while (confirmRepeat) {
        confirmOrder();
    }
    FoodID = 1;
}