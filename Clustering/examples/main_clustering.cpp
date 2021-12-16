#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <map>
#include <cassert>
#include <queue>
#include <bitset>
#include <algorithm>


using namespace std;
using namespace std::chrono;


#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "cube_basic_functions.hpp"
#include "cluster_main_functions_silhouette.hpp"

string metric = "euclidean_distance";

double FILTER_E=2;

void stand_dev(vec* nvects,int no_of_vectors,int no_of_coordinates)
	{
	double c_sum=0;
	for (int i = 0; i < no_of_vectors; ++i)
		{
		double n_sum=0;
		for (int j = 0; j < no_of_coordinates; ++j)
			{
			n_sum+=nvects[i].coord[j];
			}
		double n_avg=n_sum/no_of_coordinates;

		double d_sum=0;
		for (int j = 0; j < no_of_coordinates; ++j)
			{
			double temp=nvects[i].coord[j]- n_avg;
			d_sum+=abs(temp);
			}
		c_sum+=(d_sum/no_of_coordinates);
		}
	FILTER_E=PERC_E*c_sum/no_of_vectors;cout<<"FILTER_E "<<FILTER_E<<endl;
	}
int Mean_duplicates(vector<double>* coord)
	{
	int temp_no_coord=coord->size();
    for (int j = 1; j < temp_no_coord; ++j)
        {
        if(coord->at(j-1)==coord->at(j))
                {
                coord->erase(coord->begin() + j);
                temp_no_coord--;
            	}
        }
    return 0;
	}

int Mean_filter(vector<double>* coord,int triads)
    {
        int temp_no_coord=coord->size()-1;
        for (int j = 1; j < temp_no_coord; ++j)
            {
            double a=coord->at(j-1);
            double b=coord->at(j);
            double c=coord->at(j+1);

            if(abs(a-b)<=FILTER_E && abs(b-c)<=FILTER_E)
                {
                coord->erase(coord->begin() + j);
                temp_no_coord--;

                if(!triads)
                    j--;
                }

            }
        
    return 0;
    }
vector<double>* MeanCurve(vector<double> nvec,vector<double> qvec,int mv,int mq)
    {
    //dfd table
    vector<vector<long double>> C;
    C.resize(mv,vector<long double>(mq));

    long double dist=abs(nvec[0]-qvec[0]);
    C[0][0]=dist;


    for (int i = 1; i < mq; ++i)
        {
        long double dist=abs(nvec[0]-qvec[i]);
        C[0][i]=max(dist,C[0][i-1]);
        }

    for (int i = 1; i < mv; ++i)
        {
        long double dist=abs(nvec[i]-qvec[0]);
        C[i][0]=max(dist,C[i-1][0]);
        }

    for (int i = 1; i < mv; ++i)
        {
        for (int j = 1; j < mq; ++j)
            {
            long double dist=abs(nvec[i]-qvec[j]);

            long double mprev=min(min(C[i-1][j],C[i-1][j-1]),C[i][j-1]);
            C[i][j]=max(dist,mprev);
            }
        }
    //algorithm of page 30 in curves.pdf
    int Pi=mv-1;
    int Qi=mq-1;

    vector<tuple<double,double>> traversal;

    traversal.insert(traversal.begin(),make_tuple(Pi,Qi));

    while(Pi!=0 && Qi !=0)
        {
        //calculating min Index
        int minIdx=0;

        if(C[Pi][Qi-1]<C[Pi-1][Qi])
            {
            minIdx++;
            if(C[Pi-1][Qi-1]<C[Pi][Qi-1])
                minIdx++;
            }
        else if(C[Pi-1][Qi-1]<C[Pi-1][Qi])
            minIdx=2;

        if(minIdx==0)
            traversal.insert(traversal.begin(),make_tuple(--Pi,Qi));
        else if(minIdx==1)
            traversal.insert(traversal.begin(),make_tuple(Pi,--Qi));
        else if(minIdx==2)
            traversal.insert(traversal.begin(),make_tuple(--Pi,--Qi));
        else
            {
            cout<<"ERROR minIdx out of range 0-2"<<endl;
            }

        }
    vector<double>* mvec_ptr=new vector<double>;

    for (int i = 0; i < traversal.size(); ++i)
        {
        double cord=nvec[get<0>(traversal[i])] + qvec[get<1>(traversal[i])];cord/=2;
        mvec_ptr->push_back(cord);
       	//Mean_filter(mvec_ptr,730);
        if(mvec_ptr->size()>=MAX_MEAN)
        	{
        	Mean_filter(mvec_ptr,0);
        	if(mvec_ptr->size()>=MAX_MEAN)
        		{
        		cout<<"reached max clustersvec size"<<endl;
        		break;
        		}
        	}
        }
    Mean_filter(mvec_ptr,0);
    //cout<<" size "<<mvec_ptr->size()<<endl;
    //Mean_duplicates(mvec_ptr);cout<<" size after Mean_duplicates "<<mvec_ptr->size()<<endl;
    //Mean_filter(mvec_ptr,0); //cout<<" size after Mean_filter "<<mvec_ptr->size()<<endl;
    return mvec_ptr;
    }

long double MeanNCurves(vector<vec*> nvects,vec*  cvec)
    {

    vector<vector<double>>* prev=new vector<vector<double>>;

    if(nvects.size()<1)
    	{
    	cout<<"ERROR nvects is empty"<<endl;
    	return 1;
    	}
    for (int i = 0; i < nvects.size(); ++i)
        {
        prev->push_back(nvects[i]->coord);
        }
    //ta kanw shuffle
    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    shuffle(begin(*prev), end(*prev), e);   

    while(prev->size()>1)
        {
        vector<vector<double>>* curr=new vector<vector<double>>;//xrisimopoiume 2 vectors anti binary tree logo tis apousias tu apto stl kai poliplokotitas xwrou

        for (int i = 0; i < prev->size()-1; i+=2)
            {
            vector<double>* mvec_ptr=MeanCurve(prev->at(i),prev->at(i+1),prev->at(i).size(),prev->at(i+1).size());
            curr->push_back(*mvec_ptr);

            mvec_ptr->empty();
            delete mvec_ptr;
            }

        if(prev->size()%2==1)//an exei mono arithmo
            curr->push_back(prev->back());

        for (int i = 0; i < prev->size(); ++i)
            prev->at(i).empty();
        prev->empty();delete prev;

        prev=curr;
        }

    //cout<<"old clustersvec with size "<< cvec->coord.size()<<" and new "<<prev->at(0).size()<<endl;
   /* for (int i = 0; i < cvec->coord.size(); ++i)
    	{
    	cout<<cvec->coord[i]<<" ";
    	}
    cout<<endl;*/

    long double Avg_diff=dfd(prev->at(0),cvec->coord,prev->at(0).size(),cvec->coord.size());//cout<<"Avg diff "<<Avg_diff<<endl;
    //Avg_diff/=cvec->coord.size();

    int size=prev->at(0).size();cout<<"cluster size "<<size<<endl;
    cvec->coord.clear();
    for (int i = 0; i < size; ++i)
        {
        if(i<prev->at(0).size())
        	cvec->coord.push_back(prev->at(0)[i]);
        else
        	cvec->coord.push_back(M_big_num);
        }


    prev->at(0).empty();
    prev->empty();delete prev;

    //cout<<"new clustersvec with size "<< cvec->coord.size()<<endl;
   /* for (int i = 0; i < cvec->coord.size(); ++i)
    	{
    	cout<<cvec->coord[i]<<" ";
    	}
    cout<<endl;*/
    //cout<<"alright MeanNCurves"<<endl;
    return Avg_diff;
    }



vec* snapping(vec* nvectors,int no_of_coordinates,int no_of_vectors,double delta,vector<double> td1,vector<double>td2){
    vector<vector<vector<double>>> temp;
    temp.resize(no_of_vectors,vector<vector<double> >(no_of_coordinates,vector<double>(2)));////resize analoga 
    for(int i=0;i<no_of_vectors;i++){//kanw adistoixish me meres 
        for(int j=0;j<no_of_coordinates;j++){
            temp[i][j][0]=(j);
            temp[i][j][1]=nvectors[i].coord[j];
        }
    }
    
    int same_counter=0;
    double prev1=0,prev2=0;
    int first_flag=0;
    vector<vector<vector<double>>> temp2;
    temp2.resize(no_of_vectors,vector<vector<double> >(no_of_coordinates));////resize analoga 
    for(int i=0;i<no_of_vectors;i++){//kanw adistoixish me meres 
        first_flag=0;

        for(int j=0;j<no_of_coordinates;j++){
            if(first_flag==0){
                temp2[i][j].push_back(floor(((temp[i][j][0]-td1[j])/delta)+1/2)*delta +td1[j]);
                temp2[i][j].push_back(floor(((temp[i][j][1]-td2[j])/delta)+1/2)*delta + td2[j]);
                prev1=temp2[i][j][0];
                prev2=temp2[i][j][1];
                first_flag=1;
            }else{
                double temphold1,temphold2;
                temphold1=(floor((temp[i][j][0]/delta)+1/2));
                temphold2=(floor((temp[i][j][1]/delta)+1/2));
                if(temphold1==prev1 && temphold2==prev2){
                    same_counter++;
                }else{
                    temp2[i][j].push_back(temphold1);
                    temp2[i][j].push_back(temphold2);
                    prev1=temphold1;
                    prev2=temphold2;
                }

            }
        }
    }
    //cout<<same_counter<<endl;
    //cout<<temp2[0][1].size()<<endl;
    vec* newnvectors;
    newnvectors = new vec[no_of_vectors];//desmevoume xwro gia ta vectors
    for(int i=0;i<no_of_vectors;i++){
        newnvectors[i].name=nvectors[i].name;
    }
    for(int i=0;i<no_of_vectors;i++){
        for(int j=0;j<no_of_coordinates;j++){
            for(int k=0;k<temp2[i][j].size();k++){
                newnvectors[i].coord.push_back(temp2[i][j][k]);

            }
        }
    }
    for(int i=0;i<no_of_vectors;i++){
        int tempint=newnvectors[i].coord.size();
        while(tempint!=no_of_coordinates*2){
            newnvectors[i].coord.push_back(M_big_num);
            newnvectors[i].coord.push_back(M_big_num);
            tempint=newnvectors[i].coord.size();
        }
    }
    //delete [] nvectors;
    // for(int i=0;i<no_of_vectors;i++){
    //     cout<<newnvectors[i].name<<endl;
    // }
    return newnvectors;
}



 
//Ousiastika mia miksh ths main tou lsh me thn main tou hypercube.
int main(int argc, char *argv[]){
	int complete_flag;
    int silhouette_flag;
	int K_medians,L,k_lsh,M,k_hypercube,probes;
	char input_file[256],configuration_file[256],output_file[256],assigment[256],update[256];
	vec* nvectors;
	int no_of_vectors,no_of_coordinates;
    Lhashtables *lht=NULL;
    hypercube *cube=NULL;
	if(input_handler(argc,argv,input_file,configuration_file,output_file,assigment,update,&K_medians,&L,&k_lsh,&M,&k_hypercube,&probes,&complete_flag,&silhouette_flag))//diavasma conf file kai arguments
		return -1;

	cout<<"input_file: "<<input_file<<" configuration_file: "<<configuration_file<<" output_file :"<<output_file<<" assigment: "<<assigment<<" update: "<<update<<endl;
	cout<<"K_medians: "<<K_medians<<" L: "<<L<<" k_lsh: "<<k_lsh<<" M: "<<M<<" k_hypercube: "<<k_hypercube<<" probes: "<<probes<<" Complete: "<<complete_flag<<" silhouette: "<<silhouette_flag<<endl;

    int assigment_flag=0;
    int update_flag=0;

    if(strcmp(assigment,"Classic")==0){
        assigment_flag=0;
        if(strcmp(update,"MeanFrechet")==0){
            cout<<"aaa";
            metric="LSH_Frechet_Discrete";
            update_flag=1;
        }else if(strcmp(update,"MeanVector")==0){
            metric="euclidean_distance";
            update_flag=2;
        }else{
            cout<<"Unknown metric. Exiting."<<endl;
            return -1;
        }
    }
    else if(strcmp(assigment,"LSH")==0)
        assigment_flag=1;
    else if(strcmp(assigment,"Hypercube")==0)
        assigment_flag=2;
    else if(strcmp(assigment,"LSH_Frechet")==0){
        assigment_flag=3;
        metric="LSH_Frechet_Discrete";
        update_flag=1;
    }
    else{
        cout<<"Unknown assigment. Exiting."<<endl;
        return -1;
    }
    metric="LSH_Frechet_Discrete";
	nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);//diavasma input vector 
	if(nvectors==NULL)
	    return -1;
	printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);

	if(metric=="LSH_Frechet_Discrete")
		stand_dev(nvectors,no_of_vectors,no_of_coordinates);

	//if(update_flag==1){
        //nvectors=snapping(nvectors,no_of_coordinates,no_of_vectors,delta);
        //no_of_coordinates=no_of_coordinates*2;
    //}
    cout<<no_of_coordinates<<endl;
    cout<<"Now using Kmeans++"<<endl;
    auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

    cluster clus(K_medians,no_of_vectors,no_of_coordinates);//arxikopoihsh centroids me kmeans++
    vector<vec>* clustersvec;
    clustersvec=clus.Kmeanplus(nvectors);

    cout<<"clusters"<<endl;
    for (int i = 0; i < clustersvec->size(); ++i)
    	{
    	cout<<"cluster "<<i<<"with no_of_coordinates "<<clustersvec->at(i).coord.size()<<endl;
    	}
   	cout<<endl<<endl;
    vector<vector<vec*>>* cluster_neighbours;

    if( assigment_flag==0){//ama einai classic kaleite h repeat gia lloyds

        cout<<"Now using Lloyds"<<endl;
        cluster_neighbours=clus.repeat(nvectors,clustersvec,0,NULL);//0 gia lloyds

    }else if(assigment_flag==1 || assigment_flag==3){//ama einai me lsh ftiaxnontai oi domes  tou lsh kai kaleite h repeat gia lsh
        cout<<"Now using LSH"<<endl;
        if (assigment_flag==1)
            lht=new Lhashtables(L,no_of_coordinates,k_lsh);
        else if(assigment_flag==3)
        	{
            lht=new Lhashtables(L,2*no_of_coordinates,k_lsh);
        	}
        cout<<"lsh_start"<<endl;
        lht->lsh_start(no_of_vectors,nvectors);
        cout<<"Repeat"<<endl;
        cluster_neighbours=clus.repeat(nvectors,clustersvec,1,(void*)lht);//1 gia lht

    }else if(assigment_flag==2){//ama einai me hypercube tote ftiaxnontai oi domes tou hypercube kai kaleite h repeat gia to hypercube
        cout<<"Now using Hypercube"<<endl;
        cube=new hypercube(M,probes,no_of_coordinates,k_hypercube,no_of_vectors);
        cube->cube_start(no_of_vectors,nvectors);
        cluster_neighbours=clus.repeat(nvectors,clustersvec,2,(void*)cube);//2 gia cube

    }else {//failsafe 
        cout<<"Assigment: "<<assigment<<" Not defined :("<<endl;
        delete clustersvec;
        delete [] nvectors;
        return -1;
    }
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    double time1=((double)duration1.count()/1000000);

    vector<long double>* silhouette_vec=NULL ;
    if(silhouette_flag==1){
        cout<<"Now using silhouette"<<endl;//ypologismos toy silhouette
        silhouette_vec =clus.silhouette(cluster_neighbours,clustersvec,nvectors);
        for (int i = 0; i < silhouette_vec->size(); ++i)
            {
            cout<<"silhouette for cluster: "<<i<<" : "<<(*silhouette_vec)[i]<<endl;
            }
    }


    //synarthsh pou ektypwnei ta apotelesmata sto arxeio opws afto zhteite
    print_to_file(clustersvec,cluster_neighbours,complete_flag,output_file,assigment,no_of_coordinates,no_of_vectors,nvectors,time1,silhouette_vec);

    cout<<"Output File Created!!"<<endl;



    for (int i = 0; i < cluster_neighbours->size(); ++i)
        {
        (*cluster_neighbours)[i].clear();
        }
    cluster_neighbours->clear();
    delete cluster_neighbours;
    
    
    delete clustersvec;
    if(silhouette_vec!=NULL)
        delete silhouette_vec;
    delete [] nvectors;
    if(assigment_flag==1 || assigment_flag==3)//analoga thn methodo ginete eleftherwsh mnhmhs
        delete lht;
    if(assigment_flag==2)
        delete cube;


	return 0;
}