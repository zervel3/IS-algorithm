#include <omp.h>
# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<time.h>
# include<math.h>
int *Degree, **Edges, **Neighbors ,M, N;   
float **Pij_weight, *Influence_star , *Influence, *Influence1 ,*Susceptibility_star , *Susceptibility , *Susceptibility1 , *Fstar, *Gstar ; 
float deleta = 0.0 ;
int **Neighbor_total_try , **Neighbor_sucess_Infect ; 
void Get_size_of_vertex_and_edges(char **argv) ;
void Init()  ;
void Calculate_degree_neighbours(char **argv) ;
void Generate_uniform_Fstar_and_Gstar() ;
void Input_Fstar_and_Gstar(char **argv) ;
void Init_Influence_and_Susceptibility() ; 
void Iterate_Influence_label(int Iteration, char **argv) ;
float Judge_whether_converge(float *f1, float *f2) ;
void Parallel_Influence_and_Susceptibility(int i) ;
void Exchange_Influence_and_Susceptibility() ;
void FreeMemory() ;
void Output(char **argv) ;
void Output_detaildata(char **argv) ;
void Generate_pij_weight() ;
int main(int agrs, char **argv){
	
	srand(time(NULL)) ; // reset the time seed 
	Get_size_of_vertex_and_edges(argv) ;  // input the network structure
	Init()  ;  // initilize the variables used in the program
	Calculate_degree_neighbours(argv) ; //Calculate the in and out degree of all nodes, and generate a chain table to conserve the structure of dataset
	Input_Fstar_and_Gstar(argv) ; //Input the F_star and G_star
	Init_Influence_and_Susceptibility() ; //Before the iterative algorithm start, set the I^{(0)} and S^{(0)}
	Iterate_Influence_label(atoi(argv[4]),argv) ; // Start the IS iterative algorithm
	Generate_pij_weight() ; //After the algorithm finished, generte the pij by reconstrcuted I and S
	Output(argv) ; // Output the network structure and edge weight
	Output_detaildata(argv) ; // Output individuals' original value, I_star, S_star, F_star, G_star, reconstructed value I, S
	FreeMemory() ; // Free the memory
	return 0 ;
}
void Get_size_of_vertex_and_edges(char **argv)
{
	
	char *file_name ;
	char f1[10] = "datasets/";
	char f2[5] = ".txt" ;
	file_name = (char *) malloc((strlen(argv[1])+strlen(f1)+strlen(f2)+1)*sizeof(char)) ;
	char one_line[100] ;
	sprintf(file_name,"%s%s%s",f1,argv[1],f2) ;
	printf("file name is:%s\n",file_name) ;
	FILE *fout ;
	Edges = (int**)calloc(1,sizeof(int*)) ;  

	int ii = 0 , i , j , maxID = 0 , minID = 100  ,flag = 1 ,count_lines = 0;
	if ((fout = fopen(file_name, "r")) == NULL) { printf("file %sdoes not exist\n",argv[1]); exit(1); }   
	while (fgets(one_line, sizeof(one_line), fout) != NULL)
	{
		if (one_line[0] != '#') {
			Edges[ii] = (int*)calloc(2,sizeof(int)) ;
			flag = sscanf(one_line, "%d\t%d\n", &i, &j);
			Edges[ii][0] = i ;
			Edges[ii][1] = j ;
			ii++ ;
			Edges = (int**)realloc(Edges,(ii+1)*sizeof(int*)) ;
			if (flag != 2) { printf("failed to parse line %s\n", one_line); exit(1); }
			if (i < minID) minID = i;
			if (i > maxID) maxID = i;
			if (j < minID) minID = j;
			if (j > maxID) maxID = j;
			count_lines++;
		}
	}
	fclose(fout);
	Edges = (int**)realloc(Edges,ii*sizeof(int*)) ;
	N  = maxID - minID + 1; //the size of vertex
	M = count_lines ;  //the size of edges
	
}

void Init() {
	Degree =(int*)calloc(N,sizeof(int)) ;	
	Influence = (float*)calloc(N,sizeof(float)) ;
	Susceptibility = (float*)calloc(N,sizeof(float)) ;
	Influence1 = (float*)calloc(N,sizeof(float)) ;
	Susceptibility1 = (float*)calloc(N,sizeof(float)) ;
	Fstar = (float*)calloc(N,sizeof(float)) ;
	Gstar = (float*)calloc(N,sizeof(float)) ;
	Influence_star = (float*)calloc(N,sizeof(float)) ;
	Susceptibility_star = (float*)calloc(N,sizeof(float)) ;
}
void Calculate_degree_neighbours(char **argv)
{
	int count ;
	for(int i = 0 ; i < M ;i++)
	{
		Degree[Edges[i][0]]++ ;
		Degree[Edges[i][1]]++ ;
	}
	Pij_weight = (float**)calloc(N,sizeof(float*)) ;
	for(int i = 0 ; i < N ; i++)
	{	
		Pij_weight[i] = (float*)calloc(Degree[i],sizeof(float)) ;	
	} 
	Neighbors = (int**)calloc(N,sizeof(int*)) ;
	for(int j = 0 ; j < N ; j++)
	{	Neighbors[j] = (int*)calloc(Degree[j],sizeof(int)) ;
		count = 0 ;
		for(int i = 0 ; i < M ; i++ )
		{
			if(Edges[i][0]==j)
			{	
				Neighbors[j][count] = Edges[i][1] ;
				count++ ;
			} 
			else if(Edges[i][1]==j)
			{	
				Neighbors[j][count] = Edges[i][0] ;
				count++ ;
			}
			else
			  ; 
		}			
	}	
}
void Input_Fstar_and_Gstar(char **argv) 
{
	char *file_name1 , *file_name2 ;
	char f1[10] = "IC/Fstar/";
	char f3[10] = "IC/Gstar/";
	char f2[5] = ".txt" ;
	file_name1 = (char *) malloc((strlen(argv[1])+strlen(argv[3])+strlen(f1)+strlen(f2)+2)*sizeof(char)) ;
	file_name2 = (char *) malloc((strlen(argv[1])+strlen(argv[3])+strlen(f3)+strlen(f2)+2)*sizeof(char)) ;
	sprintf(file_name1,"%s%s_%s%s",f1,argv[1],argv[3],f2) ;  
	sprintf(file_name2,"%s%s_%s%s",f3,argv[1],argv[3],f2) ;
	printf("f1 is:%s\n",file_name1) ;
	FILE *F1, *F2 ;
	F1 = fopen(file_name1,"r") ;
	F2 = fopen(file_name2,"r") ;
	for(int i = 0 ; i < N ; i++)
	{
		fscanf(F1,"%f",&Fstar[i]) ;
		fscanf(F2,"%f",&Gstar[i]) ;
	}
	deleta = atof(argv[2])  ;
	
}
void Init_Influence_and_Susceptibility()
{
	for(int i = 0 ; i < N ; i++)
	{
		Influence[i] = 0.5 ;
		Susceptibility[i] = 0.5 ;
	}
}
void Iterate_Influence_label(int Iteration, char **argv)
{
	int i = 0  ,jj = 0 ,t0 = 0 ;
	float value1 , value2 ;
	char f1[15] = "Iteration/Emp/" ;
	char f2[5] = ".txt" ;
	char *filename ;
	filename = (char*)calloc(strlen(f1)+strlen(f2)+strlen(argv[1])+1,sizeof(char)) ;
	sprintf(filename,"%s%s%s",f1,argv[1],f2) ;
	FILE *fout ;
	fout =fopen(filename,"w") ;
	while(jj < Iteration)
	{  
		#pragma omp parallel for num_threads(8)
		for(i = 0 ; i < N ; i++)
		{	
			Parallel_Influence_and_Susceptibility(i) ;
		}
		
		value1 = Judge_whether_converge(Influence,Influence1) ;
		value2 = Judge_whether_converge(Susceptibility,Susceptibility1) ;
		fprintf(fout,"%d %f %f\n",jj,(float)value1/N, value2<0.00001/N) ;
		if((float)value1/N<0.00001&&(float)value2<0.00001/N)
		{
			break ;
		}
		else
		{
			Exchange_Influence_and_Susceptibility(Influence,Influence1) ;
			Exchange_Influence_and_Susceptibility(Susceptibility,Susceptibility1) ;
			jj++ ;
		}
		
	 }
	 fclose(fout) ;
}
void Parallel_Influence_and_Susceptibility(int i )
{
			float part1 , part2 , part3, part4 ;
			part2 = 0.0 ;
			part4 = 0.0 ;
			for(int j = 0 ; j < Degree[i] ; j++)
			{	part1 = 0.0 ;
			    part3 = 0.0 ;
				for(int k = 0 ; k < Degree[Neighbors[i][j]] ; k++)
				{
					part1 =  part1 + Influence[Neighbors[Neighbors[i][j]][k]] ;	
					part3=  part3 + Susceptibility[Neighbors[Neighbors[i][j]][k]] ;
						
				}
				part1 = (float)Gstar[Neighbors[i][j]]/(part1+deleta) ;	
				part3 = (float)Fstar[Neighbors[i][j]]/(part3+deleta) ;
				
				
				part2 = part2 + part1 ; 
				part4= part4 + part3 ; 	
			}
			Influence1[i] = (float)Fstar[i]/(part2+deleta) ;
			Susceptibility1[i] = (float)Gstar[i]/(part4+deleta) ;
	
}
float  Judge_whether_converge(float *Inf1, float *Sus){
	float sum1 = 0.0 ;
	for( int i = 0 ; i < N ; i++)
	{	
		sum1 = sum1 + fabs(Inf1[i]-Sus[i]) ;
	}
	return sum1 ;
}
void Exchange_Influence_and_Susceptibility(int *arr1, int *arr2)
{
	for(int i = 0 ; i < N ; i++)
	{
		arr1[i] = arr2[i]  ;
	}
	
}
void Output(char **argv)
{	
	
	char f1[13] = "results_Emp/";
	char f2[5] = ".txt" ;
	char *file_name = (char *) malloc((strlen(argv[1])+strlen(argv[2])+strlen(f1)+strlen(f2)+2)*sizeof(char)) ;
	sprintf(file_name,"%s%s_%s%s",f1,argv[1],argv[2],f2) ;
	int i ;
	FILE *fout ;
	fout =fopen(file_name,"w") ; 
	for(int i = 0 ; i < N ; i++)
	{	
		for(int j = 0 ; j < Degree[i] ; j++)
		{	
			fprintf(fout,"%d %d %f\n",i,Neighbors[i][j],Pij_weight[i][j]) ;
		}
	} 
	fclose(fout);	
}
void Output_detaildata(char **argv)
{
	char f1[24] = "results_Emp/detaildata/";
	char f2[5] = ".txt" ;
	char *file_name = (char *) malloc((strlen(argv[1])+strlen(argv[2])+strlen(f1)+strlen(f2)+2)*sizeof(char)) ;
	sprintf(file_name,"%s%s_%s%s",f1,argv[1],argv[2],f2) ;
	int i  ;
	FILE *fout ;
	fout = fopen(file_name,"w") ;
	fprintf(fout,"%s %s %s %s\n","Inf_est","Sus_est","F_GT","G_GT") ;
	for(i = 0 ; i < N ; i++)
		fprintf(fout,"%.4f %.4f %.4f %.4f\n",Influence1[i], Susceptibility[i] ,Fstar[i], Gstar[i]) ;
	fclose(fout)   ;	
}
void Generate_pij_weight()
{	
	for(int i = 0 ; i < N ; i++)
	{	
		for(int j = 0 ; j < Degree[i] ; j++)
		{
			Pij_weight[i][j] = Influence1[i]*Susceptibility1[Neighbors[i][j]] ;
		}
		
	}

}
void FreeMemory() 
{	
	free(Degree) ;
	for(int i = 0 ; i < M ; i++)
	{
		free(Edges[i]) ;
	}
	free(Edges) ;

	
}
