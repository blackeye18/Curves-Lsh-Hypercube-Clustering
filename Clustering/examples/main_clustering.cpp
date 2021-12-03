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





 
//Ousiastika mia miksh ths main tou lsh me thn main tou hypercube.
int main(int argc, char *argv[]){
	int complete_flag;
	int K_medians,L,k_lsh,M,k_hypercube,probes;
	char input_file[256],configuration_file[256],output_file[256],method[256];
	vec* nvectors;
	int no_of_vectors,no_of_coordinates;
    Lhashtables *lht=NULL;
    hypercube *cube=NULL;
	if(input_handler(argc,argv,input_file,configuration_file,output_file,method,&K_medians,&L,&k_lsh,&M,&k_hypercube,&probes,&complete_flag))//diavasma conf file kai arguments
		return -1;

	cout<<"input_file: "<<input_file<<" configuration_file: "<<configuration_file<<" output_file :"<<output_file<<" method: "<<method<<endl;
	cout<<"K_medians: "<<K_medians<<" L: "<<L<<" k_lsh: "<<k_lsh<<" M: "<<M<<" k_hypercube: "<<k_hypercube<<" probes: "<<probes<<" Complete: "<<complete_flag<<endl;


	nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);//diavasma input vector 
	if(nvectors==NULL)
	    return -1;
	printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
	
    
    cout<<"Now using Kmeans++"<<endl;
    auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

    cluster clus(K_medians,no_of_vectors,no_of_coordinates);//arxikopoihsh centroids me kmeans++
    vector<vec>* clustersvec;
    clustersvec=clus.Kmeanplus(nvectors);

/*
   cout<<endl<<clustersvec->size()<<endl;//afto prepei na fygei otan teleiwsoume
    for(int i=0; i<clustersvec->size();i++){
        //int temp=clustersvec->at(i);
        for(int j=0;j<no_of_coordinates;j++)
            cout<<(clustersvec->at(i)).coord.at(j)<<" ";
        cout<<endl;
    }
*/
    vector<vector<vec*>>* cluster_neighbours;

    if( strcmp(method,"Classic")==0){//ama einai classic kaleite h repeat gia lloyds

        cout<<"Now using Lloyds"<<endl;
        cluster_neighbours=clus.repeat(nvectors,clustersvec,0,NULL);//0 gia lloyds

    }else if(strcmp(method,"LSH")==0){//ama einai me lsh ftiaxnontai oi domes  tou lsh kai kaleite h repeat gia lsh
        cout<<"Now using LSH"<<endl;
        lht=new Lhashtables(L,no_of_coordinates,k_lsh);
        lht->lsh_start(no_of_vectors,nvectors);
        cluster_neighbours=clus.repeat(nvectors,clustersvec,1,(void*)lht);//1 gia lht

    }else if(strcmp(method,"Hypercube")==0){//ama einai me hypercube tote ftiaxnontai oi domes tou hypercube kai kaleite h repeat gia to hypercube
        cout<<"Now using Hypercube"<<endl;
        cube=new hypercube(M,probes,no_of_coordinates,k_hypercube,no_of_vectors);
        cube->cube_start(no_of_vectors,nvectors);
        cluster_neighbours=clus.repeat(nvectors,clustersvec,2,(void*)cube);//2 gia cube

    }else {//failsafe 
        cout<<"Method: "<<method<<" Not defined :("<<endl;
        delete clustersvec;
        delete [] nvectors;
        return -1;
    }
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    double time1=((double)duration1.count()/1000000);

/*
       
     int tempsum=0;
     for(int w=0;w<cluster_neighbours->size();w++){
        tempsum+=(*cluster_neighbours)[w].size();
        cout<<"clust "<<w<<" "<<(*cluster_neighbours)[w].size()<<endl;
     }
     cout<<"sum "<<tempsum<<endl;*/
    vector<long double>* silhouette_vec=NULL ;

    cout<<"Now using silhouette"<<endl;//ypologismos toy silhouette
    silhouette_vec =clus.silhouette(cluster_neighbours,clustersvec,nvectors);
    for (int i = 0; i < silhouette_vec->size(); ++i)
        {
        cout<<"silhouette for cluster: "<<i<<" : "<<(*silhouette_vec)[i]<<endl;
        }
    


    //synarthsh pou ektypwnei ta apotelesmata sto arxeio opws afto zhteite
    print_to_file(clustersvec,cluster_neighbours,complete_flag,output_file,method,no_of_coordinates,no_of_vectors,nvectors,time1,silhouette_vec);

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
    if(strcmp(method,"LSH")==0)//analoga thn methodo ginete eleftherwsh mnhmhs
        delete lht;
    if(strcmp(method,"Hypercube")==0)
        delete cube;


	return 0;
}