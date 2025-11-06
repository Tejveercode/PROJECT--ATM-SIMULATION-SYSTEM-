#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class ATM {
    int accNo;
    string name;
    int pin;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void deposit(double);
    void withdraw(double);
    int getAccNo() const;
    int getPin() const;
    double getBalance() const;
    void modify();
};

void ATM::createAccount() {
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

void ATM::showAccount() const {
    cout << "\nAccount No.: " << accNo;
    cout << "\nAccount Holder Name: " << name;
    cout << "\nBalance: ₹" << balance << endl;
}

void ATM::deposit(double amount) {
    balance += amount;
}

void ATM::withdraw(double amount) {
    if (amount > balance)
        cout << "\nInsufficient Balance!";
    else
        balance -= amount;
}

int ATM::getAccNo() const {
    return accNo;
}

int ATM::getPin() const {
    return pin;
}

double ATM::getBalance() const {
    return balance;
}

void ATM::modify() {
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Modify PIN: ";
    cin >> pin;
    cout << "Modify Balance: ";
    cin >> balance;
}

void writeAccount();
void displayAll();
void loginMenu(int, int);
void depositWithdraw(int, double, bool);
void modifyAccount(int);
void deleteAccount(int);

void writeAccount() {
    ATM ac;
    ofstream outFile;
    outFile.open("atm.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(ATM));
    outFile.close();
}

void displayAll() {
    ATM ac;
    ifstream inFile("atm.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened!\n";
        return;
    }
    cout << "\n\n\t\tALL ACCOUNT HOLDER DETAILS\n\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(ATM))) {
        ac.showAccount();
        cout << "---------------------------------\n";
    }
    inFile.close();
}

void loginMenu(int n, int p) {
    ATM ac;
    fstream File;
    bool found = false;
    File.open("atm.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be opened!\n";
        return;
    }

    while (File.read(reinterpret_cast<char *>(&ac), sizeof(ATM)) && !found) {
        if (ac.getAccNo() == n && ac.getPin() == p) {
            found = true;
            int choice;
            do {
                cout << "\n\n--- ATM MENU ---";
                cout << "\n1. Check Balance";
                cout << "\n2. Deposit";
                cout << "\n3. Withdraw";
                cout << "\n4. Account Details";
                cout << "\n5. Exit to Main Menu";
                cout << "\nEnter your choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    cout << "\nCurrent Balance: ₹" << ac.getBalance();
                    break;
                case 2: {
                    double amt;
                    cout << "Enter Amount to Deposit: ";
                    cin >> amt;
                    ac.deposit(amt);
                    cout << "Deposit Successful!";
                    break;
                }
                case 3: {
                    double amt;
                    cout << "Enter Amount to Withdraw: ";
                    cin >> amt;
                    ac.withdraw(amt);
                    break;
                }
                case 4:
                    ac.showAccount();
                    break;
                case 5:
                    cout << "\nExiting to Main Menu...\n";
                    break;
                default:
                    cout << "\nInvalid Choice!";
                }
            } while (choice != 5);

            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(ATM));
        }
    }

    File.close();
    if (!found)
        cout << "\nInvalid Account Number or PIN!";
}

void modifyAccount(int n) {
    ATM ac;
    fstream File;
    bool found = false;
    File.open("atm.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be opened!\n";
        return;
    }
    while (File.read(reinterpret_cast<char *>(&ac), sizeof(ATM)) && !found) {
        if (ac.getAccNo() == n) {
            ac.modify();
            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(ATM));
            cout << "\nRecord Updated Successfully!";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\nAccount Not Found!";
}

void deleteAccount(int n) {
    ATM ac;
    ifstream inFile("atm.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(ATM))) {
        if (ac.getAccNo() != n)
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(ATM));
        else
            found = true;
    }
    inFile.close();
    outFile.close();
    remove("atm.dat");
    rename("temp.dat", "atm.dat");
    if (found)
        cout << "\nAccount Deleted Successfully!";
    else
        cout << "\nAccount Not Found!";
}

int main() {
    int choice;
    do {
        cout << "\n\n\t===== ATM SIMULATION SYSTEM =====";
        cout << "\n\t1. Create New Account";
        cout << "\n\t2. Login to Account";
        cout << "\n\t3. Display All Accounts";
        cout << "\n\t4. Modify Account";
        cout << "\n\t5. Delete Account";
        cout << "\n\t6. Exit";
        cout << "\n\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2: {
            int accNo, pin;
            cout << "\nEnter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            loginMenu(accNo, pin);
            break;
        }
        case 3:
            displayAll();
            break;
        case 4: {
            int accNo;
            cout << "\nEnter Account Number to Modify: ";
            cin >> accNo;
            modifyAccount(accNo);
            break;
        }
        case 5: {
            int accNo;
            cout << "\nEnter Account Number to Delete: ";
            cin >> accNo;
            deleteAccount(accNo);
            break;
        }
        case 6:
            cout << "\nThank you for using our ATM System!\n";
            break;
        default:
            cout << "\nInvalid Choice!";
        }
    } while (choice != 6);
    return 0;
}