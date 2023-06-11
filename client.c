#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

void error(const char *msg)
{
    perror(msg);
    printf("closing connection with the server\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n,c=0,temp,subject_no,choice,count_login,count_attempt;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    float percentage,attendance,new_attendance;
    char buffer[256],prn[12],name[12],password[12];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    // create stream socket

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //build server address structure

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    //connect to server

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    else{printf("connected to server successfully\n");}

    //Enter username and password

    bzero(buffer,256);
    bzero(name,12);
    bzero(prn,12);
    bzero(password,12);
    count_login=1;
    printf("\n                                                       \n");
    printf("\n             Welcome To VIT Attendance Portal          ");
    printf("\n                     Login Below                       \n");
    printf("\n                                                       \n");
    do
    {
        

        printf("\nEnter Username : ");
        scanf("%s",name);
        n=write(sockfd,name,12);
        if (n < 0)
         error("ERROR writing to socket");

        printf("\nEnter PRN Number : ");
        scanf("%s",prn);
        n=write(sockfd,prn,12);
        if (n < 0)
         error("ERROR writing to socket");

        printf("Enter password : ");
        scanf("%s",password);
        n=write(sockfd,password,12);
        if (n < 0)
         error("ERROR writing to socket");
        n=read(sockfd,&c,sizeof(int));
        if (n < 0)
         error("ERROR reading from socket");
        if(c==0)
            printf("\nWrong Username or Password:Attempts remaining=%d\n",5-count_login);
        count_login++;
        if(count_login==6)
            error("Too many wrong attempts");
    }while(c==0);


    //Menu for instructor

    if(c==1)
    {
        printf("\n******* Welcome Instructor TO VIREP *******\n");
        while(true)
        {
            printf("\nEnter 1 To check Attendance of each student\n");
            printf("Enter 2 to update attendance of a student\n");
            printf("Enter 3 to quit\n");
            scanf("%d",&choice);
            n=write(sockfd,&choice,sizeof(int));
            if (n < 0)
                error("ERROR writing to socket");
            switch(choice)
            {
                case 1:
                    for(int i=1;i<=12;i++)
                    {
                        n=read(sockfd,name,12);
                        if (n < 0)
                            error("ERROR reading from socket");
                        printf("\n%s",name);
                        printf("\n");
                        for(int j=1;j<=5;j++)
                        {
                            n=read(sockfd,&attendance,sizeof(float));
                            if (n < 0)
                                error("ERROR reading from socket");
                            printf("Attendance in subject %d = %0.2f",j,attendance);
                            printf("\n");
                        }
                        n=read(sockfd,&percentage,sizeof(float));
                        if (n < 0)
                            error("ERROR reading from socket");
                        printf("Student Average Attendance = %0.2f",percentage);
                        printf("\n");
                    }
                    break;

                case 2:
                    count_attempt=1;
                    do
                    {
                        printf("\nEnter name of student:");
                        scanf("%s",name);
                        n=write(sockfd,name,12);
                        if (n < 0)
                            error("ERROR writing to socket");
                        printf("Enter subject no. and  new attendance\n");
                        scanf("%d " ,&subject_no);
                        scanf("%f" ,&new_attendance);
                        n=write(sockfd,&subject_no,sizeof(int));
                        if (n < 0)
                            error("ERROR writing to socket");
                        n=write(sockfd,&new_attendance,sizeof(float));
                        if (n < 0)
                            error("ERROR writing to socket");
                        n=read(sockfd,&temp,sizeof(int));
                        if (n < 0)
                            error("ERROR reading from socket");
                        if(temp==0 || (subject_no<0 || subject_no>5) || (new_attendance<0 || new_attendance>100))
                            printf("\nWrong credentials entered:Attempts remaining=%d\n",5-count_attempt);
                        count_attempt++;
                        if(count_attempt==6)
                            error("Too many wrong attempts");
                    }while(temp==0 || (subject_no<0 || subject_no>5) || (new_attendance<0 || new_attendance>100));
                    printf("Attendance of %s in subject%d updated\n",name,subject_no);
                    break;

                case 3:
                    error("Exiting Client");
                default:
                    bzero(buffer,256);
                    n=read(sockfd,buffer,255);
                    if (n < 0)
                        error("ERROR reading from socket");
                    printf("%s\n",buffer);
                    break;
            }
        }

    }

    //Menu for students

    else if(c==2)
    {
        printf("\n******* Welcome TO VIREP  %s Hope Your Doing Good *******\n",name);
        printf("\n     ******* Please Check Your Attendance Below *******\n");
        while(true)
        {
            printf("\nEnter 1 to check your attendance for each subject\n");
            printf("Enter 2 to check defaulter in any subject\n");
            printf("Enter 3 to quit\n");
            scanf("%d",&choice);
            n=write(sockfd,&choice,sizeof(int));
            if (n < 0)
                error("ERROR writing to socket");
            switch(choice)
            {
                case 1:
                    for(int i=1;i<=5;i++)
                    {
                        n=read(sockfd,&attendance,sizeof(float));
                        if (n < 0)
                            error("ERROR reading from socket");
                        printf("Attendance in Subject %d = %0.2f",i,attendance);
                        printf("\n");
                    }
                    break;
                    
                case 2:
                
                    n=read(sockfd,&attendance,sizeof(float));
                    if (n < 0)
                        error("ERROR reading from socket");
                    if(attendance<60)
                        printf("Attendance = %0.2f in subject:  ",attendance);
                    else
                        printf("0 Deafaulter");
                    while(1)
                    {
                        n=read(sockfd,&temp,sizeof(int));
                        if (n < 0)
                            error("ERROR reading from socket");
                        if(temp==-1)
                            break;
                        if(attendance<60)
                        printf("Subject%d ",temp+1);
                        else
                        printf(" ");
                    }
                    printf("\n");
                    
                    break;
                case 3:
                    error("Exiting client");
                default:
                    bzero(buffer,256);
                    n=read(sockfd,buffer,255);
                    if (n < 0)
                        error("ERROR reading from socket");
                    printf("%s",buffer);
                    printf("\n");
                    break;
            }
        }
    }
    return 0;
}
