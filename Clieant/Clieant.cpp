// Client.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "framework.h"
#include <afxsock.h>
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

struct user {
	char* name = NULL;
	char* password = NULL;
	void add(char* src, char*& dst) {
		if (dst != NULL) delete[] dst;
		int size = strlen(src);
		dst = new char[size + 1];
		strcpy_s((char*)dst, size + 1, src);
	}
	char* STK = NULL;
	user* next;
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
			if (name[i] < 'a' || name[i] > 'z') {
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

bool checkBankInfo(char* STK)
{
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

void dangky() {
	
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
		int end = 0;
		if (ClientSocket.Connect(_T("127.0.0.1 "), 1234) != 0) {
			FixConsoleWindow();
			int menu_item = 0, x = 21;
			bool menu = true;
			int flag = 1;
			while (menu) {
				drawMenu();
				GotoXY(18 + 25, x);
				cout << "-> ";
				GotoXY(18 + 20 + 25, x);
				cout << " <-";
				GotoXY(25 + 25, 21);
				cout << "Dang ky";
				GotoXY(25 + 25, 22);
				cout << "Dang nhap";
				GotoXY(25 + 25, 23);
				cout << "Tra cuu";
				GotoXY(25 + 25, 24);
				cout << "Exit";
				system("pause>nul"); // the >nul bit causes it the print no message
				if (GetAsyncKeyState(VK_DOWN) && x != 24) { // down button pressed
					GotoXY(18 + 25, x);
					cout << "  ";
					GotoXY(18 + 20 + 25, x);
					cout << "  ";
					GotoXY(18 + 21 + 25, x);
					cout << "  ";
					x++;
					GotoXY(18 + 25, x);
					cout << "-> ";
					GotoXY(18 + 20 + 25, x);
					cout << " <-";
					menu_item++;
					continue;
				}
				if (GetAsyncKeyState(VK_UP) && x != 21) { // up button pressed
					GotoXY(18 + 25, x);
					cout << "  ";
					GotoXY(18 + 20 + 25, x);
					cout << "  ";
					GotoXY(18 + 21 + 25, x);
					cout << "  ";
					x--;
					GotoXY(18 + 25, x);
					cout << "-> ";
					GotoXY(18 + 20 + 25, x);
					cout << " <-";
					menu_item--;
					continue;
				}
				if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
					ClientSocket.Send(&menu_item, sizeof(menu_item), 0);
					user user1;
					int flag = 1; // Đảm bảo gửi đủ 3 gói tin username, mk, STK
					int sendAlarm = 0; // Báo hiệu là gói tin đã ổn nên gửi đi
					int timeTL = 12; // hạn tối đa 12 lần nhập cho client
					switch (menu_item) {
					case 0:
						system("CLS");
						flag = 1;
						sendAlarm = 0;
						timeTL = 12;
						while (flag <= 3) {
							switch (flag)
							{
							case 1: cout << "Username : ";
								break;
							case 2: cout << "Password : ";
								break;
							case 3: cout << "So tai khoan : ";
								break;
							default:
								break;
							}
							int size = 100;
							int tempSize = 0;
							char buffer[100];
							int checker = 0;
							cin.getline(buffer, 100);
							switch (flag) {
							case 1:
								if (checkUserName(buffer) == true) {
									user1.add(buffer, user1.name);
									sendAlarm = 1;
									if (sendAlarm == 1) {
										ClientSocket.Receive(&checker, sizeof(bool), 0);
										if (checker == false) flag++;
										
									}
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
							default:
								break;
							}
							if (sendAlarm == 1) {
								size = strlen(buffer);
								ClientSocket.Send(&size, sizeof(size), 0);
								ClientSocket.Send(buffer, size, 0);
								sendAlarm = 0;
							}
							timeTL--;
							if (timeTL == 0) {
								break;
							}
						}
						system("CLS");
						ClientSocket.Send(&end, sizeof(end), 0);
						break;
					case 1:
						ClientSocket.Send(&end, sizeof(end), 0);
						break;
					case 2:
						ClientSocket.Send(&end, sizeof(end), 0);
						break;
					case 3:
						end = 0;
						ClientSocket.Send(&end, sizeof(end), 0);
						system("CLS");
						menu = false;
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
