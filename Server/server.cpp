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
	int ret = WSAStartup(MAKEWORD(2,2),&wsaData);
	//if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
	//	cout << "请求协议版本不是2.2\n";
	//	WSACleanup(); //清理协议版本信息
	//	return -1;
	//}
	if (0 == ret)
	{
		cout << "请求协议版本是2.2\n";
	}
	
	//2、创建socket
	SOCKET sSoc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (SOCKET_ERROR == sSoc)
	{
		cout << "创建socket失败,原因：\n\t\t--->" << GetLastError() << endl;
		return -2;
	}
	cout << "创建socket成功!\n";
	//3、确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);//本地ip地址
	addr.sin_port = htons(10086); //小端转大端  端口号在10000左右
	addr.sin_family = AF_INET;//和socket函数第一个参数保持一致
	//4、绑定
	int r = bind(sSoc, (sockaddr*)&addr, sizeof addr);
	if (-1 == r)
	{

		cout << "绑定失败,原因：\n\t\t--->" << GetLastError() << endl;
		closesocket(sSoc);
		WSACleanup(); //清理协议版本信息
		return -2;
	}
	cout << "绑定成功\n";
	//5、监听
	r = listen(sSoc,10);
	if (-1 == r)
	{
		cout << "监听失败,原因：\n\t\t--->" << GetLastError() << endl;
		closesocket(sSoc);
		WSACleanup(); //清理协议版本信息
		return -2;
	}
	cout << "监听成功\n";
	//6.等待客户端连接
	SOCKET cSoc = accept(sSoc, NULL, NULL);//服务器在接受客户端连接时可以获取客户端的协议簇
	if (SOCKET_ERROR == cSoc)
	{
		cout << "服务器崩溃：\n\t\t--------->" << GetLastError();
		closesocket(sSoc);
		WSACleanup(); //清理协议版本信息
		return -3;
	}
	cout << "有客户端连接服务器!\n";
	

	//7、通信
	char buff[256];
	while (1)
	{
		r = recv(cSoc, buff, 255, NULL);
		if (r > 0)
		{
			buff[r] = 0;  //'\0'
			cout << buff << endl;
		}
		
	}

	////8、断开连接
	closesocket(sSoc);

	////9、清理协议版本信息
	WSACleanup(); //清理协议版本信息
	cout << endl;
	system("pause");
	return 0;
}