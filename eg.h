#ifndef _EG_H
#define _EG_H

#include <list>
#include <iostream>
#include <string>

class EG
{
  public:
    EG() {symbols = std::list<char>(); cuts = std::list<EG*>();}
    EG(const EG & g);
    ~EG();

    void read(std::string & g, int & i);
    std::string toString();
    void print(int depth);
    void simplify();
    void reduce(char c, bool t);
    bool open();
    bool finished();

    char find_symbol();
    bool check_consistency(int d);
  private:
    std::list<char> symbols;
    std::list<EG*> cuts;
};

#endif
