// Client.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "framework.h"
#include <afxsock.h>
#include <iostream>
#include <string>
#include "date.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <list>

// The one and only application object

CWinApp theApp;

using namespace std;

struct user {
	char* name = NULL;
	char* password = NULL;
	void add(char* src, char*& dst) {
		if (src == NULL) return;
		if (dst != NULL) delete[] dst;
		int size = strlen(src);
		dst = new char[size + 1];
		strcpy_s((char*)dst, size + 1, src);
	}
	char* STK = NULL;
	user* next = NULL;
};
struct bookingDates
{
	char* booked = NULL;
	char* arrive = NULL;
	char* leave = NULL;
	void add(char* src, char*& dst)
	{
		if (dst != NULL) delete[] dst;
		int size = strlen(src);
		dst = new char[size + 1];
		strcpy_s((char*)dst, size + 1, src);
	}
	
	bookingDates* next;
};
struct room
{
	char* name = NULL;
	char* type = NULL;
	char* description = NULL;
	char* price = NULL;
	void add(char* src, char*& dst)
	{
		if (dst != NULL) delete[] dst;
		int size = strlen(src);
		dst = new char[size + 1];
		strcpy_s((char*)dst, size + 1, src);
	}
	bookingDates* bookedList = NULL;
};

struct hotel
{
	char* name;
	void add(char* src, char*& dst)
	{
		if (dst != NULL) delete[] dst;
		int size = strlen(src);
		dst = new char[size + 1];
		strcpy_s((char*)dst, size + 1, src);
	}
	room roomList[20] = {};
};
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}


bool checkUserName(char* name) {
	int size = strlen(name);
	if (size < 5) {
		return false;
	}
	for (int i = 0; i < size; i++) {
		if (name[i] < '0') {
			return false;
		}
		else if (name[i] > '9') {
			if (name[i] < 'A') {
				return false;
			}
			else if (name[i] > 'Z' && name[i] < 'a') {
				return false;
			}
			else if (name[i] > 'z') {
				return false;
			}
		}
	}
	return true;
}

bool  checkPassword(char* password) {
	int size = strlen(password);
	if (size < 3)
		return false;
	return true;
}

bool checkBankInfo(char* STK) {
	int size = strlen(STK);
	if (size != 10) return false;
	for (int i = 0; i < size; i++)
	{
		if (STK[i] < '0' || STK[i] > '9') return false;
	}
	return true;
}

void drawMenu()
{
	unsigned char vborder = 205, hborder = 186, lowerleft = 200, lowerright = 188, upperleft = 201, upperright = 187;
	GotoXY(17 + 25, 20);
	for (int i = 0; i < 26; i++) {
		cout << vborder;
	}
	GotoXY(16 + 25, 20); cout << upperleft;
	GotoXY(16 + 25, 21); cout << hborder;
	GotoXY(16 + 25, 22); cout << hborder;
	GotoXY(16 + 25, 23); cout << hborder;
	GotoXY(16 + 25, 24); cout << hborder;
	GotoXY(16 + 25, 25); cout << lowerleft;
	GotoXY(17 + 25, 25);
	for (int i = 0; i < 26; i++) {
		cout << vborder;
	}
	GotoXY(16 + 27 + 25, 20); cout << upperright;
	GotoXY(16 + 27 + 25, 21); cout << hborder;
	GotoXY(16 + 27 + 25, 22); cout << hborder;
	GotoXY(16 + 27 + 25, 23); cout << hborder;
	GotoXY(16 + 27 + 25, 24); cout << hborder;
	GotoXY(16 + 27 + 25, 25); cout << lowerright;
}

void dangnhap(SOCKET* hconnected, int& menuSwitch) {
	CSocket client;
	client.Attach(*hconnected);
	user user1;
	system("CLS");
	int temp = 0;
	int flag = 1;
	bool legit = 0;
	int sendAlarm = 0;
	int timeTL = 12;
	while (flag <= 2 || timeTL == 0) {
		switch (flag) {
		case 1: GotoXY(25 + 20, 21); cout << "Username : ";
			break;
		case 2: GotoXY(25 + 20, 22); cout << "Password : ";
			break;
		default:
			break;
		}
		int size = 100;
		int tempSize = 0;
		char buffer[100];
		bool checker = 0;
		cin.getline(buffer, 100);
		switch (flag) {
		case 1:
			if (checkUserName(buffer) == true) {
				user1.add(buffer, user1.name);
				sendAlarm = 1;
				flag++;
			}
			else {
				cout << "Username khong hop le! \n";
			}
			break;
		case 2:
			if (strlen(buffer) > 3) {
				user1.add(buffer, user1.password);
				sendAlarm = 1;
				flag++;
			}
			else {
				cout << "Mat khau khong hop le! \n";
			}
			break;
		default:
			break;
		}
		if (sendAlarm == 1) {
			size = strlen(buffer);
			client.Send(&size, sizeof(size), 0);
			client.Send(buffer, size, 0);
			sendAlarm = 0;
		}
		timeTL--;
	}
	client.Receive((bool*)&legit, sizeof(bool), 0);
	if (legit == 0) {
		GotoXY(25 + 20, 20); cout << "Tai khoan khong ton tai" << endl;
	}
	else {
		menuSwitch = 1;
		GotoXY(25 + 20, 20); cout << "Dang nhap thanh cong" << endl;
	}
	(void)getchar();
	*hconnected = client.Detach();
}


void dangky(SOCKET* hconnected) {
	CSocket client;
	client.Attach(*hconnected);
	user user1;
	system("CLS");
	int flag = 1;
	int sendAlarm = 0;
	int timeTL = 12;
	while (flag <= 3) {
		switch (flag) {
		case 1: GotoXY(25 + 20, 21); cout << "Username : ";
			break;
		case 2:GotoXY(25 + 20, 22); cout << "Password : ";
			break;
		case 3:GotoXY(25 + 20, 23); cout << "So tai khoan : ";
			break;
		default:
			break;
		}
		int size = 100;
		int tempSize = 0;
		char buffer[100];
		bool checker = 0;
		cin.getline(buffer, 100);
		switch (flag) {
		case 1:
			if (checkUserName(buffer) == true) {
				user1.add(buffer, user1.name);
				sendAlarm = 1;
			}
			else {
				cout << "Username khong hop le! \n";
			}
			break;
		case 2:
			if (strlen(buffer) > 3) {
				user1.add(buffer, user1.password);
				sendAlarm = 1;
				flag++;
			}
			else {
				cout << "Mat khau khong hop le! \n";
			}
			break;
		case 3:
			if (strlen(buffer) == 10) {
				int i = 0;
				for (i = 0; i < 10; i++) {
					if (buffer[i] < '0' || buffer[i] > '9') {
						break;
					}
				}
				if (i == 10) {
					user1.add(buffer, user1.STK);
					sendAlarm = 1;
					flag++;
				}
				else {
					cout << "So tai khoan khong hop le! \n";
				}
			}
			break;
		default:
			break;
		}
		if (sendAlarm == 1) {
			size = strlen(buffer);
			client.Send(&size, sizeof(size), 0);
			client.Send(buffer, size, 0);
			if (flag == 1) {
				client.Receive((bool*)&checker, sizeof(bool), 0);
				if (checker == false) {
					flag++;
				}
				else {
					cout << "Ten tai khoan da ton tai moi nhap lai" << endl;
					continue;
				}
			}
			sendAlarm = 0;
		}
		timeTL--;
		if (timeTL == 0) {
			break;
		}
	}
	*hconnected = client.Detach();
}
void timkiem(SOCKET* hconnected)
{
	CSocket client;
	client.Attach(*hconnected);
	
	system("CLS");
	
	int sendAlarm = 0;
	int timeTL = 12;
	
	 
	int size = 100;
	int tempSize = 0;
	char* buffer = new char[12];
	char* buffer2 = new char[12];
	date d;
	date d2;
	room* li = new room[3];
	bookingDates f;
	cout << "Nhap ten khach san: ";
	char* ht = new char[100];
	cin.getline(ht, 100);
	
	//cout << ht;
	while (1)
	{
		cout << "Ngay vao o :\n ";
		std::cout << "ngay : ";
		cin >> d.day;
		std::cout << "thang : ";
		cin >> d.month;
		std::cout << "nam : ";
		cin >> d.year;
		if (checkdate(d))
		{
			convertdate(d, buffer);
			f.add(buffer, f.arrive);
			sendAlarm = 1;
			break;
		}
		else {
			std::cout << "Ngay nhap khong hop le! \n";
		}
	}
	while (1)
	{
		cout << "Ngay roi di :\n ";
		cout << "ngay : ";
		cin >> d2.day;
		cout << "thang : ";
		cin >> d2.month;
		cout << "nam : ";
		cin >> d2.year;
		cout << countNoOfDays(d, d2);
		if (checkdate(d2)&&countNoOfDays(d,d2)>=0)
		{

			convertdate(d2, buffer2);
			f.add(buffer2, f.leave);
			sendAlarm = 1;
			break;
		}
		else {
			if (!checkdate)
			{
				cout << "Ngay nhap khong hop le! \n";
			}
			if (countNoOfDays(d, d2) < 0)
			{
				cout << "Ngay den chua toi ma da toi ngay roi di (T_T)\n";
			}
			
		}
	}
	 
	if (sendAlarm == 1) {
		/*
		size = strlen(ht);
		client.Send(&size, sizeof(size), 0);
		client.Send(ht, size, 0);
		cout << ht << "/" << strlen(ht) << endl;
		*/
		size = strlen(buffer);
		client.Send(&size, sizeof(size), 0);
		client.Send(buffer, size, 0);
		size = strlen(buffer2);
		client.Send(&size, sizeof(size), 0);
		client.Send(buffer2, size, 0);
		int k;
		client.Receive((int*)&k, sizeof(int), 0);
		
		cout << "Danh sach cac phong trong: " << endl;

		for (int j = 0; j < k; j++)
		{
			
			int size = 0;
			int tempSize = 10;

			char* name;
			client.Receive((char*)&size, sizeof(int), 0);

			name = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize > size) {
					tempSize = size - i;
				}
				client.Receive((char*)&name[i], tempSize, 0);

			}
			name[size] = '\0';
			li[j].name = name;
			cout << "Phong       : " << name << endl;
			delete[]name;


			char* type;
			client.Receive((char*)&size, sizeof(int), 0);
			
			type = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize > size) {
					tempSize = size - i;
				}
				client.Receive((char*)&type[i], tempSize, 0);
				
			}
			type[size] = '\0';
			
			li[j].type = type;
			cout << "Loai phong  : " << type << endl;
			delete[]type;
			client.Receive((char*)&size, sizeof(int), 0);
			
			char* des;
			des = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&des[i], tempSize, 0);
			}
			des[size] = '\0';
			li[j].description = des;
			cout << "Mo ta phong : \n" << des << endl;
			delete[]des;
			client.Receive((char*)&size, sizeof(int), 0);
			
			char* price;
			price = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&price[i], tempSize, 0);
			}
			price[size] = '\0';
			li[j].price = price;
			cout << "Gia tien    : " << price << endl;
			delete[]price;
			cout << endl << "/////////////////////////////////" << endl;;
		}
		system("pause");
	}
	sendAlarm = 0;
	*hconnected = client.Detach();
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.

		// Khoi tao Thu vien
		if (AfxSocketInit() == FALSE)
		{
			cout << "Khong the khoi tao Socket Libraray";
			return FALSE;
		}
		// Tao socket dau tien
		CSocket ClientSocket;
		ClientSocket.Create();
		// Ket noi den Server
		int end = 1;
		if (ClientSocket.Connect(_T("127.0.0.1"), 1234) != 0) {
			FixConsoleWindow();
			int menu_item = 0, x = 21;
			int switchMenu = 0;
			bool menu = true;
			while (menu) {
				drawMenu();
				GotoXY(18 + 25, x); cout << "-> ";
				GotoXY(18 + 20 + 25, x); cout << " <-";
				GotoXY(25 + 25, 21);
				if (switchMenu == 0) {
					cout << "Dang ky";
				}
				else {
					cout << "Tra cuu";
				}
				GotoXY(25 + 25, 22);
				if (switchMenu == 0) {
					cout << "Dang nhap";
				}
				else {
					cout << "Dat phong";
				}
				if (switchMenu != 0) {
					GotoXY(25 + 25, 23);
					cout << "Huy phong" << endl;
				}
				GotoXY(25 + 25, 24);
				cout << "Exit";
				system("pause>nul"); // the >nul bit causes it the print no message
				if (GetAsyncKeyState(VK_DOWN) && x != 24) { // down button pressed
					
					GotoXY(18 + 25, x); cout << "  ";
					GotoXY(18 + 20 + 25, x); cout << "  ";
					GotoXY(18 + 21 + 25, x); cout << "  ";
					
					x++;
					GotoXY(18 + 25, x); cout << "-> ";
					GotoXY(18 + 20 + 25, x); cout << " <-";
					menu_item++;
					continue;
				}
				if (GetAsyncKeyState(VK_UP) && x != 21) { // up button pressed
					GotoXY(18 + 25, x); cout << "  ";
					GotoXY(18 + 20 + 25, x); std::cout << "  ";
					GotoXY(18 + 21 + 25, x); cout << "  ";
					x--;
					GotoXY(18 + 25, x); cout << "-> ";
					GotoXY(18 + 20 + 25, x); cout << " <-";
					menu_item--;
					continue;
				}
				if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
					ClientSocket.Send(&menu_item, sizeof(menu_item), 0);
					SOCKET* hconnected = NULL;
					switch (menu_item) {
					case 0:
						if (switchMenu == 0)
						{
							hconnected = new SOCKET();
							*hconnected = ClientSocket.Detach();
							dangky(hconnected);
							ClientSocket.Attach(*hconnected);
							delete hconnected;
							system("CLS");
							ClientSocket.Send(&end, sizeof(end), 0);
							system("CLS");
						}
						else
						{
							hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
							*hconnected = ClientSocket.Detach();
							timkiem(hconnected);
							ClientSocket.Attach(*hconnected);
							delete hconnected;
							system("CLS");
							ClientSocket.Send(&end, sizeof(end), 0);
						}
						
						break;
					case 1:
						hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
						*hconnected = ClientSocket.Detach();
						dangnhap(hconnected, switchMenu);
						
						ClientSocket.Attach(*hconnected);
						delete hconnected;
						system("CLS");
						ClientSocket.Send(&end, sizeof(end), 0);
						break;
					case 2:
					{
						if (switchMenu==0)
						{
							hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
							*hconnected = ClientSocket.Detach();
							timkiem(hconnected);
							ClientSocket.Attach(*hconnected);
							delete hconnected;
							system("CLS");
							ClientSocket.Send(&end, sizeof(end), 0);
							system("CLS");
						}
						else
						{
							hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
							*hconnected = ClientSocket.Detach();
							timkiem(hconnected);
							ClientSocket.Attach(*hconnected);
							delete hconnected;
							system("CLS");
							ClientSocket.Send(&end, sizeof(end), 0);
						}
						
						
					}
					break;
					case 3:
						end = 0;
						ClientSocket.Send(&end, sizeof(end), 0);
						system("CLS");
						menu = false;
						break;
					
					default:
						break;
					}
				}
			}
		}
		else {
			cout << "Khong the ket noi den Server !!!" << endl;
		}
		// Dong ket noi
		ClientSocket.Close();
	}
	return nRetCode;
}
