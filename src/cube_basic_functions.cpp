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
//#include "knn_ranges_brutes.hpp"

extern string metric;
unsigned int hammingDistance(unsigned int n1, unsigned int n2)
	{
    unsigned int x = n1 ^ n2;
    unsigned int setBits = 0;
 
    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }
 
    	return setBits;
	}
    /*
double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0.0,1.0};
    map<int, int> hist{};
    return round(d(gen));
    }
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)
{
  assert(b != 0);
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);
}
*/
/*int h_function(vector<double> p,vector<double> v,double t){
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);
    in_prod+=t;
    double sum=in_prod/W;
    int h=floor(sum);
    return h;

}
*/
hypercube::hypercube(int M,int probes,int d ,int k,int no_of_vectors)
	:M(M),probes(probes),d(d),k(k),no_of_vectors(no_of_vectors)
	{
	labels_count=no_of_vectors/BUCKET_RATIO;
	}

void hypercube::Hashfun_init_cube(void)        
    {
    v.resize(k,vector<double> (d));

    labels.resize(k,vector<int>(labels_count));


    cube_vec.resize(powk,vector<vec*>(0));

    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrl(0,1);
    std::uniform_real_distribution<double>  distrT(0.0,W);

    for (int ki = 0; ki < k; ++ki)
        {
        t.push_back(distrT(e));

        for (int di = 0; di < d; ++di)
            {
            v[ki][di]=normal_dist_generator();
            }

        for (int la = 0; la < labels_count; ++la)
            {
            labels[ki][la]=distrl(e);
            }
        }

    }
unsigned int hypercube::hash_calc(vec* vect)
    {
    unsigned int bits=0;
    for (int ki = 0; ki < k; ++ki)
        {
        int h=h_function(vect->coord,v[ki],t[ki]);
        //cout<<"labels_count: "<<labels_count<<endl;
        int hmod=euclidean_remainder(h,labels_count);
        bits=bits<<1;
        //cout<<labels[ki][hmod];
        bits+=labels[ki][hmod];
        }
      //cout<<endl;
    return bits;
    }
int hypercube::vec_insert(vec* vect)
    {
    unsigned int ki=this->hash_calc(vect);
    //cout<<"inserting "<<ki<<endl;
    //cube_map.insert(pair <vector<bool>,vec*> (this->hash_calc(vect),vect));
    cube_vec[ki].push_back(vect);
    return 0;
    }

void hypercube::cube_start(int no_of_vectors,vec*nvectors){
	powk=pow(2,k);
	this->Hashfun_init_cube();
	int ret=0;
	for(int i=0;i<no_of_vectors;i++){
    	ret=vec_insert(&(nvectors[i]));
    	if(ret!=0){
            cout<<"Something went wrong with vec_insert"<<endl;
            return;
        }
        
	}
}

int hypercube::Cluster_LRadius(vec* cvector,unsigned int qbits,double radius,int clust_num,vector<vector<dist_vec>> *curr_clust_vec,int iteration){

    int M_count=0;
    int probes_count=0;

    for(int i=0;i<cube_vec[qbits].size();i++){//gia ta simeia me idio bin hvalue hamming distance =0
        probes_count=1;
        M_count++;
        int clustered_flag=cube_vec[qbits][i]->clustered_flag;
        if(clustered_flag==-1||clustered_flag==iteration)
        {
            if(metric=="euclidean_distance"){
                long double dist= vect_dist(cvector->coord,cube_vec[qbits][i]->coord,d);
                if(dist<radius){
                    int push_flag=1;
                    if(clustered_flag==iteration){
                        push_flag=-1;
                        int found=0;
                        for(int ci=0;ci<clust_num;ci++){
                            for(int vi=0;vi<(*curr_clust_vec)[ci].size();vi++){
                                if((*curr_clust_vec)[ci][vi].vect==cube_vec[qbits][i]){//elegxi an ine to idio simeio
                                    found=1;
                                    if((*curr_clust_vec)[ci][vi].dist<=dist){//elegxi an i apostasi sto allo cluster ine mikroteri apo to twrino
                                        push_flag=0;//den tha to vali sto twrino
                                    }else{//ean oxi
                                        push_flag=1;//tha to valei sto twrino cluster
                                        (*curr_clust_vec)[ci].erase((*curr_clust_vec)[ci].begin() + vi);//to diagrafei apto allo
                                    }
                                    break;
                                }
                            }
                            if(found)
                                break;
                        }
                    }
                    if(push_flag==1){
                        cube_vec[qbits][i]->clustered_flag=iteration;
                        (*curr_clust_vec)[clust_num].push_back(dist_vec(dist,cube_vec[qbits][i]));
                    }
                }
            }
            else{
                cout<<"No function for metric:"<<metric<<endl;
                return 1;
            }
            if (probes_count>=probes||M_count>=M)
                break;
        }

    }


    int temp=0;
    if(probes_count<=probes && M_count<=M){

        for(int hd=1;hd<k;++hd){//gia oles  tis hamming distance 3ekinontas aptin 1

            for(int ki=0;ki<powk;ki++){
                temp++;
                if(!cube_vec[ki].empty() && hammingDistance(ki,qbits)==hd){
                    probes_count++;
                    for(int i=0;i<cube_vec[ki].size();i++){
                        M_count++;
                        int clustered_flag=cube_vec[ki][i]->clustered_flag;
                        if(clustered_flag==-1||clustered_flag==iteration){
                            if(metric=="euclidean_distance"){
                                long double dist=vect_dist(cvector->coord,cube_vec[ki][i]->coord,d);
                                if(dist<radius){
                                    int push_flag=1;
                                    if(clustered_flag==iteration){
                                        push_flag=-1;
                                        int found=0;
                                        for(int ci=0;ci<clust_num;ci++){
                                            for(int vi=0; vi<(*curr_clust_vec)[ci].size();vi++){
                                                if((*curr_clust_vec)[ci][vi].vect==cube_vec[ki][i]){
                                                    found=1;
                                                    if((*curr_clust_vec)[ci][vi].dist<=dist){
                                                        push_flag=0;
                                                    }else{
                                                        push_flag=1;
                                                        (*curr_clust_vec)[ci].erase((*curr_clust_vec)[ci].begin() + vi);
                                                    }
                                                    break;
                                                }
                                            }
                                            if(found)
                                                break;
                                        }

                                    }
                                    if(push_flag==1){
                                        cube_vec[ki][i]->clustered_flag=iteration;
                                        (*curr_clust_vec)[clust_num].push_back(dist_vec(dist,cube_vec[ki][i]));
                                        }
                                    }
                                }
                                else{
                                    cout<<"No function for metric:"<<metric<<endl;
                                    return 1;
                                }

                        }
                        if(M_count>=M)
                            break;
                    }
                }
                if(probes_count>=probes||M_count>=M)
                    break;
            }
            if(probes_count>=probes||M_count>=M)
                break;
        }
    }

    return 0;

}




vector<vector<vec*>>* hypercube::ANN_cube(vec* nvect,vector<vec>* clustersvec,int no_of_vectors){
    //ipologizoume to radi= mindist meta3i 2 cluster /2
    int cluster_num=clustersvec->size();
    long double mindist=999999999999999;

    for (int ca = 0; ca < cluster_num-1; ++ca)
        {
        for (int cb = ca+1 ; cb <cluster_num;  ++cb)
            {

            long double dist=vect_dist(clustersvec->at(ca).coord,clustersvec->at(cb).coord,d);
            if(dist<=mindist)
                mindist==dist;
            }
        }

    double radii=mindist/2;
    unsigned int qbits[cluster_num];

    for(int ci = 0; ci < cluster_num; ++ci){//ipologizume ola ta binary hash values
        qbits[ci]=this->hash_calc(&(clustersvec->at(ci)));
    }

    int iteration=0;
    int total_found=0;

    vector<vector<vec*>>* cluster_neighbours=new vector<vector<vec*>>;
    cluster_neighbours->resize(clustersvec->size(),vector<vec*>(0));

    vector<vector<dist_vec>> *curr_clust_vec=new vector<vector<dist_vec>>;
    curr_clust_vec->resize(cluster_num,vector<dist_vec>());
    int vectors_found=0;

    do
        {


        for (int ci = 0; ci < cluster_num; ++ci)
            {


            int abc=this->Cluster_LRadius(&(clustersvec->at(ci)),qbits[ci],radii,ci,curr_clust_vec,iteration);
            }

        vectors_found=0;
        for (int ci = 0; ci < cluster_num; ++ci)
            {
    
            for (int vi = 0; vi < (*curr_clust_vec)[ci].size(); ++vi)
                {
                vectors_found++;
                (*cluster_neighbours)[ci].push_back((*curr_clust_vec)[ci][vi].vect);
                }

            (*curr_clust_vec)[ci].clear();
            }

        total_found+=vectors_found;
        iteration++;
        radii*=2;
        }
    while(vectors_found>=cluster_num/2);

    delete curr_clust_vec;
    int ff=0;
    //cout<<"entering brute with total_found "<<total_found<<endl;
    //brute force
    for(int i=0;i<no_of_vectors;i++){
        if(nvect[i].clustered_flag==-1){//osa den exun mpei se kapoio cluster
            total_found++;
            long double mdist;
            int mci;
            for(int ci=0;ci<cluster_num;ci++){
                if(ci==0){
                    mdist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,d);
                    mci=0;
                }
                else{
                    long double dist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,d);
                    if(dist<mdist){
                        mdist=dist;
                        mci=ci;
                    }
                }
            }
            (*cluster_neighbours)[mci].push_back(&nvect[i]);

        }
        else{//vazume to flag =-1 gia to epomeno iteration
            ff++;
            nvect[i].clustered_flag=-1;
        }
    }
    // cout<<"ff "<<ff<<endl;
    //cout<<"total_found "<<total_found<<endl;
    return cluster_neighbours;
}

