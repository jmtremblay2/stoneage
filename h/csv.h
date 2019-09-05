#ifndef ANOTHERFREAKINGCSVREADER_HPP
#define ANOTHERFREAKINGCSVREADER_HPP

#include <vector>
#include <string>
#include <map>

class CSV {
    public:
        CSV(const std::string& fname);
        void printIt() const;
        std::string operator()(int row, std::string col) const;
        std::string operator()(int row, int col) const;
        
        //bool columnIsDouble(std::string) const;
        double elemAsDouble(int row, std::string col) const;
        double elemAsDouble(int row, int col) const;
        
        double getNcol() const {return ncol;}
        double getNrow() const {return nrow;}
    protected:
        int ncol, nrow;
        
        //std::vector<bool> isDouble;
        std::vector<std::string> names;
        std::map<std::string, int> columnsId;
        std::vector<std::vector<std::string>> values;
};



#endif