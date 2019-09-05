#include "h/csv.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::stringstream;
using std::ifstream;
using std::map;
using std::getline;
using std::atof;
using std::cout;


static vector<string> splitStrings (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

static vector<double> splitDoubles (const string &s, char delim) {
    vector<double> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (atof(item.c_str()));
    }

    return result;
}


CSV::CSV(const string& fname){    
    ifstream infile(fname.c_str());
    string line;
    ncol = 0;
    nrow = 0;
    // get headers
    getline(infile, line);
    names = splitStrings(line, ',');
    ncol = names.size();
    
    // map headers to column number
    for(int i = 0; i < names.size(); ++i)
        columnsId.insert({names[i],i});
    
    // initialize each column
    values.resize(ncol);

    // fill the values;
    while(getline (infile, line)){
        auto row = splitStrings(line,',');
        for(int i = 0; i < ncol; ++i){
           values.at(i).push_back(row.at(i));
        }
    }
    
    nrow = values.at(0).size();
}

void CSV::printIt() const {
    for(auto i : names)
        cout << i << "  ";
    cout << "\n";
    /*
    for(auto row : values){
        for(auto elem : row)
            cout << elem << ' ';
        cout << "\n";
        }
    cout << '\n';
    */
    for(int row = 0; row < nrow; ++row){
        for(int col = 0; col < ncol; ++col){
            cout << values.at(col).at(row) << "  ";
        }
        cout << "\n";
    }
    cout << "\n";
}

string CSV::operator()(int row, string col) const {
    int colindex = columnsId.find(col)->second;
    return (*this)(row, colindex);
}

string CSV::operator()(int row, int col) const {
    return values.at(col).at(row);
}

double CSV::elemAsDouble(int row, string col) const {
    string elem = (*this)(row, col);
    return std::atof(elem.c_str());
}

double CSV::elemAsDouble(int row, int col) const {
    string elem = (*this)(row, col);
    return std::atof(elem.c_str());
}