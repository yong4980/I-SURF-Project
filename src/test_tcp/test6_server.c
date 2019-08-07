#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h> //AF_INET 외부 내트워크 도메인
#include<sys/types.h>
#include<sys/socket.h>

#define MAXLINE 511

int main(int argc,char* argv[]){
    int serv_sock;
    int conn_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in conn_addr;

    int addrlen, datalen;

    char buf[MAXLINE +1];
    int nbytes;

    pid_t pid;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(0);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP); // serv_sock를 소켓 파일 서술자로 만든다.
    if(serv_sock == -1){
        perror("socket() error\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
        perror("bind() error\n"); //bind(). 소켓 파일 서술자에게 ip, port 주소를 가진 구조체를 넣어준다.
        exit(0);
    }

    if(listen(serv_sock,1) == -1){ // 소켓 대기열 생성
        perror("listen() error\n");
        exit(0);
    }

    addrlen = sizeof(conn_sock);
    conn_sock = accept(serv_sock, (struct sockaddr*)&conn_addr, &addrlen); //전화오기를 기다림.

    if(conn_sock == -1){
        perror("accept() error\n");
        exit(0);
    }

    if((pid=fork())==-1){close(conn_sock);perror("fork() error");exit(0);}
    else if (pid == 0){ //자식프로세스이다.

        while(1){
            fgets(buf,sizeof(buf),stdin);
            nbytes = strlen(buf);
            write(conn_sock,buf,MAXLINE);
            if((strncmp,"exit",4) == 0){
                puts("Good Bye.");
                exit(0);
            } //자식프로세스는 stdin으로 사용자가 입력한 문자를 buf에 저장하여 소켓에다 write 시스템콜을 이용해
        }     //client에게 문자열을 보내고, exit를 입력시 종료하게 한다.
        exit(0);
    }

    else if(pid>0){ //부모프로세스이다.
        while(1){
            if((nbytes = read(conn_sock,buf,MAXLINE)) <0){
                perror("read() error\n");
                exit(0);
            }
            printf("%s",buf);
            if(strncmp(buf,"exit",4) == 0)
                exit(0);
            } //부모프로세스는 client가 소켓에 보낸 문자열을 read로 읽어 저장한다.
        }     //역시 exit 가 날라왔을시에는 종료.
    return 0;
}
