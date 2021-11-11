# include<stdio.h>
# include<time.h>
# include<stdlib.h>
# include<string.h>
# include<math.h>
typedef struct Edge_probability{
	int node1 ;
	int node2 ;
	float probability ;
}Edge_probability;
Edge_probability *Network ; //define the global vairance.
int maxID = 0 , minID = 1000000 , len_of_edges = 0  , N ; // len_of_edges : the sum of edges , N : the size of network
int *IndexTag , Seeds , *Chance_to_Infect , **Neighbor ,*Degree ; // Seeds: the choosed seeds, Neighbor, the neighbor of nodes.
int **Neighbor_total_try , **Neighbor_sucess_Infect ; 
float **Neighbor_weight , *Fstar, *Gstar ;
void Input_edge_probability(char **argv) ;
void Init() ;
void Get_the_degree_and_neighbors_of_Node() ;
int Run_IC_model(int k) ;
void Free_the_parameter() ;
void Output_Empirical_Real_pij(char **argv) ;
void Output_Empirical_Fstar_Gstar(char **argv) ;
void Print_value() ;
int main(int argn , char **argv)
{	
	srand(time(NULL)) ;
	int sum = 0 ,tmp = 0 , count =0 ;
	float th1 = 0.0 , th2 = 0.0 ;
	Input_edge_probability(argv)	 ;
	Get_the_degree_and_neighbors_of_Node() ;
	for (int i = 0 ; i < N; i++)
	{		printf("i is:%d\n",i) ;
		 	count = 0 ;
		 	Seeds = i ;
		 	sum = 0 ;
		 	while(count < atoi(argv[3]))
		 	{	
				Init()  ;  	
				tmp = Run_IC_model(1)   ;	
				sum = sum + tmp ;  
				count++ ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
				Free_the_parameter() ;		 		
			}
	}
	Output_Empirical_Real_pij(argv) ;
	Output_Empirical_Fstar_Gstar(argv) ;
	return  0 ;
} 
void Free_the_parameter()
{
	free(IndexTag) ;
	free(Chance_to_Infect) ;
}
void Input_edge_probability(char **argv)
{	FILE *fin ;
	char one_line[100] ;
	Network =(Edge_probability*)calloc(1,sizeof(Edge_probability)) ;
	int i = 0 ,j = 0 ,flag = 1 ,count_lines = 0 ;
	float pro = 0 ; 
	char f1[16] = "results_theory/" ;	
	char f2[5] = ".txt" ;
	char *file_path1 ;
	file_path1 = (char *)malloc((strlen(argv[1])+strlen(argv[2])+strlen(f1)+strlen(f2)+3)*sizeof(char)) ;
	sprintf(file_path1,"%s%s_%s%s",f1,argv[1],argv[2],f2)  ;
	
	fin = fopen(file_path1, "r") ;
	if (fin == NULL) 
	{ printf("%s\n",file_path1); exit(1); } 
	while (fgets(one_line, sizeof(one_line), fin) != NULL){
		flag = sscanf(one_line, "%d\t%d\t%f\n", &i, &j, &pro) ;
		if (i < minID) minID = i;
		if (i > maxID) maxID = i;
		if (j < minID) minID = j;
		if (j > maxID) maxID = j;
		Network[count_lines].node1 = i ;
		Network[count_lines].node2 = j ;
		Network[count_lines].probability = pro ;

		count_lines++;
		Network = (Edge_probability*)realloc(Network,(count_lines+1)*sizeof(Edge_probability)) ;	
	}
	len_of_edges = count_lines ; 
	printf("length of edges are%d\n",len_of_edges) ;
	N = maxID - minID + 1 ;

}

// this function is to get the neighbor of each nodes.
void Get_the_degree_and_neighbors_of_Node()
{	
	// get the degree of nodes
	Degree = (int*)calloc(N,sizeof(int)) ; 
	for(int i = 0 ; i < len_of_edges ;i++)
	{
		Degree[Network[i].node1]++ ;				
	}
	int index = 0 ;
	// get the neighbor of nodes.
	Neighbor = (int**)calloc(N,sizeof(int*)) ;
	Neighbor_weight = (float**)calloc(N,sizeof(float*)) ;
	Neighbor_total_try = (int**)calloc(N,sizeof(int*)) ;
	Neighbor_sucess_Infect = (int**)calloc(N,sizeof(int*)) ;
	for(int i = 0 ; i < N ; i++)
	{	index = 0 ;
		Neighbor[i] = (int*)calloc(1,sizeof(int)) ;
		Neighbor_total_try[i] = (int*)calloc(1,sizeof(int)) ;
		Neighbor_sucess_Infect[i] = (int*)calloc(1,sizeof(int)) ;
		Neighbor_weight[i] = (float*)calloc(1,sizeof(float)) ;
		for(int j = 0 ; j < len_of_edges ; j++)
		{
			if(Network[j].node1==i)
			{
				Neighbor[i][index] = Network[j].node2 ;
				Neighbor_total_try[i][index] = 0 ;
				Neighbor_sucess_Infect[i][index] = 0 ;
				Neighbor_weight[i][index] = Network[j].probability ;
				index++ ;
				Neighbor[i] =(int*)realloc(Neighbor[i],(index+1)*sizeof(int)) ;
				Neighbor_weight[i] = (float*)realloc(Neighbor_weight[i],(index+1)*sizeof(int)) ;
				Neighbor_total_try[i] = (int*)realloc(Neighbor_total_try[i],(index+1)*sizeof(int)) ;
				Neighbor_sucess_Infect[i] = (int*)realloc(Neighbor_sucess_Infect[i],(index+1)*sizeof(int)) ;
			}
		}
		Neighbor[i] =(int*)realloc(Neighbor[i],index*sizeof(int)) ;
		Neighbor_weight[i] = (float*)realloc(Neighbor_weight[i],index*sizeof(int)) ;
		Neighbor_total_try[i] = (int*)realloc(Neighbor_total_try[i],index*sizeof(int)) ;
		Neighbor_sucess_Infect[i] = (int*)realloc(Neighbor_sucess_Infect[i],index*sizeof(int)) ;		
	}	
}
int Run_IC_model(int K) 
{
	int tag = 0 ;
	float p = 0.0 ;
	int we_shoud_stop = 0 ;
	for(int i = 0 ; i <K ;i++)
		IndexTag[Seeds] = 1  ; // IndexTag = 1  means the nodes were infected. // Chance_to_Infect, if C
	while(tag == 0 )																//if Chance_to_Infect = 1 , means there is no chance 
	{	we_shoud_stop = 0  ;																		// for the node to infected its neighbor.				
		for(int i = 0 ; i < N ; i++)
		{
			if(IndexTag[i]==1&&Chance_to_Infect[i]==0) //if node1 was infected, then , it will infect its neighbor. 
			{
					for(int j =0 ;j <Degree[i] ;j++)
					{    Neighbor_total_try[i][j]++ ;
						 p = (float)(rand()%1000)/1000 ;
						 if(p<Neighbor_weight[i][j])
						 {
						 	Neighbor_sucess_Infect[i][j]++ ;
						 	IndexTag[Neighbor[i][j]] = 1 ;	
						 }	 
					}
					Chance_to_Infect[i] = 1 ;
					we_shoud_stop++ ;
			}	
		}
		if(we_shoud_stop==0) 
			break ;	 	
	}
	int Final_Influence = 0 ;
	for(int i = 0 ; i < N ; i++)
	{
			if(IndexTag[i]==1)
				Final_Influence++ ;
	}	
	return Final_Influence-1 ;
}
void Init()
{
	IndexTag = (int*)calloc(N,sizeof(int)) ;
	Chance_to_Infect = (int*)calloc(N,sizeof(int)) ; 	
	
}
void Output_Empirical_Real_pij(char **argv)
{
	FILE *fout ; 
	char f1[4] = "IC/" ;
	char f2[5] = ".txt" ;
	char *ff ; 
	ff = (char*)malloc((strlen(f1)+strlen(f2)+strlen(argv[1])+strlen(argv[3])+1)*sizeof(char)) ;
	sprintf(ff,"%s%s_%s%s",f1,argv[1],argv[3],f2) ;
	printf("%s\n",ff) ;
	fout = fopen(ff,"w")  ;
	fprintf(fout,"%s %s %s  %s\n","i","j","Omegaij","Pij") ;
	for(int i = 0 ; i < N ; i++)
	{
		for(int j = 0 ; j < Degree[i] ; j++)
		{
			fprintf(fout,"%d %d %f %f\n",i,Neighbor[i][j],(float)Neighbor_sucess_Infect[i][j]/Neighbor_total_try[i][j], Neighbor_weight[i][j]) ; 
		}
	}
	fclose(fout) ;	
}

void Output_Empirical_Fstar_Gstar(char **argv)
{
	FILE *fout ,*fout1 ; 
	char f1[10] = "IC/Fstar/" ;
	char f3[10] = "IC/Gstar/" ;
	char f2[5] = ".txt" ;
	char *ff ,*ff1; 
	ff = (char*)malloc((strlen(f1)+strlen(f2)+strlen(argv[3])+strlen(argv[1])+2)*sizeof(char)) ;
	ff1 = (char*)malloc((strlen(f3)+strlen(f2)+strlen(argv[3])+strlen(argv[1])+2)*sizeof(char)) ;
	sprintf(ff,"%s%s_%s%s",f1,argv[1],argv[3],f2) ;
	sprintf(ff1,"%s%s_%s%s",f3,argv[1],argv[3],f2) ; //文件格式 file_name_100.txt ; 
	printf("fstar is:%s\n",ff) ;
	printf("gstar is:%s\n",ff1) ;
	fout = fopen(ff,"w")  ;
	fout1 = fopen(ff1,"w")  ;
	float sum1 = 0.0 ;
	Fstar = (float*)calloc(N,sizeof(float)) ;
	Gstar = (float*)calloc(N,sizeof(float)) ;
	for(int i = 0 ; i < N ; i++)
	{
		for(int j = 0 ; j < Degree[i] ; j++)
		{
			Fstar[i] = Fstar[i] + (float)Neighbor_sucess_Infect[i][j]/Neighbor_total_try[i][j] ;
		//	Gstar[i] = Gstar[i] + (float)Neighbor_sucess_Infect[j][i]/Neighbor_total_try[j][i] ;
		}
	}
	for(int i = 0 ; i < N ; i++)
	{
		for(int j = 0 ; j < N ; j++)
		{
			for(int h = 0 ; h < Degree[j] ; h++)
			{
				if(Neighbor[j][h]==i)
				{ 
					Gstar[i] = Gstar[i] +  (float)Neighbor_sucess_Infect[j][h]/Neighbor_total_try[j][h] ;
					break ; 
				}			
			}
		}
	} 
	for(int i = 0 ; i < N ; i++)
	{
		fprintf(fout,"%f\n",Fstar[i]) ;
		fprintf(fout1,"%f\n",Gstar[i]) ;
	}
	fclose(fout) ;
	fclose(fout1) ;
}
