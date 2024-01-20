
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<fcntl.h>
using namespace std;

char** tokenize(char*str,const char* tok,int ascii)
{
 int token_count=0;
 for(int i=0;str[i]!='\0';i++)
 {
   if(str[i]==ascii)
     token_count++;
 }
 char**args=new char*[token_count+2];        
 //using strtok() which splits the the string in two parts
 char * token = strtok(str, tok); // Extract the first token
 //looping through the string to completely tokenize it
 int i=0;
   while( token != NULL ) 
   {
      args[i]=token;         //stroring all tokens in args
      i++; 
      token = strtok(NULL, tok); //NULL parameter tells strtok() to keep tokeninzing the previously passed str
   }
     args[token_count+1]=NULL;
     return args;
   
}

bool isEqual(char str1[],char str2[])
{
 for(int i=0;str1[i]!='\0' && str2[i]!='\0';i++)
 {
   if(str1[i]!=str2[i])
      {return false;}
   
 }
 return true;
}

int Execute_Command(char * command,int* fd,int* fd2,int input,int output,int redirection,int re_fd)   //-Return :1 error, 0 exit
{                                                                  //input==0(terminal) else pipe
                                                                  //output=0(terminal) else pipe 
 char exit[50]="exit\0";

  if(isEqual(command,exit))
     {
      return 0;
     }
   char**args=tokenize(command," ",32); //space and its ascii
   pid_t ret_value;
   ret_value=fork();
   if(input==1)
     close(fd[1]);
    
    
    
   if(ret_value<0)
   {
     printf("Failure in fork()");
     return -1;
   }
   else if(ret_value==0)
   {//child
     if(output==1)
        {
            dup2(fd[1],STDOUT_FILENO);//send output to pipe  
            close(fd[1]);
        }
      if(output==2)
      {
         dup2(fd2[1],STDOUT_FILENO);
      }      
     if (input==1)
      {
            dup2(fd[0],STDIN_FILENO);
            close(fd[0]);
      }
      if(redirection==1)
      {
        dup2(re_fd,STDOUT_FILENO);
        close(re_fd);
      }
      if(redirection==2)
      {
        dup2(re_fd,STDIN_FILENO);
        close(re_fd);
      }
     
     if(execvp(args[0],args)==-1)
     {
        cout<<"Invalid command\n";
        return -1;
     }
     
   
   }
   else
   {//parent proccess
     cout<<endl;
      pid_t p;
      int status;
      p=wait(&status);
      cout<<endl;
      delete[] args;
   }
 
 
 return 1;

}

int n_piped_commands(char *command)
{
    int piped_command=0;
    for(int i=0;command[i]!='\0';i++)
    {
        if(command[i]=='|')
            piped_command++;
    }
    if(piped_command>=1)
        piped_command++;
    return piped_command;

}

int redirection(char* command)         //0= none, 1=> and 2=<
{
  for(int i=0;i< strlen(command);i++)
  {
    if(command[i]=='>')
      return 1;
    else if (command[i]=='<')
      return 2;  
  }
  return 0;
}

int main()
{
    char command[100];
    char buff[1024];
    int fd[2];
    int fd2[2];
    int r;
    int r_fd;
    char** args2;

    while(1)
    {
        
        printf("Enter Shell Command: ");
        cin.getline(command,50);
        
        int piped_commands=0;
        piped_commands=n_piped_commands(command);   
        char** args=tokenize(command,"|",124);
        
        if(piped_commands==0)
        { 
          r= redirection(args[0]);
          
          if(r!=0)
          {
            if(r==1)
            {
              args2=tokenize(args[0],">",62);
              strcpy(args[0],args2[0]);
              r_fd=open(args2[1],O_CREAT | O_TRUNC | O_WRONLY);
            }
            else
            {
              args2=tokenize(args[0],"<",60);
              strcpy(args[0],args2[0]);
              r_fd=open(args2[1],O_RDONLY);
            }
            
            
          }  
          if(Execute_Command(args[0],NULL,NULL,0,0,r,r_fd)<=0)
              return 0;
            
        }
        int output=0,input=0;
        
        if(piped_commands>0)
          {
            output=1;  //send output to pipe
            if(pipe(fd)==-1)
            {
              cout<<"Cannot create pipe";
              return 0;
            }
          }
        for(int i=0;i<piped_commands;i++)
        {
            if(piped_commands !=2 && i!=0)
            { 
              if(i%2!=0)
                read(fd[0],buff,sizeof(char)*70);
              else 
                read(fd2[0],buff,sizeof(char)*70);
              if(pipe(fd)==-1)
              {
                  cout<<"Cannot create pipe";
                  return 0;
              }
              write(fd[1],buff,sizeof(char)*70);
              if(pipe(fd2)==-1)
              {
                cout<<"Cannot create pipe";
                return 0;
              }
              output=2; // print output in second pipe
            }
            if(i==piped_commands-1)
              output=0; //change output from pipe to terminal   
            r= redirection(args[i]);
            if(r!=0)
            {
              if(r==1)
              {
                args2=tokenize(args[i],">",62);
                strcpy(args[i],args2[0]);
                r_fd=open(args2[1],O_CREAT | O_TRUNC | O_RDWR);
              }
              else
              {
                args2=tokenize(args[i],"<",60);
                strcpy(args[i],args2[0]);
                r_fd=open(args2[1],O_RDONLY);
              }
            
              
            }  
            if(Execute_Command(args[i],fd,fd2,input,output,r,r_fd)<=0)
                return 0;
            
            input=1;//take input from pipe
               
            
        }
        
        
        
    }
    close(fd[0]);
}





