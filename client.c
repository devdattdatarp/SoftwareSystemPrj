// This is the client program

/*
    1. socket() -> create an endpoint for the communication
        sockfetfd = socket(AF_INET, SOCK_STREAM, 0);
    2. connect() -> initiate a connection to a socket
        connectionStatus = connect(socketfd, &socketaddr, sizeof(socketaddr));
    3. Start communicating -> `write` to and `read` from socketfd
*/
//required Header files..........................................................................................
#include <sys/types.h>  // Import for `socket`, `connect`
#include <sys/socket.h> // Import for `socket`, `connect`
#include <netinet/ip.h> // Import for `struct sockaddr_in`, `htons`
#include <stdio.h>      // Import for `perror` & `printf`
#include <unistd.h>     // Import for `_exit`, `read`, write`
#include <stdio.h>  
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


//User Structure for file------------------------------------------------------------------------------------------
//Normal account
struct normalUser{
int deleteflag;
char userName[10];
char pin[5];
int phoneNumber[10];
int ammount;
}normal_customer[1];

//Joint account
struct jointUser{
int deleteflag;
char userName[10];
char pin[5];
int phoneNumber[10];
char jointUser[10];
int ammount;
}joint_customer[1];

//Admin credentials
struct Admin{
char userName[10];
char pin[5];
}admin[1];

//Required Functions.....................................................................
int mainMenu();                    //Main menu
int validateUserName(char userName[]);
int isSpacePresentInMobileNumber(int phoneNumber[]);
int validatePhoneNumber(int phoneNumber[]);
int getNumberLength(int number[]);
//void paddString(char *Name,int size); 
int registerUser();                 // Register function
int Login();                   // Login functon


//-------------------------------------------------------------------------------
int normalbanking(struct normalUser *);    // banking menu for normal user
void normaldeposit(struct normalUser *);
void normalwithdraw(struct normalUser *);
void normalpasswordChange(struct normalUser *);
void normalviewDetails(struct normalUser *);

int jointbanking(struct jointUser *); 
void jointdeposit(struct jointUser *);
void jointwithdraw(struct jointUser *);
void jointpasswordChange(struct jointUser *);
void jointviewDetails(struct jointUser *);

//Admin function....................................................
int adminWork(struct Admin *ad);
void viewAccount();
void modifyAccount();
void deleteAccount();

//Socket Discriptor--------------------------------------------------------------------------

int socketFileDescriptor; // File descriptor that will be used for communication via socket
ssize_t readBytes, writeBytes; // Number of bytes read from / written to the network via socket
int loginChoice; //login choice
int updateStatus;
//---------------------------------------------------------------------------

void main()
{

    
    int connectStatus;        // Determines success of `connect` call

    struct sockaddr_in address; // Holds the address of the server to communicate

    
    char dataFromServer[15];

    // Create an endpoint for communicate -> here, create the client side point
    // Create a socket for TCP connection using IPv4
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Client side socket successfully created!\n");

    // Defined server's details
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8081);

    connectStatus = connect(socketFileDescriptor, (struct sockaddr *)&address, sizeof(address));
    if (connectStatus == -1)
    {
        perror("Error while connecting to server!");
        _exit(0);
    }
    printf("Client to serrver connection successfully established!\n");

    // ========================= Client - Server communication =================

    readBytes = read(socketFileDescriptor, dataFromServer, 15);
    
    if (readBytes == -1)
        perror("Error while reading from network via socket!");
    else
        printf("Data from server: %s\n", dataFromServer);

    
    
    
    //Function calling for Banking........................

     mainMenu();

    
    

    // =======================================================================

    close(socketFileDescriptor);
}

int mainMenu(){
    
     printf("\t..........WELCOME TO THANGARAJU BANKING SYSTEM........... \n\n");

     int menu_input_count=3;
     

     while(menu_input_count >0){
        writeBytes = write(socketFileDescriptor,&menu_input_count, sizeof(menu_input_count));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");
        

     printf("\tNew registration ------> Enter input 1 \n \tLogin -----------------> Enter input 2 \n \tExit ------------------> Enter input 0 \n");
     printf("\n \tUser Input:");

     int userInput;

     scanf("%d", &userInput);
     

     switch (userInput){

        case 0://User choose to Exit..
                writeBytes = write(socketFileDescriptor,&userInput, sizeof(userInput));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");

               printf("\n \t Shutting Down ...\n");
               exit(0);
        case 1://User Choose to Register...
            writeBytes = write(socketFileDescriptor,&userInput, sizeof(userInput));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");
                registerUser();
                //loginUser();
                break;
     
       case 2:
           //User Choose to Login...
           writeBytes = write(socketFileDescriptor,&userInput, sizeof(userInput));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");
           Login();
           break;

       default:
           //Exit in case user not choose either to Login or Register...
        printf("\n Invalid input..\n \n");
           menu_input_count--;
           break;
     }
    
     
     
     }
     printf("\n You have entered too many invalid inputs\n \n");


    return 0;
}
// register User function...............................

int registerUser(){


//Account creation process.....

   printf("\n \t ......NEW REGISTRATION....... \n");
   int accChoice;
      printf("\t Select account type :\n \t Normal account-------> Enter 1 \n \t Joint account--------> Enter 2\n");
      printf("\n \tUser Input:");
      scanf("%d", &accChoice);

      //sending account type choice to server ....  
      writeBytes = write(socketFileDescriptor,&accChoice, sizeof(accChoice));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");


      //Normal account opening............
        if(accChoice == 1){
            char *Username;
            printf("Please enter your name.\n");
            scanf("%s", normal_customer->userName); 
            printf("%s",normal_customer->userName);
            

            if(validateUserName(normal_customer->userName) == -1){
            printf("Please enter valid user name. It can't be left blank. \n");
            exit(0);}

             printf("\n Enter 4 digit Pin. \n");
            scanf(" %[^\n]%*c",normal_customer->pin);

            printf("Please enter your Mobile Number.\n");
            scanf("%ls", normal_customer->phoneNumber); 

            
           
            if(validatePhoneNumber(normal_customer->phoneNumber) == 1){
             printf("Mobile number entered by you is valid... \n");
         
            }else{
                printf("Mobile number entered by you is Invalid... \n");
                registerUser();
                }
       
            printf("\n Enter Iniatial ammount to be deposit. \n");
            scanf("%d",&normal_customer->ammount);
      

           
      
            printf("\n your User-ID is  %s \n",normal_customer->userName);

        //Sending Normal account data to server.........................
            writeBytes = write(socketFileDescriptor,&normal_customer, sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("\n Error while writing to network via socket!\n");
                else
                    printf("\n Normal account Data sent to server!\n");
        }

        //joint account opening............
        else if(accChoice == 2){
             printf("Please enter your name.\n");
            scanf(" %[^\n]%*c", joint_customer->userName); 
            //printf("%s",joint_customer->userName);

            if(validateUserName(joint_customer->userName) == -1){
            printf("Please enter valid user name. It can't be left blank. \n");
            exit(0);}

            printf("Please enter your Joint user name.\n");
            scanf(" %[^\n]%*c", joint_customer->jointUser); 

            if(validateUserName(joint_customer->jointUser) == -1){
            printf("Please enter valid user name. It can't be left blank. \n");
            exit(0);}
            
              
            printf("Please enter your Mobile Number.\n");
            scanf("%ls", joint_customer->phoneNumber); 

            if(validatePhoneNumber(joint_customer->phoneNumber) == 1){
            printf("Mobile number entered by you is valid... \n");
            }else{
                printf("Mobile number entered by you is Invalid... \n");
                registerUser();
                }
       
            printf("\n Enter Iniatial ammount to be deposit. \n");
            scanf("%d",&joint_customer->ammount);
      

            printf("\n Enter 4 digit Pin for User 1. \n");
            scanf(" %[^\n]%*c",joint_customer->pin);
      
           
   
            printf("\n your User-ID is  %s \n",joint_customer->userName);
            
            //Sending Joint account data to server.........................
            writeBytes = write(socketFileDescriptor,&joint_customer, sizeof(joint_customer));
                if (writeBytes == -1)
                    perror("\n Error while writing to network via socket!\n");
                else
                    printf("\n Joint account Data sent to server!\n");
            }
        else{
            printf("\n \tWrong Input..\n");
            return 0;
        }
    
    return 0;

}

int getNumberLength(int number[]){
    int length = 0;
    while(number[length]){
      length++;
    }
    return length;
}

int isSpacePresentInMobileNumber(int number[]){
      for(int i = 0; i < getNumberLength(number); i++){
           if(number[i] < 0 || number[i] > 9){
               return 0;
           }  
      }
   return 1;
}

int validatePhoneNumber(int phoneNumber[]){
   if(getNumberLength(phoneNumber) == 10 && isSpacePresentInMobileNumber(phoneNumber) == 0){
      return 1;     
   }
   return -1;
}

int validateUserName(char userName[]){
    if(strlen(userName) == 0){
        return -1;     
    }
   return 1;
}

//Login function.........................................

int Login(){
    printf("\n \t ......LOGIN....... \n");
   
      printf("\t Select account type :\n \t Normal account------------> Enter 1 \n \t Joint account------------> Enter 2\n \t Admin account-------------> Enter 3\n \t Return to main Menu--------> Enter 0");
      printf("\n \tUser Input:");
      scanf("%d", &loginChoice);

      //sending account type choice to server ....  
      writeBytes = write(socketFileDescriptor,&loginChoice, sizeof(loginChoice));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("login choice sent to server!\n");

    if(loginChoice==1){
     //Joint account login...
        char jointName[10];
        char jointPin[5];
        //char serverjointPin[4];
    
    
        printf("\n \tEnter Your Username:");
        scanf("%s",jointName);
        
        
        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor,jointName, sizeof(jointName));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");

        int jointLoginUsernameStatus;
        readBytes = read(socketFileDescriptor, &jointLoginUsernameStatus, sizeof(jointLoginUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading jointLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", jointLoginUsernameStatus);

        if(jointLoginUsernameStatus == 1){
            printf("\n User-name verified by server...\n");
        
            // pin checking...
           
            int lockingStatus;
                    readBytes=read(socketFileDescriptor,&lockingStatus,sizeof(lockingStatus));
                    if(lockingStatus == 2){
                        printf("\nfile locked");
                        return 0;
                    }
           int jointloginStatus;
            int pin_count=2;
            while(pin_count>0){
                    printf("\n Enter pin: \n");
                    scanf("%s",jointPin);
                    printf("\n Pin you have entered :%s \n",jointPin);
                    writeBytes = write(socketFileDescriptor,&jointPin,5);
                    if (writeBytes == -1)
                        perror("Error while writing login status to network via socket!");
                    else
                        printf("Login status sent to client!\n");
                    
                    readBytes = read(socketFileDescriptor,&jointloginStatus, sizeof(jointloginStatus));
                    if (readBytes == -1)
                        perror("Error while writing login status to network via socket!");
                    else
                        printf("Login status sent to client!\n");
   
                    if(jointloginStatus==1){
                        printf("\n valid credentials...\n");
                        printf("\n You are logged in..\n");
                        break;
                        }
                    else{
                        printf("\n You have entered wrong pin...Only %d more attempts remaining..\n",pin_count);
                        
                        pin_count--;
                        continue;
                        }
                        
                        }
                        if(pin_count == 0)
                        {return 0;}

                        readBytes = read(socketFileDescriptor,joint_customer,sizeof(struct jointUser));
                        if (readBytes == -1)
                        perror("Error while writing login status to network via socket!");
                        else
                        printf("Login status sent to client!\n");
                    
                    
                    printf(" \n File locking for this client\n");
                    
                    jointbanking(joint_customer);
                }
        else if(jointLoginUsernameStatus == -1){
                return 0;
            }
        
    }

else if(loginChoice == 2){
    //Joint account login...
        char jointName[10];
        char jointPin[5];
        //char serverjointPin[4];
    
    
        printf("\n \tEnter Your Username:");
        scanf("%s",jointName);
        
        
        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor,jointName, sizeof(jointName));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");

        int jointLoginUsernameStatus;
        readBytes = read(socketFileDescriptor, &jointLoginUsernameStatus, sizeof(jointLoginUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading jointLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", jointLoginUsernameStatus);

        if(jointLoginUsernameStatus == 1){
            printf("\n User-name verified by server...\n");
        
            // pin checking...
           
            int lockingStatus;
                    readBytes=read(socketFileDescriptor,&lockingStatus,sizeof(lockingStatus));
                    if(lockingStatus == 2){
                        printf("\nfile locked");
                        return 0;
                    }
           int jointloginStatus;
            int pin_count=2;
            while(pin_count>0){
                    printf("\n Enter pin: \n");
                    scanf("%s",jointPin);
                    printf("\n Pin you have entered :%s \n",jointPin);
                    writeBytes = write(socketFileDescriptor,&jointPin,5);
                    if (writeBytes == -1)
                        perror("Error while writing login status to network via socket!");
                    else
                        printf("Login status sent to client!\n");
                    
                    readBytes = read(socketFileDescriptor,&jointloginStatus, sizeof(jointloginStatus));
                    if (readBytes == -1)
                        perror("Error while writing login status to network via socket!");
                    else
                        printf("Login status sent to client!\n");
   
                    if(jointloginStatus==1){
                        printf("\n valid credentials...\n");
                        printf("\n You are logged in..\n");
                        break;
                        }
                    else{
                        printf("\n You have entered wrong pin...Only %d more attempts remaining..\n",pin_count);
                        
                        pin_count--;
                        continue;
                        }
                        
                        }
                        if(pin_count == 0)
                        {return 0;}

                        readBytes = read(socketFileDescriptor,joint_customer,sizeof(struct jointUser));
                        if (readBytes == -1)
                        perror("Error while writing login status to network via socket!");
                        else
                        printf("Login status sent to client!\n");
                    
                    
                    printf(" \n File locking for this client\n");
                    
                    jointbanking(joint_customer);
                }
        else if(jointLoginUsernameStatus == -1){
                return 0;
            }
        
   }

else if(loginChoice == 3){

    //admin account login...
       char adminName[10];
        char adminPin[4];
        //char jointPin2[4];
    
    
        printf("\n \tEnter Your Username:");
        scanf("%s",adminName);

        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor, adminName, sizeof(adminName));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");

        int adminLoginUsernameStatus;
        readBytes = read(socketFileDescriptor, &adminLoginUsernameStatus, sizeof(adminLoginUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading jointLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", adminLoginUsernameStatus);

        if(adminLoginUsernameStatus == 1){
            printf("\n User-name verified by server...\n");
        
            // pin checking...
            readBytes = read(socketFileDescriptor,&admin, sizeof(struct Admin));
            if (readBytes == -1)
                perror("Error while reading joint pin from network via socket!");
            else
                printf("\n pin successfully sent by server...\n");
            int pin_count=3;
            while(pin_count>0){
                    pin_count--;
                    printf("\n Enter pin: \n");
                    scanf("%s",adminPin);
                    printf("\n Pin you have entered :%s \n",adminPin);


                    if(strcmp(admin->pin,adminPin)==0){
                        printf("\n valid credentials...\n");
                        break;
                        }
                    else{
                        printf("\n You have entered wrong pin...Only %d more attempts remaining..\n",pin_count);
                        if(pin_count == 0)
                        {exit(0);}
                        continue;
                        }
                        }
                    printf("\n You are logged in..\n");
                    
                    int adminloginStatus=1;
                    writeBytes = write(socketFileDescriptor,&adminloginStatus, sizeof(adminloginStatus));
                    if (writeBytes == -1)
                        perror("Error while writing login status to network via socket!");
                    else
                        printf("Login status sent to client!\n");

                    adminWork(admin);
                    return 0;
                
                }
        else if(adminLoginUsernameStatus == -1){
                return 0;
        
                }
   
}
else if(loginChoice==0){
    
    return 0;
}
}

//Admin Work...........................................
int adminWork(struct Admin *ad){
    int adminChoice;
    printf("\n \t ......Admin Operation....... \n");
   
      printf("\t Select account type :\n \t Add account------------> Enter 1 \n \t Modify account------------> Enter 2\n \t View account-------------> Enter 3 \n \t Delete account------------> Enter 4\n \t Return to main Menu--------> Enter 0");
      printf("\n \tUser Input:");
      scanf("%d", &adminChoice);

      //sending Admin operation type choice to server ....  
      writeBytes = write(socketFileDescriptor,&adminChoice, sizeof(adminChoice));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("admin choice sent to server!\n");

      
       
        switch (adminChoice)
        {
        case 0://exit 
        return 0;

        case 1://calling register User function to add account ....
            registerUser();
            break;

        case 2:
            modifyAccount();
            break;

        case 3:
            viewAccount();
            break;
        
        case 4:
            deleteAccount();
            break;
        
        default:
            printf("Invalid Input....");
            exit(0);
            break;
        }
       
}
//Admin function for view account
void viewAccount(){
    int viewType;
    char viewUserID[10];
    printf("\t Select account type :\n \t Normal account------------> Enter 1 \n \t Joint account------------> Enter 2\n ");
      printf("\n \tUser Input:");
      scanf("%d", &viewType);
    printf("\n \tEnter Your Username:");
        scanf("%s",viewUserID);
        writeBytes = write(socketFileDescriptor,&viewType, sizeof(viewType));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server %d!\n",viewType);
        
        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor,&viewUserID, sizeof(viewUserID));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server %s!\n",viewUserID);

        int adminUsernameStatus;
        readBytes = read(socketFileDescriptor, &adminUsernameStatus, sizeof(adminUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading normalLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", adminUsernameStatus);

    if(adminUsernameStatus == 1){

      if(viewType == 1){

        readBytes = read(socketFileDescriptor,normal_customer,sizeof(struct normalUser));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n server has sent structure");
        normalviewDetails(normal_customer);
      }
      else if(viewType==2){
        readBytes = read(socketFileDescriptor,joint_customer,sizeof(struct jointUser));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n server has sent structure");

        jointviewDetails(joint_customer);
      }
        else{
            printf("\n Invalid Input...\n");
            exit(0);
        }
      
      
      }
      else{
        printf("\n File doesnt exist...  \n");
        exit(0);
      }
      }

//Admin function for modification..
void modifyAccount(){
    int modifyaccType;
    printf("\n \t ......Modify account....... \n");
   
      printf("\t Select account type :\n \t Normal account------------> Enter 1 \n \t Joint account------------> Enter 2\n  \t Return to main Menu--------> Enter 0");
      printf("\n \tUser Input:");
      scanf("%d", &modifyaccType);

      //sending account type choice to server ....  
      writeBytes = write(socketFileDescriptor,&modifyaccType, sizeof(modifyaccType));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Modify choice sent to server!\n");

    if(modifyaccType==1){
     //Normal account modify...
        char normalName[10];
        
    
    
        printf("\n \tEnter Your Username:");
        scanf("%s",normalName);
        
        
        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor,normalName, sizeof(normalName));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server!\n");

        int normalLoginUsernameStatus;
        readBytes = read(socketFileDescriptor, &normalLoginUsernameStatus, sizeof(normalLoginUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading normalLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", normalLoginUsernameStatus);

        if(normalLoginUsernameStatus == 1){
            printf("\n User-name verified by server...\n");
    
    readBytes = read(socketFileDescriptor,normal_customer,sizeof(struct normalUser));
            if (readBytes == -1)
                perror("Error while reading normal pin from network via socket!");
            else
                printf("usename from server: %s\n",normal_customer->userName);
            
    int normalmodifyChoice; 
    printf("\n \t Select modify type :\n \t User name------------> Enter 1 \n \t mobile number------------> Enter 2\n  \t ammount------------> Enter 3\n ");
      printf("\n \tUser Input:");
      scanf("%d",&normalmodifyChoice);

    switch (normalmodifyChoice)
    {
    case 1:
        printf("\n Enter new username:");
        scanf("%s",normal_customer->userName);
        break;

    case 2:
    printf("\n Enter new mobile number:");
        scanf("%ls",normal_customer->phoneNumber);
        break;

    case 3:
    printf("\n Enter new ammount:");
        scanf("%d",&normal_customer->ammount);
        break;

    default:
        exit(0);
        break;
    }
    writeBytes = write(socketFileDescriptor,normal_customer, sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");
    printf("\n\tAdmin operation successfully done...\n");

      }
else{printf("File doesnt exist...");
    exit(0);
}
}
else if(modifyaccType == 2){
//joint account modify...
       char jointName[10];
        
    
    
        printf("\n \tEnter Your Username:");
        scanf("%s",jointName);

        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor, jointName, sizeof(jointName));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");

        int jointLoginUsernameStatus;
        readBytes = read(socketFileDescriptor, &jointLoginUsernameStatus, sizeof(jointLoginUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading normalLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", jointLoginUsernameStatus);

        if(jointLoginUsernameStatus == 1){
            printf("\n User-name verified by server...\n");
        
            // pin checking...
            readBytes = read(socketFileDescriptor,&joint_customer, sizeof(struct jointUser));
            if (readBytes == -1)
                perror("Error while reading normal pin from network via socket!");
            else
                printf("\n pin successfully sent by server...\n");
    int jointmodifyChoice; 
    
      scanf("%d",&jointmodifyChoice);

    switch (jointmodifyChoice)
    {
    case 1:
        printf("\n Enter new username:");
        scanf("%s",joint_customer->userName);
        break;

    case 2:
    printf("\n Enter new username:");
        scanf("%ls",joint_customer->phoneNumber);
        break;

    case 3:
    printf("\n Enter new ammount:");
        scanf("%d",&joint_customer->ammount);
        break;
    
    case 4:
        printf("\n Enter new joint username:");
        scanf("%s",joint_customer->jointUser);
        break;

    default:
        exit(0);
        break;
    }
    writeBytes = write(socketFileDescriptor,joint_customer, sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to client!\n");
    printf("\n\tAdmin operation successfully done...\n");


}
else{
    printf("File doesnt exist....");
    exit(0);
}
}
else{
    printf("Invalid Input....");
    exit(0);
}
exit(0);

}

void deleteAccount(){
    int viewType;
    char viewUserID[10];
    printf("\t Select account type :\n \t Normal account------------> Enter 1 \n \t Joint account------------> Enter 2\n ");
      printf("\n \tUser Input:");
      scanf("%d", &viewType);
    printf("\n \tEnter Your Username:");
        scanf("%s",viewUserID);
        writeBytes = write(socketFileDescriptor,&viewType, sizeof(viewType));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server %d!\n",viewType);
        
        //sending Data to server for verification...

        writeBytes = write(socketFileDescriptor,&viewUserID, sizeof(viewUserID));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("Data sent to server %s!\n",viewUserID);

        int adminUsernameStatus;
        readBytes = read(socketFileDescriptor, &adminUsernameStatus, sizeof(adminUsernameStatus));
        if (readBytes == -1)
            perror("Error while reading normalLoginUsernameStatus from network via socket!");
        else
            printf("Data from server: %d\n", adminUsernameStatus);

    if(adminUsernameStatus == 1){
        int deleteflgStatus;
      if(viewType == 1){

        readBytes = read(socketFileDescriptor,&deleteflgStatus,sizeof(deleteflgStatus));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n server has sent structure");
        if(deleteflgStatus==1){printf("\n Account deleted...\n");}
        else{
            printf("error..");
            exit(0);
        }
      }
      else if(viewType==2){
        readBytes = read(socketFileDescriptor,&deleteflgStatus,sizeof(deleteflgStatus));
                if (readBytes == -1)
                    perror("\n Error while reading loginName from network via socket! \n");
                else
                    printf("\n server has sent structure");
        if(deleteflgStatus==1){printf("\n Account deleted...\n");}
        else{
            printf("error..");
            exit(0);
        }
      }
        else{
            printf("\n Invalid Input...\n");
            exit(0);
        }
      
      
      }
      else{
        printf("\n File doesnt exist...  \n");
        exit(0);
      }
    printf("\nthis will delete account\n");
}
//Banking menu for normal account.........
int normalbanking(struct normalUser *banker){
     int bmenu_input_count=3;
     while(bmenu_input_count >0){
        writeBytes = write(socketFileDescriptor,&bmenu_input_count,sizeof(bmenu_input_count));
        if (writeBytes == -1)
            perror("\n Error while writing menu count to network via socket! \n");
        else
             printf("\n banking menu count sent to server!\n");

    printf("\tDeposit Money ------> Enter input 1 \n \tWithdraw Money -----------------> Enter input 2 \n \tBalance Inquiry -----------------> Enter input 3  \n \tView Details -----------------> Enter input 4   \n \tPin Change -----------------> Enter input 5 \n \tExit ------------------> Enter input 0 \n");
     printf("\n \tUser Input:");

     int userInput_bank;

     scanf("%d", &userInput_bank);

     switch (userInput_bank){

        case 0://User choose to Exit..
                writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
               printf("\n \t Shutting Down ...\n");
               return 0;
        case 1:
           //User Choose to Register...
           writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
           normaldeposit(banker);
           break;
     
        case 2:
           //User choose to withdraw money
           writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
           normalwithdraw(banker);
           break;
        case 3:
            //Balance Inquiry
            writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            printf("\n \t Your Balance is: %d \n",banker->ammount);
            break;
        case 4:
            //View Details
            writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            normalviewDetails(banker);
            break;
        
        case 5://Passward change
        writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            normalpasswordChange(banker);
            break;

        default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           bmenu_input_count--;
           break;
     }
    
     
     
     }
     printf("\n You have entered too many invalid inputs\n \n");
    }

//Joint User Banking Menu............
int jointbanking(struct jointUser *jointbanker){
        int bmenu_input_count=3;
     while(bmenu_input_count >0){
        writeBytes = write(socketFileDescriptor,&bmenu_input_count,sizeof(bmenu_input_count));
        if (writeBytes == -1)
            perror("\n Error while writing menu count to network via socket! \n");
        else
             printf("\n banking menu count sent to server!\n");

    printf("\tDeposit Money ------> Enter input 1 \n \tWithdraw Money -----------------> Enter input 2 \n \tBalance Inquiry -----------------> Enter input 3  \n \tView Details -----------------> Enter input 4   \n \tPin Change -----------------> Enter input 5 \n \tExit ------------------> Enter input 0 \n");
     printf("\n \tUser Input:");

     int userInput_bank;

     scanf("%d", &userInput_bank);

     switch (userInput_bank){

        case 0://User choose to Exit..
                writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
               printf("\n \t Shutting Down ...\n");
               return 0;
        case 1:
           //User Choose to Register...
           writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
           jointdeposit(jointbanker);
           break;
     
        case 2:
           //User choose to withdraw money
           writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
           jointwithdraw(jointbanker);
           break;
        case 3:
            //Balance Inquiry
            writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            printf("\n \t Your Balance is: %d \n",jointbanker->ammount);
            break;
        case 4:
            //View Details
            writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            jointviewDetails(jointbanker);
            break;
        
        case 5://Passward change
            writeBytes = write(socketFileDescriptor,&userInput_bank, sizeof(userInput_bank));
                if (writeBytes == -1)
                    perror("Error while writing to network via socket!");
                else
                    printf("userInput_bank sent to server!\n");
            jointpasswordChange(jointbanker);
            break;

        default:
           //Exit in case user not choose either to Login or Register...

           printf("\n Invalid input..\n \n");
           bmenu_input_count--;
           break;
     }
    
     
     
     }
     printf("\n You have entered too many invalid inputs\n \n");
    return 0;

    }




//normal User deposit..........
void normaldeposit(struct normalUser *deposit_ammount){
    int dammount;
    printf("\nEnter ammount to deposit(in Rupees): ");
    scanf("%d",&dammount);
    deposit_ammount->ammount=deposit_ammount->ammount + dammount;
    printf("\n Ammount succesfully credited...updated balance is : %d Rs\n",deposit_ammount->ammount);
    writeBytes = write(socketFileDescriptor,deposit_ammount, sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("\n Error while writing Blance to network via socket!\n");
                else
                    printf("\n Updated Balance sent to server!\n");
    }

//joint User deposit.............
void jointdeposit(struct jointUser *deposit_ammount){
    int dammount;
    printf("\nEnter ammount to deposit(in Rupees): ");
    scanf("%d",&dammount);
    deposit_ammount->ammount=deposit_ammount->ammount + dammount;
    printf("\n Ammount succesfully credited...updated balance is : %d Rs\n",deposit_ammount->ammount);
    writeBytes = write(socketFileDescriptor,deposit_ammount, sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("\n Error while writing Blance to network via socket!\n");
                else
                    printf("\n Updated Balance sent to server!\n");
            printf("\nconfirm update:");
            scanf("%d",&updateStatus);
    
    writeBytes = write(socketFileDescriptor,&updateStatus, sizeof(updateStatus));
                if (writeBytes == -1)
                    perror("\n Error while writing Blance to network via socket!\n");
                else
                    printf("\n Updated Balance sent to server!\n");
    }

//withdrawal for normal user.......................
void normalwithdraw(struct normalUser *withdraw_ammount){
    int wammount;
    printf("\n Enter ammount to deposit(in Rupees): ");
    scanf("%d",&wammount);
    withdraw_ammount->ammount=withdraw_ammount->ammount - wammount;
    printf("\n Ammount succesfully debited..current balance is %d Rs.\n",withdraw_ammount->ammount);
    writeBytes = write(socketFileDescriptor,withdraw_ammount, sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("\n Error while writing Blance to network via socket!\n");
                else
                    printf("\n Updated Balance sent to server!\n");
    
}

//withdrawal for joint user....................
void jointwithdraw(struct jointUser *withdraw_ammount){
    int wammount;
    printf("\n Enter ammount to deposit(in Rupees): ");
    scanf("%d",&wammount);
    withdraw_ammount->ammount=withdraw_ammount->ammount - wammount;
    printf("\n Ammount succesfully debited..current balance is %d Rs.\n",withdraw_ammount->ammount);
    writeBytes = write(socketFileDescriptor,withdraw_ammount, sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("\n Error while writing Blance to network via socket!\n");
                else
                    printf("\n Updated Balance sent to server!\n");
    
}

/*struct Register balanceEnquiry(struct Register *balance){
    return balance;
}*/


//Pin change function..................................
void normalpasswordChange(struct normalUser *pinChange){
    char newPin[4];
    char oldPin[4];
    printf("\n Enter old pin:");
    scanf("%s",oldPin);
int oldPincnt=2;
while(oldPincnt>0){
if(strcmp(oldPin,pinChange->pin)==0)
{printf("\n Enter new Pin:");
    scanf("%s",newPin);
    strcpy(pinChange->pin,newPin);
    
    printf("\n Changed pin : %s\n",pinChange->pin);

    writeBytes = write(socketFileDescriptor,pinChange, sizeof(struct normalUser));
                if (writeBytes == -1)
                    perror("\n Error while writing passward change to network via socket!\n");
                else
                    printf("\n changed pin sent to server!\n");
    }
else{
printf("\n you have entered wrong pin...Please enter correct old pin \n");
oldPincnt--;
}
}
printf("\n Too many attempts ..Try after few seconds\n");
}

//Pin change function..................................

void jointpasswordChange(struct jointUser *pinChange){
    char newPin[4];
    char oldPin[4];
    printf("\n Enter old pin:");
    scanf("%s",oldPin);
int oldPincnt=2;
while(oldPincnt>0){
if(strcmp(oldPin,pinChange->pin)==0)
{printf("\n Enter new Pin:");
    scanf("%s",newPin);
    strcpy(pinChange->pin,newPin);
    printf("\n Changed pin : %s\n",pinChange->pin);

    writeBytes = write(socketFileDescriptor,pinChange, sizeof(struct jointUser));
                if (writeBytes == -1)
                    perror("\n Error while writing passward change to network via socket!\n");
                else
                    printf("\n changed pin sent to server!\n");

    }
else{
printf("\n you have entered wrong pin...Please enter correct old pin \n");
oldPincnt--;
}
}
printf("\n Too many attempts ..Try after few seconds\n");
}

//view details functions...............
void normalviewDetails(struct normalUser *view_details)
{
    printf("\n....Customer Details......\n ");
    printf("\n \tUser-name     : %s ",view_details->userName);
    printf("\n \tAccount type  : Normal account ");   
    printf("\n \tPhone No      : %ls",view_details->phoneNumber);
    printf("\n \tAmmount in Rs : %d ",view_details->ammount);
    
} 
void jointviewDetails(struct jointUser *view_details)
{
    printf("\n....Customer Details......\n ");
    printf("\n \tUser-name     : %s ",view_details->userName);
    printf("\n \tAccount type  : Joint account ");
    printf("\n \tJoint-user    : %s ",view_details->jointUser);    
    printf("\n \tPhone No      : %ls",view_details->phoneNumber);
    printf("\n \tAmmount in Rs : %d ",view_details->ammount);
    
} 
