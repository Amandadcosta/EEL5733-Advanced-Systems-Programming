#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Tuple{
    int userID;
    char action;
    char topic[16];
    int score;
    int totalscore;
    char strg[27];
} Tuple;

int main()
{
    
    struct Tuple T[100];
    struct Tuple Output_Tuple[100];
    for(int i=0;i<100;i++){
        Output_Tuple[i].score=0;
        Output_Tuple[i].totalscore=0;
    }
    FILE *file;
    file = fopen("output.txt", "w");
    if(file==NULL){
        printf("Operation failed\n");
        return 0;}
    int i=0,result=0;
    char buf[15];
    char buf_scr[3];
    while((fscanf(stdin,"%27c",T[i].strg))==1)
    {   
        T[i].userID=atoi(strncpy(buf,&((T[i].strg)[1]),4));
        strncpy(T[i].topic,&((T[i].strg)[6]),15);
        T[i].topic[15]='\0';
        
        if(T[i].strg[22]==' '){
            strncpy(buf_scr,&(T[i].strg[23]),2);
            T[i].score=atoi(buf_scr);
        }
        else{
            strncpy(buf_scr,&(T[i].strg[23]),2);
            T[i].score=atoi(buf_scr);
            T[i].score=-T[i].score;
        }
        

        //T[i].score=atoi(strncpy(buf_scr,&((T[i].strg)[23]),3));;
        //printf("%d\n",T[i].score);
        //printf("%c",T[i].strg[22]);
               
        i++;
    }
    int count=0,read_end=0;
    while(T[read_end].score!='\0'){
        int match=0;
        for(int j=0;j<count;j++){
            if((Output_Tuple[j].userID==T[read_end].userID) && strcmp(Output_Tuple[j].topic,T[read_end].topic)==0){
                Output_Tuple[j].totalscore+=T[read_end].score;
                //printf("%d\t%d\n",T[read_end].score,Output_Tuple[j].totalscore);
                //printf("%d\n",Output_Tuple[j].totalscore);  
                match=1;
                //break;
            }
        }
        if(match==0){
            Output_Tuple[count]=T[read_end];
            Output_Tuple[count].totalscore=T[read_end].score;
            //printf("new :%d",Output_Tuple[count].totalscore);
            count++;
        }
        read_end++;
    }
    for(int l=0;l<count;l++){
        printf("(%04d,%s,%d)\n",Output_Tuple[l].userID,Output_Tuple[l].topic,Output_Tuple[l].totalscore);
        fprintf(file,"(%04d,%s,%d)\n",Output_Tuple[l].userID,Output_Tuple[l].topic,Output_Tuple[l].totalscore);
        Output_Tuple[l].userID=0;
        Output_Tuple[l].totalscore=0;
    }
    if(ferror(file)!=0)
    {
        printf("Operation failed\n");
        return 0;
    }
    if(fclose(file)==EOF){
        printf("Operation failed\n");
        return 0;}
    return 0;

}
