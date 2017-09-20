#include <stdlib.h>
#include <node.h>

#include "nil.h"

#define DELETE_REQUEST 0
#define DELETE_FAIL_NOT_EXIST 1

void initRequestList(requestList *rList){
	rList->rq_num = -1;
	rList->next = NULL;
}
void printRequset(requestList *rList){
	printf("============ Requset List ============ \n");
	
	while(1){
		if(rList == NULL)
			break;

		printf("Request Number : %3d  \n", rList->rq_num);


		rList = rList->next;
	}
	printf("=======================================\n");

}
int countRequest(requestList *rList){
	int count = 0;

	for(;;){
		if(rList == NULL)
			return count;
		
		else{
			count++;
			rList = rList->next;
		}	
	}
}
requestList * newRequest(requestList *rList){
	requestList *rl = rList;

	if (rList->rq_num == -1){
		rList->rq_num = 0;
		return rList;
	}
	
	for (;;){
		if (rList->next == NULL)
			break;
		rList = rList->next;
	}

//	rList->next = (requestList*)malloc(sizeof(requestList));
  rList->next = new requestList();
	rList->next->next = NULL;
	rList->next->rq_num = rList->rq_num + 1;

	return rList->next;
}
requestList * getRequest(requestList *rList, int rq_num){
	requestList *rl;

	rl = rList;
	for (;;){
		if (rl->rq_num == rq_num){
			//printf("[NIL] Found Request \n");
			break;
		}
		rl = rl->next;
		if (rl == NULL){
			return NULL;
		}
	}

	return rl;
}
int deleteRequest(requestList *rList, int rq_num){
	requestList *rl;
	requestList *del_rl = NULL;
	requestList *prev_rl = NULL;

	rl = rList;


	if (rList->rq_num == rq_num){
		//root is target.
		del_rl = rList->next;
		memcpy(rList, rList->next, sizeof(requestList));
		free(del_rl);

		return DELETE_REQUEST;
	}
	for (;;){
		if (rl->rq_num == rq_num)
			break;

		prev_rl = rl;
		rl = rl->next;
		
		if (rl == NULL){
			return DELETE_FAIL_NOT_EXIST;
		}
	}

	prev_rl->next = rl->next;
	free(rl);

	return DELETE_REQUEST;

	
}
