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



//gia oti den yparxei sxolio yparxoun sta arxeia tou lsh kai tou hypercube
using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "cube_basic_functions.hpp"
#include "cluster_main_functions_silhouette.hpp"
//#include "knn_ranges_brutes.hpp"

long double vect_dist(vector<double> vecA,vector<double> vecB,int d)
{
    long double sum=0;
    for (int i = 0; i < d; ++i)
        {
        long double temp=abs(vecA[i]-vecB[i]);
        sum+=temp*temp;
        }
    return sqrt(sum);
}
double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d{0.0,1.0};
    map<int, int> hist{};
    return round(d(gen));
    }

hashtable::hashtable()
    {
    initialized=0;
    }
void hashtable::hashtable_init(int bnum)
    {
    buckets=(node**) malloc(bnum*sizeof(node*));
    for(int i=0;i<bnum;i++)
        buckets[i]=NULL;
    bucket_count=bnum;
    total_nodes=0;
    initialized=1;
    }

int hashtable:: hashtable_insert(vec* nvec, long hvalue)
    {
    if(not initialized){return 1;}
    total_nodes++;
    int id=hvalue % bucket_count;
    if (buckets[id]==NULL)
        {
        buckets[id]=new node;
        buckets[id]->hashvalue=hvalue;
        buckets[id]->id=id;
        buckets[id]->vect=nvec;
        buckets[id]->next=NULL;
        }
    else
        {
        node* tempn= new node;
        tempn->hashvalue=hvalue;
        tempn->id=id;
        tempn->vect=nvec;
        tempn->next=buckets[id];
        buckets[id]=tempn;
        }
    return 0;
    }


void hashtable::hashtable_print()
    {
    cout<<"total_nodes of htable "<<total_nodes;
    int j=0;
    for (int i = 0; i < bucket_count; ++i)
        {
        cout<<endl<<"bucket "<<i;

        node* currnodeptr=buckets[i];

        while(currnodeptr!=NULL)
            {
            cout<<" node "<<j<<" "<<currnodeptr->vect->name<<endl;
            cout<<"cord "<<currnodeptr->vect->coord[0];
            currnodeptr=currnodeptr->next;
            j++;
            }
        }
                 
    }
node* hashtable::search_nd(long hvalue)
    {
    int id=hvalue%bucket_count;
    if(buckets!=NULL)
        {
        if(buckets[id]!=NULL)
            return buckets[id];
        }
    return NULL;
    }
hashtable::~hashtable()
    {
    if(buckets!=NULL)
        {
        for (int i = 0; i < bucket_count; ++i)
            {
            node* currnd=buckets[i];
            while(currnd!=NULL)
                {
                node* temp=currnd;
                currnd=temp->next;
                delete temp;
                }

            }
        free(buckets);

        }
    buckets=NULL;
    }

Lhashtables:: ~Lhashtables(void){
    delete[] Lhtables;
}

Lhashtables::Lhashtables(int lh,int di,int ki)
 {  
    Lhtables= new hashtable[lh];
    L=lh;
    d=di;
    k=ki;
}

void Lhashtables::Hashfun_init(void)        
    {
    v.resize(L,vector<vector<double> >(k,vector<double>(d)));

    t.resize(L,vector<double>(k));

    r.resize(L,vector<int>(k));

    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrR(-RLIMIT,RLIMIT);
    std::uniform_real_distribution<double>  distrT(0.0,W);


    for (int Li = 0; Li < L; ++Li)
        {
        for (int ki = 0; ki < k; ++ki)
            {
            t[Li][ki]=distrT(e);    //cout<<"t "<<t[Li][ki]<<endl;
            r[Li][ki]=distrR(e);    //cout<<"r "<<r[Li][ki]<<endl;

            for (int di = 0; di < d; ++di)
                {
                v[Li][ki][di]=normal_dist_generator();
                }
            //cout<<endl;
            }
        }

}


int h_function(vector<double> p,vector<double> v,double t){
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);
    in_prod+=t;
    double sum=in_prod/W;
    int h=floor(sum);
    return h;

}
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)
{
  assert(b != 0);
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);
}

long int g_function(int h[],vector<int> r,int k){
    long int galmost=0;
    long int count_holder=0;
    for(int i=0;i<k;i++){
        count_holder=0;
        count_holder+=euclidean_remainder(r[i],PNUM);
        count_holder=count_holder*euclidean_remainder(h[i],PNUM);
        count_holder=euclidean_remainder(count_holder,PNUM);
        galmost+=count_holder;
    }
    galmost=euclidean_remainder(galmost,PNUM);
    return galmost;
}

int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors){
    int h_return;
    int h[this->k];
    int tablesize=no_of_vectors/BUCKET_RATIO;//apo diafaneies
    this->Lhtables[no_of_ht].hashtable_init(tablesize);
    long int g_notablesize;
    for(int i=0;i<no_of_vectors;i++){

            for(int ki=0;ki<this->k;ki++){

                h_return=h_function(nvectors[i].coord,this->v[no_of_ht][ki],this->t[no_of_ht][ki]);
                h[ki]=h_return;
                //cout<<"H Function Return:"<<h[ki]<<endl;
            }
            //cout<<"calling g function"<<endl;
            g_notablesize=g_function(h,this->r[no_of_ht],this->k);
            //cout<<"Inserting to HT"<<endl;
            this->Lhtables[no_of_ht].hashtable_insert(&(nvectors[i]),g_notablesize);
        
    }
    

    return 0;
}

int Lhashtables:: lsh_start(int no_of_vectors,vec *nvectors){
    this->Hashfun_init();
    int ret=0;
    for(int i=0;i<L;i++){
        ret=this->lsh_continue(i,no_of_vectors,nvectors);
        if(ret!=0){
            cout<<"Something went wrong with lsh_continue"<<endl;
            return-1;
        }

    }
    return 0;
}

int Lhashtables::Cluster_LRadius(vec* cvector,long int g_notablesize[],double radius,int clust_num,vector<vector<dist_vec>> *curr_clust_vec,int iteration)
    {
    int counter=0;

    for (int li = 0; li < L; li++)//gia  ola ta hashtables
        {
        node* currnode=Lhtables[li].search_nd(g_notablesize[li]);//pernume to antistoixo node
        int nn=0;
        while(currnode!=NULL)
            {
            nn++;
            if(currnode->vect!=NULL)
                {
                
                int clustered_flag=currnode->vect->clustered_flag;
                if(clustered_flag==-1||clustered_flag==iteration)//ean den mpike 3ana se cluster i exi mpi se allo cluster se auto to iteration
                    {
                    if(metric=="euclidean_distance")
                        {

                        long double dist=vect_dist(cvector->coord,currnode->vect->coord,d);
                        if(dist<radius)
                            {
                            int push_flag=1;
                            if(clustered_flag==iteration)//ean ine se allo cluster
                                {
                                push_flag=-1;
                                int found=0;
                                for (int ci = 0; ci < clust_num; ++ci)
                                    {
                                    for (int vi = 0; vi < (*curr_clust_vec)[ci].size(); ++vi)
                                        {
                                        if((*curr_clust_vec)[ci][vi].vect==currnode->vect)//vriskume pu ine
                                            {
                                            found=1;
                                            if((*curr_clust_vec)[ci][vi].dist<=dist)//elegxume ean exi  mikroteri apostasi sto allo cluster
                                                {
                                                push_flag=0;//tote den tha to valume sto twrino cluster
                                                }
                                            else
                                                {
                                                push_flag=1;//tha to valume sto twrino cluster
                                                (*curr_clust_vec)[ci].erase((*curr_clust_vec)[ci].begin() + vi);//to svinume apto allo cluster
                                                }

                                            break;
                                            }
                                        }
                                    if(found){break;}
                                    }
                                }

                            if(push_flag==1)//to vazume sto twrino cluster
                                {
                                currnode->vect->clustered_flag=iteration;
                                (*curr_clust_vec)[clust_num].push_back(dist_vec(dist,currnode->vect));
                                }

                            }

                        }
                    else
                        {
                        cout<<"No function for metric:"<<metric<<endl;
                        return 1;
                        }
                    }
                }
            else
                cout<<"NULL vect found"<<endl;

            currnode=currnode->next;    
            }
        //cout<<"BUcket of "<<li<<" hashtable "<<"num of nodes "<<nn<<endl;;
        }
    return 0;

    }
vector<vector<vec*>>* Lhashtables::ANN_lsh(vec* nvect,vector<vec>* clustersvec,int no_of_vectors)
    {
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
    long int g_notablesize[cluster_num][this->L];
    int h_return;
    int h[cluster_num][k];
    for (int ci = 0; ci < cluster_num; ++ci)//ipologizume ola ta hashvalues
        {
        for (int li = 0; li < L; li++)
            {
            for(int ki=0;ki<this->k;ki++)
                {

                h_return=h_function(clustersvec->at(ci).coord,this->v[li][ki],this->t[li][ki]);
                h[ci][ki]=h_return;
              
                }
       
            g_notablesize[ci][li]=g_function(h[ci],this->r[li],this->k);
            }
        }

    int iteration=0;
    int total_found=0;

    vector<vector<vec*>>* cluster_neighbours=new vector<vector<vec*>>;//pinakas me ola ta vec ana cluster
    cluster_neighbours->resize(clustersvec->size(),vector<vec*>(0));

    vector<vector<dist_vec>> *curr_clust_vec=new vector<vector<dist_vec>>;//pinakas gia ka8e iteration pu krataei kai distance
    curr_clust_vec->resize(cluster_num,vector<dist_vec>());
    int vectors_found=0;

    do
        {


        for (int ci = 0; ci < cluster_num; ++ci)//kaloume tin Cluster_LRadius gia ka8e cluster
            {


            int abc=this->Cluster_LRadius(&(clustersvec->at(ci)),g_notablesize[ci],radii,ci,curr_clust_vec,iteration);
            }

        vectors_found=0;
        for (int ci = 0; ci < cluster_num; ++ci)
            {

            for (int vi = 0; vi < (*curr_clust_vec)[ci].size(); ++vi)
                {
                vectors_found++;
                (*cluster_neighbours)[ci].push_back((*curr_clust_vec)[ci][vi].vect);//vazume ta vec ston pinaka me ola ta vec ana cluster
                }

            (*curr_clust_vec)[ci].clear();//ka8arizoume ton proswrino pinaka 
            }

        total_found+=vectors_found;
        iteration++;
        radii*=2;//diplasiazume to radii
        }
    while(vectors_found>=cluster_num/2);

    delete curr_clust_vec;
        int ff=0;
       // cout<<"entering brute with total_found "<<total_found<<endl;
        //brute calculate
        for (int i = 0; i < no_of_vectors; ++i)
            {
            if(nvect[i].clustered_flag==-1)//ean den exi mpei se kapio cluster
                {
                total_found++;
                long double mdist;
                int mci;
                for (int ci = 0; ci < cluster_num; ++ci)//to vazume ston cluster pu exi tin mikroteri apostasi
                    {

                    if(ci==0)
                        {
                        mdist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,d);
                        mci=0;
                        }
                    else
                        {
                        long double dist=vect_dist(clustersvec->at(ci).coord,nvect[i].coord,d);
                        if (dist<mdist)
                            {
                            mdist=dist;
                            mci=ci;
                            }
                        }
                    }
                (*cluster_neighbours)[mci].push_back(&nvect[i]);
                }
            else//ean exei mpei vazume to flag=-1 gia tin epomeni epanalipsi
                {
                ff++;
                nvect[i].clustered_flag=-1;
                }
            }
     //cout<<"how much lsh really found "<<ff<<endl;
    //cout<<"total_found "<<total_found<<endl;
    return cluster_neighbours;
    }

