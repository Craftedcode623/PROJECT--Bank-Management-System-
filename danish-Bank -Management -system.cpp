#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Account {
    int accNo;
    string name;
    char type;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accNo;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, name);
        cout << "Enter Account Type (S/C): ";
        cin >> type;
        type = toupper(type);
        cout << "Enter Initial Deposit Amount: ";
        cin >> balance;
        cout << "\nAccount Created Successfully!\n";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accNo
             << "\nHolder Name : " << name
             << "\nType        : " << type
             << "\nBalance     : " << balance << endl;
    }

    void deposit(double amt) { balance += amt; }
    void withdraw(double amt) {
        if (balance >= amt) balance -= amt;
        else cout << "\nInsufficient Balance!\n";
    }

    void report() const {
        cout << setw(10) << accNo << setw(20) << name
             << setw(10) << type << setw(10) << balance << endl;
    }

    int getAccNo() const { return accNo; }
};

void writeAccount() {
    Account ac;
    ofstream out("accounts.dat", ios::binary | ios::app);
    ac.createAccount();
    out.write((char*)&ac, sizeof(Account));
    out.close();
}

void displayAll() {
    Account ac;
    ifstream in("accounts.dat", ios::binary);
    if (!in) { cout << "\nFile Not Found!\n"; return; }

    cout << "\n\n\tACCOUNT HOLDER LIST\n";
    cout << setw(10) << "A/c No." << setw(20) << "Name"
         << setw(10) << "Type" << setw(10) << "Balance\n";
    cout << "------------------------------------------------------------\n";

    while (in.read((char*)&ac, sizeof(Account))) ac.report();
    in.close();
}

void displayAccount(int n) {
    Account ac; bool found = false;
    ifstream in("accounts.dat", ios::binary);
    if (!in) { cout << "\nFile Not Found!\n"; return; }

    while (in.read((char*)&ac, sizeof(Account))) {
        if (ac.getAccNo() == n) { ac.showAccount(); found = true; }
    }
    if (!found) cout << "\nAccount Not Found!\n";
    in.close();
}

void depositWithdraw(int n, bool isDeposit) {
    Account ac; fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    if (!file) { cout << "\nFile Not Found!\n"; return; }

    bool found = false;
    while (file.read((char*)&ac, sizeof(Account))) {
        if (ac.getAccNo() == n) {
            ac.showAccount();
            double amt;
            cout << "\nEnter Amount to " << (isDeposit ? "Deposit: " : "Withdraw: ");
            cin >> amt;
            if (isDeposit) ac.deposit(amt); else ac.withdraw(amt);
            int pos = -1 * (int)sizeof(ac);
            file.seekp(pos, ios::cur);
            file.write((char*)&ac, sizeof(Account));
            cout << "\nRecord Updated Successfully!\n";
            found = true;
        }
    }
    if (!found) cout << "\nRecord Not Found!\n";
    file.close();
}

void deleteAccount(int n) {
    Account ac;
    ifstream in("accounts.dat", ios::binary);
    ofstream out("Temp.dat", ios::binary);

    while (in.read((char*)&ac, sizeof(Account))) {
        if (ac.getAccNo() != n) out.write((char*)&ac, sizeof(Account));
    }
    in.close(); out.close();
    remove("accounts.dat");
    rename("Temp.dat", "accounts.dat");
    cout << "\nAccount Deleted Successfully!\n";
}

int main() {
    char choice; int num;
    cout << "\n*** BANK ACCOUNT MANAGEMENT SYSTEM ***\n";
    do {
        cout << "\n\n1. NEW ACCOUNT"
             << "\n2. DEPOSIT AMOUNT"
             << "\n3. WITHDRAW AMOUNT"
             << "\n4. BALANCE ENQUIRY"
             << "\n5. ALL ACCOUNT HOLDER LIST"
             << "\n6. CLOSE AN ACCOUNT"
             << "\n7. EXIT"
             << "\nSelect Option (1-7): ";
        cin >> choice;

        switch (choice) {
            case '1': writeAccount(); break;
            case '2': cout << "\nEnter Account No.: "; cin >> num; depositWithdraw(num, true); break;
            case '3': cout << "\nEnter Account No.: "; cin >> num; depositWithdraw(num, false); break;
            case '4': cout << "\nEnter Account No.: "; cin >> num; displayAccount(num); break;
            case '5': displayAll(); break;
            case '6': cout << "\nEnter Account No.: "; cin >> num; deleteAccount(num); break;
            case '7': cout << "\nThank You for Using the System!\n"; break;
            default: cout << "\nInvalid Option! Try Again.\n";
        }
    } while (choice != '7');
}