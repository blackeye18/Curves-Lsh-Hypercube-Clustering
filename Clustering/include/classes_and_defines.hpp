#ifndef __classes_and_defines__
#define __classes_and_defines__

#define RLIMIT 1000
#define PNUM 4294967291
#define W 4//meta3i 0 kai 6
#define BUCKET_RATIO 8
#define PERCISION 0.001
#define MAXIT 50
#define M_big_num 33554432 //2^25 
const double delta=1.2;
//const string metric = "euclidean_distance";


//gia oti den yparxei sxolio yparxoun sta arxeia tou lsh kai tou hypercube

class vec
{
public:
    string name;//to id ths grammhs-dianismatos
    vector <double> coord;
    int clustered_flag;//gia to radi search sto hypercube kai lsh, an exei hdh mpei se kapoio cluster kai se poio iteration
};


class dist_vec
    {public:
    long double dist;
    vec* vect;
    dist_vec(long double dist, vec* vect)
       : dist(dist), vect(vect)
        {};
    //~dist_vec(void);
    };




struct pqcompare
    {
    bool operator()(dist_vec const& NN1, dist_vec const& NN2)
        {
        return NN1.dist > NN2.dist;
        }
    };

class node
    {
    public:
        long long hashvalue;
        int id;
        vec* vect=NULL;
        node* next=NULL;
    };


class hashtable
    {
    protected:
        node** buckets=NULL;//array me ta buckets
        int bucket_count;//ari8mos buckets
        int total_nodes;
        int initialized;
    public:
        hashtable(void); // plithos bucket
        void hashtable_init(int);
        int hashtable_insert(vec* ,long );//eisagwgh 
        void hashtable_print();
        node* search_nd(long);
        ~hashtable(void);
    };



    class cluster
    {
    private:
        int K_medians;//number_of_clusters
        int L;//number_of_vector_hash_tables
        int k_lsh;//number_of_vector_hash_functions
        int M;//max_number_M_hypercube
        int k_hypercube;//number_of_hypercube_dimensions
        int probes;//number_of_probes
        int no_of_vectors;
        int no_of_coordinates;


    public:
        cluster(int,int,int);
        vector<vec>* Kmeanplus(vec* nvect);
        vector<vector<vec*>>* lloyds(vec* nvect,vector<vec>* clustersvec);
        vector<vector<vec*>>* repeat(vec* nvect,vector<vec>* clustersvec,int method,void* ss);
        vector<long double>* silhouette(vector<vector<vec*>>* cluster_neighbours,vector<vec>* clustersvec,vec* nvect);
    };


    class Lhashtables
    {
    protected:
        hashtable* Lhtables=NULL;//array me ta hashtables
        int L;//number of hashtables;
        int d;//dimensions
        int k;
        vector<vector<vector<double>>> v;
        vector<vector<double>> t;
        vector<vector<int>> r;
    public:
        Lhashtables(int,int,int);
        void Hashfun_init(void);
        int lsh_start(int no_of_vectors,vec *nvectors);
        int lsh_continue(int no_of_ht,int no_of_vectors, vec* nvectors);
        int Cluster_LRadius(vec* cvector,long int g_notablesize[],double radius,int clust_num,vector<vector<dist_vec>> *curr_clust_vec,int iteration);//vazei ta vec me tin apostasi tus apto cluster vec stin katalili thesi ston curr_clust_vec an i apostasi ine < radius
        vector<vector<vec*>>* ANN_lsh(vec* nvect,vector<vec>* clustersvec,int no_of_vectors);//kalei tin Cluster_LRadius me diaforetika radius kai meta kanei brute calculate(lloyds) ola ta ipoloipa vec
        //vector<dist_vec>* NN_search(vec*,int);
        //vector<vector<dist_vec>*>* find_k_nearest(vec* ,int ,int );
       //vector<dist_vec>* LRadius_search(vec* ,double );
        //vector<vector<dist_vec>*>* find_in_LRadius(vec* ,double ,int );
        ~Lhashtables(void);

    };



    class hypercube
    {
    private:
        vector<vector<double>> v;
        vector<double> t;
        vector<vector<int>> labels;

        vector<vector<vec*>> cube_vec;

        //int L;//number of hashtables;
        int d;//dimensions
        int k;//diastasi pu 8a provalonte ta simeia
        int M;//maximum cout of points checked
        int probes;//maximum count of cube_map checked
        int no_of_vectors;
        int labels_count;
        unsigned int powk;

    public:
        hypercube(int M,int probes,int d ,int k,int no_of_vectors);
        //~hypercube();
        void Hashfun_init_cube(void);//initializes tables used for hashing 
        unsigned int hash_calc(vec* vect);//returns the bin cube vector point
        int vec_insert(vec* vect);//inserts a vector to the multimap
        void cube_start(int ,vec*);
        vector<vector<vec*>>* ANN_cube(vec* nvect,vector<vec>* clustersvec,int no_of_vectors);//kalei tin Cluster_LRadius me diaforetika radius kai meta kanei brute calculate(lloyds) ola ta ipoloipa vec
        int Cluster_LRadius(vec* cvector,unsigned int qbits,double radius,int clust_num,vector<vector<dist_vec>> *curr_clust_vec,int iteration);//vazei ta vec me tin apostasi tus apto cluster vec stin katalili thesi ston curr_clust_vec an i apostasi ine < radius
        //vector<vector<dist_vec>*>* all_NN_search(vec* ,int ,int );
        //vector<dist_vec>* NN_search(vec*,int);
        //vector<dist_vec>* RANGE_search(vec* ,double );
        //vector<vector<dist_vec>*>* all_RANGE_search(vec* ,double ,int );
        //void cube_map_print();
    
    };

#endif