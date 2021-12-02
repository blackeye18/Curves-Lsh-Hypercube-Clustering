/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
#ifndef __classes_and_defines__
#define __classes_and_defines__

#define RLIMIT 1000
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6
#define BUCKET_RATIO 16
#define M_big_num 33554432 //2^25 
//const string metric = "euclidean_distance";



class vec//klash gia na filame to ka8e vector
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;//sintetagmenes simeiou
};


class dist_vec//tuple me apostasi kai to vec
    {public:
    long double dist;
    vec* vect;
    dist_vec(long double dist, vec* vect)
       : dist(dist), vect(vect)
        {};
    ~dist_vec(void);
    };




struct pqcompare//sinartisi sigkrisis gia priority queu
    {
    bool operator()(dist_vec const& NN1, dist_vec const& NN2)
        {
        return NN1.dist > NN2.dist;
        }
    };

class node//domh node gia ta hashtables
    {
    public:
        long long hashvalue;//timi hash prin to mod
        int id;//timh hash meta to mod
        vec* vect=NULL;//vec autou toou node
        node* next=NULL;//epomeno vec
    };


class hashtable//klash gia hashtable
    {
    protected:
        node** buckets=NULL;//array me ta buckets
        int bucket_count;//ari8mos buckets
        int total_nodes;//sinolikos ari8mos nodes ara kai vec
        int initialized;//1 an arxikopii8ike 0 an den
    public:
        hashtable(void); // constructor
        void hashtable_init(int);//initializer
        int hashtable_insert(vec* ,long );//eisagwgh 
        void hashtable_print();//xrisimopii8ike mono gia debugging
        node* search_nd(long);//epistrefei to arxiko node tu bucket me to antistixo hvalue
        ~hashtable(void);//destructor
    };


    class Lhashtables
    {
    protected:
        hashtable* Lhtables=NULL;//array me ta hashtables
        int L;//number of hashtables;
        int d;//dimensions
        int k;//number of hash functions
        /*
        h(p)=floor((p*v+t)/w)
        typos apo diafanies

        */
        vector<vector<vector<double>>> v;
        vector<vector<double>> t;
        vector<vector<int>> r;
    public:
        Lhashtables(int,int,int);//synarthsh arxikopoihshs 
        void Hashfun_init(void);//ypologismos uniform random twn timwn
        int lsh_start(int,vec*);//synarthsh pou eisxwrei ola ta vectors se L hastables
        int lsh_continue(int,int,vec*);//synarthsh poy eisxwrei ena vector se ena hashtable
        vector<dist_vec>* NN_search(vec*,int);//Synarthsh poy vriskei tous N nearest neighbours gia ena query
        vector<vector<dist_vec>*>* find_k_nearest(vec* ,int ,int ,double *);//vriskei tous kodinoterous geitones gia kathe query
        vector<dist_vec>* LRadius_search(vec* ,double );//Synarthsh pou vriskei tou neigbours se ena query gia L aktina
        vector<vector<dist_vec>*>* find_in_LRadius(vec* ,double ,int );//vriskei tous neighbours se ola ta query gia aktina L
        ~Lhashtables(void);//destructor

    };

class hypercube
    {
    private:
        vector<vector<double>> v;//pinakas apo v gia ton pio katw typo
        vector<double> t;//pinakas apo t gia ton pio katw typo

        //h(p)=floor(s(p*v+t)/w)
        //typos apo  sel.19 tu nnCluster.pdf

        vector<vector<int>> labels;//disdiastatos pinakas(k epi labels_count) apo 0 kai 1 xrisimopoihte gia thn f(h(p)) sel 26 tu nnCluster.pdf

        vector<vector<vec*>> cube_vec;//disdiastatos pinakas opou apo8ikeuonte diktew gia tous vectors analoga me tin binary timi tus

        //int L;//number of hashtables;
        int d;//dimensions
        int k;//diastasi pu 8a provalonte ta simeia
        int M;//maximum cout of points checked
        int probes;//maximum count of cube_map checked
        int no_of_vectors;//ari8mos vectors
        int labels_count;//ari8mos labes
        unsigned int powk;//2^k theseis pinaka cube_vec 

    public:
        hypercube(int M,int probes,int d ,int k,int no_of_vectors);
        //~hypercube();
        void Hashfun_init(void);//initializes tables used for hashing
        unsigned int hash_calc(vec* vect);//returns the bin cube vector point
        int vec_insert(vec* vect);//inserts a vector to the cube_vec
        void cube_start(int ,vec*);//kanei insert ola ta vec sto hypercube kai kalei tin Hashfun_init
        vector<vector<dist_vec>*>* all_NN_search(vec* ,int ,int,double* );//kalei tin NN_search kai ola ta query vectors
        vector<dist_vec>* NN_search(vec*,int);//KNN search gia ena query vector
        vector<dist_vec>* RANGE_search(vec* ,double );//RANGE search  gia ena query vector
        vector<vector<dist_vec>*>* all_RANGE_search(vec* ,double ,int );//kalei tin RANGE_search kai ola ta query vectors

    
    };



#endif