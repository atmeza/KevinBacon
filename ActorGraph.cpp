#include "ActorGraph.hpp"
#include <iostream>
#include <stack>
#include <fstream>
#include <fstream>
#include <utility>
#include <iostream>
#include <deque>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>


ActorGraph::ActorGraph(void)
{
}
/*
 *deconstructor
 */
ActorGraph::~ActorGraph()
{
    for(auto& v: actors){

        for(auto &e: v.second->edges){//for all its edges
            delete e;
        }
        delete v.second;
    }


}
/**
 *getYear returns year or 9999 of year that actors became connecte
 * param: start-source, end-other actor
 */
int ActorGraph::getYear(string start, string end){

    priority_queue<Vertex*,vector<Vertex*>,VertexCmp> toExplore;//for bfs search
    Vertex* begin;//start vertex
    
    int min =2015;
    for(auto temp: actors.find(start)->second->edges){
        if(temp->year<min) min = temp->year;//get year made first movie

    }
    
    while(min<2016){
        for(auto&v: actors){//for all actors set dist to -1 
            v.second->dist = numeric_limits<int>::max();
            v.second->done = false; 
            if(v.first== start){//if its the source set to 0
                begin = v.second;
                v.second->dist = 0;
                toExplore.push(begin);//push onto queue
             }
        }
        

        while(!toExplore.empty()){//while there are paths to explore
            
            auto next = toExplore.top();//start at front of queue
            toExplore.pop();
            if(next->done) continue;
            //next->done = true;
            next->done = true;
            for(auto  &n:next->edges){//for all neighbors
                //if the new paths distance to its neighbors is faster than
                if(n->year>min) continue;
                if(n->next == actors[end]) return min;
                //the neighbors old path use new path
                if(next->dist+n->weight < n->next->dist){
                    n->next->dist = next->dist +n->weight;
                    toExplore.push( n->next);
                }
                
            }
        }
     min++;// try next year
    }

    return 9999;

}

/**
 *  start- source node, end - node looking for relationship to
 *  out ->file to write to
 *  returns void
 *  findPath- prints out path from start -> end actors
 */


void ActorGraph::findPath(string start,string end, ofstream& out){


    stack<Vertex*> path;//hold path for of traversal from end to start
    priority_queue<Vertex*,vector<Vertex*>,VertexCmp> toExplore;//for bfs search
    Vertex* begin;//start vertex

    for(auto&v: actors){//for all actors set dist to -1 
        v.second->dist = numeric_limits<int>::max();
        v.second->done = false; 
        if(v.first== start){//if its the source set to 0
            begin = v.second;
            v.second->dist = 0;
            toExplore.push(begin);//push onto queue
         }
    }
    

    while(!toExplore.empty()){//while there are paths to explore
        
        auto next = toExplore.top();//start at front of queue
        toExplore.pop();
        if(next->done) continue;
        //next->done = true;
        next->done = true;
        for(auto  &n:next->edges){//for all neighbors
            //if the new paths distance to its neighbors is faster than
            //the neighbors old path use new path
            if(next->dist+n->weight < n->next->dist){
                n->next->dist = next->dist +n->weight;
                n->next->prev = next;
                toExplore.push( n->next);
            }
            
        }
    }
    auto finish = actors[end];//actor searching for

     
    while(finish!=actors[start]){
        for(auto e: finish->edges){

            if(e->next!= finish->prev) continue;
            if(e->year>finish->prev->year){
                finish->prev->movie= e->shared_movie;
                finish->prev->year = e->year;
            }
        }
        finish->prev->next = finish;
        finish = finish->prev;
    }

    
    auto temp = actors[start];
    while(true){
   
        if(temp== actors[end]){//if empty display just name and not edge

            out<<"(" <<temp->name<<")";
            break;
        }
        out<<"("<<temp->name<<")--["<<temp->movie<<"#@"<<temp->year<<"]-->";
        temp = temp->next;
    }

    
 
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges)
{
 // Initialize the file stream
  ifstream infile(in_filename);
    map<pair<int,string>, pair<deque<string>,int> > movies;//map to group actors according to movie

  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile)
  {
    string s;
    
    // get the next line
    if (!getline( infile, s )) break;
    
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;
  
    //stores weight

    while (ss)
    {
      string next;
      
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      
      record.push_back( next );
    }
    
    if (record.size() != 3)
    {
      // we should have exactly 3 columns
      continue;
    }
    
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    
    int weight = 1 +(2015-movie_year);//calculate weight


    if(!use_weighted_edges){//not using weighted edged
        weight = 1;
    }


    //insert vertex into map accoriding to name
    if(actors.find(actor_name)==actors.end()){

        Vertex* v = new Vertex(actor_name);
        v->dist = 0;
        actors.insert(make_pair(actor_name, v));
    }
    if(movies.find(make_pair(movie_year,movie_title))==movies.end()){
            
        

        deque<string> temp {actor_name};//intialize vector with actor
        movies.insert(make_pair(make_pair(movie_year,movie_title), make_pair(temp ,weight)));

    } else{//movie is already in map so add actor to list
        //map->pair->deque of actor names
        movies.find(make_pair(movie_year,movie_title))->second.first.push_back(actor_name);

    }

    
  }
  
  if (!infile.eof())
  {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }


  infile.close();
 

    //add all edges into verticies

    for( auto & m:movies){//for each movie
        
        for(auto a: m.second.first){//for all actors in that movie
            

            auto act1 = actors.find(a)->second;//get vertex for first actor

            for(auto at:m.second.first){//for every actor in that movie
                
                auto act2 = actors.find(at)->second;//get vertex of second actor

                if(act1!=act2){//if actor1 !=actor 2
//create edge from the vertex of  act1 to act2
                    int y = m.first.first;//year of movie
                    Edge* e = new Edge((string&)m.first.second,m.second.second,y,act2); 
                    act1->edges.insert(act1->edges.begin(), e);
                }

            }
        }
    }
    


  return true;
	
}
