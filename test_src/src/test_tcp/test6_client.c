#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h> // AF_INET 외부 네트워크 도메인
#include<sys/types.h>
#include<sys/socket.h>

#define MAXLINE 511

int main(int argc,char *argv[]){
    int cli_sock;
    struct sockaddr_in serv_addr;
    int datalen;
    pid_t pid;

    char buf[MAXLINE+1];
    int nbytes;

    if(argc != 3){
        printf("Usage : %s <IP> <Port> \n", argv[0]);
        exit(0);
    }

    cli_sock = socket(PF_INET, SOCK_STREAM, 0); //cli_sock을 소켓 파일 서술자로 만듭니다.

    if(cli_sock == -1){
        perror("socket() error\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //ip와
    serv_addr.sin_port = htons(atoi(argv[2])); //port의 입력

    if(connect(cli_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){ //서버에게 접근 시도
        perror("connect() error\n");
        exit(0);
    }

    if((pid=fork()) ==1){ //다중 프로세스를 위한 fork함수. 자식 프로세스 생성
        perror("fork() error\n");
        exit(0);
    }
    else if(pid == 0) { //자식 프로세스 부분. stdin로 사용자가 입력한 문자를 buf에 저장하여 소켓에다
        while(1){        //write 시스템콜을 이용해 server에게 문자를 보낸다. exit 입력시 종료
            fgets(buf,sizeof(buf),stdin);
            nbytes = strlen(buf);
            write(cli_sock,buf,MAXLINE);
            if((strncmp,"exit",4) == 0){
                puts("Good Bye.");
                exit(0);
            }
        }
        exit(0);

    }
    else if(pid>0){ //부모프로세스. server가 보낸 문자열을 받아 출력한다.
        while(1){    //역시 exit 를 받을시 종료
            if(nbytes = read(cli_sock,buf,MAXLINE) <0){
                perror("read() error\n");
                exit(0);
            }
            printf("%s",buf);
            if(strncmp(buf,"exit",4) == 0)
                exit(0);
            }
    }

    close(cli_sock);
    return 0;
}
