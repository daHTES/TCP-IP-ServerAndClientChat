#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main ()
{
	setlocale(LC_ALL, "Russian");
	string ipAddressServer = "127.0.0.1"; // IP адрес сервака по какому будем конектиться
	int portServer = 40000; // Порт для прослушки на сервере
	WSADATA WinSocksObjClient;
	WORD version = MAKEWORD(2, 2);
	int winSocksResultConnect = WSAStartup(version, &WinSocksObjClient); // переменная для запуска нашего winsocks
	if (winSocksResultConnect != 0) // проверяем, смогли ли получить коннект 
	{
		cerr << "Программа не может создать winsock! " << winSocksResultConnect << endl;
		return;
	}
	SOCKET listenSockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSockClient == INVALID_SOCKET) 
	{
		cerr << "Программа не может создать сокет, " << WSAGetLastError() << endl; // вызываем метод ошибок, вместо вызова самого сокса
	}
	sockaddr_in addressIPForClientSocket; // проделываем те же махинации, как и со структурой сокета сервера
	addressIPForClientSocket.sin_family = AF_INET;
	addressIPForClientSocket.sin_port = htons(portServer); // передаем переме. номера порта
	int sizeOfaddressIPForClientSocket = sizeof(addressIPForClientSocket); // перем. для хранения размера структуры
	inet_pton(AF_INET, ipAddressServer.c_str(), &addressIPForClientSocket.sin_addr);

	int connectResultToServer = connect(listenSockClient, (sockaddr*)&addressIPForClientSocket, sizeOfaddressIPForClientSocket); // реализуем коннект к серверу
	if (connectResultToServer == SOCKET_ERROR) // проверяем подключение
	{
		cerr << "Программа не может подключиться к серверу" << WSAGetLastError() << endl;
		closesocket(listenSockClient); // закрываем и очищаем
		WSACleanup();
		return;
	}
	char bufferSize[10000];
	string dataFromClient;
	do 
	{
		cout << " ||>.. "; // приглашаешь к вводу данных
		getline(cin, dataFromClient); // по средства ф-и getline получаем поток ввода и данные от клиента
		if (dataFromClient.size() > 0) // чекнем, чтобы юзер что то отправил сперва
		{
			int sendResultFromClient = send(listenSockClient, dataFromClient.c_str(), dataFromClient.size() + 1, 0); // переменная для хранения результата отправки данных от клиента
			if (sendResultFromClient != SOCKET_ERROR) 
			{
				ZeroMemory(bufferSize, 10000);
				int bytesReceivedFromCLient = recv(listenSockClient, bufferSize, 10000, 0); // фу-я для получение данных от клиента
				if (bytesReceivedFromCLient > 0) 
				{
					cout << "Сервер говорит: " << string(bufferSize, 0, bytesReceivedFromCLient) << endl; // выводим все в консоль
				}
			}
		}
	} while (dataFromClient.size() > 0);
	closesocket(listenSockClient);
	WSACleanup();

}