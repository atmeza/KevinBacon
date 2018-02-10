/**
 * Filename: patherfinder.cpp
 * Author: Alex Meza
 * Userid: cs100vbj
 * Description: find path from one actor to another
 * Date August 26, 2016
 **/


#include <fstream>
#include <string>
#include "ActorGraph.hpp"
#include <iostream>
#include <sstream>

int main(int argc, char*argv[]){


    ActorGraph* graph= new ActorGraph();
    
    string str(argv[2]);
    if(0==str.compare("u")){

        graph->loadFromFile(argv[1],false);
    }else{

        graph->loadFromFile(argv[1],true);
    }

    ifstream in(argv[3]);//file of actor to actor searches
    ofstream out(argv[4]);//outputfile
    
    out <<"(actor)--[movie#@year]-->(actor)--...\n";//header
    bool header = true;
    while(in){//for all paths desired



        string s;
        if(!getline(in,s)) break;//break at end of file
 
        if(header){
            
            header = false;
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
            
            graph->findPath(record[0],record[1],out);//find path bewteen actors
        //write path to output
        //
            out.put('\n');
    }
  
  if (!in.eof())
  {
    cerr << "Failed to read "  << "!\n";
    return false;
  }
   out.close();
    in.close();
    delete graph;
}

