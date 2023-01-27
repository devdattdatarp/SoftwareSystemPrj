// This is the server program

/*
    1. socket() -> create an endpoint for the communication
        sockfetfd = socket(AF_INET, SOCK_STREAM, 0);
    2. bind() -> bind a name to a socket
        bindStatus = bind(socketfd, &socketaddr, sizeof(socketfd));
    3. listen() -> listen for connections on a socket
        listenStatus = listen(socketfd, 2); // backlog -> 2
    4. accept() -> accept a connection on a socket
        connectionfd = accept(socketfd, &socketaddr, sizeof(socketaddr), 0);
    5. Start communicating -> `write` to and `read` from connectionfd
*/

#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept`
#include <netinet/ip.h> // Import for `struct sockaddr_in`, `htons`
#include <stdio.h>      // Import for `perror` & `printf`
#include <unistd.h>     // Import for `_exit`, `read`, `write`, `fork`
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


//User Structure for file------------------------------------------------------------------------------------------
//Normal account
struct normalUser{
int deleteflag;
char userName[10];
char pin[5];
int phoneNumber[10];
int ammount;
}normal_customerServer[1];

//Joint account
struct jointUser{
int deleteflag;
char userName[10];
char pin[5];
int phoneNumber[10];
char jointUser[10];
int ammount;
}joint_customerServer[1];

//Admin credentials
struct Admin{
char userName[10];
char pin[5];
}adminServer[1];


// Socket discriptors................................................................................
int socketFileDescriptor, connectionFileDescriptor;
ssize_t readBytes, writeBytes;

//Required functions.........................................................
void mainMenuServer();
void storeUser();
int loginServer();
void normalbanking(int bankingfd);
void jointbanking();

void updateNormalUserdata(struct normalUser *normal_customerServer);
void updateJointUserdata(struct jointUser *joint_customerServer);
//----------------------------------------------------------------------------
//admin functions---------------------------------
void adminbanking();
void modifyUser();
void deleteUser();
void viewUser();
//Locking requirements.....................................
static struct flock lock;

    
//--------------------------------------------------

void main()
{
   
    
    int bindStatus;
    int listenStatus;
    int clientSize;

    struct sockaddr_in address, client;

    
    char dataFromClient[100];

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Server side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8081);

    bindStatus = bind(socketFileDescriptor, (struct sockaddr *)&address, sizeof(address));
    if (bindStatus == -1)
    {
        perror("Error while binding name to socket!");
        _exit(0);
    }
    printf("Binding to socket was successful!\n");

    listenStatus = listen(socketFileDescriptor, 7);
    if (listenStatus == -1)
    {
        perror("Error while trying to listen for connections!");
        _exit(0);
    }
    printf("Now listening for connections on a socket!\n");
    while (1)
    {
        clientSize = (int)sizeof(client);
        connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&client, &clientSize);
        if (connectionFileDescriptor == -1)
            perror("Error while accepting a connection!");
        else
        {

            // ========================= Server - Client communication =================

            if (fork() == 0)
            {
                // Child
                writeBytes = write(connectionFileDescriptor, "I'm the server!", 15);
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");
                
                
                

                
                mainMenuServer();

                
            }
            else
            {
                // Parent
                close(connectionFileDescriptor);
            }
        }

        // =======================================================================
    }

    close(socketFileDescriptor);
}


void mainMenuServer(){  
    int mainMenucount=1;
    
    while(mainMenucount>0){
        readBytes = read(connectionFileDescriptor,&mainMenucount,sizeof(mainMenucount));
                if (readBytes == -1)
                    perror("Error while reading mainmenu_option from network via socket!");
                else
                    printf("client has chosen :  %d \n",mainMenucount);
    int mainmenu_option;
     readBytes = read(connectionFileDescriptor,&mainmenu_option,sizeof(mainmenu_option));
                if (readBytes == -1)
                    perror("Error while reading mainmenu_option from network via socket!");
                else
                    printf("client has chosen :  %d \n",mainmenu_option);

    switch (mainmenu_option)
       { case 0://User choose to Exit..
               printf("\n \t Shutting Down ...\n");
               exit(0);
        case 1:
           //User Choose to Register...
          printf("\n \t User Choose to Register... ...\n");
          storeUser(); 
           break;
     
       case 2:
           //User Choose to Login...
           printf("\n \t User Choose to Login... ...\n");
           loginServer();
           break;

       default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           //menu_input_count--;
           break;
     }
    }
     printf("\n Banking operation done...\n");
     exit(0);
     
     //return 0;

}


void storeUser(){
    int userType;
    int fd;
    readBytes = read(connectionFileDescriptor,&userType,sizeof(userType));
                if (readBytes == -1)
                    perror("Error while reading mainmenu_option from network via socket!");
                else
                    printf("client has chosen :  %d \n", userType);

    if(userType ==1)
{
    printf("\n User has chosen to create Normal account...\n");
    readBytes = read(connectionFileDescriptor,normal_customerServer,sizeof(normal_customerServer));
                if (readBytes == -1)
                    perror("Error while reading registerUser normal account from network via socket!");
                else
                    printf("client data is retrieved in Normal_customerServer \n");
                
    

    

    fd = open(normal_customerServer->userName,O_CREAT | O_RDWR | O_EXCL,0744);

    perror("\nAccount creation\n");
    //locking.......
            lock.l_type = F_RDLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;
            lock.l_pid = getpid();
    fcntl(fd, F_SETLKW, &lock);

    write(fd,normal_customerServer,sizeof(normal_customerServer));
    //return fd;
}
    else if(userType == 2)
{
    printf("\n User has chosen to create Joint account...\n");
    readBytes = read(connectionFileDescriptor,joint_customerServer,sizeof(joint_customerServer));
                if (readBytes == -1)
                    perror("Error while reading registerUser joint account from network via socket!");
                else
                    printf("client data is retrieved in Joint_customerServer \n");
    fd = open(joint_customerServer->userName,O_CREAT | O_RDWR | O_EXCL,0744);
    perror("\nAccount creation\n");
    write(fd,joint_customerServer,sizeof(joint_customerServer));
}
    else{
    exit(0);
}

}

int loginServer(){
    int loginStatus;
    int loginchoiceServer;
   readBytes = read(connectionFileDescriptor,&loginchoiceServer,sizeof(loginchoiceServer));
                if (readBytes == -1)
                    perror("Error while reading loginchoiceServer from network via socket!");
                else
                    printf("client has chosen :  %d \n", loginchoiceServer);

   if(loginchoiceServer == 1){
       //Normal account login
        char loginName[10];
        char normalpin[5];
        int loginfd;
        int normalLoginStatusServer;
       printf("\n User has chose to login as normal account\n");
       readBytes = read(connectionFileDescriptor,loginName,sizeof(loginName));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", loginName);
                
        loginfd = open(loginName,O_RDWR);
        perror("Accessing Data");
  
         if(loginfd == -1)
        {
            printf("\n Incorrect Username\n");
            normalLoginStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&normalLoginStatusServer,sizeof(normalLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
            
           
            
            return 0;

        }
        else{
            normalLoginStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&normalLoginStatusServer,sizeof(normalLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");

            printf("\n Correct User Id file name \n");

            //locking.......
            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;
            lock.l_pid = getpid();
            
            int lockingStatus = 1;
                struct flock savelock;
                savelock=lock;
                fcntl(loginfd,F_GETLK,&lock);
                if(lock.l_type == F_WRLCK || lock.l_type == F_RDLCK)
                {
                    printf("\n File is Locked");
                    lockingStatus=2;
                    writeBytes=write(connectionFileDescriptor,&lockingStatus,sizeof(lockingStatus));
                    return 0;
                }
                 writeBytes=write(connectionFileDescriptor,&lockingStatus,sizeof(lockingStatus));
                    

            lseek(loginfd,0,SEEK_SET);
            read(loginfd,normal_customerServer,sizeof(struct normalUser));
            printf(" \n %s \n",normal_customerServer->pin);

            

            //break;
            
            int pin_count=2;
            while(pin_count > 0){
            readBytes = read(connectionFileDescriptor,&normalpin,5);
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n",normalpin);
            
            if(strcmp(normal_customerServer->pin,normalpin)==0)
            {
                
                loginStatus=1;
                writeBytes = write(connectionFileDescriptor,&loginStatus,sizeof(loginStatus));
                if (writeBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", loginStatus);
                break;

            }
            else{
            loginStatus = 2;
            writeBytes = write(connectionFileDescriptor,&loginStatus,sizeof(loginStatus));
                if (writeBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", loginStatus);
                
            pin_count--;
            continue;

            }
            }
            if(pin_count == 0){
                return 0;
            }

            
            writeBytes = write(connectionFileDescriptor,normal_customerServer,sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("Error while writing login pin  to network via socket!");
                else
                    printf("loginpin sent to client!\n");
                
                printf("\nfile locking");
                
                // file locking...... 
                

                //fcntl(loginfd, F_SETLK, &lock);
                normalbanking(loginfd);
                
                //lock.l_type=F_UNLCK;
                //fcntl(loginfd,F_SETLK,&lock);
                

            }        
                 }
   else if(loginchoiceServer == 2){

       //Joint account login
        
        char jointloginName[10];
        char jointpin[5];
        int jloginfd;
        int jointLoginStatusServer;
       printf("\n User has chose to login as normal account\n");
       readBytes = read(connectionFileDescriptor,jointloginName,sizeof(jointloginName));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", jointloginName);
                
        jloginfd = open(jointloginName,O_RDWR);
        perror("Accessing Data");
  
         if(jloginfd == -1)
        {
            printf("\n Incorrect Username\n");
            jointLoginStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&jointLoginStatusServer,sizeof(jointLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
            
           
            
            return 0;

        }
        else{
            jointLoginStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&jointLoginStatusServer,sizeof(jointLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");

            printf("\n Correct User Id file name \n");

            //locking.......
            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;
            lock.l_pid = getpid();
            
            int jointlockingStatus = 1;
                struct flock savelock;
                savelock=lock;
                fcntl(jloginfd,F_GETLK,&lock);
                if(lock.l_type == F_WRLCK || lock.l_type == F_RDLCK)
                {
                    printf("\n File is Locked");
                    jointlockingStatus=2;
                    writeBytes=write(connectionFileDescriptor,&jointlockingStatus,sizeof(jointlockingStatus));
                    return 0;
                }
                 writeBytes=write(connectionFileDescriptor,&jointlockingStatus,sizeof(jointlockingStatus));
                    
            fcntl(jloginfd, F_SETLKW, &lock);
            lseek(jloginfd,0,SEEK_SET);
            read(jloginfd,joint_customerServer,sizeof(struct jointUser));
            printf(" \n %s \n",joint_customerServer->pin);

            

            //break;
            
            int jointpin_count=2;
            int jointloginStatus;
            while(jointpin_count > 0){
            readBytes = read(connectionFileDescriptor,&jointpin,5);
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n",jointpin);
            
            if(strcmp(joint_customerServer->pin,jointpin)==0)
            {
                
                jointloginStatus=1;
                writeBytes = write(connectionFileDescriptor,&jointloginStatus,sizeof(loginStatus));
                if (writeBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", jointloginStatus);
                break;

            }
            else{
            jointloginStatus = 2;
            writeBytes = write(connectionFileDescriptor,&jointloginStatus,sizeof(jointloginStatus));
                if (writeBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", jointloginStatus);
                
            jointpin_count--;
            continue;

            }
            }
            if(jointpin_count == 0){
                return 0;
            }

            
            writeBytes = write(connectionFileDescriptor,joint_customerServer,sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("Error while writing login pin  to network via socket!");
                else
                    printf("loginpin sent to client!\n");
                
                printf("\nfile locking");
                
                // file locking...... 
                
            lock.l_type=F_UNLCK;
                fcntl(jloginfd,F_SETLK,&lock);
                
                jointbanking(jloginfd);
                
                
                

            }        
   }
       
   else if(loginchoiceServer == 3){
   //admin account login
        char adminloginName[10];
        char adminloginPin[4];
        int adminloginfd;
        int adminLoginStatusServer;
       printf("\n User has chose to login as normal account\n");
       readBytes = read(connectionFileDescriptor,adminloginName,sizeof(adminloginName));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s",adminloginName);
                
        adminloginfd = open(adminloginName,O_RDWR);
        perror("Accessing Data");
  
         if(adminloginfd == -1)
        {
            printf("\n Incorrect Username\n");
            adminLoginStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&adminLoginStatusServer,sizeof(adminLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
            
           
            
            return 0;

        }
        else{
            adminLoginStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&adminLoginStatusServer,sizeof(adminLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");

            printf("\n Correct User Id file name \n");

            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;
            lock.l_pid = getpid();
            fcntl(adminloginfd, F_SETLKW, &lock);
            
            read(adminloginfd,adminServer,sizeof(struct Admin));
            printf(" \n %s \n",adminServer->pin);

            
            printf(" \n loginpin copied %s \n",adminServer->pin);

            writeBytes = write(connectionFileDescriptor,adminServer,sizeof(struct Admin));
                if (writeBytes == -1)
                    perror("Error while writing login pin  to network via socket!");
                else
                    printf("loginpin sent to client!\n");

            //break;
            int adminloginStatus;
            readBytes = read(connectionFileDescriptor,&adminloginStatus,sizeof(adminloginStatus));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %ls \n", &adminloginStatus);

                
                adminbanking();
                lock.l_type=F_UNLCK;
                fcntl(adminloginfd,F_SETLK,&lock);

            }        
                 }
    
    

   
   }

void adminbanking(){
    int admin_menu_input;
        int bankingfd;
        readBytes = read(connectionFileDescriptor,&admin_menu_input,sizeof(admin_menu_input));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", admin_menu_input);
        switch (admin_menu_input)
        {
        case 0://User choose to Exit..
              printf("\n \t Shutting Down ...\n");
                mainMenuServer();
        case 1:
           //admin add function...
           storeUser();
           break;
     
        case 2:
           //admin modify function
           modifyUser();
           break;
        case 3:
            //admin view account
            printf("\nView Details\n");
            viewUser();
            break;
        case 4:
            //admin delete function
            deleteUser();
            break;


        default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           mainMenuServer();
     }
        
    
    

}
void modifyUser(){
    int modifyfd;
    int modifyaccType;
    readBytes=read(connectionFileDescriptor,&modifyaccType,sizeof(modifyaccType));
            if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
            else
                printf("Modify choice sent to server!\n");

    if(modifyaccType == 1){
       //Normal account login
        char loginName[10];
        char loginPin[4];
        int loginfd;
        int normalLoginStatusServer;
       printf("\n User has chose to login as normal account\n");
       readBytes = read(connectionFileDescriptor,loginName,sizeof(loginName));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", loginName);
                
        loginfd = open(loginName,O_RDWR);
        perror("Accessing Data");
  
         if(loginfd == -1)
        {
            printf("\n Incorrect Username\n");
            normalLoginStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&normalLoginStatusServer,sizeof(normalLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
            
           
            
            //return 0;

        }
        else{
            normalLoginStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&normalLoginStatusServer,sizeof(normalLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");

            printf("\n Correct User Id file name \n");

            lseek(loginfd,0,SEEK_SET);
            read(loginfd,normal_customerServer,sizeof(struct normalUser));
                
            writeBytes = write(connectionFileDescriptor,normal_customerServer,sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("Error while writing login pin  to network via socket!");
                else
                    printf("loginpin sent to client!\n");

            //break;
            readBytes = read(connectionFileDescriptor,normal_customerServer,sizeof(struct normalUser));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", normal_customerServer->userName);
               
               lseek(loginfd,0,SEEK_SET);
               write(loginfd,normal_customerServer,sizeof(struct normalUser)); 
            }        
     }
   else if(modifyaccType  == 2){

       //Joint account login
        char jloginName[10];
        char jloginPin[4];
        int jloginfd;
        int jointLoginStatusServer;
       printf("\n User has chose to login as joint account\n");
       readBytes = read(connectionFileDescriptor,jloginName,sizeof(jloginName));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", jloginName);
                
        jloginfd = open(jloginName,O_RDWR);
        perror("Accessing Data");
  
         if(jloginfd == -1)
        {
            printf("\n Incorrect Username\n");
            jointLoginStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&jointLoginStatusServer,sizeof(jointLoginStatusServer));
                if(writeBytes == -1)
                    perror("Error while writing jointLoginStatusServer to network via socket!");
                else
                    printf("jointLoginStatusServer sent to client!\n");
            
            //return 0;

        }
        else{
            jointLoginStatusServer=2;
             writeBytes = write(connectionFileDescriptor,&jointLoginStatusServer,sizeof(jointLoginStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("jointLoginStatusServer sent to client!\n");

            printf("\n Correct User Id file name \n");

            lseek(jloginfd,0,SEEK_SET);
            read(jloginfd,joint_customerServer,sizeof(struct jointUser));
            

            writeBytes = write(connectionFileDescriptor,joint_customerServer,sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("Error while writing jlogin pin  to network via socket!");
                else
                    printf("jloginpin sent to client!\n");

            readBytes = read(connectionFileDescriptor,joint_customerServer,sizeof(struct jointUser));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", joint_customerServer->userName);
            
            
                lseek(jloginfd,0,SEEK_SET);
                write(jloginfd,joint_customerServer,sizeof(struct jointUser));

            

            }
   }
            

}
void viewUser(){
int adminviewStatusServer;
int viewTypeserver;
char viewfile[10];
int viewfd;
readBytes = read(connectionFileDescriptor,&viewTypeserver,sizeof(viewTypeserver));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %ls", &viewTypeserver);
       printf("\n User has chose to modify account by admin account\n");
       readBytes = read(connectionFileDescriptor,&viewfile,sizeof(viewfile));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", viewfile);
                
        viewfd = open(viewfile,O_RDWR);
        perror("Accessing Data");
  
         if(viewfd == -1)
        {
            printf("\n Incorrect Username\n");
            adminviewStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&adminviewStatusServer,sizeof(adminviewStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
        }
        else{
            adminviewStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&adminviewStatusServer,sizeof(adminviewStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            if(viewTypeserver==1){

            lseek(viewfd,0,SEEK_SET);
            read(viewfd,normal_customerServer,sizeof(struct normalUser));
            writeBytes = write(connectionFileDescriptor,normal_customerServer,sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
                
            }
            else{
                lseek(viewfd,0,SEEK_SET);
                read(viewfd,joint_customerServer,sizeof(struct jointUser));
                writeBytes = write(connectionFileDescriptor,joint_customerServer,sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
                }

        }
          
}

void deleteUser(){
int adminviewStatusServer;
int viewTypeserver;
char viewfile[10];
int viewfd;
int deleteflgStatus;
readBytes = read(connectionFileDescriptor,&viewTypeserver,sizeof(viewTypeserver));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %ls", &viewTypeserver);
       printf("\n User has chose to modify account by admin account\n");
       readBytes = read(connectionFileDescriptor,&viewfile,sizeof(viewfile));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %s \n", viewfile);
                
        viewfd = open(viewfile,O_RDWR);
        perror("Accessing Data");
  
         if(viewfd == -1)
        {
            printf("\n Incorrect Username\n");
            adminviewStatusServer=-1;
             writeBytes = write(connectionFileDescriptor,&adminviewStatusServer,sizeof(adminviewStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
        }
        else{
            adminviewStatusServer=1;
             writeBytes = write(connectionFileDescriptor,&adminviewStatusServer,sizeof(adminviewStatusServer));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            if(viewTypeserver==1){

            lseek(viewfd,0,SEEK_SET);
            read(viewfd,normal_customerServer,sizeof(struct normalUser));
            normal_customerServer->deleteflag=1;
            deleteflgStatus =1;
            writeBytes = write(connectionFileDescriptor,&deleteflgStatus,sizeof(deleteflgStatus));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
            
                
            }
            else{
                lseek(viewfd,0,SEEK_SET);
                read(viewfd,joint_customerServer,sizeof(struct jointUser));
                joint_customerServer->deleteflag=1;
                deleteflgStatus=1;
                writeBytes = write(connectionFileDescriptor,&deleteflgStatus,sizeof(deleteflgStatus));
                if (writeBytes == -1)
                    perror("Error while writing normalLoginStatusServer to network via socket!");
                else
                    printf("normalLoginStatusServer sent to client!\n");
                }

        }

 printf("\n Delete account...\n");        
    
}


void normalbanking(int bankingfd){
        int b_menu_input;
        
        readBytes = read(connectionFileDescriptor,&b_menu_input,sizeof(b_menu_input));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", b_menu_input);
        switch (b_menu_input)
        {
        case 0://User choose to Exit..
              printf("\n \t Shutting Down ...\n");
                mainMenuServer();
        case 1:
           //User Choose to Deposit...
           updateNormalUserdata(normal_customerServer);
           break;
     
        case 2:
           //User choose to withdraw money
           updateNormalUserdata(normal_customerServer);
           break;
        case 3:
            //Balance Inquiry
            printf("\nBalance Inquiry\n");
            break;
        case 4:
            //View Details
            printf("\nView Details\n");
            break;
        
        case 5://Passward change
        updateNormalUserdata(normal_customerServer);
            break;

        default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           mainMenuServer();
     }
        
    
    
    }


void jointbanking(){
        int b_menu_input;
        int bankingfd;
        readBytes = read(connectionFileDescriptor,&b_menu_input,sizeof(b_menu_input));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n client has sent login id :  %d \n", b_menu_input);
        switch (b_menu_input)
        {
        case 0://User choose to Exit..
              printf("\n \t Shutting Down ...\n");
                mainMenuServer();
        case 1:
           //User Choose to Deposit...
           updateJointUserdata(joint_customerServer);
           break;
     
        case 2:
           //User choose to withdraw money
           updateJointUserdata(joint_customerServer);
           break;

        case 3:
            //Balance Inquiry
            printf("\nBalance Inquiry\n");
            break;
        case 4:
            //View Details
            printf("\nView Details\n");
            break;
        
        case 5://Passward change
        updateJointUserdata(joint_customerServer);
            break;

        default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           mainMenuServer();
     }
        
    
    
    }


void updateNormalUserdata(struct normalUser *normal_customerServer){
        int bankingfd;
        readBytes = read(connectionFileDescriptor,normal_customerServer,sizeof(struct normalUser));
                if (readBytes == -1)
                    perror("Error while reading registerUser normal account from network via socket!");
                else
                    printf("client data is retrieved in Normal_customerServer \n");
            bankingfd =open(normal_customerServer->userName,O_RDWR);
            lseek(bankingfd,0,SEEK_SET);
            write(bankingfd,normal_customerServer,sizeof(struct normalUser));
    }


void updateJointUserdata(struct jointUser *joint_customerServer){
        int bankingfd;
        
        readBytes = read(connectionFileDescriptor,joint_customerServer,sizeof(struct jointUser));
                if (readBytes == -1)
                    perror("Error while reading joint account from network via socket!");
                else
                    printf("client data is retrieved in Joint_customerServer \n");
            
            lock.l_type = F_WRLCK;
            lock.l_start = 0;
            lock.l_whence = SEEK_SET;
            lock.l_len = 0;
            lock.l_pid = getpid();

            int lockingStatus = 1;
                struct flock savelock;
                savelock=lock;
                
                    
            fcntl(bankingfd, F_SETLKW, &lock);
            int updateStatus;
            readBytes=read(connectionFileDescriptor,&updateStatus,sizeof(updateStatus));
            if (readBytes == -1)
                    perror("Error while reading joint account from network via socket!");
                else
                    printf("client data is retrieved in Joint_customerServer \n");

            bankingfd =open(joint_customerServer->userName,O_RDWR);
            lseek(bankingfd,0,SEEK_SET);
            write(bankingfd,joint_customerServer,sizeof(struct jointUser));
            lock.l_type = F_UNLCK;
            fcntl(bankingfd, F_SETLKW, &lock);

    }