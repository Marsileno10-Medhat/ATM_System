#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

enum enATM {eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};
enum enQuickWithdraw {e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6, e800 = 7, e1000 = 8, eExit = 9};

struct stClient {

    string AccountNumber;
    string PinCode;
    string ClientName;
    string PhoneNumber;
    double AccountBalance;
};

const string ClientsDataFile = "Clients_data_file.txt";
stClient CurrentClient;

void ShowNormalWithdrawScreen();
void Show_ATM_MainMenu();
void GoBackToMainMenu();

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {

    string Data;
    Data += Client.AccountNumber + Separator;
    Data += Client.PinCode + Separator;
    Data += Client.ClientName + Separator;
    Data += Client.PhoneNumber + Separator;
    Data += to_string(Client.AccountBalance);
    return Data;
}

vector<string> SplitString(string Data, string Separator = "#//#") {

    vector<string> vClient;
    string Word = "";
    short Pos = 0;

    while ((Pos = Data.find(Separator)) != Separator.npos) {

        Word = Data.substr(0, Pos);

        if (Word != "") {
            vClient.push_back(Word);
        }
        Data.erase(0, Pos + Separator.length());
    }
    if (!Data.empty()) {
        vClient.push_back(Data);
    }
    return vClient;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#") {

    stClient Client;
    vector<string> vClient;
    vClient = SplitString(Line);

    Client.AccountNumber = vClient[0];
    Client.PinCode = vClient[1];
    Client.ClientName = vClient[2];
    Client.PhoneNumber = vClient[3];
    Client.AccountBalance = stod(vClient[4]); //cast string to double.
    return Client;
}

vector<stClient> LoadClientDataFromFile(string FileName, string Separator = "#//#") {

    vector<stClient> vClients;
    fstream DataFile;
    DataFile.open(FileName, ios::in);

    if (DataFile.is_open()) {

        stClient Client;
        string Line;

        while (getline(DataFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        DataFile.close();
    }
    return vClients;
}

vector<stClient> SaveClientDataToFile(string FileName, vector<stClient>& vClients, string Separator = "#//#") {

    fstream DataFile;
    DataFile.open(FileName, ios::out);
    string DataLine;

    if (DataFile.is_open()) {

        for (stClient & Client : vClients) {

            if (Client.AccountNumber == CurrentClient.AccountNumber) {
                Client = CurrentClient;
            }
            DataLine = ConvertRecordToLine(Client);
            DataFile << DataLine << endl;
        }
        DataFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumberAndPINCode (string AccountNumber, string PINcode, stClient& Client) {

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);

    for (stClient C : vClient) {

        if ((C.AccountNumber == AccountNumber) && (C.PinCode == PINcode)) {
            Client = C;
            return true;
        }
    }
    return false;
}

void ShowLoginScreen() {

    bool LoginFaild = false;
    string AccountNumber, PINcode;

    do {
        system("cls");
        cout << "----------------------------------------\n";
        cout << "           Login screeen:\n";
        cout << "----------------------------------------\n";

        if (LoginFaild) {
            cout << "Invalid account number/PIN code!\n";
        }
        
        cout << "\nEnter Accout Number:  "; 
        cin >> AccountNumber; 

        cout << "\nEnter PIN code:  "; 
        cin >> PINcode; 

        LoginFaild = !FindClientByAccountNumberAndPINCode(AccountNumber, PINcode, CurrentClient);
    } while (LoginFaild);
    Show_ATM_MainMenu();
}

bool CheckQuickWithdrawAmount(enQuickWithdraw NewAmount) {

    bool Ckeck = (NewAmount > CurrentClient.AccountBalance);
    if (Ckeck) {
        cout << "\nThe anount exceeded your balance, make another choice.\n";
        cout << "\nPress any key to contenue...";
        system("pause>0");
    }
    return Ckeck;
}

bool ConfirmTransaction() {

    char C = 'Y';
    do {
        cout << "\nAre you sure you want to perform this transaction [Y/N]:  ";
        cin >> C;

        if (toupper(C) != 'Y' && toupper(C) != 'N') {
            cout << "\nInvalid input, please try again.";
        }
    } while (toupper(C) != 'Y' && toupper(C) != 'N');

    return true;
}

void QuickWithdrawMenu() {

    system("cls");
    cout << "=============================================\n";
    cout << "               Quick withdraw\n";
    cout << "=============================================\n";
    cout << "\t[1] 20 \t\t [2] 50\n";
    cout << "\t[3] 100 \t [4] 200\n";
    cout << "\t[5] 400 \t [6] 600\n";
    cout << "\t[7] 800 \t [8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "=============================================\n";
}

void QuickWithdrawOptions(enQuickWithdraw NewAmount) {

    switch (NewAmount) {

    case enQuickWithdraw::e20:
        CurrentClient.AccountBalance -= 20;
        break;

    case enQuickWithdraw::e50:
        CurrentClient.AccountBalance -= 50;
        break;

    case enQuickWithdraw::e100:
        CurrentClient.AccountBalance -= 100;
        break;

    case enQuickWithdraw::e200:
        CurrentClient.AccountBalance -= 200;
        break;

    case enQuickWithdraw::e400:
        CurrentClient.AccountBalance -= 400;
        break;

    case enQuickWithdraw::e600:
        CurrentClient.AccountBalance -= 600;
        break;

    case enQuickWithdraw::e800:
        CurrentClient.AccountBalance -= 800;
        break;

    case enQuickWithdraw::e1000:
        CurrentClient.AccountBalance -= 1000;
        break;

    default:
        GoBackToMainMenu();
    }
}

void ShowQuickWithdrawScreen() {

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    double NewAmount = 0;

    do {
        QuickWithdrawMenu();
        cout << "Your balance is " << CurrentClient.AccountBalance << endl;
        cout << "Choose what to withdraw from [1] to [8]:  ";
        cin >> NewAmount;

    } while (CheckQuickWithdrawAmount(enQuickWithdraw(NewAmount)));

    if (ConfirmTransaction()) {
    QuickWithdrawOptions(enQuickWithdraw(NewAmount));
    vClient = SaveClientDataToFile(ClientsDataFile, vClient);
    cout << "\nDone successfully, your new balance is: " << CurrentClient.AccountBalance << endl;
    }
}

bool CheckNormalWithdrawAmount(double NewAmount) {

    bool Ckeck = (NewAmount > CurrentClient.AccountBalance);
    if (Ckeck) {
        cout << "The anount exceeded your balance, make another choice.";
        cout << "Press any key to contenue...";
        system("pause>0");
    }
    return Ckeck;
}

void NormalWithdraw(int NewAmount, vector<stClient>& vClient) {

    while ((NewAmount % 5) != 0) {

        cout << "\nEnter an amount multiple of 5:  ";
        cin >> NewAmount;
    }

    if (CheckNormalWithdrawAmount(NewAmount)) {
        ShowNormalWithdrawScreen();
    }

    if (ConfirmTransaction()) {
    CurrentClient.AccountBalance -= NewAmount;
    vClient = SaveClientDataToFile(ClientsDataFile, vClient);
    cout << "\nDone successfully, your new balance is: " << CurrentClient.AccountBalance << endl;
    }
}

void ShowNormalWithdrawScreen() {

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    double NewAmount = 0;
    
    do {
        system("cls");
        cout << "=============================================\n";
        cout << "               Quick withdraw\n";
        cout << "=============================================\n\n";

        cout << "\nEnter an amount multiple of 5:  ";
        cin >> NewAmount;

    } while (CheckNormalWithdrawAmount(NewAmount));

    NormalWithdraw(NewAmount, vClient);
}

void DepositScreen(vector<stClient>& vClient) {

    double NewAmount = 0;

    do {
        cout << "\nEnter a positive deposit amount:  ";
        cin >> NewAmount;

        if (NewAmount < 0) {
            cout << "\nInvelid amount, ";
        }

    } while (NewAmount < 0);

    if (ConfirmTransaction()) {
        CurrentClient.AccountBalance += NewAmount;
        vClient = SaveClientDataToFile(ClientsDataFile, vClient);
        cout << "\nDone successfully, your new balance is: " << CurrentClient.AccountBalance << endl;
    }
}

void ShowDepositScreen() {

    cout << "----------------------------------------\n";
    cout << "               Deposit:\n";
    cout << "----------------------------------------\n\n";

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    DepositScreen(vClient);
}

void ShowBalanceScreen() {

    cout << "----------------------------------------\n";
    cout << "             Check balance:\n";
    cout << "----------------------------------------\n";
    cout << "Your balance is " << CurrentClient.AccountBalance << endl;
}

void GoBackToMainMenu() {

    cout << "\nPress any key to go back to the main menu...";
    system("pause>0");
    Show_ATM_MainMenu();
}

short Read_ATM_MainMenuOption() {
    
    short Choice;
    cout << "Choose what do you want to do [1:5]:  ";
    cin >> Choice;

    while (Choice < 1 && Choice > 5) {
        cout << "\nInvalid input, choose what do you want to do [1:5]:  ";
        cin >> Choice;
    }
    return Choice;
}

void Perform_ATM_MainMenuOption(enATM choice) {

    switch (choice) {

        case enATM::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen(); 
        GoBackToMainMenu();
        break;
    
        case enATM::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
        
        case enATM::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;

        case enATM::eCheckBalance:
        system("cls");
        ShowBalanceScreen();
        GoBackToMainMenu();
        break;

        default:
        ShowLoginScreen();
    }
}

void Show_ATM_MainMenu() {

    system("cls");
    cout << "=============================================\n";
    cout << "               ATM main menu\n";
    cout << "=============================================\n";
    cout << "\t[1] Quick withdraw.\n";
    cout << "\t[2] Normal withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "=============================================\n";
    Perform_ATM_MainMenuOption((enATM)Read_ATM_MainMenuOption());
}

int main() {

    ShowLoginScreen();
    system("pause>0");
    return 0;
}



/*

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

enum enATM {eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5};
// enum enQuickWithdraw {e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6, e800 = 7, e1000 = 8, eExit = 9};

struct stClient {

    string AccountNumber;
    string PinCode;
    string ClientName;
    string PhoneNumber;
    double AccountBalance;
};

const string ClientsDataFile = "Clients_data_file.txt";
stClient CurrentClient;

void ShowNormalWithdrawScreen();
void Show_ATM_MainMenu();
void GoBackToMainMenu();
void ShowQuickWithdrawScreen();

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {

    string Data;
    Data += Client.AccountNumber + Separator;
    Data += Client.PinCode + Separator;
    Data += Client.ClientName + Separator;
    Data += Client.PhoneNumber + Separator;
    Data += to_string(Client.AccountBalance);
    return Data;
}

vector<string> SplitString(string Data, string Separator = "#//#") {

    vector<string> vClient;
    string Word = "";
    short Pos = 0;

    while ((Pos = Data.find(Separator)) != Separator.npos) {

        Word = Data.substr(0, Pos);

        if (Word != "") {
            vClient.push_back(Word);
        }
        Data.erase(0, Pos + Separator.length());
    }
    if (!Data.empty()) {
        vClient.push_back(Data);
    }
    return vClient;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#") {

    stClient Client;
    vector<string> vClient;
    vClient = SplitString(Line);

    Client.AccountNumber = vClient[0];
    Client.PinCode = vClient[1];
    Client.ClientName = vClient[2];
    Client.PhoneNumber = vClient[3];
    Client.AccountBalance = stod(vClient[4]); //cast string to double.
    return Client;
}

vector<stClient> LoadClientDataFromFile(string FileName, string Separator = "#//#") {

    vector<stClient> vClients;
    fstream DataFile;
    DataFile.open(FileName, ios::in);

    if (DataFile.is_open()) {

        stClient Client;
        string Line;

        while (getline(DataFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        DataFile.close();
    }
    return vClients;
}

vector<stClient> SaveClientDataToFile(string FileName, vector<stClient>& vClients, string Separator = "#//#") {

    fstream DataFile;
    DataFile.open(FileName, ios::out);
    string DataLine;

    if (DataFile.is_open()) {

        for (stClient & Client : vClients) {

            if (Client.AccountNumber == CurrentClient.AccountNumber) {
                Client = CurrentClient;
            }
            DataLine = ConvertRecordToLine(Client);
            DataFile << DataLine << endl;
        }
        DataFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumberAndPINCode (string AccountNumber, string PINcode, stClient& Client) {

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);

    for (stClient C : vClient) {

        if ((C.AccountNumber == AccountNumber) && (C.PinCode == PINcode)) {
            Client = C;
            return true;
        }
    }
    return false;
}

void ShowLoginScreen() {

    bool LoginFaild = false;
    string AccountNumber, PINcode;

    do {
        system("cls");
        cout << "=============================================\n";
        cout << "           Login screeen:\n";
        cout << "=============================================\n";

        if (LoginFaild) {
            cout << "Invalid account number/PIN code!\n";
        }
        
        cout << "\nEnter Accout Number:  "; 
        cin >> AccountNumber; 

        cout << "\nEnter PIN code:  "; 
        cin >> PINcode; 

        LoginFaild = !FindClientByAccountNumberAndPINCode(AccountNumber, PINcode, CurrentClient);
    } while (LoginFaild);
    Show_ATM_MainMenu();
}

bool ConfirmTransaction() {

    char C = 'Y';
    do {
        cout << "\nAre you sure you want to perform this transaction [Y/N]:  ";
        cin >> C;

        if (toupper(C) != 'Y' && toupper(C) != 'N') {
            cout << "\nInvalid input, please try again.";
        }
    } while (toupper(C) != 'Y' && toupper(C) != 'N');

    return true;
}

short ReadQuickWithdrawAmount() {
    
    short Choice;
    cout << "Choose what do you want to do [1:9]:  ";
    cin >> Choice;

    while (Choice < 1 && Choice > 9) {
        cout << "\nInvalid input, choose what do you want to do [1:9]:  ";
        cin >> Choice;
    }
    return Choice;
}

short QuickWithdrawOptions(short Choice) {

    switch (Choice) {

    case 1:
        return 20;
        break;

    case 2:
        return 50;
        break;

    case 3:
        return 100;
        break;

    case 4:
        return 200;
        break;

    case 5:
        return 400;
        break;

    case 6:
        return 600;
        break;

    case 7:
        return 800;
        break;

    case 8:
        return 1000;
        break;

    default:
        return 0;
    }
}

bool DepositToAccount(string AccountNumber, vector<stClient>& vClients,double Amount) {

    char ConfirmDeposit = 'Y';

    cout << "\nAre you sure you want perform this transaction [Y/N]:  ";
    cin >> ConfirmDeposit;

    if (toupper(ConfirmDeposit) == 'Y') {

        for (stClient & C : vClients) {
            
            if (C.AccountNumber == AccountNumber) {

                C.AccountBalance += Amount;
                cout << "\nDone successfully, the new balance is " << C.AccountBalance << endl;
                SaveClientDataToFile(ClientsDataFile, vClients);
                return true;
            }
        }
    }
    return false;
}

void QickWithdraw(short Choice) {

    if (Choice == 9) {
        return;
    }
    short WithdrawAmount = QuickWithdrawOptions(Choice);

    if (WithdrawAmount > CurrentClient.AccountBalance) {
        cout << "\nThe anount exceeded your balance, make another choice.\n";
        cout << "\nPress any key to contenue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
    }

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    DepositToAccount(CurrentClient.AccountNumber, vClient, WithdrawAmount * -1);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowQuickWithdrawScreen() {

    system("cls");
    cout << "=============================================\n";
    cout << "               Quick withdraw\n";
    cout << "=============================================\n";
    cout << "\t[1] 20 \t\t[2] 50\n";
    cout << "\t[3] 100 \t[4] 200\n";
    cout << "\t[5] 400 \t[6] 600\n";
    cout << "\t[7] 800 \t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "=============================================\n";
    QickWithdraw(ReadQuickWithdrawAmount());
}

int ReadNormalWithdrawAmount() {

    int NewAmount = 0;
    do {

        cout << "\nEnter an amount multiple of 5:  ";
        cin >> NewAmount;

    } while ((NewAmount % 5) != 0);
    return NewAmount;
}

void NormalWithdraw() {

    int WithdrawAmount = ReadNormalWithdrawAmount();
    
    if (WithdrawAmount > CurrentClient.AccountBalance) {
        cout << "The anount exceeded your balance, make another choice.";
        cout << "Press any key to contenue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
    }

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    DepositToAccount(CurrentClient.AccountNumber, vClient, WithdrawAmount * -1);
    CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowNormalWithdrawScreen() {

    system("cls");
    cout << "=============================================\n";
    cout << "               Normal withdraw\n";
    cout << "=============================================\n\n";

    NormalWithdraw();
}

double ReadDepositAmount() {

    double NewAmount = 0;

        do {
        cout << "\nEnter a positive deposit amount:  ";
        cin >> NewAmount;

        if (NewAmount < 0) {
            cout << "\nInvelid amount, ";
        }

    } while (NewAmount < 0);
    return NewAmount;
}

void DepositScreen() {

    int NewAmount = ReadDepositAmount();

    vector<stClient> vClient = LoadClientDataFromFile(ClientsDataFile);
    DepositToAccount(CurrentClient.AccountNumber, vClient, NewAmount);
    CurrentClient.AccountBalance += NewAmount;
}

void ShowDepositScreen() {

    cout << "=============================================\n";
    cout << "                   Deposit:\n";
    cout << "=============================================\n\n";

    DepositScreen();
}

void ShowBalanceScreen() {

    cout << "=============================================\n";
    cout << "               Check balance:\n";
    cout << "=============================================\n";
    cout << "Your balance is " << CurrentClient.AccountBalance << endl;
}

void GoBackToMainMenu() {

    cout << "\nPress any key to go back to the main menu...";
    system("pause>0");
    Show_ATM_MainMenu();
}

short Read_ATM_MainMenuOption() {
    
    short Choice;
    cout << "Choose what do you want to do [1:5]:  ";
    cin >> Choice;

    while (Choice < 1 && Choice > 5) {
        cout << "\nInvalid input, choose what do you want to do [1:5]:  ";
        cin >> Choice;
    }
    return Choice;
}

void Perform_ATM_MainMenuOption(enATM choice) {

    switch (choice) {

        case enATM::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen(); 
        GoBackToMainMenu();
        break;
    
        case enATM::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
        
        case enATM::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;

        case enATM::eCheckBalance:
        system("cls");
        ShowBalanceScreen();
        GoBackToMainMenu();
        break;

        default:
        ShowLoginScreen();
    }
}

void Show_ATM_MainMenu() {

    system("cls");
    cout << "=============================================\n";
    cout << "               ATM main menu\n";
    cout << "=============================================\n";
    cout << "\t[1] Quick withdraw.\n";
    cout << "\t[2] Normal withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "=============================================\n";
    Perform_ATM_MainMenuOption((enATM)Read_ATM_MainMenuOption());
}

int main() {

    ShowLoginScreen();
    system("pause>0");
    return 0;
}

*/
