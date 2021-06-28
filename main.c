
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcgrand.h"

#define Idle 0
#define Busy 1

double minimum(double *next_dept_time, int total_num_server, int* Server_num);

void arrival (double *sim_time, int *total_num_server, int *Num_In_Queue, int *Num_Custs_Delayed, double *next_arr_time, double *Area_Under_Q,
              float*Last_Event_Time, float *Time_Since_Last_Event, double *Time_Arrival, int *server_status, double *next_dept_time, float *mean_interarrival_time, float *mean_service_time, int *index, double* Time_arrival_sys, int * Q_limit);

void depature (double *sim_time,float *Time_Since_Last_Event,float *Last_Event_Time, double *Area_Under_Q, int *Num_In_Queue, int *server_status, double *next_dept_time,
                double *q_Delay,double *Time_Arrival,  double *Total_q_Delay,int *Num_Custs_Delayed,float *mean_service_time, int *Server_num, int *index, double* Time_arrival_sys, double *sys_delay, double *total_sys_delay, int total_num_server);


void move (double* Time_arrival_sys, int *total_servers);

float expon(float mean);
int main()
{

// Initiailize

    double sim_time = 0;
    int Server_num = 0;
    int total_num_server = 0;
    int Num_In_Queue = 0;
    int Num_Custs_Delayed = 0;
    double avg_q_delay =0;
    float avg_num_in_queue = 0;
    double next_arr_time = 0;
    double Area_Under_Q = 0;
    float Last_Event_Time = 0;
    double Total_q_Delay = 0;
    double q_Delay = 0;
    float Time_Since_Last_Event = 0;
    int K =0;
    int Num_Delays_Required = 0;
    float mean_interarrival_time =0;
    float mean_service_time =0;
    int index = -1;
    double sys_delay = 0 ;
    double total_sys_delay = 0;
    double avg_sys_delay = 0;



    printf("Enter mean_interarrival_time\n");
	scanf("%f", &mean_interarrival_time);
    next_arr_time = expon(mean_interarrival_time);

    printf("Enter mean_service_time\n");
	scanf("%f", &mean_service_time);

	printf("Enter num_delays_required\n");
	scanf ("%d", &Num_Delays_Required);

	printf("Enter total number of service desks\n");
	scanf ("%d", &total_num_server);

	double next_dept_time [total_num_server+1];
    int server_status [total_num_server+1];
    double Time_arrival_sys[total_num_server];


    printf("Enter K\n");
    scanf ("%d", &K);

    int Q_limit = K - total_num_server;
    double Time_Arrival[Q_limit+1];


    for (int i=1;i<Q_limit+1;i++){
        Time_Arrival[i] = 0;
    }



	 for (int i=1;i<total_num_server+1;i++){ // initialized all server desks to 10^30
        next_dept_time[i] = pow(10,30);
        server_status[i] = Idle;
    }
// Initialize end

   while (Num_Custs_Delayed < Num_Delays_Required){
        if (next_arr_time > minimum(next_dept_time, total_num_server, &Server_num)){  // depature event trigger

            depature (&sim_time, &Time_Since_Last_Event,&Last_Event_Time, &Area_Under_Q, &Num_In_Queue, server_status, next_dept_time,
                &q_Delay,Time_Arrival,  &Total_q_Delay,&Num_Custs_Delayed,&mean_service_time, &Server_num,  &index,  Time_arrival_sys,  &sys_delay,  &total_sys_delay,  total_num_server);


        }

        else {  // arrival event trigger
            arrival (&sim_time, &total_num_server, &Num_In_Queue, &Num_Custs_Delayed, &next_arr_time, &Area_Under_Q,
             &Last_Event_Time, &Time_Since_Last_Event, Time_Arrival, server_status, next_dept_time, &mean_interarrival_time, &mean_service_time,  &index,  Time_arrival_sys, &Q_limit);

        }

    }

    int i=1;

    while (index !=-1){
        sim_time = next_dept_time[i];
        i++;
        sys_delay = sim_time - Time_arrival_sys[0];
        total_sys_delay += sys_delay;
        index--;
    }

// report
    printf("\n");
    printf("sim_time= %.8f \n",sim_time);
    printf("num_custs_delayed= %d \n", Num_Custs_Delayed);
    printf("num_in_queue= %d \n",Num_In_Queue);

    avg_q_delay = Total_q_Delay/Num_Custs_Delayed;
    printf("avg_q_delay= %.8f \n",avg_q_delay);

    avg_num_in_queue = Area_Under_Q/sim_time;
    printf("avg_num_in_queue= %.8f \n",avg_num_in_queue);
    avg_sys_delay = total_sys_delay/Num_Custs_Delayed;
    printf ("avg_sys_delay= %.8f \n", avg_sys_delay);



}

double minimum(double *next_dept_time, int total_num_server , int *Server_num){
    double min = next_dept_time[1];
    *Server_num = 1;

    for (int i=2;i<total_num_server+1 ;i++){
        if (min > next_dept_time[i]){
            min=next_dept_time[i];
            *Server_num = i;
        }
    }

    return min;

}


void arrival (double *sim_time, int *total_num_server, int *Num_In_Queue, int *Num_Custs_Delayed, double *next_arr_time, double *Area_Under_Q,
              float*Last_Event_Time, float *Time_Since_Last_Event, double *Time_Arrival, int *server_status, double *next_dept_time, float *mean_interarrival_time, float *mean_service_time, int *index, double* Time_arrival_sys, int * Q_limit){

*sim_time = *next_arr_time;
*next_arr_time = *sim_time + expon(*mean_interarrival_time);
*Time_Since_Last_Event = *sim_time - *Last_Event_Time;
*Last_Event_Time = *sim_time;
*Area_Under_Q += (*Num_In_Queue) * (*Time_Since_Last_Event);


for (int i=1; i<*total_num_server+1;i++){
    if (server_status[i] == Idle){
           (*Num_Custs_Delayed)++;
           server_status[i]=Busy;
           next_dept_time[i]= *sim_time + expon(*mean_service_time);
           (*index) ++;
           Time_arrival_sys[*index] = *sim_time;
           return;
    }

}


    (*Num_In_Queue)++;
    if (*Num_In_Queue> *Q_limit){
        (*Num_In_Queue)--;

    }
    else {
            Time_Arrival[*Num_In_Queue] = *sim_time;
    }
}


void depature (double *sim_time,float *Time_Since_Last_Event,float *Last_Event_Time, double *Area_Under_Q, int *Num_In_Queue, int *server_status, double *next_dept_time,
                double *q_Delay,double *Time_Arrival,  double *Total_q_Delay,int *Num_Custs_Delayed,float *mean_service_time, int *Server_num, int *index, double* Time_arrival_sys, double *sys_delay, double *total_sys_delay, int total_num_server){

int  total_servers=total_num_server;

*sim_time = next_dept_time[*Server_num];
*Time_Since_Last_Event = *sim_time - *Last_Event_Time;
*Last_Event_Time = *sim_time;
*Area_Under_Q += (*Num_In_Queue)*(*Time_Since_Last_Event);

if (*Num_In_Queue ==0){
    server_status[*Server_num] = Idle;
    next_dept_time [*Server_num] = pow(10,30);
    *sys_delay = *sim_time - Time_arrival_sys[0];
    *total_sys_delay += *sys_delay;
    (*index)--;
    move (Time_arrival_sys, &total_servers);

}
 else{
    (*Num_In_Queue)--;
    *q_Delay=*sim_time-Time_Arrival[1];
   *sys_delay = *sim_time-Time_arrival_sys[0];
    *Total_q_Delay += *q_Delay;
    *total_sys_delay +=*sys_delay;
    (*index)--;

    move(Time_arrival_sys, &total_servers);
    (*index) ++;
    Time_arrival_sys[*index] = Time_Arrival[1];

    (*Num_Custs_Delayed)++;
    next_dept_time[*Server_num] = *sim_time + expon(*mean_service_time);

    for (int i=1; i<=*Num_In_Queue; i++){
        Time_Arrival[i]=Time_Arrival[i+1];
    }
 }

}




void move (double* Time_arrival_sys, int *total_servers){
    for (int i = 0; i < *total_servers ;i++){
        Time_arrival_sys[i] = Time_arrival_sys[i+1];
    }
}



float expon (float mean){

    return -mean* log(lcgrand(1));

}
