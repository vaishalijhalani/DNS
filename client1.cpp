#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <iostream>
#include <vector>
//#include <unordered_map>
#include "connection.h"
#include "hashmap.h"

using namespace std;


#define PORT 8080


int count = 0;
vector<float> avg_res;
vector<int> avg_throughput;
pthread_mutex_t mutex;

char url[11][100] ={
"www.c.d.com",
"www.asd.com",
"www.helloe.com",
"www.hellof.com",
"www.hellog.com",
"www.helloh.com",
"www.helloe.in",
"www.iitb.ac.in",
"www.cse.iitb.ac.in",
"www.asdf.in",
"www.fg.ina"};

void * threadfunc( void * threadid)
{

    float response,avgresponse_time;
    int * thread = (int*)threadid;
    vector<pair<float, float> > latency;
    long int throughput = 0;
    float time1 = (float)thread[1];
    int count1 = 0;
    int sock = client_initialise(PORT);
    clock_t startTime = clock();
    float start = ((float)startTime)/CLOCKS_PER_SEC;

    while(1)
    { 

        char  * url_search  = (char *)malloc (1024 * sizeof(char));
        char read_buffer[1024] = {0};
        char line[1024] = {0};
        clock_t measure1  = clock();
        send(sock,url[count],1024, 0);
        while(read(sock,read_buffer,1024)<0);
        clock_t measure2  = clock();
        throughput++;
        count1++;
        if(count1%50 == 0)
        latency.push_back(make_pair((float)measure1,(float)measure2));
        float secsElapsed = ((float)clock())/CLOCKS_PER_SEC - start;
        //cout << secsElapsed << " while running\n";
        float time1 = (float)thread[1];
        if(secsElapsed > time1 )
        {
            break;
        }

        count++;
        if(count > 10) count = 0;
        //shutdown(sock,SHUT_RDWR);

    }

    for(vector<pair<float, float> >:: iterator iter = latency.begin(); (iter != latency.end()); ++iter)
        response += ((iter->second)-(iter->first))/CLOCKS_PER_SEC;

    float temp = response/latency.size();
    int temp2 = throughput/time1;

    //cout << temp << " " << temp2 << endl;

    pthread_mutex_lock(&mutex);
    avg_res.push_back(temp);
    avg_throughput.push_back(temp2);
    pthread_mutex_unlock(&mutex);


    close(sock);

    

}
  
int main(int argc, char const *argv[])
{
    int i,c;
    int num1[2];
    int * i1 = (int *)malloc(sizeof *i1);
    int num = atoi(argv[1]);
    pthread_t pth[num];
    float response_ans = 0;
    float throughput_ans = 0;
    //long long int experiment = atoi(argv[3]);
    int errCode[num] = {0}; 


    for(int j =0 ;j < num ;j++)
    {
     
        //clock_t startTime1 = clock();
            num1[0]=j;
            num1[1]=atoi(argv[2]);

            if((errCode[j] = pthread_create(&pth[j], NULL, threadfunc, num1 )) != 0)
                std::cout << "thread is not created\n";
            //sleep(1); 
            count = 0;

    }

        
    for( int i1 = 0; i1 < num; i1++ ) 
    {

        if(errCode[i1] == 0)
        {
            errCode[i1] = pthread_join(pth[i1], NULL);
            if (errCode[i1]) 
            {
                cout << "Error:unable to join," << errCode[i1] << endl;
                exit(-1);
            }
        }
    }


    for(vector<float>:: iterator iter = avg_res.begin(); (iter != avg_res.end()); ++iter)
        response_ans += *iter;

    response_ans /= avg_res.size();

    for(vector<int>:: iterator iter = avg_throughput.begin(); (iter != avg_throughput.end()); ++iter)
        throughput_ans += *iter; 

    throughput_ans /= (avg_throughput.size());
    
    cout << "\n\n average throughput: " << throughput_ans << endl;

    cout << "\n average response time: " << response_ans << endl;



    
        //float secsElapsed1 = (float)(clock() - startTime1)/CLOCKS_PER_SEC;
        //std::cout << secsElapsed1*1000 << endl;

        //sleep(2);
      


   pthread_exit(NULL);

   return 0;
}
