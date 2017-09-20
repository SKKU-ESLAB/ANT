
#include "sensor_manager.h"
#include "devices.h"
/*
	(Insert Request : 무조건 list의 맨 앞에 넣음, 첫번째로 실행되도록 함.)
	Insert Request : Insert to the front of list, and make it execute first.

	(getFirstRequest : 맨 앞의 request를 반환)
	getFirstRequest : Return the request in the front-end


	(rescheduleRequest : 맨 앞의 request)
	rescheduleRequest: the request in the front-end


	// Korean
	1. 외부에서 리퀘스트 insert
	2. 리퀘스트 있으면, 맨 앞에걸 처리함.
	3. 값 업데이트
	4. 리스케줄링

	// 리퀘스트 관련 함수 뜯어 고치기
	// 리퀘스트 핸들링 함수 가지고 동작하는 센서 스레드 함수 만들기
	// dbus에서 리퀘스트 핸들링 부분 추가
	// string parsing
	// nil 대응
	
	// English
	1. Insert request
	2. If there is any request, handle the front-end request
	3. Update the value
	4. Reschedule


*/



requestHeader* initRequestHeader(){
	requestHeader *rh;

	rh = (requestHeader*)malloc(sizeof(requestHeader));
	rh->num_of_request = 0;
	rh->start = NULL;

	return rh;
}


//Add, Update, Print

requestData* getRequestByPidRqnum(sensorList* sl, int pid, int rq_num){
	requestHeader *rh = sl->rh;
	requestData* temp;
	requestData* target = NULL;
	int i;

	while (target == NULL){
		temp = sl->rh->start;
		for (i = 0; i < sl->rh->num_of_request; i++){
			if (temp->pid == pid && temp->rq_num == rq_num){
				target = temp;
				break;
			}
			temp = temp->next;
		}

		sl = sl->next;
		if (sl == NULL)
			break;
	}

	return target;
}

sensorList* getSensorListByPidRqnum(sensorList* sl, int pid, int rq_num){
	requestHeader *rh = sl->rh;
	requestData* temp;
	sensorList* target = NULL;
	int i;

	while (target == NULL){
		temp = sl->rh->start;
		for (i = 0; i < sl->rh->num_of_request; i++){
			if (temp->pid == pid && temp->rq_num == rq_num){
				target = sl;
				break;
			}
			temp = temp->next;
		}

		sl = sl->next;
		if (sl == NULL)
			break;
	}

	return target;
}


int deleteRequestByPid(sensorList* sl, int pid){
	requestData* temp;
	requestData* prev = NULL;
	int i;

	while (1){
		if (sl->rh->num_of_request == 1 && sl->rh->start->pid == pid){
			free(sl->rh->start);
			sl->rh->start = NULL;
			sl->rh->num_of_request = 0;

			break;
		}
		else if (sl->rh->num_of_request <= 1)
			break;

		temp = sl->rh->start;

		//---------------------------------------------
		for (i = 0; i < sl->rh->num_of_request; i++){
			if (temp->pid == pid){
				printf("[SM] Delete %d\n", i);

				if (prev == NULL){
					sl->rh->start = sl->rh->start->next;
					free(temp);
				}
				else{
					prev->next = temp->next;
					free(temp);
				}			
				sl->rh->num_of_request--;
				continue;
			}
			prev = temp;
			temp = temp->next;
		}
		//---------------------------------------------

		//if (i == sl->rh->num_of_request)
			//break;
	}
//	printf("Error : Can't perform unregister\n");
	return 0;


}
int deleteRequestByPidRqnum(sensorList* sl, int pid, int rq_num){
	requestData* temp;
	requestData* prev = NULL;
	int i;

	if (sl->rh->num_of_request == 1){
		free(sl->rh->start);
		sl->rh->start = NULL;
		sl->rh->num_of_request = 0;

		return 0;
	}

	temp = sl->rh->start;

	//---------------------------------------------
	for (i = 0; i < sl->rh->num_of_request; i++){
		if (temp->pid == pid && temp->rq_num == rq_num){
			if (prev == NULL){
				sl->rh->start = sl->rh->start->next;
				free(temp);
			}

			prev->next = temp->next;
			free(temp);
			
			sl->rh->num_of_request--;
			return 0;
		}
		prev = temp;
		temp = temp->next;
	}

	printf("Error : Can't perform unregister\n");
	return 1;


}
void addRequest(requestHeader *rh, requestData* input){

	if (rh->start == NULL){
		rh->start = input;
		input->next = NULL;
	}	
	else{
		input->next = rh->start;
		rh->start = input;
	}

	input->next_run_time.tv_sec = 0;
	input->next_run_time.tv_usec = 0;
	rh->num_of_request++;
}

void calculateNextTime(struct timeval* time, int interval){
	//Interval is msec
	int one_second = 1000000;

	gettimeofday(time, NULL);

	time->tv_usec += (interval * 1000); //Change from msec to usec

	while (time->tv_usec >= one_second){
		time->tv_sec++;
		time->tv_usec -= one_second;
	}

}

void printRequest(requestHeader *rh){
	int i = 0;
	requestData* rd;
	rd = rh->start;

	printf("\n=====Requset List====\n");
	for (i = 0; i < rh->num_of_request; i++){
		
		printf("RQ[%d] PID[%d] INTERVAL[%6d], Next run time [%d.%6d]\n", rd->rq_num, rd->pid, rd->sensing_interval, rd->next_run_time.tv_sec, rd->next_run_time.tv_usec);
		rd = rd->next;	
	}
	printf("=====================\n");
}

int scheduleRequest(requestHeader *rh){
	requestData* temp_rd;
	requestData* temp_rd_prev;
	requestData* first;
		
	//1. next run time update
	calculateNextTime(&(rh->start->next_run_time), rh->start->sensing_interval);

	if (rh->num_of_request <= 1)
		return 0;

	temp_rd = rh->start->next;
	temp_rd_prev = rh->start;

	while (1){
		if (temp_rd == NULL ||  //마지막인 경우 (list의 맨 마지막으로 이동)
			temp_rd->next_run_time.tv_sec > rh->start->next_run_time.tv_sec ||
			(temp_rd->next_run_time.tv_sec == rh->start->next_run_time.tv_sec) && (temp_rd->next_run_time.tv_usec >= rh->start->next_run_time.tv_usec)){
			break;
		}
		temp_rd_prev = temp_rd;
		temp_rd = temp_rd->next;		
	}

	first = rh->start;
	
	if (temp_rd == NULL){ //List의 마지막		
		rh->start = rh->start->next;
		temp_rd_prev->next = first;
		first->next = NULL;
	}
	else if (temp_rd_prev == first){ //List의 처음

	}

	else{
		rh->start = rh->start->next;
		first->next = temp_rd;
		temp_rd_prev->next = first;
	}
	
	/*
	printf("Sche===========\n");
	printf("Rq. %d, Interval. %d, next run %d.%d\n", rh->start->rq_num, rh->start->sensing_interval, rh->start->next_run_time.tv_sec, rh->start->next_run_time.tv_usec);
	printf("Rq. %d, Interval. %d, next run %d.%d\n", rh->start->next->rq_num, rh->start->next->sensing_interval, rh->start->next->next_run_time.tv_sec, rh->start->next->next_run_time.tv_usec);
	printf("===============\n");
	*/

	return 0;
}






/*

void printRequset(struct RequestList *rList){
	struct RequestData *rd;

	printf("============ Requset List ============ \n");
	
	while(1){
		if(rList == NULL)
			break;

		printf("PID : %d \n", rList->pid);
		rd = rList->data;
		
		while(1){
			if(rd == NULL)
				break;

			printf("	Rq Num :%5d, Sensor Type : %d \n", rd->rq_num, rd->sensor_type);
			rd = rd->next;			
		}
		rList = rList->next;
	}
	printf("=======================================\n");

}
int countRequeststruct(struct RequestList *rList, unsigned int pid){
	int count = 0;
	struct RequestData *rd;

	for(;;){
		if(rList == NULL)
			return -1;
		
		else if(rList->pid == pid)
			break;

		rList = rList->next;
	}
		rd = rList->data;

	for(;;){
		if(rd == NULL)
			return count;
		else{
			count++;
			rd = rd->next;
		}	
	}

	return -1;
}


unsigned int addRequset(struct RequestList *rList, unsigned int pid, struct RequestData* input){
	struct RequestList *rl;
	struct RequestList *temp_rl;
	struct RequestData *rd;
	struct RequestData *temp_rd;
	int rq_num = input->rq_num;
	
	char * path_addr = "/ant/sensor/";
	char * interface_addr = "ant.sensor.";
	char pid_char[6];

	pid_to_char(pid, pid_char);
	rl = rList;

	//일단은 getRequest로 중복 여부 체크 해 볼것.

	printf("rList valuse : %u \n", rList);
	printf("test1\n");

	for (;;){
		if (rl->pid == pid) //found
			break;
		
		else if (rl->pid == 0 || rl->next == NULL){ ////First addrequest or Not exist
			temp_rl = (struct RequestList *)malloc(sizeof(struct RequestList));
			temp_rl->pid = pid;
			sprintf(temp_rl->path, "%s%s", path_addr, pid_char);
			sprintf(temp_rl->interface, "%s%s", interface_addr, pid_char);

			temp_rd = (struct RequestData *)malloc(sizeof(struct RequestData));
			memcpy(temp_rd, input, sizeof(struct RequestData));
			temp_rd->next = NULL;
			temp_rd->upper = temp_rl;
			
			temp_rl->data = temp_rd;
			temp_rl->next = NULL;

			if(rl->pid == 0){
				memcpy(rl, temp_rl, sizeof(struct RequestList));
				free(temp_rl);
			}
			else
				rl->next = temp_rl;
			
			return 0;
		}		
		
		rl = rl->next;
	}

	

	rd = rl->data;
	while (1){
		if (rd->rq_num == rq_num){
			printf("ERROR : Already exist same requset \n");
			return -1;
		}
		if (rd->next == NULL){
			temp_rd = (struct RequestData *)malloc(sizeof(struct RequestData));
			memcpy(temp_rd, input, sizeof(struct RequestData));
			temp_rd->next = NULL;
			rd->next = temp_rd;

			return 0;
		}
		rd = rd->next;
	}
}



struct RequestData * getRequset(struct RequestList *rList, unsigned int pid, int rq_num){
	struct RequestList *rl;
	struct RequestData *rd;

	rl = rList;

	for (;;){
		if (rl->pid == pid)
			break;
		if (rl->next == NULL)
			return NULL;

		rl = rl->next;
	}

	rd = rl->data;
	if (rd == NULL)
		return NULL;

	while (1){
		if (rd->rq_num == rq_num)
			return rd;
		
		if (rd->next == NULL)
			return NULL;

		rd = rd->next;
	}
}

int deleteRequsetByRq(struct RequestList *rList, unsigned int pid, int rq_num){
	struct RequestList *rl;
	struct RequestList *prev_rl = NULL;
	struct RequestData *rd;
	struct RequestData *prev_rd = NULL;

	rl = rList;

	for (;;){
		if (rl->pid == pid)
			break;
		if (rl->next == NULL)
			return FAIL_TO_DELETE_REQUEST;

		prev_rl = rl;
		rl = rl->next;
	}

	rd = rl->data;
	if (rd == NULL)
		return FAIL_TO_DELETE_REQUEST;

	while (1){
		if (rd->rq_num == rq_num){
			if(prev_rd == NULL && rd->next != NULL){ //First
				rl->data = rd->next;
				free(rd);
			}
			if(prev_rd == NULL && rd->next == NULL){ //Only one request on specific PID
				free(rd);
				struct RequestList *temp;
				
				if(prev_rl == NULL && rl->next != NULL){ //Target RL is first 
					temp = rl->next;					
					memcpy(rl, rl->next, sizeof(struct RequestList));	
					free(temp);
				}
				else if(prev_rl == NULL && rl->next == NULL){ //Target RL is only one
					rl->pid = 0;
					rl->data = NULL;
					rl->next = NULL;
				}
				else{
					prev_rl->next = rl->next;
					free(rl);
				}
			}
			else{ //etc case
				prev_rd->next = rd->next;
				free(rd);
			}			
			return 0;
		}
		
		if (rd->next == NULL)
			return FAIL_TO_DELETE_REQUEST;

		prev_rd = rd;
		rd = rd->next;
	}
	
}

int deleteRequsetByPid(struct RequestList *rList, unsigned int pid){
	struct RequestList *rl;
	struct RequestList *prev_rl = NULL;
	struct RequestData *rd;
	struct RequestData *prev_rd = NULL;

	rl = rList;

	for (;;){
		if (rl->pid == pid)
			break;
		if (rl->next == NULL)
			return FAIL_TO_DELETE_REQUEST;

		prev_rl = rl;
		rl = rl->next;
	}

	rd = rl->data;
	if (rd == NULL)
		return FAIL_TO_DELETE_REQUEST;

	while (1){
		if(rd == NULL){ //Delete Requset List
			struct RequestList *temp;				
			if(prev_rl == NULL && rl->next != NULL){ //Target RL is first 
				temp = rl->next;					
				memcpy(rl, rl->next, sizeof(struct RequestList));	
				free(temp);
			}
			else if(prev_rl == NULL && rl->next == NULL){ //Target RL is only one
				rl->pid = 0;
				rl->data = NULL;
				rl->next = NULL;
			}
			else{
				prev_rl->next = rl->next;
				free(rl);
			}
		}

		prev_rd = rd;
		rd = rd->next;

		free(prev_rd);
		//Delete all requset fo specific PID
	}
	
}

*/
