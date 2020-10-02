#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <math.h>

#define QLEN 5
#define WSVERS MAKEWORD(2, 0)
#define BUFLEN 2000
#pragma comment(lib,"ws2_32.lib")

SOCKET msock, ssock;
SOCKET  sockets[100] = {NULL};

int cc;
time_t now;
char pts[64];
char buf[2000];
char *input;
HANDLE hThread1,hThread[100] = {NULL};

unsigned int threadID,ThreadID[105],number;
char *name[105];

struct  sockaddr_in fsin;
struct  sockaddr_in Sin;

unsigned int __stdcall Chat(PVOID PM) {

    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y-%m-%d",localtime(&now));

    sprintf(pts,"%s.txt",pts);

    FILE *fp=fopen(pts,"a");;
    if((fp=fopen(pts,"a")) == NULL) {
        printf("ERROR:data file cannot be opened!\n");
        exit(1);
    }

    char buf1[2000];
    char buf2[2000];
    char buf3[2000];
    char buf4[2000];
    char buf15[2000];

    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y,%b,%d %X %a",localtime(&now));

    fprintf(fp,"*-*-*-*-*[%s]*-*-*-*-*\n\n",pts);

    (void) time(&now);
    strftime(pts,sizeof(pts),"%X",localtime(&now));

    sockets[number] = ssock;
    SOCKET  sock = ssock;
    ThreadID[number] = threadID;
    unsigned int threadid = threadID;
    char namei[20];

    (void) recv(sock,namei, BUFLEN, 0);
    name[number]=namei;

    sprintf(buf1,"[我的进程号:%d]\n",threadid);
    sprintf(buf15,"[我的昵称:%s]\n",name[number]);

    (void) send(sock,buf1, sizeof(buf1), 0);
    (void) send(sock,buf15, sizeof(buf15), 0);
    sprintf(buf2,"<%s> %s\n加入了聊天室！\n",namei,pts);

    printf("%s",buf2);
    fprintf(fp,"%s",buf2);

    for(int i=0; i<=number; i++) {
        if(sockets[i] != NULL && sockets[i] != sock) {
            (void) send(sockets[i],buf2, sizeof(buf2), 0);
            printf("(成功将消息发送至<%d>)\n",ThreadID[i]);
        }
    }

    puts("");
    fprintf(fp,"\n");

flag1:
    cc = recv(sock, buf3, BUFLEN, 0);

    if(cc == SOCKET_ERROR || cc == 0) {

        (void) time(&now);
        strftime(pts,sizeof(pts),"%X",localtime(&now));

        sprintf( buf3,"<%s> %s\n离开了聊天室！\n",namei,pts);

        sock = NULL;
        sockets[number] = NULL;
        CloseHandle(hThread[number]);

        printf("%s", buf3);
        fprintf(fp,"%s", buf3);

        for(int i=0; i<=number; i++) {
            if(sockets[i] != NULL && sockets[i] != sock) {
                (void) send(sockets[i], buf3, sizeof(buf3), 0);
                printf("(成功将消息发送至<%d>)\n",ThreadID[i]);
            }
        }

        puts("");
        fprintf(fp,"\n");

    }

    else if(cc > 0) {

        (void) time(&now);
        strftime(pts,sizeof(pts),"%X",localtime(&now));
        sprintf(buf4,"<%s> %s\n%s\n",namei,pts,buf3);

        printf("%s",buf4);
        fprintf(fp,"%s",buf4);

        for(int i=0; i<=number; i++) {
            if(sockets[i] != NULL && sockets[i] != sock) {
                (void) send(sockets[i],buf4, sizeof(buf4), 0);
                printf("(成功将消息发送至<%d>)\n",ThreadID[i]);
            }
        }

        puts("");
        fprintf(fp,"\n");

        goto flag1;
    }
    (void) closesocket(sock);

    return 0;
}

int main(int argc, char *argv[]) {
    int alen;
    WSADATA wsadata;

    char  service[10];

    printf("请输入服务器端口：");
    gets(service);

    //注意：客户端连接服务端，IP为服务端的计算机的IP，端口为上面的service

    WSAStartup(WSVERS, &wsadata);
    msock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&Sin, 0, sizeof(Sin));

    Sin.sin_family = AF_INET;
    Sin.sin_addr.s_addr = INADDR_ANY;
    Sin.sin_port = htons((u_short)atoi(service));
    bind(msock, (struct sockaddr *)&Sin, sizeof(Sin));
    listen(msock, 5);

    system("cls");

    printf("机房聊天工具【服务端】\n");
    printf("服务器端口：[%s]\n",service);

    (void) time(&now);
    strftime(pts,sizeof(pts),"%Y/%m/%d %X",localtime(&now));
    printf("时间:%s\n-----------------------\n",pts);

    number = -1;

    while(1) {

        alen = sizeof(struct sockaddr);
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
        number ++;
        hThread[number] = (HANDLE)_beginthreadex(NULL, 0,Chat,NULL, 0, &threadID);
        //(void) recv(ssock, name[number], BUFLEN, 0);

    }

    (void) closesocket(msock);
    WSACleanup();

    return 0;
}
