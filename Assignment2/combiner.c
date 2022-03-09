#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
#include <unistd.h>
#include <sys/types.h>
#include <time.h>



struct Tuple{
    int userID;
    char action;
    char topic[16];
    int score;
    int totalscore;
    char strg[26];
    //int rtid;
    } Tuple;
    

typedef struct Buffer{
	struct Tuple *Tuples;
	pthread_mutex_t mtx;
	pthread_cond_t  condvar;
	int Count;
} Buffer;

Buffer buffer[100];

struct Tuple T[100];

int *rdrslotindex;
int numOfBufSlots;
int N;
int *slotIndex;

int *unique_UID_list = {NULL};
int counter=0;
char buf[15];
int done=0;
int unique_user_index=0;int rrnum;


void *mapper(void *t);
void *reducer(void *t);



void printBuffer (struct Buffer buffer[N]) {
char pp[30];

	for(int i=0; i<N; i++){
		for(int j=0; j<numOfBufSlots; j++){
			sprintf(pp,"(%04d,%s,%d)\n",buffer[i].Tuples[j].userID,buffer[i].Tuples[j].topic,buffer[i].Tuples[j].score);
			printf("Mapper Printing buffer %d:\t %s", i, pp);
		}
		printf("\n");
	}
	printf("done printing buffer\n");
	return;
}

int main(int argc, char *argv[])
{
if ( argc != 3 )	                                /* the number of arguments passed in the command must be 3 */
{
	printf("Too few arguments\n" );
	return ( EXIT_FAILURE );
}
numOfBufSlots=atoi(argv[1]);                                /*num_of_tuples or number of slots in each communication buffer or Buffersize*/ 
N=atoi(argv[2]);                                            /*N=Number of reducer threads*/


pthread_t ThreadID[N+1];                                        /*One for producer(0) remaining for consumers */
long t=0;
void *status;


pthread_create(&ThreadID[0], NULL, mapper,(void *)t);          /*create the mapper thread*/
for(t = 0; t < N; t++)
 {
 	usleep(10000);
    pthread_create(&ThreadID[t+1], NULL, reducer,(void *)t);      /*create the N reducer threads*/
 } 

pthread_join(ThreadID[0], &status);                            /*main joins the mapper thread*/
//printf("main joined mapper\n");
for(t = 0; t < N; t++)
{
    pthread_join(ThreadID[t+1], &status);                        /*main joins the reducer thread*/
    //printf("Main joined reducer %ld\n", t);
}
    
pthread_exit(NULL);
printf("Mapper still running though it should not?");
}


/*creating functions */
int matchRrid(int userID){
int rrnum;
for (int i = 0; i < unique_user_index; i++) 
    {if(userID == unique_UID_list[i]) {
        //printf("%d",i);
        return i;  }
        }         // returns the reducer's id who's userID matched
                  
return -1;                
}    


void add_To_Buffer(int rrNum, struct Tuple TT) {

    //printf("slot index =%d %d\n",slotIndex[rrNum],TT.userID);
    buffer[rrNum].Tuples[slotIndex[rrNum]] = TT;             //store tuple T[i] at slot index of specific reducer ID(rrNum) Buffer
    buffer[rrNum].Count = buffer[rrNum].Count + 1;
    }

void *mapper(void *t)
{
struct Tuple T[100];
//printf("Mapper printing\n");
int i=0;
pthread_t rtid[N];
int rrnum=0;
unique_UID_list = (int *)malloc(sizeof(int *));
slotIndex = (int *) calloc(N, sizeof(int));

int res;
//buffer=(Buffer*) malloc(sizeof(Buffer));
for(int i=0; i<N; i++){
  
    buffer[i].Tuples = (struct Tuple*)malloc(numOfBufSlots * sizeof(struct Tuple ));		
    buffer[i].mtx = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    buffer[i].condvar = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    buffer[i].Count = 0;
    }
    while(1)                        // same as while(1)
    {
    	res=fscanf(stdin,"%25c",T[i].strg)==1;
        //sleep(0.5);
        if(res != 1){
		done = 1;
		break;}
    int i=0;
    int result;
    int s;
     
        T[i].userID=atoi(strncpy(buf,&((T[i].strg)[1]),4));
        T[i].action=T[i].strg[6];
        strncpy(T[i].topic,&((T[i].strg)[8]),15);
        T[i].topic[15]='\0';
        counter++;
        switch((T[i].action))
        {
            case 'P':
                T[i].score=50;
                break;
            case 'L':
                T[i].score=20;
                break;
            case 'D':
                T[i].score=-10;
                break;
            case 'C':
                T[i].score=30;
                break;
            case 'S':
                T[i].score=40;
                break;
            default:
                printf("Invalid score");
                break;
           
        }
    //printf("(%04d,%s,%d)\n",T[i].userID,T[i].topic,T[i].totalscore);
           
    rrnum = matchRrid((T[i].userID));
    if(rrnum==-1)
	    {
	 unique_UID_list=(int*)realloc(unique_UID_list,(unique_user_index+1)*sizeof(int));
         unique_UID_list[unique_user_index++] = T[i].userID;              	//add unique User IDs
  	 rrnum = matchRrid(T[i].userID);                             	//return the ReducerNumber that stores tuples with this userID    
		}
		
	
		if(unique_user_index>N)
		{
		int dif=unique_user_index-N;
		
		unique_UID_list=(int*)realloc(unique_UID_list,*(unique_UID_list+dif)*sizeof(int));
         	
		slotIndex=realloc(slotIndex, N*2*sizeof(int));
		rdrslotindex=(int*)realloc(rdrslotindex, N*2*sizeof(int));
		int currentN=N;
		N=N+1;
		
		for(int i=currentN; i<=unique_user_index; i++){
  
    		buffer[i].Tuples = (struct Tuple*)malloc(numOfBufSlots * sizeof(struct Tuple ));		
    		buffer[i].mtx = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    		buffer[i].condvar = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    		buffer[i].Count = 0;
    		}
    		pthread_t ThreadID[N];				//check
    		for(int t = currentN+1; t < unique_user_index; t++)
 		{
 		printf("extra t=%d",t);
 			usleep(10000);
    		pthread_create(&ThreadID[t+1], NULL, reducer,(void *)(long)t);      /*create the N reducer threads*/
 		} 
	}
		
		
         	s=pthread_mutex_lock(&buffer[rrnum].mtx);                    //acquire lock or mutex to buffer of specific reducer with reducerID=rrnum
        
         if (s != 0)
         {printf("mapper pthread_mutex_lock\n");}
            
        
	 while(buffer[rrnum].Count == numOfBufSlots)
	 {
         s=pthread_cond_wait(&buffer[rrnum].condvar, &buffer[rrnum].mtx);
         if (s != 0)
         {printf("mapper pthread_cond_wait\n");}
         }
	 
	 
         add_To_Buffer(rrnum, T[i]);
         slotIndex[rrnum] = (slotIndex[rrnum] + 1) % (numOfBufSlots);          // incrementing slotIndex and then % is used instead of resetting the slotIndex
         //printf("rrnum = %d, count = %d\n", rrnum, buffer[rrnum].Count);

        i++;
         pthread_cond_signal(&buffer[rrnum].condvar);
         pthread_mutex_unlock(&buffer[rrnum].mtx);
	 
        }
        //printBuffer(buffer);
        for(int i=0; i<N; i++){
		  pthread_cond_signal(&buffer[i].condvar);
	  }
        pthread_exit(NULL);
        //return t;
        }     
        



void *reducer(void *t)
{
    long rrtid = (long)t;
    
    char inputTuple[30];
    int count = 0;
    //printf("I am Reducer %ld\n",rrtid);
 
    rdrslotindex = (int *) calloc(N, sizeof(int));
    struct Tuple Output_Tuple[100];
    struct Tuple Rdr_Tup[100];
    
    for(int i=0;i<100;i++){
        Output_Tuple[i].score=0;
        Output_Tuple[i].totalscore=0;
    }
    while(1){

         pthread_mutex_lock(&buffer[rrtid].mtx);
         while(buffer[rrtid].Count == 0 && done == 0){
    	     //printf("In reducer %ld done = %d\n",rrtid, done);
    	     pthread_cond_wait(&buffer[rrtid].condvar, &buffer[rrtid].mtx);
         }
        if(done && buffer[rrtid].Count == 0){
            break;
         }
//        char pp[30];
        while(buffer[rrtid].Count > 0){
        
//printf("From Buffer:\t(%04d,%s,%d)\n",buffer[rrtid].Tuples[rdrslotindex[rrtid]].userID, buffer[rrtid].Tuples[rdrslotindex[rrtid]].topic,buffer[rrtid].Tuples[rdrslotindex[rrtid]].score);

//		strcpy(inputTuple, pp);
//		//printf("inputTuple-----------------------%s\n",inputTuple);
//             char *new = inputTuple;
//             buffer[rrtid].Count = buffer[rrtid].Count - 1;
//             printf("rdrslotindex[%ld] = %d\n", tid, rdrslotindex[rrtid]);
//             rdrslotindex[rrtid] = (rdrslotindex[rrtid] + 1) % numOfBufSlots;
            
         int match=0;
         for(int j=0;j<count;j++){
             if(strcmp(Output_Tuple[j].topic,buffer[rrtid].Tuples[rdrslotindex[rrtid]].topic)==0){
             
    		  //Output_Tuple[j].userID=buffer[rrtid].Tuples[rdrslotindex[rrtid]].userID;
    		  //strcpy(Output_Tuple[j].topic, buffer[rrtid].Tuples[rdrslotindex[rrtid]].topic);    
                 Output_Tuple[j].totalscore+=buffer[rrtid].Tuples[rdrslotindex[rrtid]].score;
                 //printf("Reducer %ld:/t(%04d,%s,%d)\n", rrtid, Output_Tuple[count].userID, Output_Tuple[count].topic, Output_Tuple[count].totalscore);
             match=1;
                 }
         }//end for loop
         if(match==0){
             Output_Tuple[count].userID=buffer[rrtid].Tuples[rdrslotindex[rrtid]].userID;
             strcpy(Output_Tuple[count].topic,buffer[rrtid].Tuples[rdrslotindex[rrtid]].topic);
             Output_Tuple[count].totalscore=buffer[rrtid].Tuples[rdrslotindex[rrtid]].score;
             count++;
         }//end if
        
         buffer[rrtid].Count = buffer[rrtid].Count - 1;
         //printf("rdrslotindex[%ld] = %d\n", rrtid, rdrslotindex[rrtid]);
         rdrslotindex[rrtid] = (rdrslotindex[rrtid] + 1) % numOfBufSlots;
          
         } //end while loop
 
       pthread_mutex_unlock(&buffer[rrtid].mtx);
	pthread_cond_signal(&buffer[rrtid].condvar);
	
	
}//end while(1)
//return t;
for(int l=0;l<count;l++){
        //printf("Reducer %ld:\t(%04d,%s,%d)\n", rrtid, Output_Tuple[l].userID, Output_Tuple[l].topic, Output_Tuple[l].totalscore);
        printf("(%04d,%s,%d)\n", rrtid, Output_Tuple[l].userID, Output_Tuple[l].topic, Output_Tuple[l].totalscore);
        } //end for

pthread_exit(NULL);

}//end reducer



