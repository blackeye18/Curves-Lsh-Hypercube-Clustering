/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
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
#include <chrono>
#include <queue>

using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "knn_ranges_brutes.hpp"

dist_vec::~dist_vec()//adeios destructor, den xreiazetai
	{

	};


node* hashtable::search_nd(long hvalue)//epistrefei thn swsth thesh tou hastbable vash tou hashvalue
    {
    int id=hvalue%bucket_count;
    if(buckets!=NULL)
        {
        if(buckets[id]!=NULL)
            return buckets[id];
        }
    return NULL;
    }

long double vect_dist(vector<double> vecA,vector<double> vecB,int d)//ypologizei thn efklideia apostash metaksi 2 vector ston d diastato xwro
{
    long double sum=0;
    for (int i = 0; i < d; ++i)
        {
        long double temp=abs(vecA[i]-vecB[i]);
        sum+=temp*temp;
        }
    return sqrt(sum);
}

vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos){
    long double dist;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    for(int i=0;i<no_of_vectors;i++){
        if(metric=="euclidean_distance"){
            dist=vect_dist(qvector[pos].coord,nvectors[i].coord,no_of_coordinates);//ypologismos apostashs tou query apo ola ta input vectors
            Q.push(dist_vec(dist,&(nvectors[i])));//ta vazoume se priority queue kai kratame ta N
        }else{
         cout<<"No function for metric:"<<metric<<endl;
         return NULL;
        }
        //cout<<dist<<endl;
    }

     vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    
    for (int i = 0; i < N; ++i)
        {
        if(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);//kratame ta N kodinotera apo to priority queue
            dsvec->push_back(tempdv);
            Q.pop();
            }
        else
            break;
        }
        
    return dsvec;      
    
}

vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N,double* time_per_query_brute){
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;
    for(int i=0;i<queries_no_of_vectors;i++){//gia kathe query kaloume thn brutecalculate kai kratame ta N kodinotera vector
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

        dsvec2->push_back(brute_calculate(qvectors,nvectors,no_of_vectors,no_of_coordinates,N,i));
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        time_per_query_brute[i]+=time1;//kratame ton xrono ekteleshs kathe query
    }
    return dsvec2;
}

vector<dist_vec>* Lhashtables::NN_search(vec* nvector,int N)//synarthsh gia to knn
{
    int counter=0;
    int element_count=0;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    //int g_notablesize=0;
    long int g_notablesize[this->L];
    int h_return;
    int h[k];

    for (int li = 0; li < L; li++)
        {
        for(int ki=0;ki<this->k;ki++)
            {

            h_return=h_function(nvector->coord,this->v[li][ki],this->t[li][ki]);//ypologizoume ta h tou query gia kathe hastable
            h[ki]=h_return;
            //cout<<"H Function Return:"<<h[ki]<<endl;
            }
        //cout<<"calling g function"<<endl;
        g_notablesize[li]=g_function(h,this->r[li],this->k);//ypologizoume to g tou query gia kathe hashtable

        node* currnode=Lhtables[li].search_nd(g_notablesize[li]);//briskoume thn antistoixh thesh sto ht

        while(currnode!=NULL)//gia ola ta stoixeia sthn sygkekrimenh thesh sto hast table
            {
            if(currnode->vect!=NULL)
            	{	
            	//if(currnode->hashvalue==NULL){break;}
            	if(currnode->hashvalue == g_notablesize[li])//query trick
                	{
                    counter++;
                    if(metric=="euclidean_distance"){
                    	long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);//ypologizoume thn eyklideia apostash
                    	Q.push(dist_vec(dist,currnode->vect));//to eisxwroume sto priority queue
                    }else{
                        cout<<"No function for metric:"<<metric<<endl;
                        return NULL;
                    }

                	}
                }
              //else
              	//cout<<"NULL vect found"<<endl;
            currnode=currnode->next;    
            }

        }
    if(counter<N){//ama den exoume vrei arketa stoixei me query trick psaxnoume kai ta ypoloipa sto analogo bucket
        for(int li=0;li<L;li++){
            node* currnode=Lhtables[li].search_nd(g_notablesize[li]);

            while(currnode!=NULL)
                {
                if(currnode->vect==NULL){break;}
                //if(currnode->hashvalue==NULL){break;}
                if(currnode->hashvalue != g_notablesize[li])//ama den ta exoume ypologisei hdh
                    {
                        counter++;
                    if(metric=="euclidean_distance"){
                        long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);//ypologizoume thn eyklideia apostash
                        Q.push(dist_vec(dist,currnode->vect));
                    }else{
                        cout<<"No function for metric:"<<metric<<endl;
                        return NULL;
                    }
                    }
                currnode=currnode->next;    
                }

        }
    }
    
    vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    
    for (int i = 0; i < N; ++i)//kratame tous n neighbours 
        {
        if(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        else
            break;
        }
        
    return dsvec;      
}


vector<vector<dist_vec>*>* Lhashtables:: find_k_nearest(vec* qvectors,int N,int queries_no_of_vectors,double* time_per_query_lsh){
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2=new vector<vector<dist_vec>*>;

    for(int i=0;i<queries_no_of_vectors;i++){
        auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

        vector<dist_vec>* temp=NN_search(&(qvectors[i]),N);// gia kathe query kaloume thn nnsearch
        if(temp==NULL){
            delete dsvec2;
            return NULL;
        }
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);
        double time1=((double)duration1.count()/1000000);
        time_per_query_lsh[i]+=time1;//kratame ton xrono gia kathe query
        dsvec2->push_back(temp);
    }
    return dsvec2;
}

vector<dist_vec>* Lhashtables::LRadius_search(vec* nvector,double R)// peripou idia me to NN search alla elegxoume an h apostash einai <= me to radius
    {
    int counter=0;
    int element_count=0;
    priority_queue<dist_vec, vector<dist_vec>, pqcompare> Q;
    //int g_notablesize=0;
    long int g_notablesize[this->L];
    int h_return;
    int h[k];

    for (int li = 0; li < L; li++)
        {
        for(int ki=0;ki<this->k;ki++)
            {

            h_return=h_function(nvector->coord,this->v[li][ki],this->t[li][ki]);
            h[ki]=h_return;
            //cout<<"H Function Return:"<<h[ki]<<endl;
            }
        //cout<<"calling g function"<<endl;
        g_notablesize[li]=g_function(h,this->r[li],this->k);

        node* currnode=Lhtables[li].search_nd(g_notablesize[li]);
        int nn=0;
        while(currnode!=NULL)
            {
            nn++;
            if(currnode->vect!=NULL)
          	{
            	counter++;
                if(metric=="euclidean_distance"){
            	   long double dist=vect_dist(nvector->coord,currnode->vect->coord,d);
            	   if(dist<R)
                        Q.push(dist_vec(dist,currnode->vect));
                }else{
                    cout<<"No function for metric:"<<metric<<endl;
                    return NULL;
                }
            }
            else
              	cout<<"NULL vect found"<<endl;
            currnode=currnode->next;    
            }
        //cout<<"BUcket of "<<li<<" hashtable "<<"num of nodes "<<nn<<endl;;
        }
    
    vector<dist_vec>* dsvec=new vector<dist_vec>;//=new vector<dist_vec>;
    
    
        while(!Q.empty())
            {
            dist_vec tempdv(Q.top().dist,Q.top().vect);
            dsvec->push_back(tempdv);
            Q.pop();
            }
        
        
    return dsvec;      
}

vector<vector<dist_vec>*>* Lhashtables:: find_in_LRadius(vec* qvectors,double R,int queries_no_of_vectors){
    //vector<dist_vec> dsvec;
    vector<vector<dist_vec>*>* dsvec2;
    dsvec2= new vector<vector<dist_vec>*>;
    for(int i=0;i<queries_no_of_vectors;i++){
        dsvec2->push_back(LRadius_search(&(qvectors[i]),R));//kalei gia kathe query thn lradius search
    }
    
    return dsvec2;
}

