#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct Node
{
	void *data;
	struct Node *next;
};

struct Node* newNode(void *data)
{
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	if (temp == NULL)
		return temp;
	temp->data = data;
	temp->next = NULL;
	return temp;
}

struct queue {
	struct Node *front;
	struct Node *rear;
	int length;
};

int main()
{
 	queue_t q;
	int data[] = {1,2,3,4,5,6,7,8,9,10};
	int i;
	
	q = queue_create();
	struct Node* t;
	for (i = 0; i < 10;i++)
	{
		queue_enqueue(q,&data[i]);
		if(i == 0)
			t = q->front;
		else
			t = t->next;
		printf("in queue data[%d] = %d\n", i, *(int*)t->data);
		
 	}
	return 0;
}



queue_t queue_create(void)
{
	queue_t myQ = (queue_t)malloc(sizeof(struct queue));
	if (myQ == NULL)
		return myQ;
	myQ->front = myQ->rear = NULL;
	myQ->length = 0;

	return myQ;
}

int queue_destroy(queue_t queue)
{	
	if (queue == NULL || queue->front != NULL)
		return -1;
	else 
	{
		free(queue);
		return 0;
	}
}

int queue_enqueue(queue_t queue, void *data)
{
	struct Node *temp = newNode(data);
	if (temp == NULL || data == NULL || queue == NULL)
		return -1;
	else if (queue->rear == NULL)
		queue->front = queue->rear = temp;
	else
	{
		queue->rear->next = temp;
		queue->rear = temp;
	}
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (data == NULL || queue == NULL || queue->front == NULL)
		return -1;
	data = &queue->front->data;
	struct Node* temp = queue->front;
	queue->front = queue->front->next;
	free(temp);
	if (queue->front == NULL)
		queue->rear = NULL;
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (data == NULL || queue == NULL)
		return -1;
	struct Node* temp = queue->front;
	struct Node* prev;
	if (temp != NULL && temp->data == data)
	{
		queue->front = temp->next;
		free(temp);
		if (queue->front == NULL)
		        queue->rear = NULL;
	}
	else
	{
		while (temp != NULL && temp->data != data)
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return -1;
		prev->next = temp->next; 
		free(temp);
	}
	queue->length--; 
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func == NULL)
		return -1;
	
	int it;
	struct Node* curNode = queue->front;
	void * curData;

	while(curNode != NULL)
	{
		curData = curNode->data;
		it = (*func)(queue, curData, arg);

		if(it == 1)
		{
			if (data != NULL)
				data = &curData;

			return 0;
		}
		else if(it != 0) 
			return -1; 
	
		curNode = curNode->next;		
	}
	
	return -1; // not found
}

int queue_length(queue_t queue)
{
	return queue->length;
}
