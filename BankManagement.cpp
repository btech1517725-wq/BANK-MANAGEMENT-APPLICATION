#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string name;
    string phone;
    double balance;

public:
    BankAccount() {
        accountNumber = 0;
        name = "";
        phone = "";
        balance = 0.0;
    }

    BankAccount(int accNo, string n, string p, double bal) {
        accountNumber = accNo;
        name = n;
        phone = p;
        balance = bal;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Amount deposited successfully!\n";
        } else {
            cout << "Invalid deposit amount.\n";
        }
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount.\n";
            return false;
        }

        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }

        balance -= amount;
        cout << "Amount withdrawn successfully!\n";
        return true;
    }

    void display() const {
        cout << "\n-----------------------------------\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Customer Name  : " << name << endl;
        cout << "Phone Number   : " << phone << endl;
        cout << "Balance        : Rs. "
             << fixed << setprecision(2) << balance << endl;
        cout << "-----------------------------------\n";
    }

    void saveToFile(ofstream &file) const {
        file << accountNumber << '\n';
        file << name << '\n';
        file << phone << '\n';
        file << fixed << setprecision(2) << balance << '\n';
    }

    bool loadFromFile(ifstream &file) {
        if (!(file >> accountNumber))
            return false;

        file.ignore();

        getline(file, name);
        getline(file, phone);
        file >> balance;
        file.ignore();

        return true;
    }
};

class Bank {
private:
    const string fileName = "bank_accounts.dat";

    bool accountExists(int accNo) {
        ifstream file(fileName);
        BankAccount account;

        while (account.loadFromFile(file)) {
            if (account.getAccountNumber() == accNo) {
                return true;
            }
        }

        return false;
    }

    void rewriteFile(BankAccount &updatedAccount) {
        ifstream input(fileName);
        ofstream temp("temp.dat");

        BankAccount account;

        while (account.loadFromFile(input)) {
            if (account.getAccountNumber() ==
                updatedAccount.getAccountNumber()) {

                updatedAccount.saveToFile(temp);
            } else {
                account.saveToFile(temp);
            }
        }

        input.close();
        temp.close();

        remove(fileName.c_str());
        rename("temp.dat", fileName.c_str());
    }

public:

    void createAccount() {
        int accNo;
        string name, phone;
        double initialDeposit;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        if (accountExists(accNo)) {
            cout << "Account already exists.\n";
            return;
        }

        cin.ignore();

        cout << "Enter Customer Name: ";
        getline(cin, name);

        cout << "Enter Phone Number: ";
        getline(cin, phone);

        cout << "Enter Initial Deposit: Rs. ";
        cin >> initialDeposit;

        if (initialDeposit < 0) {
            cout << "Invalid initial deposit.\n";
            return;
        }

        BankAccount newAccount(
            accNo, name, phone, initialDeposit
        );

        ofstream file(fileName, ios::app);

        if (!file) {
            cout << "Unable to open database file.\n";
            return;
        }

        newAccount.saveToFile(file);
        file.close();

        cout << "\nAccount created successfully!\n";
    }

    void depositMoney() {
        int accNo;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        ifstream file(fileName);
        BankAccount account;
        bool found = false;

        while (account.loadFromFile(file)) {
            if (account.getAccountNumber() == accNo) {
                found = true;

                cout << "Current Balance: Rs. "
                     << account.getBalance() << endl;

                cout << "Enter Deposit Amount: Rs. ";
                cin >> amount;

                account.deposit(amount);

                if (amount > 0)
                    rewriteFile(account);

                break;
            }
        }

        file.close();

        if (!found)
            cout << "Account not found.\n";
    }

    void withdrawMoney() {
        int accNo;
        double amount;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        ifstream file(fileName);
        BankAccount account;
        bool found = false;

        while (account.loadFromFile(file)) {
            if (account.getAccountNumber() == accNo) {
                found = true;

                cout << "Current Balance: Rs. "
                     << account.getBalance() << endl;

                cout << "Enter Withdrawal Amount: Rs. ";
                cin >> amount;

                if (account.withdraw(amount)) {
                    rewriteFile(account);
                }

                break;
            }
        }

        file.close();

        if (!found)
            cout << "Account not found.\n";
    }

    void checkBalance() {
        int accNo;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        ifstream file(fileName);
        BankAccount account;
        bool found = false;

        while (account.loadFromFile(file)) {
            if (account.getAccountNumber() == accNo) {
                account.display();
                found = true;
                break;
            }
        }

        file.close();

        if (!found)
            cout << "Account not found.\n";
    }

    void displayAllAccounts() {
        ifstream file(fileName);
        BankAccount account;
        bool found = false;

        cout << "\n========== ALL ACCOUNTS ==========\n";

        while (account.loadFromFile(file)) {
            account.display();
            found = true;
        }

        file.close();

        if (!found)
            cout << "No accounts available.\n";
    }
};

int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "       BANK MANAGEMENT SYSTEM       \n";
        cout << "====================================\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Check Balance\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "====================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bank.createAccount();
            break;

        case 2:
            bank.depositMoney();
            break;

        case 3:
            bank.withdrawMoney();
            break;

        case 4:
            bank.checkBalance();
            break;

        case 5:
            bank.displayAllAccounts();
            break;

        case 6:
            cout << "\nThank you for using the Bank Management System!\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}
