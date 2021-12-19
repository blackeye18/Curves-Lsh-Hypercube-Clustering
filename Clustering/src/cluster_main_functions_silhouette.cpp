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
#include "frechet_curves.hpp"
//#include "knn_ranges_brutes.hpp"
extern string metric;






long double Mean_coord(vector<vec*> nvects,vec*  cvec,int d)
 	{
 	long double Avg_diff=0;
 	long double Sum_diff=0;
 	for (int di = 0; di < d; ++di)//gia oles tis diastaseis
 		{
 		double sum=0;
 		for (int ni = 0; ni < nvects.size(); ++ni)
 			{
 			sum+=nvects[ni]->coord[di];//prosthetume ola ta seimeia tis antistixis diastasis
 			}

 		double Avg=sum/nvects.size();//diairoume to sum me to sinolo twn simeiwn
 		Sum_diff+=abs(cvec->coord[di]-Avg);//vriskume to a8roisma tis apolitis diaforas proigumenou cluster apto neo
 		cvec->coord[di]=Avg;//vazume tin nea timi tu cluster stin antisixi diastasi
 		}
 	Avg_diff=Sum_diff/d;//vriskume to meso oro tis diaforas
 	return Avg_diff;
 	}



cluster::cluster(int K_medians,int no_of_vectors,int no_of_coordinates)//constructor of cluster class
:K_medians(K_medians),no_of_vectors(no_of_vectors),no_of_coordinates(no_of_coordinates){};

vector<vec>* cluster::Kmeanplus(vec* nvect)//kmeans ++ sel 45-46 nnCluster.pdf
    {
    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    std::uniform_int_distribution<int>  distrC(0,no_of_vectors-1);


    int first_clust=distrC(e);

    vector<vec>*clusters=new vector<vec>;
    clusters->resize(K_medians);
    int counter=0;


    for(int i=0;i<K_medians;i++)
        (clusters->at(i)).coord.resize(no_of_coordinates);//arxikopoioume tis diastaseis tu pinaka me ta clusters 

    (clusters->at(counter)).coord=nvect[first_clust].coord;//pernume ena tixeo cluster me vazi tin uniform katanomi

    counter++;//valame to prwto ara cluster count +1

    vector<long double> partial_sums;
    vector<int> r;

    while(counter<K_medians)
        {
        r.clear();
        partial_sums.clear();
        long double sum=0;
        for (int i = 0; i < no_of_vectors; ++i)
            {

            int tempflag=0;
            for(int k=0;k<counter;k++){//elegxoume an ine idi to simeio cluster
                if(nvect[i].coord==(clusters->at(k)).coord)
                    tempflag=1;
            }
            if(tempflag==0)//an den ine idi cluster
                {
                long double mdist;
                int mci;
                for (int ci = 0; ci < counter; ++ci)//vriskume elaxisti apostasi meta3i tu simeiou i kai enos kentroides
                    {
                    //int clust=clusters->at(ci);
                    if(ci==0)
                    {
                        if(metric=="euclidean_distance"){
                            mdist=vect_dist((clusters->at(ci)).coord,nvect[i].coord,no_of_coordinates);
                            mci=0;
                        }else if(metric=="LSH_Frechet_Discrete"){
                            mdist=dfd((clusters->at(ci)).coord,nvect[i].coord,(clusters->at(ci)).coord.size(),nvect[i].coord.size());
                            mci=0;
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
                    }
                    else
                        {
                        long double dist;
                        if(metric=="euclidean_distance"){
                            dist=vect_dist((clusters->at(ci)).coord,nvect[i].coord,no_of_coordinates);
                        }else if(metric=="LSH_Frechet_Discrete"){
                            dist=dfd((clusters->at(ci)).coord,nvect[i].coord,(clusters->at(ci)).coord.size(),nvect[i].coord.size());
                        }
                        if (dist<mdist)
                            {
                            mdist=dist;
                            mci=ci;
                            }
                        }
                    }
                sum+=mdist*mdist;
                partial_sums.push_back(sum);
                r.push_back(i);
                }
            }
        std::uniform_real_distribution<long double>  distrX(0,sum);//epilegume ena ari8mo me tin uniform katanomi meta3i 0 kai sum
        long double X=distrX(e);
        int position=upper_bound(partial_sums.begin(), partial_sums.end(), X)-partial_sums.begin();//xrisimopoiume tin etoimi sinartisi upper bound
        
        if(position>0)
            if(partial_sums[position-1]>=X)//epidi i sinartisi elegxi an o ari8mos pu 8a mas epistrepsi ine < tu X kai emis to thelume <=
                position--;
  
        (clusters->at(counter)).coord=nvect[r[position]].coord;//isagwgi tu neou kentroides
        counter++;
        }
    return clusters;
    }


vector<vector<vec*>>* cluster::lloyds(vec* nvect,vector<vec>* clustersvec)//sinartisi  gia algoritmo lloyds 1 epanalipsi
    {
    vector<vector<vec*>>* lloydsclust=new vector<vector<vec*>>;
    lloydsclust->resize(clustersvec->size(),vector<vec*>(0));

    for (int i = 0; i < no_of_vectors; ++i)//gia ola ta vec
        {

        long double mdist;
        int mci;
        for (int ci = 0; ci < clustersvec->size(); ++ci)//vriskume tin mikroteri apostasi se kentroides
            {

            if(ci==0)
            {
                if(metric=="euclidean_distance"){
                    mdist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,no_of_coordinates);
                    mci=0;
                }else if(metric=="LSH_Frechet_Discrete"){
                    mdist=dfd(clustersvec->at(ci).coord,nvect[i].coord,clustersvec->at(ci).coord.size(),nvect[i].coord.size());
                    mci=0;
                }
            }
            else
                {
                long double dist;
                if(metric=="euclidean_distance"){
                    dist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,no_of_coordinates);
                }else if(metric=="LSH_Frechet_Discrete"){
                    dist=dfd(clustersvec->at(ci).coord,nvect[i].coord,clustersvec->at(ci).coord.size(),nvect[i].coord.size());
                }
                if (dist<mdist)
                    {
                    mdist=dist;
                    mci=ci;
                    }
                }
            }
        ((*lloydsclust)[mci]).push_back(&(nvect[i]));//isagume to vec sto kentroides me tin mikroteri apostasi
        }
    return lloydsclust;
    }

vector<vector<vec*>>* cluster::repeat(vec* nvect,vector<vec>* clustersvec,int method,void* ss)//Algorithm (EM) sel 34 nnCluster.pdf
	{
	vector<vector<vec*>>* cluster_neighbours=NULL;
	long double diff=0;
	int iteration=0;
	do
		{
		iteration++;
		//to kanw free an den ine NULL
		if(cluster_neighbours!=NULL)
			{
			for (int ci = 0; ci < cluster_neighbours->size(); ++ci)
        		{
        		(*cluster_neighbours)[ci].clear();
        		}
    		cluster_neighbours->clear();
    		delete cluster_neighbours;
    		cluster_neighbours=NULL;
			}

		long double sum_diff=0;
		//analoga me tin methodo vazume ta simeia sta clusters
		if(method==0)
			{
			cluster_neighbours=this->lloyds(nvect,clustersvec);
			}
		else if(method==1)
			{
            int EFLAG=1;
            while(EFLAG)
                {
                EFLAG=0;
			 Lhashtables* lhtables=(Lhashtables*) ss;

			 cluster_neighbours=lhtables->ANN_lsh(nvect,clustersvec,no_of_vectors);

                for (int ci = 0; ci < clustersvec->size(); ++ci)
                    {
                    if (cluster_neighbours->at(ci).size()<1)
                        {
                        cout<<"theres an empty cluster repeating ANN with no "<<ci<<endl;
                        EFLAG=1;
                        }
                    }
                }
			}
		else if (method==2)
			{
			//edw ine gia hypercube
            hypercube* hcube=(hypercube*) ss;
            cluster_neighbours=hcube->ANN_cube(nvect,clustersvec,no_of_vectors);
			}

		//vriskume ta nea clusters
		for (int ci = 0; ci < clustersvec->size(); ++ci)
			{
            if(metric=="euclidean_distance")
		      sum_diff+=Mean_coord(cluster_neighbours->at(ci),&(clustersvec->at(ci)),no_of_coordinates);
            else if(metric=="LSH_Frechet_Discrete"){
                //cout<<"Mphka N curves"<<endl;
                sum_diff+=MeanNCurves(cluster_neighbours->at(ci),&(clustersvec->at(ci)));
                //cout<<"ghhka N curves"<<endl;
            }

			}
			
		diff=sum_diff/clustersvec->size();//ipologizoume tin diafora
        cout<<endl<<"_____________________________________________________"<<endl;
        cout<<"it "<<iteration<<" diff "<<diff<<endl;
        cout<<"_____________________________________________________"<<endl;
		}
	while(diff>=PERCISION && iteration<MAXIT);//oso i diafora ine megaliteri tu percision kai den exume ftasei ton megisto ari8mo epanalipsewn
     //cout<<"diff "<<diff<<" iteration "<<iteration<<endl;
	return cluster_neighbours;
	}
	
vector<long double>* cluster::silhouette(vector<vector<vec*>>* cluster_neighbours,vector<vec>* clustersvec,vec* nvect) //sel 53-54 nnCluster.pdf
    {
    vector<long double>* S=new vector<long double>;


    for (int ci = 0; ci < cluster_neighbours->size(); ++ci)//gia ola ta clusters
        {
         long double sumSi=0;
        int cluster_size=(*cluster_neighbours)[ci].size();
        vector<vector<long double>> prev_dist;
        prev_dist.resize(cluster_size,vector<long double>(cluster_size));

        for (int vi = 0; vi <cluster_size ; ++vi)
            {
            //ipologizoume to  A(i)
            long double sumA=0;
            for (int ni = 0; ni <cluster_size ; ++ni)//simeio sto idio cluster
                {
                if(ni!=vi)
                    {
                    long double dist;
                    if(vi<ni)//ean to vi< n1 den exume 3anaipologisei tin apostasi 
                    {   
                        if(metric=="euclidean_distance"){
                            dist=vect_dist((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[ci][ni]->coord,no_of_coordinates);
                            prev_dist[vi][ni]=dist;//apo8ikevume tin apostasi gia mellontiki xrisi
                        }else if(metric=="LSH_Frechet_Discrete"){
                            dist=dfd((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[ci][ni]->coord,(*cluster_neighbours)[ci][vi]->coord.size(),(*cluster_neighbours)[ci][ni]->coord.size());
                            prev_dist[vi][ni]=dist;//apo8ikevume tin apostasi gia mellontiki xrisi
                        }else{
                            cout<<"No function for metric:"<<metric<<endl;
                            return NULL;
                        }
                    }
                    else
                        dist=prev_dist[ni][vi];//tin exume ipologisei idi

                    sumA+=dist;
                    }
                }
            long double Avga=sumA/(cluster_size-1);

            //ipologizoume to 2o pio kontino cluster
            long double nextbestclust_dist;
            int nextbestclust_ci;

            for (int nci = 0; nci < cluster_neighbours->size(); ++nci)
                {
                if(nci!=ci)
                    {


                    if(nci==0||(ci==0 && nci==1))//gia tin arxiki apostasi
                    {   if(metric=="euclidean_distance"){
                            nextbestclust_dist=vect_dist(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
                            nextbestclust_ci=nci;
                        }else if(metric=="LSH_Frechet_Discrete"){
                            nextbestclust_dist=dfd(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,clustersvec->at(nci).coord.size(),(*cluster_neighbours)[ci][vi]->coord.size());
                            nextbestclust_ci=nci;
                        }
                    }
                    else
                        {
                        long double dist;
                        if(metric=="euclidean_distance"){
                            dist=vect_dist(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,no_of_coordinates);
                        }else if (metric=="LSH_Frechet_Discrete"){
                            dist=dfd(clustersvec->at(nci).coord,(*cluster_neighbours)[ci][vi]->coord,clustersvec->at(nci).coord.size(),(*cluster_neighbours)[ci][vi]->coord.size());
                        }
                        if(dist<nextbestclust_dist)
                            {
                            nextbestclust_dist=dist;
                            nextbestclust_ci=nci;
                            }
                        }
                    }
                }

            //ipologizoume to b(i)
            long double sumB=0;

            int Bcluster_size=(*cluster_neighbours)[nextbestclust_ci].size();


            for (int nbvi = 0; nbvi <Bcluster_size ; ++nbvi)//ipologizoume to  B(i)
                {
                long double dist;
                if(metric=="euclidean_distance"){
                    dist=vect_dist((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[nextbestclust_ci][nbvi]->coord,no_of_coordinates);
                }else if (metric=="LSH_Frechet_Discrete"){
                    dist=dfd((*cluster_neighbours)[ci][vi]->coord,(*cluster_neighbours)[nextbestclust_ci][nbvi]->coord,(*cluster_neighbours)[ci][vi]->coord.size(),(*cluster_neighbours)[nextbestclust_ci][nbvi]->coord.size());
                }
                sumB+=dist;
                }

            long double Avgb=sumB/Bcluster_size;

            long double si=(Avgb-Avga)/max(Avga,Avgb);
            sumSi+=si;
            }
        long double AvgSi=sumSi/cluster_size;
        S->push_back(AvgSi);
        }
    return S;
    }