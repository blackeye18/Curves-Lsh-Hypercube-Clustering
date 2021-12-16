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
#include "cube_basic_functions.hpp"
#include "lsh_basic_functions.hpp"
#include "knn_ranges_brutes.hpp"
#include "config.hpp"
#include "types.hpp"
#include "point.hpp"
#include "interval.hpp"
#include "curve.hpp"
#include "frechet.hpp"
#include "simplification.hpp"
#include "discrete_cont.hpp"
extern string metric;
extern double delta;

double normal_dist_generator(void)
    {
    std::random_device rd{};
    std::mt19937 gen{rd()};//pseudo random genarator https://www.cplusplus.com/reference/random/mt19937/
    //https://www.cplusplus.com/reference/random/normal_distribution/
    std::normal_distribution<float> d{0.0,1.0};//kanoniki katanomi N~(0,1) sel 20 nnCluster.pdf
    return round(d(gen));
    }

hashtable::hashtable()//contstructos twn hashtables 
    {
    initialized=0;
    }
void hashtable::hashtable_init(int bnum)//synarthsh pou arxikopoiei ta hashtables
    {
    buckets=(node**) malloc(bnum*sizeof(node*));
    for(int i=0;i<bnum;i++)
        buckets[i]=NULL;
    bucket_count=bnum;
    total_nodes=0;
    initialized=1;
    }

int hashtable:: hashtable_insert(vec* nvec, long hvalue)//synarthsh pou eisxwrei ena vector sto hashtable vash tou apotelesmatos ths synarthshs g
    {
    if(not initialized){return 1;}
    total_nodes++;
    int id=hvalue % bucket_count;
    if (buckets[id]==NULL)//mia aplh eisxwrhsh se lista, eisxwrei sthn arxh
        {
        buckets[id]=new node;
        buckets[id]->hashvalue=hvalue;//apothikevoyme to hashvalue (id) gia na kanoume to trick pou anaferetai stis diafanies me to id sthn knn
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


void hashtable::hashtable_print()//voithitikh synarthsh pou xrhsimopoihsame gia debug, wste na gnwrizoume ti mpainei sta hashtables
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

hashtable::~hashtable()//destructos, apeleftherwsh mnhmhs
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

Lhashtables:: ~Lhashtables(void){//destructor gia thn domh Lhastables, apeleftherwsh mnhmhs
    delete[] Lhtables;
}

Lhashtables::Lhashtables(int lh,int di,int ki)//synarthsh pou arxikopoiei ta dedomena vash twn orismatwn tou programmatos
 {
    Lhtables= new hashtable[lh];
    L=lh;
    d=di;
    k=ki;
}

void Lhashtables::Hashfun_init(void)//synarthsh pou ypologizei tyxaia tis times pou xrisimopooioun oi synarthseis h kai epeita g
    {
    v.resize(L,vector<vector<double> >(k,vector<double>(d)));////resize analoga to plithws twn hashtables kai twn posws diastasewn einai to input file

    t.resize(L,vector<double>(k));//resize analoga to plithws twn hashtables kai tou k(poses synarthseis H tha kalestoun)

    r.resize(L,vector<int>(k));

    td1.resize(L,vector<double>(d));

    td2.resize(L,vector<double>(d));


    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrR(-RLIMIT,RLIMIT);//vazoume ena orio opws mas proteine o k Emiris sthn dialeksh
    std::uniform_real_distribution<double>  distrT(0.0,W);
    std::uniform_real_distribution<double>  distrTd1(0.0,delta);
    std::uniform_real_distribution<double>  distrTd2(0.0,delta);


    for(int Li=0;Li<L;Li++){
        for(int di=0;di<d;di++){
            td1[Li][di]=distrTd1(e);
            td2[Li][di]=distrTd2(e);
        }
    }


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


int h_function(vector<double> p,vector<double> v,double t){//ypologismos ths synarthshs h opws anaferetai stis diafanies
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);//vriskoume to ginomeno twn 2 vector(p kai v ) ta opoia einai idiwn diastasewn
    in_prod+=t;//prosthetoume thn tyxaia timh t 
    double sum=in_prod/W;//diairoume me w meta3i 0 kai 6, define sto header classes
    int h=floor(sum);//pairnoume to katw orio tou apotelesmatos
    return h;

}
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)//ypologismos tou eyklidiou apolytou
{
  assert(b != 0);//sigourevoume oti den kanoume diairesh me 0
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);//an ypoloipo megalytero 0 epistrefoume to ypoloipo diaforetika epistrefoume to apolyto prosthetontas thn apolyth timh tou b
}

long int g_function(int h[],vector<int> r,int k){//synarthsh g opws afth anaferetai stis diafanies, xwris thn diairesh me to tablesize gia na to krathsoume ws id
    long int galmost=0;
    long int count_holder=0;
    for(int i=0;i<k;i++){
        count_holder=0;//exoume spasei to mod opws proteinete stis diafanies gia apofygh overflow
        count_holder+=euclidean_remainder(r[i],PNUM);
        count_holder=count_holder*euclidean_remainder(h[i],PNUM);
        count_holder=euclidean_remainder(count_holder,PNUM);
        galmost+=count_holder;
    }
    galmost=euclidean_remainder(galmost,PNUM);
    return galmost;
}

int Lhashtables:: lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors){//synarthsh pou eisxwrei ena vector se ena hashtable
    int h_return;
    int h[this->k];
    int tablesize=no_of_vectors/BUCKET_RATIO;//apo diafaneies
    this->Lhtables[no_of_ht].hashtable_init(tablesize);
    long int g_notablesize;
    vec * snapped_paded_nvectors;
    if(metric=="LSH_Frechet_Discrete")
        snapped_paded_nvectors=snapping(nvectors,nvectors[0].coord.size(),no_of_vectors,delta,this->td1[no_of_ht],this->td2[no_of_ht]);
    else if(metric=="LSH_Frechet_Continuous")
        {
        snapped_paded_nvectors=new vec[no_of_vectors];
        //snapped_paded_nvectors=nvectors;
        for(int k=0;k<no_of_vectors;k++){
            snapped_paded_nvectors[k]=nvectors[k];
        }

        key_cont(snapped_paded_nvectors,no_of_vectors,nvectors[0].coord.size(),delta,this->td1[no_of_ht]);//kalw tis synarthseis gia na vrw to key

        }
    for(int i=0;i<no_of_vectors;i++){
        for(int ki=0;ki<this->k;ki++){//ypologizoume thn timh twn h, afto ginetai k fors
            if(metric=="LSH_Frechet_Discrete"||metric=="LSH_Frechet_Continuous")
                h_return=h_function(snapped_paded_nvectors[i].coord,this->v[no_of_ht][ki],this->t[no_of_ht][ki]);
            else if(metric=="euclidean_distance")
                 h_return=h_function(nvectors[i].coord,this->v[no_of_ht][ki],this->t[no_of_ht][ki]);
            h[ki]=h_return;
            //cout<<"H Function Return:"<<h[ki]<<endl;
        }
        //cout<<"calling g function"<<endl;
        g_notablesize=g_function(h,this->r[no_of_ht],this->k);//ypologizoume thn timh tou g xwris to mod tablesize
        //cout<<"Inserting to HT"<<endl;
        this->Lhtables[no_of_ht].hashtable_insert(&(nvectors[i]),g_notablesize);//kanoume insert to vector sthn antistoixh thesh
    }
    if(metric=="LSH_Frechet_Discrete"||metric=="LSH_Frechet_Continuous")
        delete [] snapped_paded_nvectors;

    return 0;
}

int Lhashtables:: lsh_start(int no_of_vectors,vec *nvectors){//synarthsh pou eisxwrei ola ta vectors se L hashtables 
    this->Hashfun_init();
    int ret=0;
    for(int i=0;i<L;i++){
        ret=this->lsh_continue(i,no_of_vectors,nvectors);
        if(ret!=0){
            cout<<"Something went wrong with lsh_continue"<<endl;
            return-1;
        }

    }
    /*
    cout<<endl<<endl<<endl<<"PRINTING HASHTABLES:"<<endl;
    for(int i=0;i<L;i++){
        cout<<"hashtable "<<i<<endl;
        this->Lhtables[i].hashtable_print();
        cout<<endl<<endl;
    }
*/
    return 0;

}

