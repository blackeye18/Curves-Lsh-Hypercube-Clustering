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



vec* snapping(vec* nvectors,int no_of_coordinates,int no_of_vectors,double delta){
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
                temp2[i][j].push_back(floor((temp[i][j][0]/delta)+1/2));
                temp2[i][j].push_back(floor((temp[i][j][1]/delta)+1/2));
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
    delete [] nvectors;
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

	nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);//diavasma input vector 
	if(nvectors==NULL)
	    return -1;
	printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
	if(update_flag==1){
        nvectors=snapping(nvectors,no_of_coordinates,no_of_vectors,delta);
        no_of_coordinates=no_of_coordinates*2;
    }
    cout<<no_of_coordinates<<endl;
    cout<<"Now using Kmeans++"<<endl;
    auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

    cluster clus(K_medians,no_of_vectors,no_of_coordinates);//arxikopoihsh centroids me kmeans++
    vector<vec>* clustersvec;
    clustersvec=clus.Kmeanplus(nvectors);

    vector<vector<vec*>>* cluster_neighbours;

    if( assigment_flag==0){//ama einai classic kaleite h repeat gia lloyds

        cout<<"Now using Lloyds"<<endl;
        cluster_neighbours=clus.repeat(nvectors,clustersvec,0,NULL);//0 gia lloyds

    }else if(assigment_flag==1 || assigment_flag==3){//ama einai me lsh ftiaxnontai oi domes  tou lsh kai kaleite h repeat gia lsh
        cout<<"Now using LSH"<<endl;
        lht=new Lhashtables(L,no_of_coordinates,k_lsh);
        lht->lsh_start(no_of_vectors,nvectors);
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