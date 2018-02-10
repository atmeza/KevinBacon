/**
 * Filename: actorconnections.cpp
 * Author: Alex Meza
 * Userid: cs100vbj
 * Description: driver of program actorconnections
 */


#include <fstream>
#include <string>
#include "ActorGraph.hpp"
#include <iostream>
#include <sstream>



/**
 *function:main
 @param argc - number of arguments
 argv- name of program, file containing movie actor, 
 * */
int main(int argc, char*argv[]){

    ActorGraph* graph= new ActorGraph();

    graph->loadFromFile(argv[1],false);
       ifstream in(argv[2]);//file of actor to actor searches
    ofstream out(argv[3]);//outputfile
    
    bool header = true;
    out<<"Actor1\tActor2\tYear\n";
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
            
            out<<record[0]<<"\t"<<record[1]<<"\t"<<graph->getYear(record[0],record[1]);
        //write path to output
        //
            out.put('\n');

    }
    out.close();
    in.close();

    delete graph;
    return 0;
}
