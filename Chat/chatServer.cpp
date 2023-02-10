#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <Windows.h>

using namespace std;
constexpr auto NUM = 1024;
//��������NUM���ͻ��˵�socket
SOCKET cSoc[NUM];

//��ǰ�����Ϸ�����������
int num;

void Conmunication(int n)
{
	//7��ͨ��
	char buff[256];
	char temp[256];
	while (1)
	{
		int r = recv(cSoc[n], buff, 255, NULL);
		if (r > 0)
		{
			buff[r] = 0;  //'\0'
			cout << n << "��Ϣ:" << buff << endl;
			sprintf_s(temp, sizeof(temp),"%d:%s", n, buff);
			//���͸��������Ϸ������Ŀͻ���
			for (int i = 0; i < num; i++)
			{
				send(cSoc[i], temp, strlen(temp), NULL);
			}
		}

	}
}

int main()
{
	num = 0;
	//1��ȷ��Э��汾
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 == ret)
	{
		cout << "����Э��汾��2.2\n";
	}

	//2������socket
	SOCKET sSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSoc)
	{
		cout << "����socketʧ��,ԭ��\n\t\t--->" << GetLastError() << endl;
		return -2;
	}
	cout << "����socket�ɹ�!\n";
	//3��ȷ����������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//����ip��ַ
	addr.sin_port = htons(10086); //С��ת���  �˿ں���10000����
	addr.sin_family = AF_INET;//��socket������һ����������һ��
	//4����
	int r = bind(sSoc, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{

		cout << "��ʧ��,ԭ��\n\t\t--->" << GetLastError() << endl;
		closesocket(sSoc);
		WSACleanup(); //����Э��汾��Ϣ
		return -2;
	}
	cout << "�󶨳ɹ�\n";
	//5������
	r = listen(sSoc, 10);
	if (-1 == r)
	{
		cout << "����ʧ��,ԭ��\n\t\t--->" << GetLastError() << endl;
		closesocket(sSoc);
		WSACleanup(); //����Э��汾��Ϣ
		return -2;
	}
	cout << "�����ɹ�\n";
	//6.�ȴ��ͻ�������
	
	for (int i = 0; i < NUM; i++)
	{
		cSoc[i] = accept(sSoc, NULL, NULL);//�������ڽ��ܿͻ�������ʱ���Ի�ȡ�ͻ��˵�Э���
		if (SOCKET_ERROR == cSoc[i])
		{
			cout << "������������\n\t\t--------->" << GetLastError();
			closesocket(sSoc);
			WSACleanup(); //����Э��汾��Ϣ
			return -3;
		}
		cout << i << "�ſͻ���" << cSoc[i] << "���ӷ�����!\n";
		num++;
		CreateThread(NULL, NULL,
			(LPTHREAD_START_ROUTINE)Conmunication, (LPVOID)i,
			NULL, NULL);
	}
	

	cout << endl;
	system("pause");
	return 0;
}