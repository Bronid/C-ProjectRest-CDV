#include "orderHistory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <json/value.h>
#include <json/json.h>
#include "jsonFunctions.h"

using namespace std;

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

void historyList() {
    for (int i = 0; i < readFromFile(historyPATH)["History"].size(); i++) {
        int spaceForNumber = 1;
        if (i >= 99) spaceForNumber = 2;
        cout << "|" << string((historyLengths[0] / 2) - spaceForNumber, ' ');
        if (i < 9) cout << 0;
        cout << i + 1 << string((historyLengths[0] / 2), ' ');

        // #, Date, ID
        for (int f = 1; f < 3; f++) {
            cout << "|" << string((historyLengths[f] / 2) + (historyVars[f].length() / 2) -
                (readFromFile(historyPATH)["History"][i][historyVars[f]].asString().length() / 2), ' ');
            cout << readFromFile(historyPATH)["History"][i][historyVars[f]].asString() <<
                string((historyLengths[f] / 2) + (historyVars[f].length() / 2) -
                    (readFromFile(historyPATH)["History"][i][historyVars[f]].asString().length() / 2), ' ');
        }

        for (int a = 0; a < readFromFile(historyPATH)["History"][i]["Food"].size(); a++) {
            for (int f = 3; f <= 7; f++) {

                // Price, N, N*P, 
                if (f != 3 && f != 7) {
                    string numberAsString;
                    stringstream ss;
                    ss << readFromFile(historyPATH)["History"][i][historyVars[f]][a].asFloat();
                    ss >> numberAsString;
                    int even = 0;
                    if (numberAsString.length() % 2 == 0) even = 1;
                    cout << "|";
                    cout << string((historyLengths[f] / 2) + (historyVars[f].length() / 2) - (numberAsString.length() / 2) + even, ' ');

                    cout << numberAsString;

                    cout << string((historyLengths[f] / 2) + (historyVars[f].length() / 2) - (numberAsString.length() / 2), ' ');
                }

                // Food
                if (f == 3) {
                    int odd = 0;
                    if (readFromFile(historyPATH)["History"][i][historyVars[f]][a].asString().length() % 2 != 0) odd = 1;
                    cout << "|" << string((historyLengths[f] / 2) + (historyVars[f].length() / 2) -
                        (readFromFile(historyPATH)["History"][i][historyVars[f]][a].asString().length() / 2) - odd, ' ');
                    cout << readFromFile(historyPATH)["History"][i][historyVars[f]][a].asString() <<
                        string((historyLengths[f] / 2) + (historyVars[f].length() / 2) -
                            (readFromFile(historyPATH)["History"][i][historyVars[f]][a].asString().length() / 2), ' ');
                }

                // Free space from sum "--"
                if (f == 7) {
                    cout << "|" << string((historyLengths[f] / 2) + 1, ' ');
                    cout << "-" << string((historyLengths[f] / 2) + 1, ' ');
                    cout << "|" << endl;
                    break;
                }      
            }

            if (a != readFromFile(historyPATH)["History"][i]["Food"].size() - 1) draw_emptyspace();
        }
        draw_emptyspaceforsum();
        string numberAsString;
        stringstream ss;
        ss << readFromFile(historyPATH)["History"][i][historyVars[7]].asFloat();
        ss >> numberAsString;
        int even = 0;
        if (numberAsString.length() % 2 == 0) even = 1;
        cout << string((historyLengths[7] / 2) + (historyVars[7].length() / 2) - (numberAsString.length() / 2) + even, ' ');

        cout << numberAsString;

        cout << string((historyLengths[7] / 2) + (historyVars[7].length() / 2) - (numberAsString.length() / 2), ' ');

        cout << "|";

        cout << endl;
        
        draw_line();
    }
}

void orderHistory() {
    draw_header();
    historyList();
}

