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

//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc == 15 || argc== 11)
    {
        //printf("Error in # of args\n");
        return 1;
    }
    return 0;
}
//h synarthsh eixe graftei se c kathws sthn arxh ksekinisame to project se c kai oxi se c++
int input_handler(int argc, char *argv[],int* k, int* L, int* N,double* R,char (&input_file)[256], char (&query_file)[256], char (&output_file)[256]){
    char temp[256];
    if(argsOK(argc,argv)){
        if(argc==15){//an exoun dothei oloi oi parametroi apo command line
            printf("All parameters given from command line...\n");
            strcpy((input_file),argv[2]);
            strcpy((query_file),argv[4]);
            strcpy((output_file),argv[10]);
            *(k)=atoi(argv[6]);
            if(*(k)==0)//bad user safe
                *(k)=1;
            *(L)=atoi(argv[8]);
            if(*(L)==0)
                *(L)=1;
            *(N)=atoi(argv[12]);
            *(R)=atof(argv[14]);
        }else{//an exoun dothei kapoioi marametroi apo command line
            printf("Almost all parameters given from command line... Using default for k and L\n");
            strcpy((input_file),argv[2]);
            strcpy((query_file),argv[4]);
            strcpy((output_file),argv[6]);
            *(k)=4;
            *(L)=5;
            *(N)=atoi(argv[8]);
            *(R)=atof(argv[10]);

        }
    }else{//ama den exei dothei kamia parametros apo command line tis zhtame oles apo ton xrhsth kata thn ektelesh
        printf("Parameters were NOT given from command line, please input them to contiue!\n");
        //default times
        *(k)=4;
        *(L)=5;
        *(N)=1;
        *(R)=10000;
        printf("Type the path to the input_file: ");
        scanf("%s",(input_file));
        //printf("\n");
        printf("Type the path to the query_file: ");
        scanf("%s",(query_file));
        //printf("\n");
        printf("Type the path to the output_file: ");
        scanf("%s",(output_file));

        printf("Type the value of k or type d for default value(4) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(k)=4;
        else{
            if(isdigit(temp[0]))
                *(k)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");//troll ama kapoios paei na dokimasei na krasarei thn efarmogh dinodas lathos input
                return(1);
            }
        }
        strcpy(temp,"");

        printf("Type the value of L or type d for default value(5) ");
        scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(L)=5;
        else{
            if(isdigit(temp[0]))
                *(L)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");
        printf("Type the value of N or type d for default value(1) ");
         scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(N)=1;
        else{
             if(isdigit(temp[0]))
                *(N)=atoi(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");
        printf("Type the value of R or type d for default value(10000) ");
         scanf("%s",temp);
        if(strcmp(temp,"d")==0)
            *(R)=10000;
        else{
             if(isdigit(temp[0]))
                *(R)=atof(temp);
            else{
                printf("You did not enter a number or d! I will not tolerate this! Exiting...\n");
                return(1);
            }
        }
        strcpy(temp,"");
        //printf("\n");
    }
    return 0;
}


//anoigei to input file kai topothetei ta stoixeia se vec, kratontas tis syntetagmenes kai to onoma kathe dianismatos
vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors){
    //cout<<"aaa1";
    int counter=0;
    int flag=0;
    ifstream input;
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }

    string sline;
    string tok;


    (*no_of_vectors)=0;
    (*no_of_coordinates)=-1;//-1 giati metrame kai to arxiko id pou den einai coord

    while (getline(input,sline))//metrame posa dianismata kai poses diastaseis exoume
    {
        (*no_of_vectors)++;
        
        if((*no_of_vectors)==1){
            stringstream line(sline);

            while(line>> tok)
                (*no_of_coordinates)++;
        }
    }
    input.close();
    input.clear();
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }
    vec* nvectors;
    nvectors = new vec[(*no_of_vectors)];//desmevoume xwro gia ta vectors
    while (getline(input,sline))
    {       flag=0;
            stringstream line(sline);
            while(line>> tok) {//gia stoixeio
                if(flag==0){
                    //cout<<"TOK:"<<tok<<endl;
                    nvectors[counter].name=tok;//an eimste sto prwto stoixeio tis grammhs tote einai to onoma pou to kratame san string
                    flag=1;
                }
                else
                    nvectors[counter].coord.push_back(stof(tok));//diaforetika einai synentetagmenh tou dianismatos
                    
            }
        counter++;
    }
    //cout<<"NEW FUNCTION: no_of_vectors:"<<(*no_of_vectors)<<"no_of_coordinates:"<<(*no_of_coordinates)<<endl;
    return nvectors;
            
}

void print_vectors(vec *nvectors,int no_of_vectors,int no_of_coordinates){//voithitikh synarthsh ektypwnei ola ta vectors
    for(int i=0;i<no_of_vectors;i++){
        cout<<"Dianisma :"<<i<<endl;
        for(int j=0;j<no_of_coordinates;j++){
        //for(auto j: vector[i].coord){
            //cout<<j<<' ';
            if(j==0)
                cout<<"Name-id: "<<nvectors[i].name<<endl;
            
            cout<<nvectors[i].coord[j]<<" ";
        }
    }
    cout<<endl;
}
//synarthsh pou ektypwnei to output arxeio opws afto zhteite

int print_to_file(char output_file[256],string lsh_or_hypercube,vector<vector<dist_vec>*>* dsvec2,int queries_no_of_vectors,vec* qvectors,double *time_per_query_lsh,double *time_per_query_brute,vector<vector<dist_vec>*>* dsvec3,vector<vector<dist_vec>*>* dsvec4,double R){
    ofstream outfile;
    outfile.open(output_file);

    for(int i=0;i<queries_no_of_vectors;i++){//gia kathe query
        outfile<<"Query: ";
        outfile<<qvectors[i].name;
        outfile<<endl;
        vector<dist_vec>* dstemp2 =(*dsvec2)[i];
        vector<dist_vec>* dstemp3 =(*dsvec3)[i];
        
        int size2=dstemp2->size();

        for(int j=0;j<size2;j++){//ektypwnei tous kontinoterous geitones poy vrike <=N
        	dist_vec ds2=(*dstemp2)[j];
        	dist_vec ds3=(*dstemp3)[j];
        	

            outfile<<"Nearest neighbour-";
            outfile<<j+1<<": "<<ds2.vect->name<<endl;
            outfile<<lsh_or_hypercube;
            outfile<<ds2.dist<<endl;
            outfile<<"distanceTrue: "<<ds3.dist<<endl;
        }
        
    
        outfile<<"tLSH: ";
        outfile<<time_per_query_lsh[i]<<endl;
        outfile<<"tTrue: "<<time_per_query_brute[i]<<endl;
        outfile<<R<<"-near neighbours:"<<endl;//den kserw an thelei R h thelei to noumero tou R

        vector<dist_vec>* dstemp4 =(*dsvec4)[i];
        int size4=dstemp4->size();
        for(int r=0;r<size4;r++)//ektypwnei tous geitones pou vretikan me thn xrhsh ths lradius
        	{
        	dist_vec ds4=(*dstemp4)[r];
            outfile<<ds4.vect->name<<endl;
        	}
    }
    outfile.close();

    return 0;
}
//synarthsh pou rwtaei ton xrhsth pows thelei na synexisei meta apo kathe treksimo tou kwdika
int repeat_handler(vec* nvectors, vec* qvectors,char* input_file,char*query_file,char* output_file,Lhashtables *lht){
    cout<<"To end programm type 0, To repeat with new query_file and input_file type 1, To repeat with new input_file type 2, To repeat with new query_file type 3 : "<<endl;
    int input;
    char temp[256];
    char scanret[256];
    while(1){
        strcpy(temp,"");
        strcpy(scanret,"");
        scanf("%s",scanret);
        if(isdigit(scanret[0]))
            input=stoi(scanret);//ama den einai arithmos
        else
            input=-5;
        if(input==0){//ama einai 0 kanei exit kai ta frees ginontai sthn main
            return -1;
        }
        else if(input==1){//ama exoume kainourio input file kai query file zhtame ta onomata kai eleftherwnoume thn mnhmh twn paliwn domwn
            
            cout<<"Please enter input_file"<<endl;
            scanf("%s",temp);
            strcpy(input_file,temp);
            cout<<"Please enter query_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(query_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] nvectors;
            delete [] qvectors;
            delete lht;
            return 0;
        }else if(input ==2){//ama exoume mono kainourio input file zhtame onoma kai eleftherwnoume thn mnhmh tou lsh kathw tha prepei na ksanaypologistei
            cout<<"Please enter input_file"<<endl;
            scanf("%s",temp);
            strcpy(input_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] nvectors;
            delete lht;
            return 1;
        }else if(input==3){//ama exoume mono kainourio query file tote apeleftherwnoume thn mnhmh twn paliwn queries kai den diagramoume tis domes tou lsh afou einai oi idies afou exoume idio input file
            cout<<"Please enter query_file"<<endl;
            scanf("%s",temp);
            strcpy(query_file,temp);
            cout<<"Please enter output_file"<<endl;
            strcpy(temp,"");
            scanf("%s",temp);
            strcpy(output_file,temp);
            delete [] qvectors;
            return 2;
        }else cout<<"You typed something wrong, please try again!"<<endl;
    }

}