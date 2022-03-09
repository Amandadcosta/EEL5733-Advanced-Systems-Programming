#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Tuple{
    int userID;
    char action;
    char topic[16];
    int score;
    char strg[26];
    

} Tuple;

//(1111,P,history) from a file	
//(1111,history,50) on stdout

int main()
{
    char buf[15];
    
    // FILE *file;
    // file = fopen("input.txt", "r");
    // if(file==NULL){
    //     printf("Operation failed\n");
    //     return 0;}
    FILE *file1;
    file1 = fopen("Mapper_Output.txt", "w");
    if(file1==NULL){
        printf("Operation failed\n");
        return 0;}
    
    static struct Tuple T[100];
    
    int i=0;
    int result;
    //while(fgets(buf,2,file)!=NULL)
    //while((result=fscanf(file,"(%4s,%1c,%15s), ",(&T[i].userID),(&T[i].action),(T[i].topic)))!=EOF)
    while(fscanf(stdin,"%25c",T[i].strg)==1)
    //fscanf(file,"(%d[^,],%c,%15s[^)])%c", T[i].userID, &T[i].action, T[i].topic, &separator);
    
    {   
        
    
        //printf("%s\n",T[i].strg);
        T[i].userID=atoi(strncpy(buf,&((T[i].strg)[1]),4));
        T[i].action=T[i].strg[6];
        //sscanf(T[i].action,"%c",(T[i].strg)[6]);
        //T[i].action=(strncpy(a,&((T[i].strg)[6]),1));
        strncpy(T[i].topic,&((T[i].strg)[8]),15);
        T[i].topic[15]='\0';
        
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
 	//printf("%d\n",sizeof(T[i].userID));   
        //printf("(%i,%s,%c)\n",(T[i].userID),(T[i].topic),(T[i].action));
        if(T[i].score<0){
            printf("(%04d,%s,%d)\n",T[i].userID,T[i].topic,T[i].score);
            fprintf(file1,"(%04d,%s,%d)\n",T[i].userID,T[i].topic,T[i].score);
        }
        else{
            printf("(%04d,%s, %d)\n",T[i].userID,T[i].topic,T[i].score);
            fprintf(file1,"(%04d,%s, %d)\n",T[i].userID,T[i].topic,T[i].score);
            }
    i++;
    }
    
    if(ferror(file1)!=0)
    {
        printf("Operation failed\n");
        return 0;
    }
    if(fclose(file1)==EOF){
        printf("Operation failed\n");
        return 0;}
     

    //printf("Operation successful");

    return 0;

}
