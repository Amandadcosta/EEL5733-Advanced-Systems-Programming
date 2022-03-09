#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h> 
#include <time.h>
#include <pthread.h> 
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
const int max_size = 100;

struct Account
{
    int number, balance;
    int available_status;
    int condvar;
} Account;

struct Transfer
{
    int from, to, amount;
} Transfer;


typedef struct Buffer
{
  
  struct Transfer *trnsf;
  pthread_mutex_t mtx;
  pthread_cond_t  condvar;
	
  int Count;
}Buffer;

pthread_mutex_t transaction_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;			//global mutex for transactions to check available_status and then proceed

pthread_cond_t transaction_condvar = (pthread_cond_t) PTHREAD_COND_INITIALIZER;			//global condition variable for transactions to check available_status and then proceed
    
int numOfBufSlots=10;
struct Account accounts[10000];
struct Transfer transfers[10000];

struct Buffer buf[100];
int *t;
void *banktransfer(void *t);

int accounts_count = 0;
int transfers_count = 0;

int N;
int done=0;
int tid;
int *slotIndex;
int *outslotindex;
int acc_trans_count=0;

void printer(){
     printf("\n");

     for (int i = 0; i < accounts_count; i++)
     {
         printf("%d %d\n", accounts[i].number, accounts[i].balance);
     }

     for (int i = 0; i < transfers_count; i++)
     
     {
     	
         printf("Transfer %d %d %d\n", transfers[i].from, transfers[i].to, transfers[i].amount);
     }
    
}


int main(int argc, char const *argv[])
{
    // printf("%s\n", argv[1]);

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    struct stat stat_struct;
    if (stat(argv[1], &stat_struct) == -1)
    {
        perror("stat error");
        exit(EXIT_FAILURE);
    }
    N=atoi(argv[2]);
    slotIndex=(int*)malloc(N*sizeof(int));
    outslotindex=(int*)malloc(N*sizeof(int));
    char *filecon = malloc(stat_struct.st_size);
    pthread_t ThreadID[N];                                        
    long t=0;
    void *status;
    
        
        
    
     for(int i=0; i< N; i++){		//initialize buffers which is array of struct transfer
    buf[i].trnsf = (struct Transfer*)malloc(numOfBufSlots * sizeof(struct Transfer));
    buf[i].mtx = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    buf[i].condvar = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    buf[i].Count = 0;
    }
    
    for(t = 0; t < N; t++)       //creating the threads
    {
 	usleep(10000);
        pthread_create(&ThreadID[t], NULL, banktransfer,(void *)t);      
        }
       int j=0;
    while (1)
    {
    	int res=(fscanf(input_file, "%[^\n] ", filecon) != EOF);
    	if(res != 1 || res == EOF){
		//printf("main is done\n");		
		done = 1;
		break;}
        char line[strlen(filecon) + 1];
        strcpy(line, filecon);
	//printf("%s\n",filecon);
        char * token = strtok(line, " ");
        
        if(token != NULL)
        {
            char word[strlen(token) + 1];
            strcpy(word, token);

            if (strcmp(word, "Transfer") != 0)
            {
                // printf("%s", word);
                accounts[accounts_count].number = atoi(word);
                token = strtok(NULL, " ");

                char balance[strlen(token) + 1];
                strcpy(balance, token);
                // printf("%s", balance);
                accounts[accounts_count].balance = atoi(balance);
                token = strtok(NULL, " ");
		 
		 
		 accounts[accounts_count].available_status=1;								//initially all accounts are set to available or 1
               
                accounts_count++;
            } 
            else
            {	
            	
                token = strtok(NULL, " ");
		 tid=transfers_count%N;		
    
                char from[strlen(token) + 1];
                strcpy(from, token);
                int s;
                s=pthread_mutex_lock(&buf[tid].mtx);                    //acquire lock or mutex to buffer of specific trnsaction tid's buffer 
        
         	if (s != 0)
         	{
         	perror("main doesn't have pthread_mutex_lock\n");			
         	}
         	
         	while(buf[tid].Count == numOfBufSlots)				
	 	{
         	s=pthread_cond_wait(&buf[tid].condvar, &buf[tid].mtx);
         	if (s != 0)
         	 {  	perror("main pthread_cond_wait\n");         	}
         	}
                buf[tid].trnsf[slotIndex[tid]].from = atoi(from);
                
                
                //transfers[transfers_count].from = atoi(from);
                token = strtok(NULL, " ");

                char to[strlen(token) + 1];
                strcpy(to, token);
                // printf("%s", to);
                buf[tid].trnsf[slotIndex[tid]].to = atoi(to);
    
                //transfers[transfers_count].to = atoi(to);
                token = strtok(NULL, " ");

                char amount[strlen(token) + 1];
                strcpy(amount, token);
                buf[tid].trnsf[slotIndex[tid]].amount = atoi(amount);
    
                //transfers[transfers_count].amount = atoi(amount);
                token = strtok(NULL, " ");
		 
		 slotIndex[tid] = (slotIndex[tid] + 1) % (numOfBufSlots);          // incrementing slotIndex and then % is used instead of resetting the slotIndex
         
		 buf[tid].Count = buf[tid].Count + 1;
    		 //printf("tid:    %d      Count:    %d\n",tid,buf[tid].Count);
                transfers_count++;
                pthread_cond_signal(&buf[tid].condvar);
         	 pthread_mutex_unlock(&buf[tid].mtx);
         	 
            }//else ends
            
        }//if ends

        
    }//while ends
    
    //printer();  //print inputfile
    fclose(input_file);
    for(int i=0; i<N; i++){
	pthread_cond_signal(&buf[i].condvar);			//especially to signal the last thread that input file is done
    	}
    
    if(done==1){
    
    //if(acc_trans_count==transfers_count){
       for(int l=0;l<accounts_count;l++){
        printf("%d %d\n",accounts[l].number,accounts[l].balance);
        } //end for
	}
    pthread_exit(NULL);
    //exit(EXIT_SUCCESS);
}//main ends




void transferamount(int ptid, struct Transfer B)
{


//printf("Source Acc No: %d \t Original balance: %d \t Destination Acc No: %d \t Original balance: %d\n",accounts[B.from-1].number, accounts[B.from-1].balance,accounts[B.to-1].number, accounts[B.to-1].balance);
accounts[B.from-1].balance-= B.amount;
//sleep(0.5);
accounts[B.to-1].balance+= B.amount;
//printf("Source Acc No: %d \t New balance: %d \t Destination Acc No: %d \t New balance: %d\n",accounts[B.from-1].number, accounts[B.from-1].balance,accounts[B.to-1].number, accounts[B.to-1].balance);
//sleep(1);



}

void resettingAvailable(int pid,struct Transfer B)
{
int s;
s=pthread_mutex_lock(&transaction_mutex);
if (s != 0)
 {
  perror("transaction doesn't have pthread_mutex_lock\n");			
 }
 //set resources to available
accounts[B.from-1].available_status=1;
accounts[B.to-1].available_status=1;


pthread_cond_broadcast(&transaction_condvar);										
pthread_mutex_unlock(&transaction_mutex);
	
}


void *banktransfer(void *t)
{
long ptid = (long)t;
    int c=0;
    int s;
    //struct Transfer Output_Tuple[100];
    //int count = 0;
    
 
    while(1){
	
         s=pthread_mutex_lock(&buf[ptid].mtx);
         if (s != 0)
         {
         perror("thread doesn't have pthread_mutex_lock\n");			
         }
         while(buf[ptid].Count == 0 && done == 0){
    	     pthread_cond_wait(&buf[ptid].condvar, &buf[ptid].mtx);
         }
         if(done && buf[ptid].Count == 0){
            break;
         }
         
        if(buf[ptid].Count > 0){
        //printf("tid: %ld \t Transfer %d \t %d \t %d\n",ptid,buf[ptid].trnsf[outslotindex[ptid]].from,buf[ptid].trnsf[outslotindex[ptid]].to,buf[ptid].trnsf[outslotindex[ptid]].amount);
    	//implementing transaction part below
         s=pthread_mutex_lock(&transaction_mutex);
         if (s != 0)
         {
         perror("transaction doesn't have pthread_mutex_lock\n");			
         }
         while(accounts[buf[ptid].trnsf[outslotindex[ptid]].from-1].available_status !=1 ||accounts[buf[ptid].trnsf[outslotindex[ptid]].to-1].available_status !=1 ) //check status of source and destination account locks
        { pthread_cond_wait(&transaction_condvar, &transaction_mutex);
		}   
		
	//set resources to unavailable
	accounts[buf[ptid].trnsf[outslotindex[ptid]].from-1].available_status=0;
	accounts[buf[ptid].trnsf[outslotindex[ptid]].to-1].available_status=0;
	
	pthread_mutex_unlock(&transaction_mutex);						//release mutex for other threads to check 
	
	transferamount(ptid,buf[ptid].trnsf[outslotindex[ptid]]);				//call function to transfer amount from source to destination
	resettingAvailable(ptid,buf[ptid].trnsf[outslotindex[ptid]]);     
        
        buf[ptid].Count = buf[ptid].Count - 1;
        outslotindex[ptid] = (outslotindex[ptid] + 1) % numOfBufSlots;
        acc_trans_count++;  
    }//end if
    
    pthread_mutex_unlock(&buf[ptid].mtx);
    pthread_cond_signal(&buf[ptid].condvar);
	
   }//end while
   

   pthread_exit(NULL);
	
   }//end thread

