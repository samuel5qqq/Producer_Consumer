#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define BUFFER_SIZE 5
#define TRUE 1
pthread_t id;
pthread_attr_t attribute;
pthread_mutex_t mutex;
sem_t full,empty;
int pnumber,cnumber;
int number;
int count;
int t;
int ID,sequence;
int buffer[BUFFER_SIZE];
int insert_item(int item);
int remove_item(int *item);
int insert_item(int item){
	if(count < BUFFER_SIZE){
		buffer[count] = item;
		count++;
		return 0;
	}
	else
	    return -1;
}
int remove_item(int *item) {
	if( count > 0){
		*item = buffer[(count-1)];
		count--;
		return 0;
	}
	else
	    return -1;
}
void *producer(void *param) {
	int item,waittime,RR;
	int i=0;
	srand(time(NULL));
    while (TRUE) {
    	number++;
    	ID++;
    	sequence++;
    	t=1;
    	for(int i=0;i < ID;i++)
	        t=t*2;
    	RR=rand()%99;
    	item=RR+100*sequence+100000*ID;
        waittime = rand()%t;
        
        sleep(waittime);
        
        sem_wait(&empty);
        
        pthread_mutex_lock(&mutex);
        
        if (insert_item(item))
            fprintf(stderr,"report error condition");
        else
            printf("producer %d produced %d\n",ID,item);
    
        pthread_mutex_unlock(&mutex);
        
        sem_post(&full);
        
        if(number==pnumber){
        	printf("Producer done!!\n");
        	break;
		}
    }
}
void *consumer(void *param) {
    int item,waittime;
    int a=0;
    while (TRUE) {
    	t=1;
    	a++;
    	for(int i=0;i < ID;i++)
	        t=t*2;
    	waittime=rand()%t;
    	
        sleep(waittime);
        
        sem_wait(&full);
        
        pthread_mutex_lock(&mutex);
        
        if (remove_item(&item))
            fprintf(stderr,"report error condition");
        else
            printf("consumer %d consumed %d\n",a,item);
        
        pthread_mutex_unlock(&mutex);
        
        sem_post(&empty);
        
        if(a == cnumber){
        	printf("Consumer done!!\n");
        	break;
		}
    }
}

int main(int argc,char *argv[]){
	
	pthread_mutex_init(&mutex,NULL);
	sem_init( &empty, 0, BUFFER_SIZE);
	sem_init( &full, 0, 0);
	count = 0;
	ID=0;
	number=0;
	int sleeptime;
	printf("Sleeptime: ");
	scanf("%d",&sleeptime);
	printf("Number of producers: ");
	scanf("%d",&pnumber);
	printf("Number of consumers: ");
	scanf("%d",&cnumber);

	pthread_create(&id,&attribute,producer,NULL);
	pthread_create(&id,&attribute,consumer,NULL);

    sleep(sleeptime);
	printf("Exit the program");
	return 0;
}

