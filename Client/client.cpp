#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <Windows.h>

using namespace std;

int main()
{
	//1��ȷ��Э��汾
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 == ret)
	{
		cout << "����Э��汾��2.2\n";
	}

	//2������socket
	SOCKET cSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == cSoc)
	{
		cout << "����socketʧ��,ԭ��\n\t\t--->" << GetLastError() << endl;
		return -2;
	}
	cout << "����socket�ɹ�!\n";
	//3����ȡ��������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//����ip��ַ
	addr.sin_port = htons(10086); //С��ת���  �˿ں���10000����
	addr.sin_family = AF_INET;//��socket������һ����������һ��
	
	//4.���ӷ�����
	int r = connect(cSoc, (sockaddr*)&addr, sizeof addr);
	if(-1 == r)
	{
		cout << "���ӷ���ʧ�ܣ�\n\t\t--------->" << GetLastError();
		closesocket(cSoc);
		WSACleanup(); //����Э��汾��Ϣ
		return -3;
	}
	cout << "���ӷ������ɹ�!\n";

	//5��ͨ��
	char buff[256];
	while (1)
	{
		memset(buff, 0, 256);
		cout << "����ʲô��Ϣ: \n";
		cin >> buff;
		send(cSoc, buff,strlen(buff),NULL);
	}
	cout << endl;
	system("pause");
	return 0;
	}