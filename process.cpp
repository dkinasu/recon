/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "process.h"


 void binary_to_hex(char *dst, char *src, size_t src_size)
 {
 	int x;

 	for (x = 0; x < src_size; x++)
 		sprintf(&dst[x * 2], "%.2x", (unsigned char)src[x]);
 	return;
 }


char *hex_to_binary(char *src, size_t src_size)
{
	int x;

	char *result = (char *)malloc((src_size + 1) / 2);
	for (x = 0; x < src_size; x++){
		char c = src[x];
		unsigned int bin = (c > '9') ?
		   (tolower(c) - 'a' + 10) : (c - '0');
		if (x % 2 == 0) {
			result[x / 2] = bin << 4;
		} else {
			result[x / 2] |= bin;
		}
	}
	return result;
}


void Print_set(set<int> &f)
{
	set<int>::iterator fi;

	printf("Free_H_nodes: ");
	for(fi=f.begin(); fi != f.end(); fi++)
	{
		printf(" %2d ", *fi);
	}
	printf("\n");
}


float Sum(std::vector<struct Read_request> ReadTrace)
{
	float t = 0;
	for(int i = 0; i < ReadTrace.size(); i++)
	{
		if(ReadTrace[i].G_value != -1)
			t += ReadTrace[i].G_value;
	}
	return t/ReadTrace.size();
}

void Write_Process_no_Dedup(struct traceline *T_line, int ec, int policy, int recon)
{
	int unique = 0;
	long long last_trace_time = 0;

	struct fp_node *fp = NULL;

	/*no dedup*/
//	sys_last_busy_time += SHA1_COST;

	{
//		printf("FP [MISS]!\n");
		unique = 1;
		T_line->datablk = datablk_serial;
		datablk_serial++;
		fp = Init_fp_node(T_line);
//		Add_fp(fp);
		T_line->fp = fp;
	}


	/*No access cache*/
	{
		N_cache_miss++;
//		printf("line[%4lld]: cache miss[%4lld]\n", total_line, N_cache_miss);

		if (ec == 1)
		{
			if(unique == 1)//new pblk
			{

				if (soe.full == 0)
				{
					tmp = T_line->Arrive_Time;
				}

				T_line->Finish_Time = sys_last_busy_time + ACK_TIME;

				if(T_line->Finish_Time > W_Req_Tbl[recon][T_line->Request_index].Finish_time)
				{
					W_Req_Tbl[recon][T_line->Request_index].Finish_time = T_line->Finish_Time;
					W_Req_Tbl[recon][T_line->Request_index].Lasting_time = W_Req_Tbl[recon][T_line->Request_index].Finish_time - W_Req_Tbl[recon][T_line->Request_index].Arrive_time;
					W_Req_Tbl[recon][T_line->Request_index].straggler = T_line->trace_num;
				}

				//remove timeover restriction
				if(T_line->Arrive_Time > (tmp + PLACEMENT_THRESHOLD))
				{
					;
				}

//				printf("Push SOE\n");
				Push_SOE(*T_line);
				unique = 0;

			}


			if (Is_SOE_full(ec_k))//encoding
			{
//				printf("SOE is full! Starting Placement.\n");
			    sys_last_busy_time += ENCODE_COST;

			   	int s_p = Placement(soe, policy, recon);
//			   	Finish_time_SOE(s_p, recon);
//			   	printf("encoding finished!\n");
//			   	Print_Cluster_Time();
//			   	Print_SOE();
			   	Reset_SOE(ec_k);
			}



		}
		else if(ec == 0)//no Erasure coding
		{

		}

	}


}



// Process Write Request: 6 cases
//void Write_Process(struct traceline *T_line, int ec, int policy)
//{
//	int unique = 0;
//	long long last_trace_time = 0;
//
//
//	struct fp_node *fp = NULL;
//
//	sys_last_busy_time += SHA1_COST;
//
//	fp = Find_fp(T_line->Sha1);
//	fp_search_count++;
//
//	if (fp != NULL) //duplicates
//	{
//		w_case_3++;
////		printf("FP hit![%lld]!\n", w_case_3);
//		total_fp_hit++;
//		fp->hit++;
//
//		unique = 0;
//
//
//		T_line->Finish_Time = sys_last_busy_time + ACK_TIME;
//
//		if(T_line->Finish_Time > W_Req_Tbl[policy][T_line->Request_index].Finish_time)
//		{
//			W_Req_Tbl[policy][T_line->Request_index].Finish_time = T_line->Finish_Time;
//			W_Req_Tbl[policy][T_line->Request_index].Lasting_time = W_Req_Tbl[policy][T_line->Request_index].Finish_time - W_Req_Tbl[policy][T_line->Request_index].Arrive_time;
//			W_Req_Tbl[policy][T_line->Request_index].straggler = T_line->trace_num;
//		}
//
//
//		T_line->fp = fp;
//
//		if (T_line->fp->p == NULL)
//		{
//			Search_fp_in_SOE(T_line);
//		}
//		else
//		{
//			Cluster[T_line->fp->p->node_num]->dup_count++;
//			Cluster[T_line->fp->p->node_num]->speclative_read_access += 1;
//
//			for(int w = 0; w < QUEUE_NUM; w++)
//			{
//				std::set<int>::iterator it;
//				it = W_Req_Tbl[policy][T_line->Request_index].remain.find(T_line->fp->p->node_num + w*ec_node);
//
//				if(it != W_Req_Tbl[policy][T_line->Request_index].remain.end())
//				{
//					W_Req_Tbl[policy][T_line->Request_index].remain.erase(it);
//					break;
//				}
//			}
//
//			W_Req_Tbl[policy][T_line->Request_index].datablks.push_back(T_line->fp->p->data_blk_num);
//
//			W_Req_Tbl[policy][T_line->Request_index].used_nodes.insert(T_line->fp->p->node_num);
//			if(W_Req_Tbl[policy][T_line->Request_index].used_nodes.size() == ec_k)
//				W_Req_Tbl[policy][T_line->Request_index].used_nodes.clear();
//		}
//
//	}
//	else// new fp
//	{
////		printf("FP [MISS]!\n");
//		unique = 1;
//		T_line->datablk = datablk_serial;
//		datablk_serial++;
//		fp = Init_fp_node(T_line);
//		Add_fp(fp);
//		T_line->fp = fp;
//	}
//
//	//check cache.
//
//	sys_last_busy_time += ACCESS_CACHE;
//
//	if(Routine_N_cache(T_line->fp))
//	{
//		N_cache_hit++;
//		printf("line[%4lld]: cache hit[%lld]\n", total_line, N_cache_hit);
//		// printf("line:[%lld] RequestID: %s duplicated! Last_time[%lld]\n", total_line, T_line->RequestID,  T_line->Finish_Time - T_line->Arrive_Time);
//	}
//	else
//	{
//		N_cache_miss++;
////		printf("line[%4lld]: cache miss[%4lld]\n", total_line, N_cache_miss);
//
//		if (ec == 1)
//		{
//			if(unique == 1)//new pblk
//			{
//
//				if (soe.full == 0)
//				{
//					tmp = T_line->Arrive_Time;
//				}
//
//				T_line->Finish_Time = sys_last_busy_time + ACK_TIME;
//
//				if(T_line->Finish_Time > W_Req_Tbl[policy][T_line->Request_index].Finish_time)
//				{
//					W_Req_Tbl[policy][T_line->Request_index].Finish_time = T_line->Finish_Time;
//					W_Req_Tbl[policy][T_line->Request_index].Lasting_time = W_Req_Tbl[policy][T_line->Request_index].Finish_time - W_Req_Tbl[policy][T_line->Request_index].Arrive_time;
//					W_Req_Tbl[policy][T_line->Request_index].straggler = T_line->trace_num;
//				}
//
//				//remove timeover restriction
//				if(T_line->Arrive_Time > (tmp + PLACEMENT_THRESHOLD))
//				{
//					;
//				}
//
////				printf("Push SOE\n");
//				Push_SOE(*T_line);
//				unique = 0;
//
//			}
//
//
//			if (Is_SOE_full(ec_k))//encoding
//			{
////				printf("SOE is full! Starting Placement.\n");
//			    sys_last_busy_time += ENCODE_COST;
//
//			   	int s_p = Placement(soe, policy);
//			   	Finish_time_SOE(s_p, policy);
////			   	Print_SOE();
//			   	Reset_SOE(ec_k);
//			}
//
//
//
//		}
//		else if(ec == 0)//no Erasure coding
//		{
//
////			//regardless of duplicates or not, go the the node and write.
////			CurNode = global_storage[T_line->fp->p->pblk_num]->node_num;
////
////			if (unique == 1)//new pblk
////			{
////				if (sys_last_busy_time >= Cluster[CurNode]->Last_Busy_Time)
////				{
////						Cluster[CurNode]->Last_Busy_Time = sys_last_busy_time + WRITE_LATENCY + DATA_TRANSMISSION;
////				}
////				else
////				{
////						Cluster[CurNode]->Last_Busy_Time += WRITE_LATENCY + DATA_TRANSMISSION;
////				}
////
////				T_line->Finish_Time = Cluster[CurNode]->Last_Busy_Time;
////			}
////			else//duplicates & cache miss: write into cache instead of swap.
////			{
////				//get into cache.
////				T_line->Finish_Time = sys_last_busy_time;
////			}
//		}
//
//	}
//
//
//
//	// printf("-----------------------------------------------------------\n");
//
//}

void Update_R_Request_Time(struct traceline *T_line, int policy)
{

	// printf("Update_R_Request_Time!\n");

	if (R_Req_Tbl[policy][T_line->Request_index].Arrive_time > T_line->Arrive_Time)
	{
		printf("[Error]:R_Request[%lld].Arrive_Time Wrong!\n", T_line->Request_index);
		printf("[%5lld]T_line->Arrive_Time: %lld   Req_Tbl[T_line->Request_index].Arrive_time: %lld\n", total_line, T_line->Arrive_Time, R_Req_Tbl[policy][T_line->Request_index].Arrive_time);
		R_Req_Tbl[policy][T_line->Request_index].Arrive_time = T_line->Arrive_Time;
	}
	

	if (R_Req_Tbl[policy][T_line->Request_index].Finish_time < T_line->Finish_Time)
	{
		R_Req_Tbl[policy][T_line->Request_index].Finish_time = T_line->Finish_Time;
		R_Req_Tbl[policy][T_line->Request_index].Lasting_time = R_Req_Tbl[policy][T_line->Request_index].Finish_time - R_Req_Tbl[policy][T_line->Request_index].Arrive_time;

		if(R_Req_Tbl[policy][T_line->Request_index].straggler < (T_line->Finish_Time - T_line->Arrive_Time))
		{
		    R_Req_Tbl[policy][T_line->Request_index].straggler = T_line->Finish_Time - T_line->Arrive_Time;
		}
	}
	
	R_Req_Tbl[policy][T_line->Request_index].reponse_times.push_back(T_line->Finish_Time - T_line->Arrive_Time);

	Calculate_G_access_per_request(T_line->Request_index+1, policy);


//	R_Req_Tbl[policy][T_line->Request_index].G_value = 1 - ((float) R_Req_Tbl[policy][T_line->Request_index].Lasting_time / R_Req_Tbl[policy][T_line->Request_index].datablks.size() / R_Req_Tbl[policy][T_line->Request_index].straggler);

//	printf("[%5lld] R_Req_Tbl[%5lld]: F_time= %5lld G=%5.3f\n", total_line, T_line->Request_index, R_Req_Tbl[policy][T_line->Request_index].Finish_time, R_Req_Tbl[policy][T_line->Request_index].G_value);


}

float Calculate_G_access_per_request(long long Count_RIO, int recon)
{
	std::map<int, int>result;
	std::map<int, int>::iterator ri;
	result.clear();

	{
		int n_node;
		int max = -1;

		for(int j = 0; j < R_Req_Tbl[recon][Count_RIO-1].datablks.size(); j++)
		{
			n_node = global_storage[blk_mapping[R_Req_Tbl[recon][Count_RIO-1].datablks[j]]]->node_num;
			result[n_node]++;

		}

		for(ri = result.begin(); ri != result.end(); ri++)
		{
			if(max < ri->second)
				max = ri->second;
		}

		float G_value = 1 - (float)(R_Req_Tbl[recon][Count_RIO-1].datablks.size()/ec_node)/ max;
		R_Req_Tbl[recon][Count_RIO-1].G_value = G_value;
	}

	return R_Req_Tbl[recon][Count_RIO-1].G_value;
}


void Calculate_lasting_T_per_request(long long Count_RIO, int policy)
{
//	R_Req_Tbl[policy][Count_RIO-1].Finish_time = max;


}

void Cal_total_statistics(int recon)
{
	long long sum_access = 0;
	long long ave_access = 0;
	long long max_access = -1;
	float G_access = 0;

	//calculate final G_access
	for(int i = 0; i < ec_node; i++)
	{
		sum_access += Cluster[i]->read_access;

		if(max_access < Cluster[i]->read_access)
			max_access = Cluster[i]->read_access;
	}

	ave_access = (float)sum_access / ec_node;

	G_access = 1 - (float)ave_access/max_access;

//	printf("ave: %lld, max: %lld\n", ave_access, max_access);
	Final_G_access[recon] = (G_access);

	if(recon == 1)
	{
		;
	}

	total_l = 0;
	for(int i = 0; i < R_Req_Tbl[recon].size(); i++)
	{
		total_l += R_Req_Tbl[recon][i].Lasting_time;

		Cal_CDF(R_Req_Tbl[recon][i].G_value, recon);
	}

	total_latency[recon]=total_l;
}

void Cal_CDF(float& value, int recon)
{
//	printf("size of: %ld\n", cdf[recon].size());

	cdf[recon][5]++; //100%

	//CDF
	if(value <= 0.8) //80%
	{
		cdf[recon][4]++;
	}

	if(value <= 0.6) //60%
	{
		cdf[recon][3]++;
	}

	if(value <= 0.4) //40%
	{
		cdf[recon][2]++;
	}

	if(value <= 0.2)//20%
	{
		cdf[recon][1]++;
	}

	if(value == 0) //0
	{
		cdf[recon][0]++;
	}

}

void RW_CDF_calculation(vector<long long> &array, int write)
{
	std::vector<struct Request>tmp;
	int value = -1;

	if(write == 1)
	{
		tmp = W_Req_Tbl[0];
	}
	else
	{
		tmp = R_Req_Tbl[0];
	}


	for(int i = 0; i < tmp.size(); i++)
	{
		value = tmp[i].datablks.size();

		array[5]++; //100%

		//CDF
		if(value <= 4 * ec_node) //4 * ec_node%
		{
			array[4]++;
		}

		if(value <= 2* ec_node) //2 * ec_node%
		{
			array[3]++;
		}

		if(value <= ec_node) //1 * ec_node%
		{
			array[2]++;
		}

		if(value <= ec_node / 2)// 1/2 * ec_node%
		{
			array[1]++;
		}

		if(value == 0) //0
		{
			array[0]++;
		}
	}

}

float Calculate_distance_access()
{
	float result = 0;
	float average = -1;
	long long sum = 0;
	long long max = -1;

	for(int i = 0; i < ec_node; i++)
	{
		sum += Cluster[i]->read_access;
		if(max < Cluster[i]->read_access)
		{
			max = Cluster[i]->read_access;
		}
	}

	average = (float) sum / ec_node;

	result = 1 - average/max;

	return result;

}


float Calculate_distance_lbt()
{
	float result = 0;
	float average = -1;
	long long sum = 0;
	long long max = -1;

	for(int i = 0; i < ec_node; i++)
	{
//		if(Cluster[i]->Last_Busy_Time == 0)
//		{
//			printf("[%lld] [%d] Time == 0!\n", total_line, i);
//		}
		sum += Cluster[i]->Last_Busy_Time;

		if(max < Cluster[i]->Last_Busy_Time)
			max = Cluster[i]->Last_Busy_Time;
	}

	average = (float) sum / ec_node;

	/*square of the difference*/
//	sum = 0;
//
//	for(int i = 0; i < ec_node; i++)
//	{
//		sum += (Cluster[i]->Last_Busy_Time - average)* (Cluster[i]->Last_Busy_Time - average);
//	}
//
//	result = (float) sum / ec_node;


	/*1-average/max*/
	result = 1 - average/max;

	return result;

}


//Process a Read request
void Read_first_by_FP(struct traceline *T_line, int policy, int recon)
{

	struct fp_node *fp = NULL;
	int CurNode = -1;
	int queue = -1;
	long long datapblk_num = -1;
	long long pblk_num = -1;

	// printf("-------------------------line[%lld] Read----------------------\n", total_line);

	fp = Find_fp(T_line->Sha1);


	if (fp != NULL) //Read hit
	{
		T_line->fp = fp;

		datapblk_num = fp->p->data_blk_num;
		pblk_num = fp->p->pblk_num;


	}
	else //not found fp. therefore using last_datablk_by_fp.
	{
		readline_not_by_fp++;//read_not_by_fp

		if(datablk_serial == 0)
			return ;

		datapblk_num = (last_datablk_by_fp + 1) % datablk_serial;

		pblk_num = blk_mapping[datapblk_num];

		T_line->fp = global_storage[pblk_num]->fp;

	}

	//update last_datablk_by_fp
	if(last_datablk_by_fp != datapblk_num)
		last_datablk_by_fp = datapblk_num;


	R_Req_Tbl[policy][T_line->Request_index].datablks.push_back(datapblk_num);
	R_Req_Tbl[policy][T_line->Request_index].arrive_times.insert(std::make_pair(datapblk_num, T_line->Arrive_Time));

	if(recon == 1)
	{
		return ;
	}

	CurNode = global_storage[pblk_num]->node_num;

//	printf("RID: %s [dataNum: %3lld] P_num[%5lld]  Node_num[%3d]\n", T_line->RequestID, datapblk_num, pblk_num, CurNode);


	//check cache
	//sys_last_busy_time += ACCESS_CACHE;

	if(Routine_N_cache(T_line->fp))//N_cache hit
	{
		r_case_2++;
		N_cache_hit++;

		T_line->Finish_Time = sys_last_busy_time;
	}
	else//N_cache miss
	{
		r_case_4++;
		N_cache_miss++;

		if(recon == 1)
		{
			printf("using Recon!\n");
		}

		else
		{
//			printf("Using normal!\n");

			//starting access nodes
			if(Cluster[CurNode]->failed != 1 && global_storage[pblk_num]->corrupted != 1)
			{

				Cluster[CurNode]->total_access_count++;
				Cluster[CurNode]->read_access++;

				queue = (Cluster[CurNode]->read_access - 1) % QUEUE_NUM;

//				if (sys_last_busy_time > Cluster[CurNode]->queques[queue])
				if (sys_last_busy_time > Cluster[CurNode]->Last_Busy_Time)
				{
						Cluster[CurNode]->Last_Busy_Time = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
				}
				else
				{
				//		printf("Blocked[Node Busy]!\n");
						read_blk_num++;
						Cluster[CurNode]->Last_Busy_Time += READ_LATENCY + DATA_TRANSMISSION;
				}

	//			printf("%5lld\n", Cluster[CurNode]->Last_Busy_Time);
			//	sys_last_busy_time = Cluster[CurNode]->Last_Busy_Time + ACCESS_CACHE + SHA1_COST; // the metadata needs to get the read data and compute the sha1 and write into cache.

				if(T_line->Finish_Time < Cluster[CurNode]->Last_Busy_Time + ACK_TIME)
					T_line->Finish_Time = Cluster[CurNode]->Last_Busy_Time + ACK_TIME;

		//		Print_Cluster_Time();
			}
			else
			{
		//		printf("Access [blk: %5lld] in Node[%2d]: Failure!\n", pblk_num, CurNode);
				long long t = Degraded_read(pblk_num);

				if(T_line->Finish_Time < t)
					T_line->Finish_Time = t;
			}
		}

		Update_R_Request_Time(T_line, policy);
	}



}

//Process a Read request by mapping blk to eliminate missing read.
void Read_by_lpblk(struct traceline *T_line, int recon)
{
	int CurNode = 0;

//	int queue = -1;
	// printf("-------------------------line[%lld] Read----------------------\n", total_line);

	long long datapblk_num = (T_line->Address / 8 + 1) % (datablk_serial) ;

	long long pblk_num = blk_mapping[datapblk_num];

	CurNode = global_storage[pblk_num]->node_num;



	//check cache
//	sys_last_busy_time += ACCESS_CACHE;

//	printf("size of R_Req_Tbl %ld\n")
	R_Req_Tbl[recon][T_line->Request_index].datablks.push_back(datapblk_num);
	R_Req_Tbl[recon][T_line->Request_index].arrive_times.insert(std::make_pair(datapblk_num, T_line->Arrive_Time));

	if(recon == 1)
	{
		return ;
	}
	else
	{
//		printf("[%5lld]RID: %s  A_t: %5lld [dataNum: %3lld] Node_num[%3d]\n", total_line, T_line->RequestID, T_line->Arrive_Time  , datapblk_num, CurNode);

		r_case_4++;
		N_cache_miss++;

		//starting access nodes

		if(Cluster[CurNode]->failed != 1 && global_storage[pblk_num]->corrupted != 1)
		{

			Cluster[CurNode]->total_access_count++;
			Cluster[CurNode]->read_access++;

//			queue = (Cluster[CurNode]->read_access - 1) % QUEUE_NUM;
//			printf("ACCESS Node[%3d]: %5lld --> ", CurNode, Cluster[CurNode]->Last_Busy_Time);

			if (sys_last_busy_time > Cluster[CurNode]->Last_Busy_Time)
			{
				Cluster[CurNode]->Last_Busy_Time = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
				//there we assume client first go to check the MDS and then cache miss and then go to Cluster.
			}
			else
			{
			//	printf("Blocked[Node Busy]!\n");
				read_blk_num++;
				Cluster[CurNode]->Last_Busy_Time += READ_LATENCY + DATA_TRANSMISSION;
			}

//			printf("%5lld\n", Cluster[CurNode]->Last_Busy_Time);
	//		sys_last_busy_time = Cluster[CurNode]->Last_Busy_Time + ACCESS_CACHE + SHA1_COST; // the metadata needs to get the read data and compute the sha1 and write into cache.

			if(T_line->Finish_Time < Cluster[CurNode]->Last_Busy_Time + ACK_TIME)
				T_line->Finish_Time = Cluster[CurNode]->Last_Busy_Time + ACK_TIME;

//			Print_Cluster_Time();
		}
		else
		{
//			printf("Access [blk: %5lld] in Node[%2d]: Failure!\n", pblk_num, CurNode);
			long long t = Degraded_read(pblk_num);

			if(T_line->Finish_Time < t)
				T_line->Finish_Time = t;
		}

		Update_R_Request_Time(T_line, recon);
	}

}



long long Degraded_read(long long pblk_num)
{
	long long stripe_num;

	int Cur_node = -1;

	long long last_finish_time = -1;

	vector<long long>tmp;

	degraded_read++;

	stripe_num = global_storage[pblk_num]->stripe_num;

	for(int i = 0; i < stripe_tbl[stripe_num].data.size() ; i++)
	{
		long long p = stripe_tbl[stripe_num].data[i];
		int failed_node = global_storage[p]->node_num;
		if(Cluster[failed_node]->failed != 1)
			tmp.push_back(stripe_tbl[stripe_num].data[i]);
	}

	for(int i = 0; i < stripe_tbl[stripe_num].parity.size() ; i++)
	{
		tmp.push_back(stripe_tbl[stripe_num].parity[i]);
	}

	std::random_shuffle(tmp.begin(), tmp.end());

	tmp.resize(ec_k);

//	printf("Helper p_block[Node]:  ");
//	for(int i = 0; i < tmp.size() ; i++)
//	{
//		printf(" %5lld [%2lld] ", tmp[i], tmp[i] % ec_node);
//	}
//	printf("\n");

	for(int i = 0; i < tmp.size() ; i++)
	{
		Cur_node = tmp[i] % ec_node;
		int queue = -1 ;


		if(Cluster[Cur_node]->failed != 1 && global_storage[tmp[i]]->corrupted != 1)
		{
			Cluster[Cur_node]->total_access_count++;
			Cluster[Cur_node]->read_access++;
			Cluster[Cur_node]->degrade_access++;

//			printf("ACCESS Node[%d]: %5lld --> ", Cur_node, Cluster[Cur_node]->Last_Busy_Time);

			if (sys_last_busy_time > Cluster[Cur_node]->Last_Busy_Time)
			{
				Cluster[Cur_node]->Last_Busy_Time = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
			}
			else
			{
			//	printf("Blocked[Node Busy]!\n");
				read_blk_num++;
				Cluster[Cur_node]->Last_Busy_Time += READ_LATENCY + DATA_TRANSMISSION;
			}

//			printf("%5lld\n", Cluster[Cur_node]->Last_Busy_Time);

			if(last_finish_time < Cluster[Cur_node]->Last_Busy_Time + ACK_TIME)
			{
				last_finish_time = Cluster[Cur_node]->Last_Busy_Time + ACK_TIME;
			}
		}
		else //Degraded read
		{
			printf("[Data: %5lld] Access [blk: %5lld] in Node[%2d]: Failure!\n", blk_mapping[tmp[i]], tmp[i], Cur_node);
			last_finish_time = Degraded_read(tmp[i]);
		}


	}

	return last_finish_time;
}



void Read_Table(std::vector<struct Read_request> &Read_trace, int raw)
{

	long long last_finish_time = 0;
	long long pblk_num = 0;
	int queue = -1;

	for(int i = 0; i < Read_trace.size(); i++)
	{
		last_finish_time = 0;

		if(raw != 1)
		{
			Reset_time();
		}

		for(int j = 0; j < Read_trace[i].datablks.size(); j++)
		{

			pblk_num = blk_mapping[Read_trace[i].datablks[j]];

			if(Routine_N_cache(global_storage[pblk_num]->fp))//N_cache hit
			{

				N_cache_hit++;

				last_finish_time = sys_last_busy_time;
			}
			else//N_cache miss
			{

				N_cache_miss++;

				//starting access nodes
				CurNode = global_storage[pblk_num]->node_num;

				//The node is okay
				if(Cluster[CurNode]->failed != 1 && global_storage[pblk_num]->corrupted != 1)
				{
					Cluster[CurNode]->total_access_count++;
					Cluster[CurNode]->read_access++;

					queue = (Cluster[CurNode]->read_access-1) % QUEUE_NUM;

//					if(raw == 1)
//					{
//						printf("ACCESS Node[%d]: %5lld --> ", CurNode, Cluster[CurNode]->Last_Busy_Time);
//					}

					if (sys_last_busy_time > Cluster[CurNode]->queques[queue])
					{
						Cluster[CurNode]->queques[queue] = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
					}
					else
					{
	//					printf("Blocked[Node Busy]!\n");
						read_blk_num++;
						Cluster[CurNode]->queques[queue] += READ_LATENCY + DATA_TRANSMISSION;
					}

	//				printf("%5lld\n", Cluster[CurNode]->Last_Busy_Time);
					//sys_last_busy_time = Cluster[CurNode]->Last_Busy_Time + ACCESS_CACHE + SHA1_COST; // the metadata needs to get the read data and compute the sha1 and write into cache.

					if(last_finish_time < Cluster[CurNode]->queques[queue] + ACK_TIME)
						last_finish_time = Cluster[CurNode]->queques[queue] + ACK_TIME;
				}
				else //Degraded read
				{
//					printf("Access [blk: %5lld] in Node[%2d]: Failure!\n", pblk_num, CurNode);
					long long t = Degraded_read(pblk_num);

					if(last_finish_time < t)
						last_finish_time = t;
				}

			}


			if (Read_trace[i].Finish_time < last_finish_time)
			{
				Read_trace[i].Finish_time = last_finish_time;
				Read_trace[i].Lasting_time = Read_trace[i].Finish_time - Read_trace[i].Arrive_time;
				Read_trace[i].t_straggler = Read_trace[i].datablks[j];

//				Read_trace[i].G_value = 1 - ((float) Read_trace[i].Lasting_time / Read_trace[i].datablks.size() / Read_trace[i].straggler_pblk);
			}

		}

		Read_trace[i].G_value = 1 - ((float)(READ_LATENCY + DATA_TRANSMISSION) / Read_trace[i].Lasting_time);

	}

}




void Delete_Process(struct traceline *T_line, int ec)
{
	//metrics[DELETE_NUM].total++;
	//Del_file(&laddr_tree, T_line->file_path);
}


void Print_multimap(multimap<long long, int> &A)
{
	multimap<long long, int>::iterator mi;

	for(mi = A.begin(); mi != A.end(); mi++)
	{
		printf("map[%5lld]  == %d\n", mi->first, mi->second);
	}

}



//analysis Read_request[Count_RIO]
long long Schedule_request(long long Count_RIO, int policy)
{
//	printf("test!\n");

	long long finish_time = -1;

	map <long long, vector<long long> >S;
	map <long long, long long >Latest_Arrive_T_per_S;
	map <long long, long long >::iterator li;
	map <long long, vector<long long> >::iterator si;

//	if(total_line == 6175)
//	{
//		printf("Stop here!\n");
//	}



	S.clear();
	Latest_Arrive_T_per_S.clear();

	/*Getting the stripe infor*/
	for(int i = 0; i < R_Req_Tbl[policy][Count_RIO].datablks.size(); i++)
	{
		long long data = R_Req_Tbl[policy][Count_RIO].datablks[i];
		long long arrive_time = R_Req_Tbl[policy][Count_RIO].arrive_times[data];
//		printf(" [%5lld: %lld   ] ", data, arrive_time);
		S[data/ec_k].push_back(data);

		li = Latest_Arrive_T_per_S.find(data/ec_k);
		if (li != Latest_Arrive_T_per_S.end())//found
		{
			if(Latest_Arrive_T_per_S[data/ec_k] > arrive_time)
			{
				Latest_Arrive_T_per_S[data/ec_k] = arrive_time;
			}
		}
		else// not found
		{
			Latest_Arrive_T_per_S[data/ec_k] = arrive_time;
		}

//		printf("[%lld]: Arrive_time: %lld\n", total_line, Latest_Arrive_T_per_S[data/ec_k]);
	}

//	printf("\n");



	/*print stripe composition of a request*/
//	for(si = S.begin(); si != S.end(); si++)
//	{
//		printf("Stripe[%5lld]:", si->first);
//		for(int i = 0; i < si->second.size(); i++)
//		{
//			printf("%5lld ", si->second[i]);
//		}
//		printf("\n");
//	}


	/*Rescheduling*/
	vector< vector<int> >selection;
	vector<long long> fullstripe_num;
	map<int, int>recon_G;
	selection.clear();
	fullstripe_num.clear();

	multimap<long long, int>result;
	multimap<long long, int>::iterator ri;
	set<int> free_nodes;
	set<int> H_nodes;

	//printf("Reschedule    : ");
	//process first and last stripe
	for(si = S.begin(); si != S.end(); si++)
	{
		if(S[si->first].size() < ec_k)//process non-full stripes
		{
			//process the request with normal read
			for(int j = 0; j < S[si->first].size(); j++)
			{
				long long t;
				long long datablk = S[si->first][j];
				long long pblk_num = blk_mapping[datablk];
				int node_num = global_storage[pblk_num]->node_num;
				long long arrive_time = R_Req_Tbl[policy][Count_RIO].arrive_times[S[si->first][j]];//for non-reschedule ones, it should be the same.
//				long long arrive_time = Latest_Arrive_T_per_S[datablk/ec_k];

				if(sys_last_busy_time < arrive_time)
				{
					sys_last_busy_time = arrive_time;
				}

				//process the request with normal read
				if(Cluster[node_num]->failed != 1 && global_storage[pblk_num]->corrupted != 1)
				{
//					Print_Cluster_Time();
//					printf("  ] ");
					Cluster[node_num]->total_access_count++;
					Cluster[node_num]->read_access++;
					recon_G[node_num]++;



//					printf("Data: %5lld A_T: %lld ACCESS Node[%3d]: %5lld --> ", datablk, arrive_time , node_num, Cluster[node_num]->Last_Busy_Time);

					if (sys_last_busy_time > Cluster[node_num]->Last_Busy_Time)
					{
						Cluster[node_num]->Last_Busy_Time = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
					}
					else
					{
						//printf("Blocked[Node Busy]!\n");
						read_blk_num++;
						Cluster[node_num]->Last_Busy_Time += READ_LATENCY + DATA_TRANSMISSION;
					}

					t = Cluster[node_num]->Last_Busy_Time;

//					printf("%5lld\n", Cluster[node_num]->Last_Busy_Time);

					//Print_Cluster_Time();
				}
				else//degraded read
				{
//					printf(" X] ");
//					printf("[Data: %5lld] Access [blk: %5lld] in Node[%2d]: Failure!\n", datablk, pblk_num, node_num);
					t = Degraded_read(pblk_num);
				}

				if(t < arrive_time)
				{
					printf("[%lld] datablk: %lld t=%lld arrive_time = %lld\n", total_line, datablk, t, arrive_time);
				}


				if(finish_time < t)
				{
					finish_time = t;
				}

			}
		}
		else
		{
			fullstripe_num.push_back(si->first);

			long long cur_stripe = fullstripe_num.back();

			selection.resize(fullstripe_num.size());

			for(int i = 0; i < ec_node; i++)
			{
				result.insert(std::make_pair(Cluster[i]->Last_Busy_Time, i));
				//including the healthy nodes
				if(Cluster[i]->failed != 1)
					free_nodes.insert(i);
			}

			H_nodes = free_nodes;
			for(int j = 0; j < ec_k; j++)
			{

			//Print_set(free_nodes);
				ri = result.begin();
			//	printf("Begin() [%d]: %5lld\n", ri->second, ri->first);
				while(H_nodes.find(ri->second)==H_nodes.end())// it is not a free node
				{
			//		printf("Find a used node: %d\n", ri->second);
					ri++;
				}

				//found a healthy node with the least loads.

				int chosen_node = ri->second;

				long long pblk_num = ec_node * cur_stripe + chosen_node;
//				long long datablk_num = global_storage[pblk_num]->data_blk_num;

//				long long datablk_num_2 = ec_k * cur_stripe + chosen_node;
//				long long pblk_num_2 = blk_mapping[datablk_num_2];

				long long arrive_time = Latest_Arrive_T_per_S[cur_stripe];
				long long lbt = -1;
				long long t;

			//	printf(" [N:%5d: %lld ", chosen_node, arrive_time);

				if(sys_last_busy_time < arrive_time)
				{
					sys_last_busy_time = arrive_time;
				}


				if(Cluster[chosen_node]->failed != 1 && global_storage[pblk_num]->corrupted != 1)
				{
			//		printf("  ] ");
					//Print_Cluster_Time();
					Cluster[chosen_node]->total_access_count++;
					Cluster[chosen_node]->read_access++;
					recon_G[chosen_node]++;



					if (sys_last_busy_time > Cluster[chosen_node]->Last_Busy_Time)
					{
						Cluster[chosen_node]->Last_Busy_Time = sys_last_busy_time + READ_LATENCY + DATA_TRANSMISSION;
					}
					else
					{
								//printf("Blocked[Node Busy]!\n");
						read_blk_num++;
						Cluster[chosen_node]->Last_Busy_Time += READ_LATENCY + DATA_TRANSMISSION;
					}

					t = Cluster[chosen_node]->Last_Busy_Time;

					lbt = Cluster[chosen_node]->Last_Busy_Time;
					result.insert(std::make_pair(lbt, chosen_node));
					result.erase(ri);
					selection[fullstripe_num.size()-1].push_back(chosen_node);
					H_nodes.erase(chosen_node);

					//printf("Chosen Node[%d]: %5lld---> %5lld\n", chosen_node, lbt - (READ_LATENCY + DATA_TRANSMISSION), lbt);

					//printf("After insert New\n");
					//Print_multimap(result);

					//printf("After Erase old\n");
					//Print_multimap(result);

					//Print_Cluster_Time();

				}
				else//degraded read
				{
					//printf(" X] ");
					//printf("Access [blk: %5lld] in Node[%2d]: Failure!\n", pblk_num, CurNode);
					t = Degraded_read(pblk_num);
				}

				if(t < arrive_time)
				{
					printf("[%lld] t=%lld arrive_time = %lld\n", total_line, t, arrive_time);
				}


				if(finish_time < t)
				{
					finish_time = t;
				}

			}

		}

	}

//	selection.resize(fullstripe_num.size());
//
//	for(int i = 0; i < ec_node; i++)
//	{
//		result.insert(std::make_pair(Cluster[i]->Last_Busy_Time, i));
//		//including the healthy nodes
//		if(Cluster[i]->failed != 1)
//			free_nodes.insert(i);
//	}

//	Print_multimap(result);

//	printf("\n");

	for(int i = 0; i < fullstripe_num.size(); i++)
	{
		vector <int> tmp;
		for(int j = 0; j < S[fullstripe_num[i]].size(); j++)
		{
			long long datablk = S[fullstripe_num[i]][j];
			long long pblk_num = blk_mapping[datablk];
			int node_num = global_storage[pblk_num]->node_num;
			tmp.push_back(node_num);
		}

		if(tmp != selection[i])
		{
			recon_stripe_count++;
			R_Req_Tbl[policy][Count_RIO].recon = 1;

//			printf("Reconed:  ");
//			for(int j = 0; j < selection[i].size(); j++)
//			{
//				printf("%2d->[%2d] ", tmp[j], selection[i][j]);
//			}
//
//			printf("\n");
//
//			Print_Cluster_Time();

			break;

		}
	}


	/*Calculate access G*/
	int n_node;
	int max = -1;

	map<int, int>::iterator ji;
	for(ji = recon_G.begin(); ji != recon_G.end(); ji++)
	{
		n_node = ji->first;
		if(max < recon_G[n_node])
			max = recon_G[n_node];
	}

	float G_value = 1 - (float)(R_Req_Tbl[policy][Count_RIO].datablks.size()/ec_node)/ max;
	R_Req_Tbl[policy][Count_RIO].G_value = G_value;



	R_Req_Tbl[policy][Count_RIO].Finish_time = finish_time;
	if(R_Req_Tbl[policy][Count_RIO].Finish_time < R_Req_Tbl[policy][Count_RIO].Arrive_time)
	{
		printf("Error: Finish_time < Arrive_time!\n");
		abnormal++;
	}
	R_Req_Tbl[policy][Count_RIO].Lasting_time = R_Req_Tbl[policy][Count_RIO].Finish_time - R_Req_Tbl[policy][Count_RIO].Arrive_time;

//	printf("Request[%5lld]: L_time: %7lld-%7lld=%7lld\n", Count_RIO, R_Req_Tbl[policy][Count_RIO].Finish_time, R_Req_Tbl[policy][Count_RIO].Arrive_time, R_Req_Tbl[policy][Count_RIO].Lasting_time);

//	if(selection.size() != 0)
//	{
//		printf("Chosen Nodes: ");
//		for(int i = 0; i < selection.size(); i++)
//		{
//			printf("%d ", selection[i]);
//		}
//		printf("\n");
//	}

	return finish_time;
}

int Request(struct traceline *T_line, int recon)
{

	if(strcmp("-1", Last_RequestID) == 0 && strcmp(T_line->Type, "read") == 0 )
	{
		return -1;
	}


	if (strcmp(T_line->RequestID, Last_RequestID) == 0)
	{
//		printf("Old request!\n");

		if (strcmp(T_line->Type, "write") == 0)
		{
			T_line->Request_index = (Count_WIO-1);
		}
		else//read
		{
			T_line->Request_index = (Count_RIO-1);
		}
	}
	else//new RequestID
	{
		Clear_CurRequest();

		CurReq.Arrive_time = T_line->Arrive_Time;
		CurReq.Finish_time = T_line->Finish_Time;
		CurReq.RequestID = strdup(T_line->RequestID);
		CurReq.Type = strdup(T_line->Type);
		CurReq.straggler = T_line->trace_num;

		if (strcmp(T_line->Type, "write") == 0)
		{
			CurReq.W_Req_Index = Count_WIO;
			T_line->Request_index = CurReq.W_Req_Index;
			Count_WIO++;
			W_Req_Tbl[recon].push_back(CurReq);
		}
		else//read
		{
//			Reset_time();

			CurReq.R_Req_Index = Count_RIO;
			T_line->Request_index = CurReq.R_Req_Index;
			Count_RIO++;
			R_Req_Tbl[recon].push_back(CurReq);

//			if(T_line->Request_index == 271)
//			{
//				printf("R_request[%lld]---[%6lld]\n", T_line->Request_index, total_line);
//			}

		}

		//process last read
		if(last_read == 1)
		{
			if(recon == 1)
			{
				if((0 <= last_read_request_index) && (last_read_request_index < R_Req_Tbl[recon].size()))
				{
					Schedule_request(last_read_request_index, recon);
				}
				else
				{
					printf("[%5lld]Out of boundary! last_read_request_index = %lld size=%lu \n", total_line, last_read_request_index, R_Req_Tbl[recon].size());
				}
			}

//			printf("Request[%5lld]: L_time: %7lld-%7lld=%7lld\n", Count_RIO-1, R_Req_Tbl[recon][Count_RIO-2].Finish_time, R_Req_Tbl[recon][Count_RIO-2].Arrive_time, R_Req_Tbl[recon][Count_RIO-2].Lasting_time);
//			printf("Before calculating last request!\n");
//			Print_Cluster_Time();

//			load_balancer_lbt[recon].push_back(Calculate_distance_lbt());
			load_balancer_access[recon].push_back(Calculate_distance_access());
		}


		if(Last_RequestID != NULL)
			free(Last_RequestID);
		Last_RequestID = strdup(T_line->RequestID);

		if(strcmp(T_line->Type, "read") == 0)
		{
			last_read = 1;
		}
		else
		{
			last_read = 0;
		}

		last_read_request_index = T_line->Request_index;

	}




	return 0;

}

void Print_Req_Tbl(std::vector< vector<struct Request> >&Req_Tbl, int write)
{
	if(write == 1)
	{
		printf("----------------------Write_Request_Table--------------------\n");
	}
	else
		printf("----------------------Read_Request_Table--------------------\n");

//	printf("RequestID\tType\tLasting_time\tArrive_time\tFinish_time\n");
	std::cout << "RID\tType\t \t \t Datablk_set(BA) \t Datablk_set(RA) \t Datablk_set(DA)\n";

	for(int w = 0; w < Req_Tbl[0].size(); w++)
	{
//		std::cout << Req_Tbl[0][w].RequestID << "\t" << Req_Tbl[0][w].Type << "\t";
//
//		for (int i = 0; i < Req_Tbl.size(); i++)
//		{
////			if (strcmp(Req_Tbl[i].Type, "write") == 0)
////			{
////				W_Request_lasting_time += Req_Tbl[i].Lasting_time;
////			}
////			else
////			{
////				R_Request_lasting_time += Req_Tbl[i].Lasting_time;
////			}
//
////			std::cout << Req_Tbl[0][i].RequestID << "\t" << Req_Tbl[0][i].Type << "\t" << "\t" << Req_Tbl[0][i].Lasting_time << "\t" << Req_Tbl[0][i].Arrive_time
////				<< "\t" << Req_Tbl[0][i].Finish_time << "\t";
//
////			cout << i ;
//
//
//
//			printf("[ ");
//
//			if(Req_Tbl[i][w].datablks.size() <= ec_node)
//			{
//				for(int j = 0; j < Req_Tbl[i][w].datablks.size(); j++)
//				{
//					printf("%lld ", Req_Tbl[i][w].datablks[j]);
//				}
//			}
//			else
//				printf("%ld ",Req_Tbl[i][w].datablks.size());
//
//			printf("] ");
//
//
//			if(write != 1)//read trace
//			{
//				printf(" %.2f",  Req_Tbl[i][w].G_value);
//			}
//
//			printf("\t\t");
//
//
//		}

		printf("\n");
	}




	printf("\n");
}

void Process_ReadTrace(std::vector<struct Read_request> &Read_trace, std::vector<struct Request> &W_R_Tbl, int tracetype, struct Result &res, int iteratimes)
{
	//	/*----------All whole-request-read------------------*/
	switch (tracetype)
	{
		    case ALL_REQ_READ :
		    {
		    	Generalize_ReadTrace_ALL(Read_trace, W_R_Tbl, WRITE);
		    	Read_Table(Read_trace, 0);
//		    	Print_ReadTrace(Read_trace);
		    	Calculate_Result(res, Read_trace, tracetype);
		    	break;
		    }

		    case ALL_REQ_READ_RAN :
		    {
		    	float sum = 0;
		    	float Ave_G = 0;

		    	for(int seed = 0; seed < iteratimes; seed++)
		    	{

		    		Generalize_ReadTrace_random(Read_trace, W_R_Tbl, seed);

		    		Read_Table(Read_trace, 0);
//		    		Print_ReadTrace(Read_trace);
		    		sum += Sum(Read_trace);
		    		//printf("[%d] sum = %5.3f\n", seed, sum);
		    	}

		    	Ave_G = sum / iteratimes;
		    	res.value.insert(std::make_pair(ALL_REQ_READ_RAN,Ave_G));
		    	break;
		    }
		    case ALL_RANDOM:
		    {
		    	float sum = 0;
		    	float Ave_G = 0;
		    	for(int seed = 0; seed < iteratimes; seed++)
		    	{
		    		Generalize_ReadTrace_All_random(Read_trace, datablk_serial, seed, ec_node);

		    		Recover_Cluster();
		    		Create_Node_Failure(seed);

		    		Read_Table(Read_trace, 0);
//		    		Print_ReadTrace(Read_trace);
		    		sum += Sum(Read_trace);
		    		//printf("[%d] sum = %5.3f\n", seed, sum);
		    	}

		    	Ave_G = sum / iteratimes;
		    	res.value.insert(std::make_pair(ALL_RANDOM,Ave_G));
		    	break;
		    }

		    case ALL_SHORT:
		    {
		    	Generalize_ReadTrace_ALL_SPECIAL(Read_trace, W_R_Tbl, WRITE, tracetype);
		    	Read_Table(Read_trace, 0);
		    //	Print_ReadTrace(Read_trace);
		    	Calculate_Result(res, Read_trace, tracetype);
		    	break;
		    }
		    case ALL_LONG :
		    {
		    	Generalize_ReadTrace_ALL_SPECIAL(Read_trace, W_R_Tbl, WRITE, tracetype);
		    	Read_Table(Read_trace, 0);
		    //	Print_ReadTrace(Read_trace);
		    	Calculate_Result(res, Read_trace, tracetype);
		    	break;
		    }
		    case NO_TRACE:
		    {
		    	printf("No Read trace Processed!\n");
		    	break;
		    }
		    default:
		    	printf("ERROR");
		        break;


	}

}


void Deal_last_request(long long Count_RIO, int recon)
{
//	printf("Process Last Request!\n");
	long long last_read_index = R_Req_Tbl[recon].size()-1;

	if(recon == 1)
	{
		if(R_Req_Tbl[recon][last_read_index].Finish_time == -1)
		{
			Schedule_request(last_read_index, recon);
		}
	}

//	total_l += R_Req_Tbl[recon][Count_RIO-1].Lasting_time;


//	Print_Cluster_Time();
	load_balancer_lbt[recon].push_back(Calculate_distance_lbt());
	load_balancer_access[recon].push_back(Calculate_distance_access());

//	printf("Request[%5lld]: L_time: %7lld-%7lld=%7lld\n", Count_RIO-1, R_Req_Tbl[recon][Count_RIO-1].Finish_time, R_Req_Tbl[recon][Count_RIO-1].Arrive_time, R_Req_Tbl[recon][Count_RIO-1].Lasting_time);
}

struct Result Process(char **files, int trace_start, int trace_end, struct traceline *T_line, int ec, int policy, int iteratimes, int degraded, int recon)
{
	FILE *file = NULL;
	struct Result res;
	std::vector<struct Read_request> Read_trace;
	total_line = 0;
	bad_line = 0;
	empty_line = 0;
	effective_line_count = 0;
	int last_op = -1; // 1 is for write, 2 is for read

	res.policy = policy;

	switch(policy)
	{
		case 0:
			res.p_name = (char *)"BA";
			break;
		case 1:
			res.p_name = (char *)"RA";
			break;
	    case 2:
	    	res.p_name = (char *)"DA";
	    	break;
	    default:
	    	res.p_name = (char *)"Wrong Policy";
	}

//	if(degraded == 0)
//	{
//		if(recon == 1)
//		{
//			printf("-----------------[%-10s]   using Recon!-------------------\n", res.p_name);
//		}
//
//		else
//		{
//			printf("-----------------[%-10s]   using Normal!-------------------\n", res.p_name);
//
//		}
//	}

	char buffer[MAX_PATH_SIZE + MAX_META_SIZE];

	for (int i = 0; i < trace_end - trace_start; ++i)
	{
		file = fopen(files[i], "r");
		
		if (file == NULL) 
		{
			printf("Open tracefile[%d] fail %s\n", i, strerror(errno));
			continue;
		}
		
		//new file we'll reset the time.
		Reset_time();
		global_time = 0;

	    while(fgets(buffer, MAX_PATH_SIZE + MAX_META_SIZE, file) != NULL)
	    {
	    	total_line++;

//	    	if(total_line == 142)
//	    	{
//	    		printf("now [%lld]\n", total_line);
//	    	}
		    if (strcmp(buffer, "\n") == 0)
		    {
		        empty_line++;
	    		continue;
		    }
	    	
		    if(Split_Trace(buffer, T_line) == 0)
		    {
		    	effective_line_count++;

//				printf("line[%4lld]: A_time: %lld pos: %lld ReqID: %s Type:%s \n", total_line, T_line->Arrive_Time, T_line->pos, T_line->RequestID, T_line->Type);

				if(Request(T_line, recon) == -1)
					continue;

				if(strcmp(T_line->Type, "write") == 0)
				{
					write_count++;

					if(last_op == -1)
						last_op = 1;
					else if(last_op == 2)
					{
//						printf("[OP]Read --> Write\n");
//						Reset_time();
						last_op = 1;
					}

					if(sys_last_busy_time < T_line->Arrive_Time)
		    		{
		    			sys_last_busy_time = T_line->Arrive_Time;
		    		}

//					Write_Process(T_line, ec, policy);
					Write_Process_no_Dedup(T_line, ec, policy, recon);
					
				} 
				else if (strcmp(T_line->Type, "read") == 0)
				{

					if(soe.full != 0)
					{
						sys_last_busy_time += ENCODE_COST;

			   			int s_p = Placement(soe, policy, recon);
			   			Finish_time_SOE(s_p, recon);
			   			Reset_SOE(ec_k);

					}

					if(last_op == -1)
					{
						last_op = 2;
						continue;
					}
					else if(last_op == 1)
					{
//						Print_Cluster_Time();
//						Reset_time();
						last_op = 2;
//						printf("----------------------------Start Read Process!------------------------\n");

//						Cluster[0]->failed = 1;

						if(degraded == 1)
						{
							Recover_Cluster();
							Cluster[iteratimes]->failed = 1;
//							printf("Node[%2d] Failed!\n", iteratimes);
						}

					}

					if(recon == 0)
					{
						if(sys_last_busy_time < T_line->Arrive_Time)
						{
							sys_last_busy_time = T_line->Arrive_Time;
						}
					}
					else//recon scheduler system_last_busy_no_change
					{

					}
					read_count++;
//					Read_first_by_FP(T_line, policy, recon);
					Read_by_lpblk(T_line, recon);
				}
				else
				{
					other_count++;
				}
				
				Clear_Traceline(T_line);	
		    }
		    else
		    {
		    	bad_line++;
		    }

		}


//	    float a = Calculate_G(Count_RIO-1, policy);
//		Cal_CDF(a);

		if(soe.full != 0)
		{
//			printf("[End of File]--> Placement!\n");
			sys_last_busy_time += ENCODE_COST;

			int s_p = Placement(soe, policy, recon);
			Finish_time_SOE(s_p, recon);
//			Print_SOE();
//			Print_stripe(s_p);
			Reset_SOE(ec_k);
		}

		Deal_last_request((long long)R_Req_Tbl[recon].size(), recon);
	}
	

//	Print_Cluster_Time();

//	Print_stripe_tbl();
//	Print_Blk_Mapping();

//	if(recon == 1)
//	{
//		Generalize_ReadTrace_ALL(Read_trace, R_Req_Tbl[recon], READ);
//		Print_ReadTrace(Read_trace);
//	}

//	Read_Table(Read_trace, 1);
//	Print_ReadTrace(Read_trace);
//	Output_ReadTrace(Read_trace, policy, 1);

//	Calculate_Result(res, Read_trace, RAW_TRACE);

	if(degraded == 0)
	{
		/*------------------------------------------------------------Generate read_countRead_Trace-------------------------------------------------*/
//		Process_ReadTrace(Read_trace, W_Req_Tbl[0], ALL_REQ_READ, res, iteratimes);
//		Process_ReadTrace(Read_trace, W_Req_Tbl[0], ALL_REQ_READ_RAN, res, iteratimes);
//		Process_ReadTrace(Read_trace, W_Req_Tbl[0], ALL_RANDOM, res, iteratimes);
//		Process_ReadTrace(Read_trace, W_Req_Tbl[0], ALL_SHORT, res, iteratimes);
//		Process_ReadTrace(Read_trace, W_Req_Tbl[0], ALL_LONG, res, iteratimes);
	    /*-------------------------------------------------------------------------------------------------------------------------------------------*/

		if(recon == 1)
	    {
		    printf("totalline: %lld || writecount: %lld || readcount: %lld || recon_read: %lld degraded_read: %lld \n", total_line, write_count, read_count, recon_stripe_count, degraded_read);
		    printf("R/W [%4.3f] Abnormal %lld\n", (float) read_count / write_count, abnormal);
	    }
	}

	Cal_total_statistics(recon);

	return res;

}



