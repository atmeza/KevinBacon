#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>


using namespace std;

class ActorGraph {
   protected:

        
   public:
      ActorGraph(void);
      ~ActorGraph();
        void findPath(string actor1, string actor2, ofstream& out);
        int getYear(string start, string end);
   /**
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as
     * 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
     bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    

    private:
 
    //forward decleration
    class Vertex;
    class Edge;
    map<string, Vertex*> actors;//holds pointers to Verticies
    

    

        class   Edge {//connection between actors
            //movie in common
        public:
        
        
        
            string shared_movie;
            int weight;//weight
            int year;
            Vertex* next;//connected actor
        //constructor 
            Edge(string& movie, int& weight,int& year, Vertex*& coactor):shared_movie(movie),weight(weight),year(year),next(coactor){
                
                }
            

        };



        class Vertex{//actors

            friend bool comp(Vertex* one, Vertex* other);


        public:

            string name;
            vector<Edge*> edges;//hold connections to other actors
            Vertex* prev;//path to next
            int dist;//distance from source
            string movie;//keep track of movie that links actor
            int year;//keep track of movie year that links
            bool done;
            Vertex* next;
            Vertex(string name):name(name){dist=0;
                movie = "";
                year = 0;
                prev = 0;
                next = 0;
            }


            bool operator<(const Vertex& other){return dist<other.dist;}
        };
        bool comp(Vertex* one, Vertex* other);

        class VertexCmp {
        public:
            bool operator()(Vertex*& lhs, Vertex*& rhs)const {
                return lhs->dist>rhs->dist;
            }

        };
};


#endif // ACTORGRAPH_HPP
