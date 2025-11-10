#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ATM {
    int accNo;
    string name;
    int pin;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accNo;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Set a 4-digit PIN: ";
        cin >> pin;
        cout << "Enter Initial Deposit Amount: ";
        cin >> balance;
        cout << "\nAccount Created Successfully!\n";
    }

    void showAccount() {
        cout << "\nAccount No.: " << accNo;
        cout << "\nAccount Holder Name: " << name;
        cout << "\nBalance: ₹" << balance << endl;
    }

    bool login(int n, int p) {
        return (accNo == n && pin == p);
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposit Successful!";
    }

    void withdraw(double amount) {
        if (amount > balance)
            cout << "\nInsufficient Balance!";
        else {
            balance -= amount;
            cout << "Withdrawal Successful!";
        }
    }

    int getAccNo() { return accNo; }
    int getPin() { return pin; }
};

void saveAccount(ATM &ac) {
    ofstream file("atm.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&ac), sizeof(ATM));
    file.close();
}

bool loadAccount(int n, int p, ATM &ac) {
    ifstream file("atm.dat", ios::binary);
    while (file.read(reinterpret_cast<char*>(&ac), sizeof(ATM))) {
        if (ac.getAccNo() == n && ac.getPin() == p) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void displayAll() {
    ATM ac;
    ifstream file("atm.dat", ios::binary);
    if (!file) {
        cout << "\nNo Accounts Found!\n";
        return;
    }
    cout << "\n\n\t\tALL ACCOUNT HOLDER DETAILS\n\n";
    while (file.read(reinterpret_cast<char*>(&ac), sizeof(ATM))) {
        ac.showAccount();
        cout << "---------------------------------\n";
    }
    file.close();
}

int main() {
    ATM user;
    int choice;

    do {
        cout << "\n\n\t===== ATM SIMULATION SYSTEM =====";
        cout << "\n\t1. Create New Account";
        cout << "\n\t2. Login to Account";
        cout << "\n\t3. Display All Accounts";
        cout << "\n\t4. Exit";
        cout << "\n\tEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            user.createAccount();
            saveAccount(user);
        }
        else if (choice == 2) {
            int acc, pin;
            cout << "\nEnter Account Number: ";
            cin >> acc;
            cout << "Enter PIN: ";
            cin >> pin;

            if (loadAccount(acc, pin, user)) {
                int option;
                do {
                    cout << "\n\n--- ATM MENU ---";
                    cout << "\n1. Check Balance";
                    cout << "\n2. Deposit";
                    cout << "\n3. Withdraw";
                    cout << "\n4. Account Details";
                    cout << "\n5. Exit to Main Menu";
                    cout << "\nEnter your choice: ";
                    cin >> option;

                    switch (option) {
                        case 1:
                            user.showAccount();
                            break;
                        case 2: {
                            double amt;
                            cout << "Enter Amount to Deposit: ";
                            cin >> amt;
                            user.deposit(amt);
                            break;
                        }
                        case 3: {
                            double amt;
                            cout << "Enter Amount to Withdraw: ";
                            cin >> amt;
                            user.withdraw(amt);
                            break;
                        }
                        case 4:
                            user.showAccount();
                            break;
                        case 5:
                            cout << "\nExiting to Main Menu...\n";
                            break;
                        default:
                            cout << "\nInvalid Choice!";
                    }
                } while (option != 5);
            } else {
                cout << "\nInvalid Account Number or PIN!";
            }
        }
        else if (choice == 3) {
            displayAll();
        }
        else if (choice == 4) {
            cout << "\nThank you for using our ATM System!\n";
        }
        else {
            cout << "\nInvalid Choice!";
        }
    } while (choice != 4);

    return 0;
}
