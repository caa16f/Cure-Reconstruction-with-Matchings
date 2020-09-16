#define _USE_MATH_DEFINES
#include <iostream>
#include <cstring>
#include <cctype>
#include <stdio.h>
#include<math.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <fstream>
using namespace std;


//Compute the nearest neighbor edges
std::vector<int> compute_nn_edges( vector<pair<int,int> > vector) {
    std::vector<int> rVect;
    double shortestDistance = 1000;
    double distance = 0;
    int index;

    //Find the shortest distance
    for (int i = 0; i < vector.size(); i++) {
        for (int j = 0; j < vector.size(); j++) {
            if (i == j) {
                //Do not calculate if i = j , it will just equal 0
            } else {
                //Distance formula
                distance = sqrt(pow(vector[i].first - vector[j].first, 2) + (pow(vector[i].second - vector[j].second, 2)));

                //Comparing to shortestdistance and saving it to index if true
                if (distance <= shortestDistance) {
                    shortestDistance = distance;
                    index = j;
                }
            }
        }

        //Push the index to rVect thus returning a vector of edges where i is x and rVect[i] = y
        rVect.push_back(index);
    }
    return rVect;
}

/*
    This whole function is a smorgasbord of for loops and data structural changes. Please excuse my horrible explanations
*/
void compute_matching(std::vector<int> vector, std::vector<pair<int,int> >points){



    std::vector<int> sl_vertex_index;               //Holds index of single and looking vertices
    std::vector<pair<int,int> > sl_pairs;           //Holds vector of pairs of single and looking vertices pairs
    std::vector<double> sl_ww;                      //Parrallel to the sl_pairs , holds the weights of the edges


    //If there is only one occurance of a index in the G1 array then that means that index
    // is a sl-vertex. This is kinda speculation but i really think its true and has ultimately gotten me the right answer
    int count = 0;
    for(int i = 0; i < vector.size(); i++){
        for(int j = 0 ; j < vector.size(); j++){
            if(vector[i] == vector[j]){
                count++;
            }
        }
        if(count == 1){
            sl_vertex_index.push_back(vector[i]);
        }
        count = 0;
    }

    //Next we need to compute weight for i and j
    //In order to compute the weights we first must find out if the edge makes an admissible angle
    for(int i = 0; i < sl_vertex_index.size(); i++){
        for(int j = 0 ; j < sl_vertex_index.size(); j++) {
                if( i != j){
                    //Calculate the angle made from 3 points. The 3 points are the two sl vertices and the
                    // edge connected to either of the sl vertices. We must calculate two angles for each admissible edge
                    //If both the results of the edge calculation fit into the range for theta = pi/2 then it is an admissible edge
                    double result = atan2(points[sl_vertex_index[j]].second - points[sl_vertex_index[i]].second, points[sl_vertex_index[j]].first - points[sl_vertex_index[i]].first) -
                                    atan2(points[i].second - points[sl_vertex_index[i]].second, points[i].first - points[sl_vertex_index[i]].first);
                    //P1 : center point
                    //P1 : points[sl_vertex_index[i]] P2: points[i] P3: points[sl_vertex_index[j]]
                    double result2 = atan2(points[sl_vertex_index[i]].second - points[sl_vertex_index[j]].second, points[sl_vertex_index[i]].first - points[sl_vertex_index[j]].first) -
                                    atan2(points[j].second - points[sl_vertex_index[j]].second, points[j].first - points[sl_vertex_index[j]].first);
                    //P1 :  points[sl_vertex_index[j]] P2: points[j] : P3:  points[sl_vertex_index[i]]


                    //Check whether is or is not an admissible edge
                    if (result > M_PI/2 || result < -(M_PI/2)){
                        if(result2  > M_PI/2 || result2 < -(M_PI/2)){
                            //Add tp vectors to store the pairs and their appropriate weights
                            sl_pairs.push_back(make_pair(sl_vertex_index[i],sl_vertex_index[j]));
                            //Compute the weight in a single line. The weight is the distance formula squared
                            sl_ww.push_back((points[sl_vertex_index[i]].first - points[sl_vertex_index[j]].first) * (points[sl_vertex_index[i]].first - points[sl_vertex_index[j]].first) + (points[sl_vertex_index[i]].second - points[sl_vertex_index[j]].second) * ((points[sl_vertex_index[i]].second - points[sl_vertex_index[j]].second)));
                        }
                    }
                }
        }
    }


    //Remove most weighted duplicate pair from list of pairs.
    //If there exist a connection between 1 x index and mulitple y index then the lowest weight point (highest preference)
    //will remain in the list
    for(int i = 0; i < sl_pairs.size(); i++) {
        for(int j = 0 ; j < sl_pairs.size(); j++){
            if(sl_pairs[i].first == sl_pairs[j].first && i != j){
                if(sl_pairs[i].second != sl_pairs[j].second){
                    if(sl_ww[i] <= sl_ww[j]){
                        sl_pairs.erase(sl_pairs.begin()+j);
                        sl_ww.erase(sl_ww.begin()+j);
                    }else{
                        sl_pairs.erase(sl_pairs.begin()+i);
                        sl_ww.erase(sl_ww.begin()+i);
                    }
                }
            }
        }
    }

    //sl_pairs
    //sl_vertex_index

    //Create a list to hold the edges between nearest neighbors
    std::vector<pair<int,int> > sl_vertex_list;
    //Eliminate Duplicate or unnecessary information
    for(int i = 0; i < sl_vertex_index.size(); i++){
         pair<int, int> temp = make_pair(i,sl_vertex_index[i]);
         sl_vertex_list.push_back(temp);
    }

    //If (x,y) and (y,x) exist in the list then eliminate (y,x) this will make the routing later much easier
    for(int i = 0; i < sl_vertex_list.size(); i++) {
        for (int j = 0; j < sl_vertex_list.size(); j++) {
            if (sl_vertex_list[i].first == sl_vertex_list[j].second && sl_vertex_list[i].second == sl_vertex_list[j].first){
                sl_vertex_list.erase(sl_vertex_list.begin()+j);
            }
        }
    }
    //Do the same elimination for the single and looking pairs sl_pairs
    for(int i = 0; i < sl_pairs.size(); i++) {
        for (int j = 0; j < sl_pairs.size(); j++) {
            if (sl_pairs[i].first == sl_pairs[j].second && sl_pairs[i].second == sl_pairs[j].first){
                sl_pairs.erase(sl_pairs.begin()+j);
            }
        }
    }


    //Now lets compute our output
    //Set index to be the first value in the route 0 by default
    int next_index = 0;
    std::cout << next_index  << endl;

    //Create a vector or pairs to hold the complete list of vectors both nearest neighbor and gale-shapely
    std::vector<pair<int,int> > complete;


    //I am moving both edge lists into a single list to make it easier to handle
    for(int i = 0; i < sl_vertex_list.size() ; i++){
        complete.push_back(sl_vertex_list[i]);
    }
    for(int i = 0; i < sl_pairs.size() ; i++){
        complete.push_back(sl_pairs[i]);
    }

    int temp = 0;
    bool empty = true;
    for(int i = 0; i < complete.size()/2; i++){
        //By default empty is trye unless proven otherwise
            empty = true;
        for(int j = 0; j < complete.size(); j++) {
            //check if the first number is the next index
            if(complete[j].first == next_index ){
                //if so then it is not false, set new index and print
                empty = false;
                next_index = complete[j].second;
                cout << next_index << endl;

            }else if(complete[j].first != next_index && complete[j].second == next_index && empty == true){
                //If no link was found then that must mean the link is reversed, reverse the link here
                next_index = complete[j].first;
                cout << next_index << endl;
            }
        }

    }


}

int main( int argc , char**argv) {
    string line;    //Hold parsed lines


    //Vector or pairs that will hold the points
    vector<pair<int , int> >points;


    int q = 0;
    //Read standard in
    while(getline(cin, line)){
        //I know what you're thinking "why do you need this if statement"
        //Well the answer is idk. The only way the inner if statement works
        //is if the if statment is there.
        //Since i am dynammically allocating a vector the total number of elements
        //is not needed
        if(q == 0){
            //Read in pairs. cin the x comma and y but dont do anything with the comma
            int x , y; char comma;
            if (cin >> x >> comma >> y){
                //Add pair to vector
                points.push_back(make_pair(x,y));
            }
        }else{
                int x , y; char comma;
                if (cin >> x >> comma >> y){
                    points.push_back(make_pair(x,y));
                }
            }
        q++;

    }

    //Compute nearest neighbors and place in G1
    vector<int> G1 = compute_nn_edges(points);

    //Compute the remaining points with G1 and the points
    compute_matching(G1,points);

    return 0;
}
