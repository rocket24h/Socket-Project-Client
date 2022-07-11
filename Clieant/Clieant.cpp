// Client.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "framework.h"
#include <afxsock.h>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include "date.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <list>
using namespace cv;
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


struct  image {
	Mat data;
	int width;
	int height;
	void readImageFile(string fileName) {
		this->data = imread(fileName);
		height = data.size[0];
		width = data.size[1];
	}
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
	int k = 0;
	char* buffer = new char[12];
	char* buffer2 = new char[12];
	date d;
	date d2;
	room* li = new room[10];
	bookingDates f;
	cout << "Nhap ten khach san: ";
	char* ht = new char[100];
	cin.getline(ht, 100);

	//cout << ht;
	while (timeTL != 0) {
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
		timeTL--;
	}
	timeTL = 12;
	while (timeTL != 0) {
		cout << "Ngay roi di :\n ";
		cout << "ngay : ";
		cin >> d2.day;
		cout << "thang : ";
		cin >> d2.month;
		cout << "nam : ";
		cin >> d2.year;
		cout << countNoOfDays(d, d2);
		if (checkdate(d2) && countNoOfDays(d, d2) >= 0)
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
		client.Receive((int*)&k, sizeof(int), 0);
		cout << "Danh sach cac phong trong: " << endl;
		for (int j = 0; j < k; j++) {
			int size = 0;
			int tempSize = 10;
			client.Receive((char*)&size, sizeof(int), 0);
			if (size == 0) break;
			li[j].name = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&li[j].name[i], tempSize, 0);

			}
			li[j].name[size] = '\0';
			cout << "Phong       : " << li[j].name << endl;
			client.Receive((char*)&size, sizeof(int), 0);
			if (size <= 0) break;
			li[j].type = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&li[j].type[i], tempSize, 0);

			}
			li[j].type[size] = '\0';
			cout << "Loai phong  : " << li[j].type << endl;
			client.Receive((char*)&size, sizeof(int), 0);
			if (size <= 0) break;
			li[j].description = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&li[j].description[i], tempSize, 0);
			}
			li[j].description[size] = '\0';
			cout << "Mo ta phong : \n" << li[j].description << endl;
			client.Receive((char*)&size, sizeof(int), 0);
			if (size <= 0) break;
			li[j].price = new char[size + 1];
			for (int i = 0; i < size; i = i + tempSize) {
				if (i + tempSize >= size) {
					tempSize = size - i;
				}
				client.Receive((char*)&li[j].price[i], tempSize, 0);
			}
			li[j].price[size] = '\0';
			cout << "Gia tien    : " << li[j].price<< endl;
			cout << endl << "/////////////////////////////////" << endl;
		}
		cout << endl;
	}
	char* nameRoom = NULL;
	int type = 0;
	cout << "Khach hang muon xem anh co phong nao khong ?" << endl;
	cin >> type;
	client.Send(&type, sizeof(int), 0);
	int Images = 3;
	uchar* dataReceiver = NULL;
//	client.Receive((int*)&Images, sizeof(int), 0);
	image sudo;
	size = 0;
	int payLoads;
	vector <uchar> data;
	for (int i = 0; i < Images; i++) {
		Mat reSize;
		client.Receive((int*)&sudo.height, sizeof(int), 0);
		client.Receive((int*)&sudo.width, sizeof(int), 0);
		client.Receive((int*)&size, sizeof(int), 0);
		dataReceiver = new uchar[size];
		if (size == 0) return;
		payLoads = 100000;
		for (int j = 0; j < size; j = payLoads + j) {
			if (j + payLoads >= size) {
				payLoads = size - j;
			}
			client.Receive((uchar*)&dataReceiver[j], payLoads, 0);
		}
		copy(&dataReceiver[0], &dataReceiver[size - 1], back_inserter(data));
		sudo.data = imdecode(data, 1);
		resize(sudo.data, reSize, Size(680, 480));
		switch (i) {
		case 0: imshow("Phong ngu", reSize);
			break;
		case 1: imshow("Phong khach", reSize);
			break;
		case 2: imshow("Phong tam", reSize);
			break;
		default:
			break;
		}
		size = 0;
		delete[] dataReceiver;
		dataReceiver = NULL;
		data.clear();
		client.Send(&size, sizeof(size), 0);
	}
	system("CLS");
	cout << "Danh sach cac phong trong: " << endl;
	for (int i = 0; i < k; i++) {
		cout << "Phong       : " << li[i].name << endl;
		cout << "Loai phong  : " << li[i].type << endl;
		cout << "Mo ta phong : " << li[i].description << endl;
		cout << "Gia tien    : " << li[i].price << endl;
		cout << endl << "/////////////////////////////////" << endl;
	}
	waitKey(0);
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
				GotoXY(25 + 25, 23);
				if (switchMenu != 0) {					
					cout << "Huy phong" << endl;
				}
				GotoXY(25 + 25, 24);
				if (switchMenu == 0) {
					cout << "Thoat";
				}
				else {
					cout << "Dang xuat";
				}
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
						}
						else
						{
							hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
							*hconnected = ClientSocket.Detach();
							timkiem(hconnected);
							ClientSocket.Attach(*hconnected);
						//	delete hconnected;
						}
						ClientSocket.Send(&end, sizeof(end), 0);
						system("CLS");
						break;
					case 1:
						if (switchMenu == 0)
						{
							hconnected = new SOCKET(); // Bien doi cai socket thanh mot bien co the truyen vao ham
							*hconnected = ClientSocket.Detach();
							dangnhap(hconnected, switchMenu);
							ClientSocket.Attach(*hconnected);
							delete hconnected;
							system("CLS");
							ClientSocket.Send(&end, sizeof(end), 0);
						}
						else {
							// phan dat phong o day
						}
						break;
					case 2: 
						if (switchMenu == 0) {
						}
						else
						{					
						}
					break;
					case 3:
						if (switchMenu == 0) {
							end = 0;
							menu = false;
						}
						else {
							switchMenu = 0;
							ClientSocket.Send(&switchMenu, sizeof(int), 0);				
						}
						system("CLS");
						ClientSocket.Send(&end, sizeof(end), 0);
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
