
#include <cstdio>
#include <iostream>
#include <ctime>
#include <stdlib.h>

#include "para.h"

using namespace std;

void Normalize(std::vector<struct Result> &Final)
{
	struct Result tmp;

	if(DA > Final.size()-1)
		return;

	tmp = Final[DA];

	for(int i = 0; i < Final.size(); i++)
	{
		Final[i].value[RAW_TRACE] /=  tmp.value[RAW_TRACE];
		Final[i].value[ALL_REQ_READ] /=  tmp.value[ALL_REQ_READ];
		Final[i].value[ALL_REQ_READ_RAN] /=  tmp.value[ALL_REQ_READ_RAN];
		Final[i].value[ALL_RANDOM] /=  tmp.value[ALL_RANDOM];
	//   printf("%-2d %-15s %10.5f %10.5f %10.5f\n", Final[i].policy, Final[i].p_name, Final[i].value[ALL_REQ_READ], Final[i].value[ALL_REQ_READ_RAN], Final[i].value[ALL_RANDOM]);
	    printf("%-2d %-5s %10.5f %10.5f %10.5f %10.5f %10.5f\n", Final[i].policy, Final[i].p_name, (float)Final[i].total_latency / tmp.total_latency, Final[i].value[RAW_TRACE], Final[i].value[ALL_REQ_READ], Final[i].value[ALL_REQ_READ_RAN], Final[i].value[ALL_RANDOM]);
	}

}


void Output_Statictics(int trace_start, int trace_end, int degraded, int node_num, int k, int placement)
{
	char *suffix = (char *)".sts";

	char *R_output = (char *)malloc(500 * sizeof(char));
	struct Result tmp;

	char *failure = NULL;
	if(degraded == 0)
		failure = (char *)"Normal";
	else
		failure = (char *)"Degraded";

	char *p_policy = NULL;
	if(placement == 0)
		p_policy = (char *)"BA";
	else if (placement == 1)
		p_policy = (char *)"RA";






	sprintf(R_output, "[%d, %d]%s%d%s%d%s%s%s%s%s", node_num, k, file_prefix, trace_start, "-", trace_end, suffix, "_", failure, "_", p_policy);

	FILE *R_out;

    //open result
    R_out = fopen(R_output, "wb+");

    char result_buf[4096];

	if(degraded == 1)
	{
		sprintf(result_buf,"\n-------------------Degraded total results!-----------------\n");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		sprintf(result_buf, "Ave_latency_read :  %-8.3f %-8.3f\n",(float) degraded_latency[0]/degraded_latency[1],(float) 1);
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		sprintf(result_buf, "Ave_G_access_read:  %-8.3f %-8.3f\n", degraded_G_access[0], degraded_G_access[1]);
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);


		sprintf(result_buf,"\n-------------------CDF!-----------------\n");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		for(int i = 0; i < cdf.size(); i++)
		{
			char * str;

			if(i == 0)
			{
				str = (char *)"Baseline ";
			}
			else
				str = (char *)"EC       ";

			sprintf(result_buf,"%s: ", str);
			result_buf[strlen(result_buf)] = '\0';
			fwrite(result_buf, 1, strlen(result_buf), R_out);


			for(int j = 0; j < cdf[i].size(); j++)
			{
				sprintf(result_buf, "%6.3f  ", (float)cdf[i][j]/cdf[i][cdf[i].size()-1]);
				result_buf[strlen(result_buf)] = '\0';
				fwrite(result_buf, 1, strlen(result_buf), R_out);
			}

			sprintf(result_buf,"\n");
			result_buf[strlen(result_buf)] = '\0';
			fwrite(result_buf, 1, strlen(result_buf), R_out);

		}





	}
	else
	{

		sprintf(result_buf,"%-8s %10s %8s %8s %8s %8s %8s \n", "Index", "R_ID", "Size", "l_b", "l_r", "GA_b", "GA_r");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);


		for(int i = 0; i < R_Req_Tbl[0].size(); i++)
		{
	//    	if(R_Req_Tbl[0][i].Lasting_time != R_Req_Tbl[1][i].Lasting_time)
			{
				sprintf(result_buf,"%-8d %10s %8lu %8lld %8lld %8.3f %8.3f\n", i, R_Req_Tbl[0][i].RequestID, R_Req_Tbl[0][i].datablks.size(), R_Req_Tbl[0][i].Lasting_time, R_Req_Tbl[1][i].Lasting_time, load_balancer_access[0][i], load_balancer_access[1][i]);
				result_buf[strlen(result_buf)] = '\0';
				fwrite(result_buf, 1, strlen(result_buf), R_out);
			}
		}


		sprintf(result_buf,"\n-------------------Printout total results!-----------------\n");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);


		sprintf(result_buf, "Total_latency_read :      %-8.3f %-8.3f\n",(float) total_latency[0]/total_latency[1],(float) 1);
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		sprintf(result_buf, "Final_G_access_read:      %-8.3f %-8.3f\n", Final_G_access[0], Final_G_access[1]);
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);


		sprintf(result_buf,"\n-------------------CDF!-----------------\n");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		for(int i = 0; i < cdf.size(); i++)
		{
			char * str;

			if(i == 0)
			{
				str = (char *)"Baseline ";
			}
			else
				str = (char *)"EC       ";

			sprintf(result_buf,"%s: ", str);
			result_buf[strlen(result_buf)] = '\0';
			fwrite(result_buf, 1, strlen(result_buf), R_out);


			for(int j = 0; j < cdf[i].size(); j++)
			{
				sprintf(result_buf, "%6.3f  ", (float)cdf[i][j]/cdf[i][cdf[i].size()-1]);
				result_buf[strlen(result_buf)] = '\0';
				fwrite(result_buf, 1, strlen(result_buf), R_out);
			}

			sprintf(result_buf,"\n");
			result_buf[strlen(result_buf)] = '\0';
			fwrite(result_buf, 1, strlen(result_buf), R_out);

		}
	}

	fclose(R_out);

}

int main(int argc, char** argv) 
{

    trace = atoi(argv[8]);

    if(trace == 0)//desktop  4& 5
    {
    	file_prefix = (char *)"mobi.trace.";
    	cout << "Debug trace" << endl;
    }
    else if(trace == 1)//desktop  4& 5
    {
    	file_prefix = (char *)"trace-desktop-ubuntu-";
    	file_suffix = (char *)".sha1";
    	cout << "Trace: Desktop[4| 5]" << endl;
    }
    else if(trace == 2)// hadoop 2,3,6 8-18,20,21
    {
    	file_prefix = (char *)"hivetpch";
    	file_suffix = (char *)"-ubuntu.trace";
    	cout << "Trace: Hadoop [2,3,6 8-18,20,21]" << endl;

    }else if(trace == 3)//transaction  1 & 2
    {
    	file_prefix = (char *)"tpcc-config";
    	file_suffix = (char *)".sha1";
    	cout << "Trace: Transaction [1 & 2]" << endl;
    }
    else
    {
    	printf("Wrong trace!\n");
    	return 0;
    }

//    char **default_setting;
//    Create_Default_Setting(&default_setting);
//    Argv_Parse(8, default_setting, file_prefix, file_suffix);

	if(Argv_Parse(argc, argv, file_prefix, file_suffix) == -1)
		return -1;

    if(ec_node < NODE_NUM)
    {
    	ec_node = NODE_NUM;
    }

    EC_Para(ec_node);
    
    struct Result t;

    cdf.resize(Scheduler_num);
    load_balancer_lbt.resize(Scheduler_num);
    load_balancer_access.resize(Scheduler_num);

    R_Req_Tbl.resize(Scheduler_num);
    W_Req_Tbl.resize(Scheduler_num);

    for(int i = 0; i < Scheduler_num; i++)
    {
    	Init(ec_node, 0);

    	cdf[i].resize(6, 0);
    	//printf("size of cdf[%d]: %ld\n", i, cdf[i].size());

    	if(degraded == 0)
    	{

    		Process(files, trace_start, trace_end, &T_line, ERASURE, P_Policy, 200, degraded, i);

    		Reset_all(ec_node, 0);
        	total_l = 0;
        	last_read = -1;
        	last_read_request_index = -1;
    	}
    	else //Degraded
    	{
    		long long latency_sum = 0;
    		double G_sum = 0;

    		for(int j = 0; j < ec_node; j++)
    		{
    			Process(files, trace_start, trace_end, &T_line, ERASURE, P_Policy, j, degraded, i);

//        		printf("total_latency[%d]= %lld Final_G_access[%d] = %f\n", i, total_latency[i], i, Final_G_access[i]);
    			latency_sum += total_latency[i];
    			G_sum += Final_G_access[i];

    			Reset_all(ec_node, 0);

    			total_l = 0;

    			last_read = -1;
    			last_read_request_index = -1;
    			total_latency.clear();
    			Final_G_access.clear();

    			R_Req_Tbl.clear();
    			W_Req_Tbl.clear();
    		    R_Req_Tbl.resize(Scheduler_num);
    		    W_Req_Tbl.resize(Scheduler_num);
    		}


    		degraded_latency.push_back(latency_sum);
    		degraded_G_access.push_back((float)G_sum/ec_node);
    	}


    }

    Output_Statictics(trace_start, trace_end, degraded, ec_node, ec_k, P_Policy);

    return 0;
}
