#include "h/csv.h"

#include <iostream>

using std::cout;
int main(){
    CSV c("data/buildings.csv");
    c.printIt();
    for(int i = 0; i < c.getNrow(); ++i){
        for(int j = 0; j < c.getNcol(); ++j){
            cout << c.elemAsDouble(i,j) << "  ";
        }
        cout << "\n";
    }
    cout << "\n";
    return 0;
}