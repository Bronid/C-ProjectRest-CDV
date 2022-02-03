#include <iostream>
#include <fstream>
#include <string>
#include "jsonFunctions.h"

using namespace std;

//pobieranie ilosci liter w nazwie produktu
int getMaxFoodSize() {
    int MaxFoodSize = 0;
    for (int i = 0; i < readFromFile(menuPATH)["Food"].size(); i++) {
        if (readFromFile(menuPATH)["Food"][i]["name"].asString().length() > MaxFoodSize) {
            MaxFoodSize = readFromFile(menuPATH)["Food"][i]["name"].asString().length();
        }
    }
    return MaxFoodSize;
}

int numberLength = 4;
int dateLength = 8;
int idLength = 12;
int foodLength = 2 + getMaxFoodSize();
int priceLength = 4;
int countLength = 4;
int countMultipliedByPriceLength = 4;
int sumLength = 8;
string historyVars[] = { "#", "Date", "ID", "Food", "Price", "N", "P * N", "Sum" };
int historyLengths[] = { numberLength, dateLength, idLength, foodLength, priceLength, countLength, 
countMultipliedByPriceLength, sumLength };

//GUI ^
void draw_line() {
    int lineLenght = 0;
    for (int i = 0; i < 8; i++) {
        lineLenght += historyLengths[i];
        lineLenght += historyVars[i].length();
    }
    cout << string(lineLenght + 8, '-') << endl;
}

void draw_header() {
    bool draw = true;
    draw_line();
    for (int i = 0; i < 8; i++) {
        cout << "|" << string(historyLengths[i] / 2, ' ') << historyVars[i] << string(historyLengths[i] / 2, ' ');
    }
    cout << "|" << endl;
    draw_line();
}

void draw_emptyspace() {
    for (int i = 0; i < 3; i++) {
        cout << "|";
        int emptyLength = 0;
        emptyLength += historyLengths[i];
        emptyLength += historyVars[i].length();
        cout << string(emptyLength, ' ');
    }
}

void draw_emptyspaceforsum() {
    cout << "|";
    for (int i = 0; i < 7; i++) {
        int emptyLength = 0;
        emptyLength += historyLengths[i] / 2;
        emptyLength += historyLengths[i] / 2;
        emptyLength += historyVars[i].length();
        cout << string(emptyLength, ' ');
        cout << "|";
    }
}

void draw_freespacesum() {
    cout << "|" << string((historyLengths[7] / 2) + 1, ' ');
    cout << "-" << string((historyLengths[7] / 2) + 1, ' ');
    cout << "|" << endl;
}
//GUI v

//pobieranie i wypisywanie (sumy)
void draw_sum(int firstindex) {
    string numberAsString;
    stringstream ss;
    ss << readFromFile(historyPATH)["History"][firstindex][historyVars[7]].asFloat();
    ss >> numberAsString;
    int even = 0;
    if (numberAsString.length() % 2 == 0) even = 1;
    cout << string((historyLengths[7] / 2) + (historyVars[7].length() / 2) - (numberAsString.length() / 2) + even, ' ');

    cout << numberAsString;

    cout << string((historyLengths[7] / 2) + (historyVars[7].length() / 2) - (numberAsString.length() / 2), ' ');

    cout << "|";
}

//pobieranie i wypisywanie (ceny, ilosci)
void draw_priceAndN(int firstindex, int indexvar, int secondindex) {
    string numberAsString;
    stringstream ss;
    ss << readFromFile(historyPATH)["History"][firstindex][historyVars[indexvar]][secondindex].asFloat();
    ss >> numberAsString;
    int even = 0;
    if (numberAsString.length() % 2 == 0) even = 1;
    cout << "|";
    cout << string((historyLengths[indexvar] / 2) + (historyVars[indexvar].length() / 2) - (numberAsString.length() / 2) + even, ' ');

    cout << numberAsString;

    cout << string((historyLengths[indexvar] / 2) + (historyVars[indexvar].length() / 2) - (numberAsString.length() / 2), ' ');
}

//pobieranie i wypisywanie (produktu)
void draw_food(int firstindex, int secondindex) {
    int odd = 0;
    if (readFromFile(historyPATH)["History"][firstindex][historyVars[3]][secondindex].asString().length() % 2 != 0) odd = 1;
    cout << "|" << string((historyLengths[3] / 2) + (historyVars[3].length() / 2) -
        (readFromFile(historyPATH)["History"][firstindex][historyVars[3]][secondindex].asString().length() / 2) - odd, ' ');
    cout << readFromFile(historyPATH)["History"][firstindex][historyVars[3]][secondindex].asString() <<
        string((historyLengths[3] / 2) + (historyVars[3].length() / 2) -
            (readFromFile(historyPATH)["History"][firstindex][historyVars[3]][secondindex].asString().length() / 2), ' ');
}

//pobieranie i wypisywanie (numeru, daty, ID)
void draw_numDateID(int firstindex, int indexvar) {
    if (indexvar == 0) {
        int spaceForNumber = 1;
        if (firstindex >= 99) spaceForNumber = 2;
        cout << "|" << string((historyLengths[0] / 2) - spaceForNumber, ' ');
        if (firstindex < 9) cout << 0;
        cout << firstindex + 1 << string((historyLengths[0] / 2), ' ');
    }
    else {
        cout << "|" << string((historyLengths[indexvar] / 2) + (historyVars[indexvar].length() / 2) -
            (readFromFile(historyPATH)["History"][firstindex][historyVars[indexvar]].asString().length() / 2), ' ');
        cout << readFromFile(historyPATH)["History"][firstindex][historyVars[indexvar]].asString() <<
            string((historyLengths[indexvar] / 2) + (historyVars[indexvar].length() / 2) -
                (readFromFile(historyPATH)["History"][firstindex][historyVars[indexvar]].asString().length() / 2), ' ');
    }
}

//wypisywanie historii zamowien
void historyList(bool isFiltered, string IDorDate) {
    float AllHistorySum = 0;
    for (int i = 0; i < readFromFile(historyPATH)["History"].size(); i++) {

        if (isFiltered) {
            if (readFromFile(historyPATH)["History"][i]["ID"].asString() != IDorDate && readFromFile(historyPATH)["History"][i]["Date"].asString() != IDorDate) break;
        }

        // #, Date, ID
        for (int f = 0; f < 3; f++) {
            draw_numDateID(i, f);
        }

        for (int a = 0; a < readFromFile(historyPATH)["History"][i]["Food"].size(); a++) {
            for (int f = 3; f <= 7; f++) {

                // Price, N, N*P, 
                if (f != 3 && f != 7) {
                    draw_priceAndN(i, f, a);
                }

                // Food
                if (f == 3) {
                    draw_food(i, a);
                }

                // Free space for sum "-"
                if (f == 7) {
                    draw_freespacesum();
                    break;
                }      
            }

            if (a != readFromFile(historyPATH)["History"][i]["Food"].size() - 1) draw_emptyspace();
        }
        draw_emptyspaceforsum();
        draw_sum(i);
        AllHistorySum += readFromFile(historyPATH)["History"][i][historyVars[7]].asFloat();
        cout << endl;
        draw_line();
    }
    cout << "SUMA CALKOWITA: " << AllHistorySum << endl;
}

//glowna funkcja, pobieranie i wypisywanie dla dnia/ID albo wypisywanie calej historii
void orderHistory(bool isFiltered) {
    if (isFiltered) {
        int keyPressed;
        string IDorDate;
        do {
            cout << endl << "Opcje do wyboru:" << endl << "[0] Powrot" << endl << "[1] Historia po dacie" << endl << "[2] Historia po ID"
                << endl << endl << "Prosze podac opcje: ";
            cin >> keyPressed;

            switch (keyPressed) {
            case 0:
                break;
            case 1:
                cout << endl << "Podaj Date w formie [RRRR-MM-DD]: ";
                cin >> IDorDate;
                draw_header();
                historyList(true, IDorDate);
                break;
            case 2:
                cout << endl << "Podaj ID: ";
                cin >> IDorDate;
                draw_header();
                historyList(true, IDorDate);
                break;
            default:
                cout << endl << "Nie wybrano poprawnej opcji." << endl;
                break;
            }

        } while (keyPressed != 0);

    }
    else {
        draw_header();
        historyList(false, "-");
    }
}

