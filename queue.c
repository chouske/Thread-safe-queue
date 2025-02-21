#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct queue {
    int maxsize;
    int cursize;
    void **elements;
    int tail;
    int head;
    pthread_mutex_t queuemutex;
} queue_t;
queue_t *queue_new(int size) {
    queue_t *myqueue = malloc(sizeof(queue_t));
    myqueue->maxsize = size;
    myqueue->cursize = 0;
    myqueue->elements = calloc(size, sizeof(void *));
    //myqueue->queuelock = rwlock_new(READERS, 0);
    pthread_mutex_init(&(myqueue->queuemutex), NULL);
    myqueue->head = 0;
    myqueue->tail = 0;
    return myqueue;
}
void queue_delete(queue_t **q) {
    free((*q)->elements);
    //rwlock_delete(&((*q)->queuelock));
    pthread_mutex_destroy(&((*q)->queuemutex));
    free(*q);
    *q = NULL;
}
void queue_headtail(queue_t *q) {
    printf("head: %d tail: %d\n", q->head, q->tail);
}
void queue_print(queue_t *q) {
    static int counter = 1;
    printf("%d: ", counter);
    counter++;
    ;
    printf("[");
    for (int i = 0; i < (q->maxsize); i++) {
        if (((q->elements)[i]) == NULL) {
            printf("N, ");
        } else {
            printf("%d, ", *((int *) (q->elements)[i]));
        }
    }
    printf("]\n");
    queue_headtail(q);
}
bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }
    while (true) {
        pthread_mutex_lock(&(q->queuemutex));
        int cursize = q->cursize;
        int maxsize = q->maxsize;
        if (cursize != maxsize) {
            break;
        }
        pthread_mutex_unlock(&(q->queuemutex));
    }
    //reader_lock(q->queuelock);//Fix
    /*if((q->cursize) == (q->maxsize)){//this
		printf("push failure\n");//area
		return false;//soon
	}*/
    (q->elements)[q->tail] = elem;
    //queue_print(q);
    //printf("value: %d\n", *((int *)(q->elements)[q->tail]))
    if ((q->tail) == ((q->maxsize) - 1)) {
        q->tail = 0;
        //writer_unlock(q->queuelock);
    } else {
        q->tail = (q->tail) + 1;
    }
    q->cursize = (q->cursize) + 1;
    pthread_mutex_unlock(&(q->queuemutex));
    //queue_print(q);
    //queue_headtail(q);
    return true;
}
bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return false;
    }
    while (true) {
        pthread_mutex_lock(&(q->queuemutex));
        int cursize = q->cursize;
        if (cursize != 0) {
            break;
        }
        pthread_mutex_unlock(&(q->queuemutex));
    }
    //Acquire lock
    /*reader_lock(q->queuelock);
	while((q->cursize) == 0){

        }*/
    /*if((q->cursize) == 0){
		printf("pop failure\n");
		//Release lock
		return false;
	}*/
    //reader_unlock(q->queuelock);
    //writer_unlock(q->queuelock);
    *elem = (q->elements)[q->head];
    (q->elements)[q->head] = NULL;
    //writer_unlock(q->queuelock);
    //reader_lock(q->queuelock);
    if ((q->cursize) == 1) {
        //reader_unlock(q->queuelock);
        //writer_lock(q->queuelock);
        q->head = 0;
        q->tail = 0;
    } else {
        if ((q->head) == ((q->maxsize) - 1)) {
            //reader_unlock(q->queuelock);
            //writer_lock(q->queuelock);
            q->head = 0;

        } else {
            //reader_unlock(q->queuelock);
            //writer_lock(q->queuelock);
            q->head = (q->head) + 1;
        }
    }
    q->cursize = (q->cursize) - 1;
    //writer_unlock(q->queuelock);
    //queue_print(q);
    //queue_headtail(q);
    //
    //Release lock
    pthread_mutex_unlock(&(q->queuemutex));
    return true;
}
