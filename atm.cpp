#include <iostream>
#include <fstream>
#include <limits>
#include <cstring>
#include<conio.h>
#include<iomanip>
#include<string>

const char ACC_DATA_FILE[] = "accounts.dat";
const char TEMP_ACC_DATA_FILE[] = "temp.dat";

using namespace std;

class Account
{
private:
    char name[50];
    int accountNumber;
    int pin;
    double balance;
    bool active;

public:
    void setAccount(const char *name, int accountNumber, int pin, double balance, bool active)
    {
        strncpy(this->name, name, sizeof(this->name));
        this->name[sizeof(this->name) - 1] = '\0';
        this->accountNumber = accountNumber;
        this->pin = pin;
        this->balance = balance;
        this->active = active;
    }

    const char *getName() const
    {
        return name;
    }

    int getAccountNumber() const
    {
        return accountNumber;
    }

    int getPIN() const
    {
        return pin;
    }

    double getBalance() const
    {
        return balance;
    }

    bool isActive() const
    {
        return active;
    }

    void deposit(double amount) 
    {
        balance += amount;
        // return true;
    }

    bool withdraw(double amount)
    {
        if (amount > balance)
        {
            cout<<"insufficient balance";
            return false;
        }

            balance -= amount;
            return true;

        // return false;
    }

    void displayAccount() const
    {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: $" << balance << endl;
        cout << "Account Status: " << (active ? "Active" : "Inactive") << endl;
    }
};

class Atm
{
private:
    Account acc;



bool readAccount(int accNo, Account& acc)
{
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be opened. Exiting." << endl;
        return false;
    }

    bool found = false;
    Account tempAccount;

    while (inFile.read(reinterpret_cast<char*>(&tempAccount), sizeof(Account)))
    {
        if (tempAccount.getAccountNumber() == accNo && tempAccount.isActive())
        {
            found = true;
            acc = tempAccount;
            break;
        }
    }

    inFile.close();

    if (!found)
    {
        cout << "Account not found or inactive." << endl;
        return false;
    }

    return true;
}

bool updateAccount(int accNo, const Account& newAcc, bool updateActiveStatus = true)
{
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be opened. Exiting." << endl;
        return false;
    }

    ofstream outFile("temp_accounts.dat", ios::binary);
    if (!outFile)
    {
        cout << "File could not be created. Exiting." << endl;
        inFile.close();
        return false;
    }

    Account acc;
    bool accountUpdated = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account)))
    {
        if (acc.getAccountNumber() == accNo)
        {
            acc.setAccount(newAcc.getName(), accNo, newAcc.getPIN(), newAcc.getBalance(), newAcc.isActive());
            accountUpdated = true;
        }
        outFile.write(reinterpret_cast<const char*>(&acc), sizeof(Account));
    }

    inFile.close();
    outFile.close();

    if (!accountUpdated)
    {
        cout << "Account not found." << endl;
        remove("temp_accounts.dat");
        return false;
    }

    if (remove("accounts.dat") != 0)
    {
        cout << "Error deleting the original account file." << endl;
        remove("temp_accounts.dat");
        return false;
    }

    if (rename("temp_accounts.dat", "accounts.dat") != 0)
    {
        cout << "Error renaming the temporary account file." << endl;
        return false;
    }

    if (updateActiveStatus && newAcc.isActive() != acc.isActive())
    {
        if (newAcc.isActive())
        {
            cout << "Account activated successfully." << endl;
        }
        else
        {
            cout << "Account deactivated successfully." << endl;
        }
    }
    else
    {
        cout << "Account updated successfully." << endl;
    }

    return true;
}


public:
    void createAccount()
    {
        ofstream outFile(ACC_DATA_FILE, ios::binary | ios::app);
        if (!outFile)
        {
            cout << "Error opening file." << endl;
            return;
        }

        char name[50];
        int accountNumber, pin;
        double balance;

        cout << "Enter Name: ";
        cin.ignore();
        cin.getline(name, sizeof(name));
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter PIN: ";
        cin >> pin;
        cout << "Enter Initial Balance: $";
        cin >> balance;

        Account newAccount;
        newAccount.setAccount(name, accountNumber, pin, balance, true);

        outFile.write(reinterpret_cast<const char *>(&newAccount), sizeof(Account));

        outFile.close();

        cout << "Account created successfully." << endl;
        return;
    }

  
    void deposit(int accNo, int pin)
{
    if (!readAccount(accNo, acc))
    {
        cout << "Account not found or inactive." << endl;
        return;
    }

    if (acc.getPIN() != pin)
    {
        cout << "Invalid PIN." << endl;
        return;
    }

    double amount;
    cout << "Enter the amount to deposit: $";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount." << endl;
        return;
    }

    acc.deposit(amount);

    if (updateAccount(accNo, acc))
    {
        cout << "Amount deposited successfully." << endl;
        cout << "Current Balance: $" << acc.getBalance() << endl;
    }
    else
    {
        cout << "Failed to update account." << endl;
    }
}

void withdraw(int accNo, int pin)
{
    if (!readAccount(accNo, acc))
    {
        cout << "Account not found or inactive." << endl;
        return;
    }

    if (acc.getPIN() != pin)
    {
        cout << "Invalid PIN." << endl;
        return;
    }

    double amount;
    cout << "Enter the amount to withdraw: $";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount." << endl;
        return;
    }

    if (acc.withdraw(amount))
    {
        if (updateAccount(accNo, acc))
        {
            cout << "Amount withdrawn successfully." << endl;
            cout << "Current Balance: $" << acc.getBalance() << endl;
        }
        else
        {
            cout << "Failed to update account." << endl;
        }
    }
    else
    {
        cout << "Insufficient balance." << endl;
    }
}


    void displayAccount(int accNo)
    {
        if (!readAccount(accNo,acc))
        {
            cout << "Account not found or inactive." << endl;
            return;
        }

        acc.displayAccount();
        return;
    }

    void deleteAccount(int accNo)
    {
        if (!readAccount(accNo,acc))
        {
            cout << "Account not found or inactive." << endl;
            return;
        }
        updateAccount(accNo,acc);
    // if (updateAccount(accNo, acc, false))
    // {
    //     cout << "Account deleted successfully." << endl;
    // }
    // else
    // {
    //     cout << "Failed to delete account." << endl;
    // }



        cout << "Account deleted successfully." << endl;
        return;
    }


void updateAccount()
{
    int accNo;
    cout << "Enter Account Number to update: ";
    cin >> accNo;

    if (!readAccount(accNo,acc))
    {
        cout << "Account not found or inactive." << endl;
        return;
    }

    char name[50];
    int pin;
    double balance;
    bool active;

    cout << "Enter new Name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(name, sizeof(name));
    cout << "Enter new PIN: ";
    cin >> pin;
    cout << "Enter new Balance: $";
    cin >> balance;
    cout << "Enter new Account Status (0 for inactive, 1 for active): ";
    cin >> active;

    if (active == 0)
    {
        cout << "Invalid account status. Account status remains unchanged." << endl;
        active = acc.isActive(); // Preserve the existing account status
    }

    acc.setAccount(name, accNo, pin, balance, active);

    updateAccount(accNo,acc);

    cout << "Account updated successfully." << endl;
    return;
}
    

    void activateAccount(int accNo)
    {
        if (!readAccount(accNo,acc))
        {
            cout << "Account not found or inactive." << endl;
            return;
        }

        acc.setAccount(acc.getName(), accNo, acc.getPIN(), acc.getBalance(), true);

        updateAccount(accNo,acc);

        cout << "Account activated successfully." << endl;
        return;
    }

    void deactivateAccount(int accNo)
    {
        if (!readAccount(accNo,acc))
        {
            cout << "Account not found or inactive." << endl;
            return;
        }

        acc.setAccount(acc.getName(), accNo, acc.getPIN(), acc.getBalance(), false);

        updateAccount(accNo,acc);

        cout << "Account deactivated successfully." << endl;
        return;
    }
};

int main()
{
    Atm atm;
    int option, accNo, pin;

    do
    {
        cout << endl
             << "********** Automated Teller Machine **********" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Display Account" << endl;
        cout << "5. Delete Account" << endl;
        cout << "6. Update Account" << endl;
        cout << "7. Activate Account" << endl;
        cout << "8. Deactivate Account" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter option: ";
        cin >> option;

        switch (option)
        {
        case 1:
            atm.createAccount();
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            atm.deposit(accNo, pin);
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> pin;
            atm.withdraw(accNo, pin);
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accNo;
            atm.displayAccount(accNo);
            break;

        case 5:
            cout << "Enter Account Number: ";
            cin >> accNo;
            atm.deleteAccount(accNo);
            break;

        case 6:
            atm.updateAccount();
            break;

        case 7:
            cout << "Enter Account Number: ";
            cin >> accNo;
            atm.activateAccount(accNo);
            break;

        case 8:
            cout << "Enter Account Number: ";
            cin >> accNo;
            atm.deactivateAccount(accNo);
            break;

        case 9:
            cout << "Thank you for using the ATM. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }

        cout << endl;

    } while (option != 9);

    return 0;
}

