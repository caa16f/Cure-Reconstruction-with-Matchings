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



std::vector<int> compute_nn_edges( vector<pair<int,int> > vector) {
    std::vector<int> rVect;
    double shortestDistance = 1000;
    double distance = 0;
    int index;
    for (int i = 0; i < vector.size(); i++) {
        for (int j = 0; j < vector.size(); j++) {
            if (i == j) {
                //Do not calculate if i = j , it will just equal 0
            } else {
                distance = sqrt(pow(vector[i].first - vector[j].first, 2) + (pow(vector[i].second - vector[j].second, 2)));
                if (distance <= shortestDistance) {
                    shortestDistance = distance;
                    index = j;
                }
            }
        }
        rVect.push_back(index);
    }
    return rVect;
}

std::vector<int> compute_matching(std::vector<int> vector, std::vector<pair<int,int> >points){
    //If there is only one occurance of a index in the G1 array then that means that index
    // is a sl-vertex

    std::vector<int> sl_vertex_index;
    std::vector<pair<int,int> > sl_pairs;
    std::vector<double> sl_ww;


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

    for(int i = 0; i < sl_vertex_index.size(); i++){
        for(int j = 0 ; j < sl_vertex_index.size(); j++) {
                if( i != j){
                    //Calculate the angle made from 3 points. The 3 points are the two sl vertices and the
                    // edge connected to either of the sl vertices. We must calculate two angles for each admissible edge
                    //If both the results of the edge calculation fit into the range for theta = pi/2 then it is an admissible edge
                    double result = atan2(points[sl_vertex_index[j]].second - points[sl_vertex_index[i]].second, points[sl_vertex_index[j]].first - points[sl_vertex_index[i]].first) -
                                    atan2(points[i].second - points[sl_vertex_index[i]].second, points[i].first - points[sl_vertex_index[i]].first);
                    //P1 : points[sl_vertex_index[i]] P2: points[i] P3: points[sl_vertex_index[j]]
                    double result2 = atan2(points[sl_vertex_index[i]].second - points[sl_vertex_index[j]].second, points[sl_vertex_index[i]].first - points[sl_vertex_index[j]].first) -
                                    atan2(points[j].second - points[sl_vertex_index[j]].second, points[j].first - points[sl_vertex_index[j]].first);
                    //P1 :  points[sl_vertex_index[j]] P2: points[j] : P3:  points[sl_vertex_index[i]]





                    if (result > M_PI/2 || result < -(M_PI/2)){
                        if(result2  > M_PI/2 || result2 < -(M_PI/2)){
                            std::cout << "Result(s) " <<"  (" <<  points[sl_vertex_index[i]].first << ", " <<points[sl_vertex_index[i]].second << ")   (" <<points[i].first << ", "  << points[i].second << ")  ("<< points[sl_vertex_index[j]].first<< ", "<<points[sl_vertex_index[j]].second<< ") " << result * 180/3.141592654 << " " <<result2 * 180/3.141592654 <<endl;
                            sl_pairs.push_back(make_pair(sl_vertex_index[i],sl_vertex_index[j]));
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
                        bestW = sl_ww[i];
                        sl_pairs.erase(sl_pairs.begin()+j);
                        sl_ww.erase(sl_ww.begin()+j);
                    }else{
                        bestW = sl_ww[j];
                        sl_pairs.erase(sl_pairs.begin()+i);
                        sl_ww.erase(sl_ww.begin()+i);
                    }
                }
            }
        }
    }
    //Run Gale-Shapely on the sl_pairs and sl_ww vectors
    for(int i = 0; i < sl_pairs.size(); i++){
        std::cout << "(" << sl_pairs[i].first << "," << sl_pairs[i].second << ") " << endl;
    }

    //Run Gale-Shapely on the sl_pairs and sl_ww vectors
    for(int i = 0; i < sl_pairs.size(); i++){
        std::cout << "(" << sl_ww[i] << ") " << endl;
    }

    std::cout << 0 ;

    for(int i = 0; i < points.size(); i++){

    }


    return sl_vertex_index;

}

int main( int argc , char**argv) {
    string line;    //Hold parsed lines
    int totalPoints;        //Hold the total number of points
    ifstream MyReadFile("test.txt");

    vector<pair<int , int> >points;

    int q = 0;
    while(getline(MyReadFile, line)){
        if(q == 0){
            int x , y; char comma;
            if (MyReadFile >> x >> comma >> y){
                points.push_back(make_pair(x,y));
            }
        }else{
                int x , y; char comma;
                if (MyReadFile >> x >> comma >> y){
                    points.push_back(make_pair(x,y));
                }
            }
        q++;

    }

    MyReadFile.close();

    vector<int> G1 = compute_nn_edges(points);
    for(int i = 0; i < G1.size(); i++)
    {
    //    cout << G1[i] << endl;
    }

    vector<int> G2 = compute_matching(G1,points);
    for(int i = 0; i < G2.size(); i++)
    {
        cout << "Line " << i<< " " << "--> " << G2[i] << endl;
    }
    return 0;
}
