#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <math.h>

#define BUFLEN 2000
#define WSVERS MAKEWORD(2, 0)
#pragma comment(lib,"ws2_32.lib")

SOCKET    sock,sockets[100] = {NULL};
char    *packet = NULL;
char *pts,*input;
HANDLE hThread;
unsigned threadID;

unsigned int __stdcall Chat(PVOID PM ) {
    time_t    now;
    (void) time(&now);
    pts = ctime(&now);
    char buf[2000];

    while(1) {
        int cc = recv(sock, buf, BUFLEN, 0);
        if(cc == SOCKET_ERROR|| cc == 0) {
            printf("错误:%d.----",GetLastError());
            printf("与服务器失去连接！\n");
            CloseHandle(hThread);
            (void)closesocket(sock);
            break;
        } else if(cc > 0) {
            printf("%s\n",buf);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {

    time_t    now;
    (void) time(&now);
    pts = ctime(&now);

    char host[50],service[10];

    printf("请输入服务器IP地址：");
    gets(host);//IP为服务端的计算机的IP 

    printf("请输入服务器端口：");
    gets(service);//端口请自行更改 

    struct  sockaddr_in sin;
    WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons((u_short)atoi(service));
    sin.sin_addr.s_addr = inet_addr(host);

    sock = socket(PF_INET, SOCK_STREAM,0);

    system("cls");

    printf("您已连接至：\nIp：[%s]\n端口：[%s]\n\n",host,service);

    char namei[20];
    printf("请输入您的昵称：");
    gets(namei);

    connect(sock, (struct sockaddr *)&sin, sizeof(sin));

    (void) send(sock,namei, sizeof(namei), 0);

    system("cls");

    printf("*-----------------------------*\n");
    printf("|         机房聊天工具        |\n");
    printf("|          【客户端】         |\n");
    printf("|   提示：输入\'help\'查看指令  |\n");
    printf("*-----------------------------*\n");

    hThread = (HANDLE)_beginthreadex(NULL, 0,Chat, NULL, 0, &threadID);

    while(1) {

        char buf1[2000];
        gets(buf1);

        if(!strcmp(buf1 ,"help")){
            puts("");
            puts("---------------------");
            puts("机房聊天工具 命令大全");
            puts("help 查看命令");
            puts("exit 退出聊天室");
            puts("---------------------");
            puts("");
        }

        if(!strcmp(buf1 ,"exit"))
            goto end;

        (void) send(sock,buf1, sizeof(buf1), 0);

        (void) time(&now);
        pts = ctime(&now);

    }

end:
    CloseHandle(hThread);
    closesocket(sock);
    WSACleanup();

    printf("按任意键以继续...\n");
    getchar();
    return 0;
}
