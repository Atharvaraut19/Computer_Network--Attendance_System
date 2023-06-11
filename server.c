#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#define SERVER_ADDR "127.0.0.1"
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     FILE *fptr,*fptr2,*fptr3;
     char name_file[12],password_file[12],prn_file[12],name[12],prn[12],password[12],ins[]="admin";

     int sockfd, newsockfd,portno;
     int temp,subject_no,choice,count_login,count_attempt;
     socklen_t clilen;
     bool found=false,loop;
     char buffer[256];
     float attendance[5],percentage,average_percentage,min_percentage,min_attendance,new_attendance;
     struct sockaddr_in serv_addr, cli_addr;
     int n,failed[5]={0},c;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     //build server address structure

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(portno);

     //bind local port number

     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");

     //specify number of concurrent
     //clients to listen for

     listen(sockfd,5);
     while(1){
     printf("\nwaiting for client connection on [IP %s TCP port %d]\n",SERVER_ADDR,portno);

     //Wait for client

     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
     if (newsockfd < 0)
        error("ERROR on accept");
     else
        printf("received connection from host [IP %s TCP port %d] \n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
     bzero(buffer,256);
     bzero(name,12);
     bzero(prn,12);
     bzero(password,12);
     count_login=1;

     //open password file

        fptr=fopen("user_pass.txt","r");
        if(fptr==NULL)
            error("Unable to access user password file");
        else
            printf("user password file opened\n");
         do
         {
        //send and receive data from client
            found=false;
            fseek(fptr,0,SEEK_SET);
            n=read(newsockfd,name,12);
            if (n < 0)
                error("ERROR reading from socket");
            n=read(newsockfd,prn,12);
            if (n < 0)
                error("ERROR reading from socket");
            n=read(newsockfd,password,12);
            if (n < 0)
                error("ERROR reading from socket");
            while (fscanf(fptr,"%s",name_file)==1)
            {
                fscanf(fptr,"%s",prn_file);
                fscanf(fptr,"%s",password_file);
                if(!strcmp(name_file,name) && !strcmp(prn_file,prn)&&!strcmp(password_file,password))
                {
                    found=true;
                    break;
                }

            }
            if(found==false)
            {
                temp=0;
                n=write(newsockfd,&temp,sizeof(int));
                if (n < 0)
                    error("ERROR writing to socket");
            }
            else
            {
                if(strcmp(name,ins)==0)
                {
                    temp=1;
                    n=write(newsockfd,&temp,sizeof(int));
                    if (n < 0)
                        error("ERROR writing to socket");
                }

                else
                {
                    temp=2;
                    n=write(newsockfd,&temp,sizeof(int));
                    if (n < 0)
                        error("ERROR writing to socket");
                }
            }
            count_login++;
            if(count_login==6)
            {
                fclose(fptr);
                close(newsockfd);
                printf("Client disconnected\n");
                goto endloop;

            }
         }while(temp==0);
        fclose(fptr);
        //open student attendance file

        fptr2=fopen("student_attendance.txt","r");
        if(fptr2==NULL)
            error("Unable to access student attendance file");
        else
            printf("student attendance file opened\n");
       
        //for instructor

        if(strcmp(name,ins)==0)
        {
            loop=true;
            while(loop)
            {
                n=read(newsockfd,&choice,sizeof(int));
                if (n < 0)
                    error("ERROR reading from socket");
                switch(choice)
                {
                    case 1:
                        fseek(fptr2,0,SEEK_SET);
                        while (fscanf(fptr2,"%s", name_file)==1)
                        {
                            n=write(newsockfd,name_file,12);
                            if (n < 0)
                                error("ERROR writing to socket");
                            for(int i=0;i<=4;i++)
                            {
                                fscanf(fptr2,"%f",&attendance[i]);
                                n=write(newsockfd,&attendance[i],sizeof(float));
                                if (n < 0)
                                    error("ERROR writing to socket");

                            }
                            percentage=(attendance[0]+attendance[1]+attendance[2]+attendance[3]+attendance[4])/5.0;
                            n=write(newsockfd,&percentage,sizeof(float));
                            if (n < 0)
                                error("ERROR writing to socket");
                        }
                        break;
                    
                    case 2:
                        count_attempt=1;
                        do
                        {
                            fseek(fptr2,0,SEEK_SET);
                            n=read(newsockfd,name,12);
                            if (n < 0)
                                error("ERROR reading from socket");
                            n=read(newsockfd,&subject_no,sizeof(int));
                            if (n < 0)
                                error("ERROR reading from socket");
                            n=read(newsockfd,&new_attendance,sizeof(float));
                            if (n < 0)
                                error("ERROR reading from socket");
                            found=false;
                            while (fscanf(fptr2,"%s" ,name_file)==1)
                            {
                                for(int i=0;i<=4;i++)
                                    fscanf(fptr2,"%f",&attendance[i]);
                                if(!strcmp(name_file,name))
                                {
                                    found=true;
                                    break;
                                }
                            }
                            if(found==false)
                            {
                                temp=0;
                                n=write(newsockfd,&temp,sizeof(int));
                                if (n < 0)
                                    error("ERROR writing to socket");
                            }
                            else
                            {
                                temp=1;
                                n=write(newsockfd,&temp,sizeof(int));
                                if (n < 0)
                                    error("ERROR writing to socket");
                            }
                            count_attempt++;
                            if(count_attempt==6)
                            {
                                fclose(fptr2);
                                close(newsockfd);
                                printf("Client disconnected\n");
                                goto endloop;

                            }
                        }while(temp==0 || (subject_no<0 || subject_no>5) || (new_attendance<0 || new_attendance>100));
                        fptr3=fopen("temp.txt","w+");
                        if(fptr3==NULL)
                            error("Unable to open temporary file");
                        else
                            printf("temporary file opened\n");
                        fseek(fptr2,0,SEEK_SET);
                        while(fscanf(fptr2,"%s" ,name_file)==1)
                        {
                            fprintf(fptr3,"%s",name_file);
                            fprintf(fptr3,"\n");
                            for(int i=0;i<=4;i++)
                            {
                                fscanf(fptr2,"%f",&attendance[i]);
                                if(strcmp(name_file,name)==0 && i==subject_no-1)
                                {
                                    fprintf(fptr3,"%0.2f",new_attendance);
                                    fprintf(fptr3,"\n");
                                }
                                else
                                {
                                    fprintf(fptr3,"%0.2f",attendance[i]);
                                    fprintf(fptr3,"\n");
                                }

                            }
                        }
                        fclose(fptr2);
                        fseek(fptr3,0,SEEK_SET);
                        fptr2=fopen("student_attendance.txt","w");
                        if(fptr2==NULL)
                            error("Unable to open student attendance file for writing\n");
                        while(fscanf(fptr3,"%s" ,name_file)==1)
                        {
                            fprintf(fptr2,"%s",name_file);
                            fprintf(fptr2,"\n");
                            for(int i=0;i<=4;i++)
                            {
                                fscanf(fptr3,"%f",&attendance[i]);
                                fprintf(fptr2,"%0.2f",attendance[i]);
                                fprintf(fptr2,"\n");
                            }
                        }
                        fclose(fptr3);
                        fclose(fptr2);
                        fptr2=fopen("student_attendance.txt","r");
                        if(fptr2==NULL)
                            error("Unable to open student attendance file\n");
                        break;

                    case 3:
                        fclose(fptr2);
                        loop=false;
                        close(newsockfd);
                        printf("Client disconnected\n");
                        break;
                    default:
                        n=write(newsockfd,"Wrong choice entered",12);
                        if (n < 0)
                                error("ERROR writing to socket");
                        break;
                }
            }
        }

        //For students

        else
        {
            fseek(fptr2,0,SEEK_SET);
            while (fscanf(fptr2,"%s" ,name_file)==1)
            {
                for(int i=0;i<=4;i++)
                    fscanf(fptr2,"%f",&attendance[i]);
                if(!strcmp(name_file,name))
                    break;
            }
            percentage=(attendance[0]+attendance[1]+attendance[2]+attendance[3]+attendance[4])/5.0;
            min_attendance=attendance[0];
            for(int i=1;i<=4;i++)
            {
                if(attendance[i]<min_attendance)
                    min_attendance=attendance[i];
            }
            loop=true;
            while(loop)
            {
                n=read(newsockfd,&choice,sizeof(int));
                if (n < 0)
                    error("ERROR reading from socket");
                switch(choice)
                {
                    case 1:
                        for(int i=0;i<=4;i++)
                        {
                            n=write(newsockfd,&attendance[i],sizeof(float));
                            if (n < 0)
                                error("ERROR writing to socket");
                        }
                        break;
                    case 2:
                    
                    n=write(newsockfd,&min_attendance,sizeof(float));
                        if (n < 0)
                                error("ERROR writing to socket");
                        for(int i=0;i<=4;i++)
                        {
                            if(attendance[i]==min_attendance)
                            {
                                n=write(newsockfd,&i,sizeof(int));
                                if (n < 0)
                                    error("ERROR writing to socket");
                            }
                        }
                        temp=-1;
                        n=write(newsockfd,&temp,sizeof(int));
                        if (n < 0)
                                error("ERROR writing to socket");

                        break;

                    case 3:
                        fclose(fptr2);
                        loop=false;
                        close(newsockfd);
                        printf("Client disconnected\n");
                        break;
                    default:
                        n=write(newsockfd,"Wrong choice entered",12);
                        if (n < 0)
                                error("ERROR writing to socket");
                        break;
                }
            }

        }
    endloop: printf(" ");
     }
     return 0;
}

