#include <iostream>
#include <fstream>
#include <cstdlib>
#include <json/value.h>
#include <json/json.h>
#include <ctime>
#include "jsonFunctions.h"


using namespace std;

const int maxOrders = 10;
int menuWidth = 40;
int orderedFood[maxOrders];
int FoodCounter = 0;
string menuHeader = " MENU ";
bool confirmRepeat = true;

void clearOrderedFoodList() {
    for (int i = 0; i < maxOrders; i++) orderedFood[i] = NULL;
}

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

void orderList() {
    Json::Value food = readFromFile(menuPATH)["Food"];
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
        if (f + 1 == maxOrders && FoodNumber != FoodCounter) {
            FoodNumber++;
            count = 0;
            // żeby f == 0;
            f = -1;
        }
    }
    cout << endl << "Suma: " << Sum;
}

void addToOrder() {
    cout << "Prosze podac numery id dan jakie maja zostac dodane do zamowienia," << endl
        << "wpisanie 0 spowoduje przejscie do nastepnego kroku." << endl;
    int keyPressed;
    int OrderedFoodAmount = 0;
    int StartNumber = 0;

    for (int i = 0; i < maxOrders; i++) {
        if (orderedFood[i] != NULL) {
            StartNumber++;
        }
    }

    for (int i = StartNumber; i < maxOrders; i++) {
        cout << endl << "Prosze podac numer id dania: ";
        cin >> keyPressed;
        if (keyPressed > FoodCounter || keyPressed < 0) {
            i--;
            cout << "Masz do wyboru tylko " << FoodCounter << " dan";
            continue;
        }
        if (keyPressed == 0) {
            break;
        }
        orderedFood[i] = keyPressed;
        OrderedFoodAmount++;
    }
    cout << endl << "Dodano lacznie " << OrderedFoodAmount << " dan:" << endl;
    orderList();
}

void endofOrder() {
    writeToHistoryFile(orderedFood, maxOrders, FoodCounter);
    //clearing array after all operations
    clearOrderedFoodList();
    FoodCounter = 0;
}

void sortOrderedFood() {
    int tempSortArray[maxOrders];
    int tempIndex = 0;
    for (int i = 0; i < maxOrders; i++) {
        if (orderedFood[i] != NULL) {
            tempSortArray[tempIndex] = orderedFood[i];
            tempIndex++;
        }
    }

    clearOrderedFoodList();

    for (int i = 0; i < tempIndex; i++) {
        orderedFood[i] = tempSortArray[i];
    }
}

void deleteFromOrder() {
    int deleteorderedFood[maxOrders];
    int keyPressed;
    do {
        cout << endl << "Co trzeba usunac?" << endl << "wpisanie 0 spowoduje przejscie do nastepnego kroku."
            << endl << "Podaj ID: ";
        cin >> keyPressed;
        for (int i = 0; i < maxOrders; i++) {
            if (keyPressed == 0) break;
            if (keyPressed == orderedFood[i]) {
                orderedFood[i] = NULL;
                cout << endl << "ID " << keyPressed << " Usunięto!" << endl;
                break;
            }
            if (i + 1 == maxOrders) {
                cout << endl << "ID " << keyPressed << " nie byl dodany do listy zamowien" << endl;
            }
        }
    } while (keyPressed != 0);
    sortOrderedFood();
    orderList();
}

void editOrder() {
    bool keyIsPressedRight = false;
        while (!keyIsPressedRight) {
            char keyPressed;
            cout << endl << "Wpisz '+' czyli 'Dodac do zamowienia' albo '-' czyli 'Usunac z zamowienia'" <<
                endl << "Podaj operacje: ";
            cin >> keyPressed;
            if (keyPressed == '+') {
                keyIsPressedRight = true;
                addToOrder();
            }
            if (keyPressed == '-') {
                keyIsPressedRight = true;
                deleteFromOrder();
            }
        }
}

void confirmOrder() {
    bool isConfirmed = false;
    while (!isConfirmed) {
        char keyPressed;
        cout << endl << endl << "Potwierdzic zamowienie[T/N]?: ";
        cin >> keyPressed;
        if (keyPressed == 'T' || keyPressed == 't') {
            isConfirmed = true;
            confirmRepeat = false;
            endofOrder();
        }
        else if (keyPressed == 'N' || keyPressed == 'n') {
            isConfirmed = true;
            editOrder();
        }
        else {
            cout << endl << "Wpisz T czyli TAK albo N czyli NIE";
        }
    }
}

void menuContent() {
    menuTop();
    Json::Value types = readFromFile(menuPATH)["Types"];
    Json::Value food = readFromFile(menuPATH)["Food"];
    for (int i = 0; i < types.size(); i++) {
        cout << "| " << types[i].asString();
        for (int g = 0; g < menuWidth - (types[i].asString().size() + 2); g++) cout << " ";
        cout << "|" << endl;
        for (int f = 0; f < food.size(); f++) {
            if (food[f]["type"].asString() == types[i].asString()) {
                FoodCounter++;
                cout << "| " << FoodCounter << "." << food[f]["name"].asString();
                dotLine(food[f]["name"].asString().size());
                cout << food[f]["price"].asFloat() << " |" << endl;
            }
        }
        menuEmptySpace();
    }
    menuBottom();
    cout << endl;
}

void takeOrder() {
    confirmRepeat = true;
    menuContent();
    addToOrder();
    while (confirmRepeat) {
        confirmOrder();
    }
}