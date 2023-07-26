#include <stdio.h>
#include <stdlib.h>
 
#define MAX 100
 
typedef struct
{
    int pid;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int arrival_time;
    int finish_time;
    int priority;
    int remaining_time;
} Process;

void swap(int *b,int *c);
int findlargest(Process p[],int at);
void print_table(Process p[], int n);
void print_gantt_chart(Process p[], int n);
void print_table_srtf(Process p[], int n, int tem[]);
void print_gantt_chart_srtf(Process p[], int n,int total,int c[],int m,int d[]);
void print_table_lrtf(Process p[], int n, int tem[],int prefinaltotal,int totaltime);
void print_gantt_chart_lrtf(Process p[], int n,int total,int c[],int m,int d[]);
void first_come_first_serve();
void shortest_job_first();
void round_robin();
void priority();
void shortest_remaining_time_first();
void longest_remaining_time_first();


int main() {

   printf("\n----------------------- All CPU Scheduling Algorithms At One Place -------------------------\n");
   printf("-----------------------          BY : 21BCE228 | 21BCE231            -------------------------\n\n");
   printf("1) First-Come First-Served\n");
   printf("2) Shortest Job First\n");
   printf("3) Round Robin Scheduling\n");
   printf("4) Priority Scheduling\n");
   printf("5) Shortest Remaining Time First\n");
   printf("6) Longest remaining time first\n\n");
   printf("Which CPU Algorithm you want to perform : ");

   int n;
   scanf("%d",&n);
   switch(n) {
      case 1: 
         printf("\nFirst-Come First-Served\n");
         first_come_first_serve();
         break;
      case 2:
         printf("\nShortest Job First\n");
         shortest_job_first();
         break;
      case 3:
         printf("\nRound Robin Scheduling\n");
         round_robin();
         break;
      case 4:
         printf("\nPriority Scheduling\n");
         priority();
         break;
      case 5:
         printf("\nShortest Remaining Time First\n");
         shortest_remaining_time_first();
         break;
      case 6:
         printf("\nLongest remaining time first\n");
         longest_remaining_time_first();
         break;
      default:
         printf("\nPlease Enter Valid Option !!!\n");
   }

   return 0;
}

void print_table(Process p[], int n)
{
    int i;
 
    puts("+-----+--------------+------------+------------ +--------------+-----------------+");
    puts("| PID | Arrival Time | Burst Time | Finish Time | Waiting Time | Turnaround Time |");
    puts("+-----+--------------+------------+-------------+--------------+-----------------+");
 
    for(i=0; i<n; i++) {
        printf("| %2d  |      %2d      |     %2d     |      %2d     |      %2d      |        %2d       |\n"
               , p[i].pid,p[i].arrival_time, p[i].burst_time,p[i].finish_time, p[i].waiting_time, p[i].turnaround_time );
        puts("+-----+--------------+------------+-------------+--------------+-----------------+");
    }
 
}

void swap(int *b,int *c)
{
    int tem;
    tem=*c;
    *c=*b;
    *b=tem;
}

void print_gantt_chart(Process p[], int n)
{
    int i, j;
    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=1;j<=p[i].burst_time;j++)
        {
            printf("p[%d] ",p[i].pid);
        }
    }
    printf("\n");  
}

void first_come_first_serve() {
   Process p[MAX];
    int i, j, n;
    int sum_waiting_time = 0, sum_turnaround_time=0;
    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Number of processes: %d\n", n);
    printf("Enter Arrival time and burst time for each process:\n");
    for(i=0; i<n; i++) {
        printf("P[%d] : ", i+1);
        fscanf(fp, "%d %d", &p[i].arrival_time, &p[i].burst_time);
        printf("%d %d\n",p[i].arrival_time,p[i].burst_time);
        p[i].pid = i+1;
        p[i].waiting_time = p[i].turnaround_time = 0;
    }
    fclose(fp);

    int pos=0,temp=0;
    for(int i=0;i<n;i++)
    {
        pos=i;
        for(int j=i+1;j<n;j++)
        {
            if(p[j].arrival_time < p[pos].arrival_time)
                pos=j;
        }
       
        temp=p[i].burst_time;
        p[i].burst_time=p[pos].burst_time;
        p[pos].burst_time=temp;
 
        temp=p[i].pid;
        p[i].pid=p[pos].pid;
        p[pos].pid=temp;

        temp=p[i].arrival_time;
        p[i].arrival_time=p[pos].arrival_time;
        p[pos].arrival_time=temp;
    }
    
    // calculate waiting time and turnaround time
    p[0].finish_time=p[0].burst_time;
    p[0].turnaround_time = p[0].burst_time;
 
    for(i=1; i<n; i++) {
        p[i].finish_time=p[i-1].finish_time + p[i].burst_time;
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
 
    // calculate sum of waiting time and sum of turnaround time
    for(i=0; i<n; i++) {
            sum_waiting_time += p[i].waiting_time;
            sum_turnaround_time += p[i].turnaround_time;
    }
  
    // print table
    puts(""); // Empty line
    print_table(p, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);
 
    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
    print_gantt_chart(p, n);
}

void print_table_sjf(Process p[], int n,int pi[],int bt[],int at[])
{
    int i;
 
    puts("+-----+--------------+------------+-------------+--------------+-----------------+");
    puts("| PID | Arrival Time | Burst Time | Finish Time | Waiting Time | Turnaround Time |");
    puts("+-----+--------------+------------+-------------+--------------+-----------------+");
 
    for(i=0; i<n; i++) {
        printf("| %2d  |      %2d      |    %2d      |      %2d     |      %2d      |        %2d       |\n"
               , pi[i],at[i], bt[i],p[i].finish_time, p[i].waiting_time, p[i].turnaround_time );
        puts("+-----+--------------+------------+-------------+--------------+-----------------+");
    }
 
}

void shortest_job_first() {
    Process p[MAX];
    int i, j, n,check_ar=0;
    int sum_waiting_time = 0, sum_turnaround_time=0;
    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Number of processes: %d\n", n);
    int at[n],bt[n],pi[n];
    printf("Enter Arrival time and burst time for each process:\n");
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        pi[i]=p[i].pid;
        printf("P[%d] : ", i+1);
        fscanf(fp, "%d %d", &p[i].arrival_time, &p[i].burst_time);
        printf("%d %d\n",p[i].arrival_time,p[i].burst_time);
        at[i]=p[i].arrival_time;
        if(i==0)
         check_ar=p[i].arrival_time;
         
        if(check_ar!=p[i].arrival_time )
         check_ar=1;

        bt[i]=p[i].burst_time;
        p[i].waiting_time = p[i].turnaround_time = 0;
        // p[i].waiting_time = p[i].turnaround_time  = 0;
    }
    if(check_ar!=0)
    {
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n-i-1;j++)
            {
                if(p[j].arrival_time>p[j+1].arrival_time)
                {
                      swap(&p[j].pid,&p[j+1].pid);
                      swap(&p[j].arrival_time,&p[j+1].arrival_time);
                      swap(&p[j].burst_time,&p[j+1].burst_time);
                }
            }
        }
    }
    if(check_ar!=0)
    {
        p[0].waiting_time=p[0].arrival_time;
        p[0].turnaround_time=p[0].burst_time-p[0].arrival_time;
        p[0].finish_time=p[0].turnaround_time;
        sum_waiting_time=sum_waiting_time+p[0].waiting_time;
        sum_turnaround_time=sum_turnaround_time+p[0].turnaround_time;
        for(int i=1;i<n;i++)
        {
            int min=p[i].burst_time;
            for(int j=i+1;j<n;j++)
            {
                if(min>p[j].burst_time && p[j].arrival_time<=p[i-1].finish_time)
                {
                      min=p[j].burst_time;
                      swap(&p[i].pid,&p[j].pid);
                      swap(&p[i].arrival_time,&p[j].arrival_time);
                      swap(&p[i].burst_time,&p[j].burst_time);
                }
                
            } 
            // completion time of the process
            p[i].finish_time=p[i-1].finish_time+p[i].burst_time;
            
            // Turn Around Time of the process
            // compl-Arival        
        }
        for(int i=1;i<n;i++)
        {
            p[i].turnaround_time=p[i].finish_time-at[i];
            sum_turnaround_time=sum_turnaround_time+p[i].turnaround_time;
            p[i].waiting_time=p[i].turnaround_time-bt[i];
            sum_waiting_time=sum_waiting_time+p[i].waiting_time;   
        } 
    }
    
    // if all the process are arrived at same time
    else
    {
        p[0].finish_time=p[0].burst_time;
        p[0].turnaround_time=p[0].burst_time;
        for(int i=0;i<n;i++)
        {
            int min=p[i].burst_time;
            for(int j=i+1;j<n;j++)
            {
                if(min>p[j].burst_time && p[j].arrival_time<=p[i].finish_time)
                {
                    min=p[j].burst_time;
                      swap(&p[i].pid,&p[j].pid);
                      swap(&p[i].arrival_time,&p[j].arrival_time);
                      swap(&p[i].burst_time,&p[j].burst_time);
                }
                
            }    
            if(i>0){        
             // completion time of the process
            p[i].finish_time=p[i-1].finish_time+p[i].burst_time;
            
            // Turn Around Time of the process
            // compl-Arrival
            p[i].turnaround_time=p[i].finish_time-p[i].arrival_time;
            p[i].waiting_time=p[i].turnaround_time - p[i].burst_time;
            }
            sum_waiting_time=sum_waiting_time+p[i].waiting_time;
            sum_turnaround_time=sum_turnaround_time+p[i].turnaround_time;
        }
    }
    
    // print table
    puts(""); // Empty line
    print_table_sjf(p, n,pi,bt,at);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);
 
    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          *****          ");
    print_gantt_chart(p, n);

}

void print_gantt_chart_rr(Process p[], int n,int quantum)
{
     int t[n],max=0,k=0,remain=n,flag=0;
     int sum_waiting_time=0,sum_turnaround_time=0;
     printf("0");
       for(int time=0,i=0;remain!=0;)
       {
              if(p[i].remaining_time<=quantum && p[i].remaining_time>0)
              {
                     time = time + p[i].remaining_time;
                     printf(" -> [P%d] <- %d",p[i].pid,time);
                     p[i].remaining_time=0;
                     flag=1;
              }
              else if(p[i].remaining_time > 0)
              {
                     p[i].remaining_time = p[i].remaining_time - quantum;
                     time = time + quantum;
                     printf(" -> [P%d] <- %d",p[i].pid,time);
              }
              if(p[i].remaining_time==0 && flag==1)
              {
                     remain--;
                     p[i].turnaround_time = time-p[i].arrival_time;
                     p[i].waiting_time = time-p[i].arrival_time-p[i].burst_time;
                     sum_turnaround_time = sum_turnaround_time + p[i].turnaround_time;
                     sum_waiting_time = sum_waiting_time + p[i].waiting_time;
                     flag=0;
              }
              if(i==n-1)
                     i=0;
              else if(p[i+1].arrival_time <= time)
                     i++;
              else
                     i=0;
       }
       for(int i=0;i<n;i++)
       {
              p[i].finish_time=p[i].turnaround_time + p[i].arrival_time;
       }
       puts("");
        // print table
    puts(""); // Empty line
    print_table(p, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);
    
}

void round_robin() {
    Process p[MAX];
    int i, j, n,check_ar=0,quantum;
    int sum_waiting_time = 0, sum_turnaround_time=0;
    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Enter Time Slice OR Quantum Number : ");
    fscanf(fp, "%d",&quantum);
    printf("Time quantum is : %d",quantum);
    printf("Number of processes: %d\n", n);
    int at[n],bt[n],pi[n];
    printf("Enter Arrival time for each process:\n");
    
    printf("Enter Arrival time, Burst time for each process:\n");
    for(i=0; i<n; i++) {
        printf("P[%d] : ", i+1);
        fscanf(fp, "%d %d", &p[i].arrival_time, &p[i].burst_time);
        printf("%d %d\n",p[i].arrival_time,p[i].burst_time);
        p[i].pid = i+1;
        p[i].waiting_time = p[i].turnaround_time = 0;
        p[i].remaining_time=p[i].burst_time;
    }
    fclose(fp);

       int temp=0,pos=0;
    for(int i=0;i<n;i++)
    {
        pos=i;
        for(int j=i+1;j<n;j++)
        {
            if(p[j].arrival_time < p[pos].arrival_time)
                pos=j;
        }
       
        temp=p[i].burst_time;
        p[i].burst_time=p[pos].burst_time;
        p[pos].burst_time=temp;
 
        temp=p[i].pid;
        p[i].pid=p[pos].pid;
        p[pos].pid=temp;

        temp=p[i].arrival_time;
        p[i].arrival_time=p[pos].arrival_time;
        p[pos].arrival_time=temp;
    }
    puts(""); // Empty line
    puts(" ------------------ GANTT CHART  ------------------ ");
    puts(""); 
    print_gantt_chart_rr(p, n,quantum);
}

void print_table_priority(Process p[], int n,int pi[],int bt[],int at[])
{
    int i;
 
    puts("+-----+--------------+------------+----------+-------------+--------------+-----------------+");
    puts("| PID | Arrival Time | Burst Time | Priority | Finish Time | Waiting Time | Turnaround Time |");
    puts("+-----+--------------+------------+----------+-------------+--------------+-----------------+");
 
    for(i=0; i<n; i++) {
        printf("| %2d  |      %2d      |     %2d     |    %2d    |      %2d     |      %2d      |        %2d       |\n"
               , pi[i],at[i], bt[i],p[i].priority,p[i].finish_time, p[i].waiting_time, p[i].turnaround_time );
        puts("+-----+--------------+------------+----------+-------------+--------------+-----------------+");
    }
 
}

void priority() {
     Process p[MAX];
    int i, j, n,check_ar=0;
    int sum_waiting_time = 0, sum_turnaround_time=0;
    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Number of processes: %d\n", n);
    int at[n],bt[n],pi[n];
    printf("Enter Arrival time, Burst Time and Priority for each process:\n");
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        pi[i]=p[i].pid;
        printf("P[%d] : ", i+1);
        fscanf(fp, "%d %d %d", &p[i].arrival_time, &p[i].burst_time, &p[i].priority);
        printf("%d %d %d\n",p[i].arrival_time,p[i].burst_time,p[i].priority);
        at[i]=p[i].arrival_time;
        bt[i]=p[i].burst_time;
        p[i].waiting_time = p[i].turnaround_time = 0;
        if(i==0)
         check_ar=p[i].arrival_time;
         
        if(check_ar!=p[i].arrival_time )
         check_ar=1;
    }
    if(check_ar!=0)
    {
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n-i-1;j++)
            {
                if(p[j].arrival_time>p[j+1].arrival_time)
                {
                      swap(&p[j].pid,&p[j+1].pid);
                      swap(&p[j].arrival_time,&p[j+1].arrival_time);
                      swap(&p[j].burst_time,&p[j+1].burst_time);
                      swap(&p[j].priority,&p[j+1].priority);
                }
            }
        }
    }
    
    // logic of Priority scheduling ( non preemptive) algo
    // if all the process are arrived arrival_time different time
    if(check_ar!=0)
    {
        p[0].turnaround_time=p[0].burst_time;
        // p[0].finish_time for completion time
        p[0].finish_time=p[0].burst_time;
        p[0].waiting_time=p[0].turnaround_time - p[0].burst_time;
        sum_waiting_time=sum_waiting_time+p[0].waiting_time;
        sum_turnaround_time=sum_turnaround_time+p[0].turnaround_time;
        for(int i=1;i<n;i++)
        {
            int min=p[i].priority;
            for(int j=i+1;j<n;j++)
            {
                if(min>p[j].priority && p[j].arrival_time<=p[i-1].finish_time)
                {
                      min=p[j].priority;
                      swap(&p[i].pid,&p[j].pid);
                      swap(&p[i].arrival_time,&p[j].arrival_time);
                      swap(&p[i].burst_time,&p[j].burst_time);
                      swap(&p[i].priority,&p[j].priority);
                      
                }
                
            } 
            p[i].finish_time=p[i-1].finish_time+p[i].burst_time;          
        }
        for(int i=1;i<n;i++)
        {
            p[i].turnaround_time=p[i].finish_time-at[i];
            sum_turnaround_time=sum_turnaround_time+p[i].turnaround_time;
            p[i].waiting_time=p[i].turnaround_time-bt[i];
            sum_waiting_time=sum_waiting_time+p[i].waiting_time;   
        } 
    }
    
    // if all the process are arrived at same time
    else
    {
        p[0].finish_time=p[0].burst_time;
        p[0].turnaround_time=p[0].burst_time;
        for(int i=0;i<n;i++)
        {
            int min=p[i].priority;
            for(int j=i+1;j<n;j++)
            {
                if(min>p[j].priority && p[j].arrival_time<=p[i].finish_time)
                {
                    min=p[j].priority;
                      swap(&p[i].pid,&p[j].pid);
                      swap(&p[i].arrival_time,&p[j].arrival_time);
                      swap(&p[i].burst_time,&p[j].burst_time);
                       swap(&p[i].priority,&p[j].priority);
                }
                
            }    
            if(i>0){        
             // completion time of the process
            p[i].finish_time=p[i-1].finish_time+p[i].burst_time;
            
            // Turn Around Time of the process
            // compl-Arrival
            p[i].turnaround_time=p[i].finish_time-p[i].arrival_time;
            p[i].waiting_time=p[i].turnaround_time - p[i].burst_time;
            }
            sum_waiting_time=sum_waiting_time+p[i].waiting_time;
            sum_turnaround_time=sum_turnaround_time+p[i].turnaround_time;
        }
    }
    
    // print table
    puts(""); // Empty line
    print_table_priority(p, n,pi,bt,at);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);
 
    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          *****          ");
    print_gantt_chart(p, n);
}

void print_table_srtf(Process p[], int n, int tem[])
{
    int i;
 
    puts("+-----+--------------+------------+------------ +--------------+-----------------+");
    puts("| PID | Arrival Time | Burst Time | Finish Time | Waiting Time | Turnaround Time |");
    puts("+-----+--------------+------------+-------------+--------------+-----------------+");
 
    for(i=0; i<n; i++) {
        printf("| %2d  |      %2d      |     %2d     |      %2d     |      %2d      |        %2d       |\n"
               , p[i].pid,p[i].arrival_time, tem[i],p[i].finish_time, p[i].waiting_time, p[i].turnaround_time );
        puts("+-----+--------------+------------+-------------+--------------+-----------------+");
    }
}

void print_gantt_chart_srtf(Process p[], int n,int total,int c[],int m,int d[])
{
    int i, j,l[m+1];
    // printing process id in the middle
    c[m]=total;
   for(int i=0;i<=m;i++)
   {
     l[i]=c[i];
     c[i]=c[i+1]-c[i];
   }
   printf("\n");
   for(int i=0;i<m;i++)
   {
     for(int j=0;j<c[i];j++)
     {
        printf("p[%d] ",p[d[i]].pid);
     }
   }
   printf("\n");    
}

void shortest_remaining_time_first() {
    Process p[MAX];
    int i, j, n,tem[MAX];
    int sum_waiting_time = 0, sum_turnaround_time=0,total=0;
    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Number of processes: %d\n", n);
    printf("Enter Arrival time and burst time for each process:\n");
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        printf("P[%d] : ", i+1);
        fscanf(fp, "%d %d", &p[i].arrival_time, &p[i].burst_time);
        printf("%d %d\n",p[i].arrival_time,p[i].burst_time);
        tem[i]=p[i].burst_time;
        total+=p[i].burst_time;
        // p[i].waiting_time = p[i].turnaround_time  = 0;
    }
    int pos=0,temp=0,pk=n-1,c[MAX],m=0,d[MAX];
    for(int i=0;i<n;i++)
    {
        pos=i;
        for(int j=i+1;j<n;j++)
        {
            if(p[j].arrival_time < p[pos].arrival_time)
                pos=j;
        }
       
        temp=p[i].burst_time;
        p[i].burst_time=p[pos].burst_time;
        p[pos].burst_time=temp;
 
        temp=p[i].pid;
        p[i].pid=p[pos].pid;
        p[pos].pid=temp;

        temp=p[i].arrival_time;
        p[i].arrival_time=p[pos].arrival_time;
        p[pos].arrival_time=temp;
    }
    int small=0,k=0,t[MAX];
    for (i = 0; i < total; i++) 
    { 
    small = 3200; 
    for (j = 0; j < n; j++) 
    { 
      if ((p[j].burst_time != 0) && (p[j].arrival_time <= i) && (p[j].burst_time < small)) 
      { 
        small = p[j].burst_time; 
        k = j; 
      } 
    }
    if(pk!=k || i==0){
        c[m]=i;
        d[m]=k;
        m++;
    } 
    p[k].burst_time--; 
    t[i] = k; 
    pk=k;
  } 
 
  k = 0; 
 
  for (i = 0; i < total; i++) 
  { 
    for (j = 0; j < n; j++) 
    { 
      if (t[i] == j) 
      { 
        p[j].finish_time = i; 
        p[j].waiting_time++; 
      } 
    } 
  } 
  for(int i=0;i<n;i++)
  {
    p[i].finish_time+=1;
    p[i].turnaround_time=p[i].finish_time - p[i].arrival_time;
    p[i].waiting_time = p[i].turnaround_time - tem[i];
    sum_turnaround_time+=p[i].turnaround_time;
    sum_waiting_time+=p[i].waiting_time;
  }
    
    // print table
    puts(""); // Empty line
    print_table_srtf(p, n, tem);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) n);
 
    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          *****          ");
    print_gantt_chart_srtf(p, n,total,c,m,d);
}
 
void print_ganttchart_lrtf(Process *processes, int n) {
    int i, j, total_burst_time = 0;
    for (i = 0; i < n; i++) {
        total_burst_time += processes[i].burst_time;
    }

    int *chart = (int *) malloc(sizeof(int) * total_burst_time);

    for (i = 0; i < total_burst_time; i++) {
        int max_remaining_time = -1, max_index = -1;
        for (j = 0; j < n; j++) {
            if (processes[j].arrival_time <= i && processes[j].remaining_time > 0) {
                if (processes[j].remaining_time > max_remaining_time) {
                    max_remaining_time = processes[j].remaining_time;
                    max_index = j;
                }
            }
        }
        if (max_index != -1) {
            processes[max_index].remaining_time--;
            chart[i] = processes[max_index].pid;
            if (processes[max_index].remaining_time == 0) {
                processes[max_index].turnaround_time = i - processes[max_index].arrival_time + 1;
                processes[max_index].waiting_time = processes[max_index].turnaround_time - processes[max_index].burst_time;
            }
        } else {
            chart[i] = -1;
        }
    }

    printf("\n\n*****Gantt Chart:*****\n");
    for (i = 0; i < total_burst_time; i++) {
        if (chart[i] == -1) {
            printf(" Idle ");
        } else {
            printf(" P%d ", chart[i] + 1);
        }
    }
    printf("\n\n");
    free(chart);
}

void longest_remaining_time_first() {
    int i, n;

    FILE *fp;
    fp = fopen("schedulinginput.txt", "r");
    if(fp == NULL) {
        printf("Error: Unable to open file %s\n", "schedulinginput.txt");
        return;
    }
    fscanf(fp, "%d", &n);
    printf("Number of processes: %d\n", n);
    Process *processes = (Process *) malloc(sizeof(Process) * n);
    printf("Enter Arrival time and burst time for each process:\n");
    for(i=0; i<n; i++) {
        processes[i].pid = i+1;
        fscanf(fp, "%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        printf("%d %d\n",processes[i].arrival_time,processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    int total_waiting_time = 0, total_turnaround_time = 0;
    print_ganttchart_lrtf(processes, n);

    for (i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    puts("+-----+--------------+------------+------------ +--------------+----");
    puts("| PID | Arrival Time | Burst Time | Waiting Time | Turnaround Time |");
    puts("+-----+--------------+------------+-------------+--------------+----");
 
    for(int i=0; i<n; i++) {
        printf("| %2d  |      %2d      |     %2d     |      %2d      |        %2d       |\n"
               , i+1,processes[i].arrival_time, processes[i].burst_time,processes[i].waiting_time, processes[i].turnaround_time );
        puts("+-----+--------------+------------+-------------+--------------+----");
    }

    printf("\nAverage Waiting Time: %.2f\n", (float) total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float) total_turnaround_time / n);

    free(processes);

}