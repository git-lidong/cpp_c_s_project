#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <Windows.h>

using namespace std;

int main()
{
	//1、确定协议版本
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 == ret)
	{
		cout << "请求协议版本是2.2\n";
	}

	//2、创建socket
	SOCKET cSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == cSoc)
	{
		cout << "创建socket失败,原因：\n\t\t--->" << GetLastError() << endl;
		return -2;
	}
	cout << "创建socket成功!\n";
	//3、获取服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//本地ip地址
	addr.sin_port = htons(10086); //小端转大端  端口号在10000左右
	addr.sin_family = AF_INET;//和socket函数第一个参数保持一致
	
	//4.连接服务器
	int r = connect(cSoc, (sockaddr*)&addr, sizeof addr);
	if(-1 == r)
	{
		cout << "连接服务失败：\n\t\t--------->" << GetLastError();
		closesocket(cSoc);
		WSACleanup(); //清理协议版本信息
		return -3;
	}
	cout << "连接服务器成功!\n";

	//5、通信
	char buff[256];
	while (1)
	{
		memset(buff, 0, 256);
		cout << "发送什么信息: \n";
		cin >> buff;
		send(cSoc, buff,strlen(buff),NULL);
	}
	cout << endl;
	system("pause");
	return 0;
	}