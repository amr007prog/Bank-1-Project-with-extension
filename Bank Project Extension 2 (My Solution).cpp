


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string FileName = "Clients.txt";

void ShowMainMenu();

void ShowTransactionsMenu();

enum enTranactionsMenuOption { eDeposit = 1 , eWithDraw = 2,
eTotalBalance = 3 , eMainMenu = 4};

struct stClient {
	string AccoutNumber;
	string Pincode;
	string Name;
	string Phone;
	double AccountBalance;
	int Deposit = 0;
	int WithDraw = 0;
	bool MarkForDelete = false;
};

vector <string> SplitString(string S1, string Delim = "#//#") {

	vector <string> vSplitString;

	int Pos = 0;

	string sWord;

	while ((Pos = S1.find(Delim)) != std::string::npos) {

		sWord = S1.substr(0, Pos);

		if (sWord != "") {
			vSplitString.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}
	if (S1 != "") {
		vSplitString.push_back(S1);
	}
	return vSplitString;
}

stClient ConvertLineToRecord(string S1, string Delim = "#//#") {

	stClient Client;
	vector <string> vSplitString = SplitString(S1, Delim);

	Client.AccoutNumber = vSplitString[0];
	Client.Pincode = vSplitString[1];
	Client.Name = vSplitString[2];
	Client.Phone = vSplitString[3];
	Client.AccountBalance = stod(vSplitString[4]);

	return Client;
}

string ConvertRecordToLine(stClient Client, string Delim = "#//#") {

	string Line = "";

	Line += Client.AccoutNumber + Delim;
	Line += Client.Pincode + Delim;
	Line += Client.Name + Delim;
	Line += Client.Phone + Delim;
	Line += to_string(Client.AccountBalance);

	return Line;

}

bool ClientExistByAccountNumber(string AccountNumber, string FileName) {

	vector <stClient> vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);

			if (Client.AccoutNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();

	}
	return false;
}

stClient ReadNewClientInfo() {

	stClient Client;

	cout << "Enter Account Number: ";
	getline(cin >> ws, Client.AccoutNumber);

	while (ClientExistByAccountNumber(Client.AccoutNumber, FileName)) {

		cout << "\nClient With [" << Client.AccoutNumber << "] already exists, Enter Another Account Number: ";
		getline(cin >> ws, Client.AccoutNumber);
	}

	cout << "Enter PicCode: ";
	getline(cin, Client.Pincode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}

vector <stClient> LoadClientsDataFromFile(string FileName) {

	vector <stClient> vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in);



	if (MyFile.is_open()) {
		string Line;
		stClient Client;
		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintClientRecordLine(stClient Client) {

	cout << "| " << setw(15) << left << Client.AccoutNumber;
	cout << "| " << setw(10) << left << Client.Pincode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen() {

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "Accout Number";
	cout << "| " << setw(10) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";
	cout
		<< "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t No Clients Availabe In The System!";
	}
	else {
		for (stClient Client : vClients) {
			PrintClientRecordLine(Client);
			cout << endl;
		}
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;


}

void PrintClientCard(stClient Client) {

	cout << "\nThe Following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccount Number: " << Client.AccoutNumber;
	cout << "\nPin Code      : " << Client.Pincode;
	cout << "\nName          : " << Client.Name;
	cout << "\nPhone         : " << Client.Phone;
	cout << "\nAccount balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client) {

	for (stClient &C : vClients) {

		if (C.AccoutNumber == AccountNumber) {
			Client = C;
			return true;
		}

	}
	return false;
}

stClient ChangeClientRecord(string AccountNumber) {

	stClient Client;

	Client.AccoutNumber = AccountNumber;

	cout << "\n\nEnter PinCode: ";
	getline(cin >> ws, Client.Pincode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

	for (stClient& C : vClients) {

		if (C.AccoutNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}

	}
	return false;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open()) {

		for (stClient C : vClients) {

			if (C.MarkForDelete == false) {

				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}

		}
		MyFile.close();
	}
	return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << stDataLine << endl;
		MyFile.close();
	}

}

void AddNewClient() {

	stClient Client = ReadNewClientInfo();
	AddDataLineToFile(FileName, ConvertRecordToLine(Client));

}

void AddNewClients() {

	char AddMore = 'Y';
	do {
		cout << "Adding New Client:\n\n";

		AddNewClient();

		cout << "Client Added Successfully, do you want to add more? Y/N: ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

	char Ans = 'Y';
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre You Sure You Want To Delete This Client? Y/N: ";
		cin >> Ans;
		if (toupper(Ans) == 'Y') {

			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);

			SaveClientsDataToFile(FileName, vClients);

			vClients = LoadClientsDataFromFile(FileName);

			cout << "\n\nClient Deleted successfully.";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number(" << AccountNumber << ") Not Found!";
		return false;
	}
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients) {

	char Ans = 'Y';
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre You Sure You Want To Update This Client Info? Y/N: ";
		cin >> Ans;

		if (toupper(Ans) == 'Y') {

			for (stClient& C : vClients) {

				C = ChangeClientRecord(AccountNumber);
				break;
			}

		}

		SaveClientsDataToFile(FileName, vClients);

		cout << "\n\nClient Updated Successfully.";
		return true;

	}
	else {
		cout << "\nClient With Accout Number(" << AccountNumber << ") is Not Found!";
		return false;
	}
}

string ReadAccountNumber() {

	string AccountNumber;
	cout << "\nPlease Enter Account Number: ";
	cin >> AccountNumber;
	return AccountNumber;

}

void ShowDeleteClientScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector <stClient> vCleints = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vCleints);
}

void ShowUpdateClientScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";

	AddNewClients();
}

void ShowFindClientScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	stClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\nClient With Account Number [" << AccountNumber << "] is not found!";
	}
}

void ShowEndScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

stClient Deposit(stClient &Client) {

	cout << "\n\nPlease enter Deposit Amount: ";
	cin >> Client.Deposit;

	Client.AccountBalance += Client.Deposit;

	return Client;

}

bool DepositClient(string AccountNumber, vector <stClient>& vClients) {

	char Ans = 'Y';
	stClient Client;
	
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] does not exist.\n";
		cout << "\nPlease enter another Account Number: ";
		cin >> AccountNumber;
	}
	
		

			PrintClientCard(Client);

			Deposit(Client);
			
			cout << "\n\nAre You Sure You Want To Perform This Transaction? Y/N: ";
			cin >> Ans;

			if (toupper(Ans) == 'Y') {

			
				for (stClient& C : vClients) {
					if (C.AccoutNumber == Client.AccoutNumber) {
						C = Client;   
						break;
					}
				}

				SaveClientsDataToFile(FileName, vClients);

				cout << "\nDone Successfully , New Balance is " << Client.AccountBalance;

				return true;

			
		}
		
	
	
}

void ShowDepositScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DepositClient(AccountNumber , vClients);

}

int CalculateTotalBalance(vector <stClient> vClients) {

	int TotalBalance = 0;

	for (stClient C : vClients) {

		TotalBalance += C.AccountBalance;
		
	}
	return TotalBalance;
}

void PrintClientBalanceList(stClient Client) {

	cout << "| " << setw(15) << left << Client.AccoutNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(34) << left << Client.AccountBalance;

}

void PrintClientBalancesList() {

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << setw(15) << left << "Accout Number";
	
	cout << "| " << setw(40) << left << "Client Name";
	
	cout << "| " << setw(34) << left << "Balance";
	cout
		<< "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t No Clients Availabe In The System!";
	}
	else {
		for (stClient Client : vClients) {
			PrintClientBalanceList(Client);
			cout << endl;
		}
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "\n\t\t\t\t\tTotal Balances = " << CalculateTotalBalance(vClients);

}

stClient WithDraw(stClient& Client) {

	cout << "\n\nPlease enter WithDraw Amount: ";
	cin >> Client.WithDraw;

	while (Client.WithDraw > Client.AccountBalance) {
		cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance;
		cout << "\nPlease enter another amount: ";
		cin >> Client.WithDraw;
	}

	Client.AccountBalance = Client.AccountBalance - Client.WithDraw;

	return Client;

}

bool WithDrawClient(string AccountNumber, vector <stClient>& vClients) {
	char Ans = 'Y';
	stClient Client;
	
	while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		cout << "\nClient with Account Number [" << AccountNumber << "] does not exist.\n";
		cout << "\nPlease enter another Account Number: ";
		cin >> AccountNumber;
	}

	PrintClientCard(Client);

	WithDraw(Client);

	cout << "\n\nAre You Sure You Want To Perform This Transaction? Y/N: ";
	cin >> Ans;

	if (toupper(Ans) == 'Y') {

		for (stClient& C : vClients) {
			if (C.AccoutNumber == Client.AccoutNumber) {
				C = Client;
				break;
			}
		}

		SaveClientsDataToFile(FileName, vClients);


		cout << "\nDone Successfully , New Balance is " << Client.AccountBalance;

		return true;


	}

}

void ShowWithDrawScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tWithDraw Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	WithDrawClient(AccountNumber, vClients);

}

void GoBackToTransationsMenu() {
	cout << "\n\nPress any key to go back to transations Menu...";
	system("pause>0");
	ShowTransactionsMenu();
}

enum enMainMenuOptions {
	ClientsList = 1, eAddNewClient = 2,
	DeleteClient = 3, UpdateClient = 4,
	FindClient = 5, Transactions = 6,
	Exit = 7
};

void GoBackToMainMenu() {

	cout << "\n\nPress any key to go back to main menu...";
	system("Pause>0");
	ShowMainMenu();

}

short ReadTransactionsMenuOption() {

	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> Choice;
	
	return Choice;
}

void PerformTrasactionsMenuOption(enTranactionsMenuOption TransactionOption) {

	switch (TransactionOption) {
	case enTranactionsMenuOption::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransationsMenu();
		break;
	case enTranactionsMenuOption::eWithDraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransationsMenu();
		break;
	case enTranactionsMenuOption::eTotalBalance:
		system("cls");
		PrintClientBalancesList();
		GoBackToTransationsMenu();
		break;
	case enTranactionsMenuOption::eMainMenu:
		system("cls");
		ShowMainMenu();
		break;
	}

}

void ShowTransactionsMenu() {
	system("cls");
	cout << "===========================================\n";
	cout << "\tTransactions Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] WithDraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "===========================================\n";
	PerformTrasactionsMenuOption((enTranactionsMenuOption)ReadTransactionsMenuOption());
}

short ReadMainMenuOption() {


	short Choice = 0;
	do {
		cout << "Choose What You Want to do? [1 to 7]?";
		cin >> Choice;
	} while (Choice < 1 || Choice > 7);
	return Choice;
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOptions) {

	switch (MainMenuOptions) {
	case enMainMenuOptions::ClientsList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::DeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::UpdateClient:
		system("Cls");
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::FindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::Transactions:
		system("cls");
		ShowTransactionsMenu();
		break;
	case enMainMenuOptions::Exit:
		system("cls");
		ShowEndScreen();
		break;
	}

}

void ShowMainMenu() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Trasactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
	PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}





int main()
{
	ShowMainMenu();
	system("pause>0");
}




