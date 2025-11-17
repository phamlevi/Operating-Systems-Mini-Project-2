# include "types.h"
# include "stat.h"
# include "user.h"
# include "pstat.h"
# define NCHILD 5
static void spawn(char*prog,char*arg){
    char*argv[3]={prog,arg,0};
    if(fork()==0){ exec(prog,argv); printf(1,"exec %s failed\n",prog); exit(); }
}
int main(int argc,char*argv[]){
    struct pstat ps; int i;
    int start=uptime();
    spawn("cpubound","600000");
    spawn("cpubound","600000");
    spawn("iobound","40");
    spawn("iobound","40");
    spawn("mixed","30");
    for(i=0;i<NCHILD;i++) wait();
    int end=uptime(); int makespan=end-start;
    if(getpinfo(&ps)<0){ printf(1,"getpinfo failed\n"); exit(); }

    int total_resp=0,resp_cnt=0,total_turn=0,turn_cnt=0,busy_ticks=0;
    printf(1,"\nFINAL STATS (pid name ticks wait start first end)\n");
    for(i=0;i<NPROC;i++){ if(ps.inuse[i] && ps.pid[i]>2){
        int turnaround=(ps.end_tick[i]>0?ps.end_tick[i]:end)-ps.start_tick[i];
        int response=(ps.first_run[i]>=0?ps.first_run[i]:end)-ps.start_tick[i];
        printf(1,"%d %-12s %d %d %d %d %d\n",ps.pid[i],ps.name[i],ps.ticks[i],
        ps.wait_ticks[i],ps.start_tick[i],ps.first_run[i],ps.end_tick[i]);
        if(ps.end_tick[i]>0){ total_turn+=turnaround; turn_cnt++; }
        if(ps.first_run[i]>=0){ total_resp+=response; resp_cnt++; }
        busy_ticks+=ps.ticks[i];
    }}
    int avg_resp=(resp_cnt? total_resp/resp_cnt:-1);
    int avg_turn=(turn_cnt? total_turn/turn_cnt:-1);
    int util_permille=(makespan>0?(busy_ticks*1000)/makespan:0);
    printf(1,"\nMAKESPAN (ticks): %d\n",makespan);
    printf(1,"AVG RESPONSE (ticks): %d\n",avg_resp);
    printf(1,"AVG TURNAROUND (ticks): %d\n",avg_turn);
    printf(1,"CPU UTILIZATION: %d.%d%%\n",util_permille/10,util_permille%10);
    exit();
}