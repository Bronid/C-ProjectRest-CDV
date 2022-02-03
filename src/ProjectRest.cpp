#include <iostream>
#include <fstream>
#include <string>
#include <json/value.h>
#include <json/json.h>
#include "TakeOrder.h"
#include "orderHistory.h"
#include "jsonFunctions.h"

using namespace std;

// Start of application
int main() {
    cout << "Start aplikacji." << endl;
    cout << "Witam w " << readFromFile(setupPATH)["restaurant_name"].asString() << endl;

    int keyPressed;

    do {
        cout << endl << "Opcje: " << endl << "[0] Zamkniecie programu" << endl
            << "[1] Zlozenie zamowienia" << endl << "[2] Historia zamowien" << endl
            << "[3] Historia zamowien dla danego dnia/ID" << endl << endl
            << "Prosze podac opcje: ";

        cin >> keyPressed;

        switch (keyPressed) {
        case 0:
            break;
        case 1:
            takeOrder();
            break;
        case 2:
            orderHistory(false);
            break;
        case 3:
            orderHistory(true);
            break;
        default:
            cout << endl << "Nie wybrano poprawnej opcji." << endl;
            break;
        }

    } while (keyPressed != 0);

    cout << "Podano 0 - program konczy dzialanie";
    return EXIT_SUCCESS;
}

