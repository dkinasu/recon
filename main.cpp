
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


void Output_Statictics(int trace_start, int trace_end, std::vector< std::vector<long long> >cdf, int degraded, int node_num, int k)
{
	char *suffix = (char *)".sts";

	char *R_output = (char *)malloc(500 * sizeof(char));
	struct Result tmp;

	char *failure = NULL;
	if(degraded == 0)
		failure = (char *)"Normal";
	else
		failure = (char *)"Degraded";

	sprintf(R_output, "[%d, %d]%s%d%s%d%s%s%s", node_num, k, file_prefix, trace_start, "-", trace_end, suffix, "_", failure);

	FILE *R_out;

    //open result
    R_out = fopen(R_output, "wb+");

    char result_buf[4096];

	sprintf(result_buf,"%-8s %-8s %-8s %-8s %-8s \n", "index", "lbt_base", "lbt_recon", "G_access-base", "G_access_recon");
	result_buf[strlen(result_buf)] = '\0';
	fwrite(result_buf, 1, strlen(result_buf), R_out);


    for(int i = 0; i < load_balancer_lbt[0].size(); i++)
    {
    	sprintf(result_buf,"%-8d %-8.3f %-8.3f %-8.3f %-8.3f \n", i, load_balancer_lbt[0][i], load_balancer_lbt[1][i], load_balancer_access[0][i], load_balancer_access[1][i]);
    	result_buf[strlen(result_buf)] = '\0';
    	fwrite(result_buf, 1, strlen(result_buf), R_out);
    }

    sprintf(result_buf,"\n");
    result_buf[strlen(result_buf)] = '\0';
    fwrite(result_buf, 1, strlen(result_buf), R_out);


    sprintf(result_buf, "Total_latency_read: %-8.3f %-8.3f\n",(float) total_latency[0]/total_latency[1],(float) 1);
    result_buf[strlen(result_buf)] = '\0';
    fwrite(result_buf, 1, strlen(result_buf), R_out);

    sprintf(result_buf, "G_access_read:      %-8.3f %-8.3f\n", total_access[0], total_access[1]);
    result_buf[strlen(result_buf)] = '\0';
    fwrite(result_buf, 1, strlen(result_buf), R_out);

    if(degraded == 0)
    {

		sprintf(result_buf, "-------------Read_table[Printout]---------\n");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		sprintf(result_buf, "%-8s %-8s %-8s %-8s %-8s %-8s %-8s \n", "Index", "ID", "Read_size", "L_base", "L_recon", "G_base", "G_recon");
		result_buf[strlen(result_buf)] = '\0';
		fwrite(result_buf, 1, strlen(result_buf), R_out);

		for(int i = 0; i < R_Req_Tbl[0].size(); i++)
		{
			sprintf(result_buf,"%-8d %-8s %-8lu %-8lld %-8lld %-8.3f %-8.3f \n", i, R_Req_Tbl[0][i].RequestID, R_Req_Tbl[0][i].datablks.size(), R_Req_Tbl[0][i].Lasting_time, R_Req_Tbl[1][i].Lasting_time, R_Req_Tbl[0][i].G_value, R_Req_Tbl[1][i].G_value);
			result_buf[strlen(result_buf)] = '\0';
			fwrite(result_buf, 1, strlen(result_buf), R_out);

		}

    }
//    if(degraded == 0)
//    {
//		sprintf(result_buf,"\n----------Read Profiling!--------------------\n");
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//		sprintf(result_buf,"%-8d %-8d %-8d %-8d %-8d\n", 0, 1/2*ec_node, ec_node, 2*ec_node, 4*ec_node);
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//		for(int i = 0; i < R_trace_CDF.size(); i++)
//		{
//			sprintf(result_buf, "%.3f\t", (float) R_trace_CDF[i] / R_Req_Tbl[0].size());
//			result_buf[strlen(result_buf)] = '\0';
//			fwrite(result_buf, 1, strlen(result_buf), R_out);
//		}
//
//		sprintf(result_buf, "\n----------Write--------------------\n");
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//		for(int i = 0; i < W_trace_CDF.size(); i++)
//		{
//			sprintf(result_buf, "%.3f\t", (float) W_trace_CDF[i] / W_Req_Tbl[0].size());
//			result_buf[strlen(result_buf)] = '\0';
//			fwrite(result_buf, 1, strlen(result_buf), R_out);
//		}
//
//		sprintf(result_buf, "\n\n");
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//
//
//
//
//		sprintf(result_buf, "----------(Raw Trace) CDF of different placement!--------------------");
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//		for(int i = 0; i < cdf.size(); i++)
//		{
//			   sprintf(result_buf, "\n");
//			   result_buf[strlen(result_buf)] = '\0';
//			   fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//			   for(int j = 0; j < cdf[i].size(); j++)
//			   {
//				   sprintf(result_buf, "%6.3f ", (float)cdf[i][j]/cdf[i][cdf[i].size()-1]);
//				   result_buf[strlen(result_buf)] = '\0';
//				   fwrite(result_buf, 1, strlen(result_buf), R_out);
//			   }
//		}
//
//		sprintf(result_buf, "\n\n");
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//    }
//
//
//    sprintf(result_buf, "\n%-2s %-5s %10s %10s %10s %10s %10s\n", "No", "Name", "Ave_ltc", "RAW_TRACE", "ALL_REQ_READ", "ALL_REQ_READ_RAN", "ALL_RANDOM");
//    result_buf[strlen(result_buf)] = '\0';
// 	fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//
// 	for(int i = 0; i < Final.size(); i++)
// 	{
// 		sprintf(result_buf, "%-2d %-10s %10.5f %10.5f %10.5f %10.5f  %10.5f %10.5f %10.5f\n", Final[i].policy, Final[i].p_name, (float)Final[i].total_latency, Final[i].value[RAW_TRACE], Final[i].value[ALL_REQ_READ], Final[i].value[ALL_REQ_READ_RAN], Final[i].value[ALL_RANDOM], Final[i].value[ALL_SHORT], Final[i].value[ALL_LONG]);
// 		result_buf[strlen(result_buf)] = '\0';
// 		fwrite(result_buf, 1, strlen(result_buf), R_out);
// 	}
//
//
//	sprintf(result_buf, "\n---------------Normalized results---------------------------\n");
//	result_buf[strlen(result_buf)] = '\0';
//	fwrite(result_buf, 1, strlen(result_buf), R_out);
//
//
//	for(int i = 0; i < Final.size(); i++)
//	{
//		Final[i].value[RAW_TRACE] /=  tmp.value[RAW_TRACE];
//		Final[i].value[ALL_REQ_READ] /=  tmp.value[ALL_REQ_READ];
//		Final[i].value[ALL_REQ_READ_RAN] /=  tmp.value[ALL_REQ_READ_RAN];
//		Final[i].value[ALL_RANDOM] /=  tmp.value[ALL_RANDOM];
//		Final[i].value[ALL_SHORT] /=  tmp.value[ALL_SHORT];
//		Final[i].value[ALL_LONG] /=  tmp.value[ALL_LONG];
//
//	    sprintf(result_buf, "%-2d %-10s %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n", Final[i].policy, Final[i].p_name, (float)Final[i].total_latency / tmp.total_latency, Final[i].value[RAW_TRACE], Final[i].value[ALL_REQ_READ], Final[i].value[ALL_REQ_READ_RAN], Final[i].value[ALL_RANDOM], Final[i].value[ALL_SHORT], Final[i].value[ALL_LONG]);
//		result_buf[strlen(result_buf)] = '\0';
//		fwrite(result_buf, 1, strlen(result_buf), R_out);
//	}

	fclose(R_out);

}

int main(int argc, char** argv) 
{

	trace = atoi(argv[10]);
//	trace = 0;

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


    char **default_setting;
    Create_Default_Setting(&default_setting);
    Argv_Parse(8, default_setting, file_prefix, file_suffix);

//	Argv_Parse(argc, argv, file_prefix, file_suffix);
//	Policy = atoi(argv[6]);

    if(ec_node < NODE_NUM)
    {
    	ec_node = NODE_NUM;
    }

    EC_Para(ec_node);
    
    struct Result t;

    std::vector< std::vector<long long> >cdf;
    load_balancer_lbt.resize(Scheduler_num);
    load_balancer_access.resize(Scheduler_num);

    R_Req_Tbl.resize(Scheduler_num);
    W_Req_Tbl.resize(Scheduler_num);

    for(int i = 0; i < Scheduler_num; i++)
    {
    	Init(ec_node, 0);

    	if(degraded == 0)
    	{
    		t = Process(files, trace_start, trace_end, &T_line, ERASURE, P_Policy, 200, degraded, i);
    	}
    	else //Degraded
    	{
    		long long latency_sum = 0;
    		long long G_sum = 0;
    		for(int j = 0; j < ec_node; j++)
    		{
    			Process(files, trace_start, trace_end, &T_line, ERASURE, P_Policy, j, degraded, i);
    			latency_sum += total_l;
    		}

    		t.total_latency = latency_sum / ec_node;
    		t.value[RAW_TRACE] = G_sum / ec_node;
    	}


    	cdf.push_back(CDF);
    	Final.push_back(t);
    	Reset_all(ec_node, 0);
    	total_l = 0;
//    	R_Req_Tbl.clear();
//    	R_Req_Tbl.resize(1);
    }

    Output_Statictics(trace_start, trace_end, cdf, degraded, ec_node, ec_k);

    return 0;
}
