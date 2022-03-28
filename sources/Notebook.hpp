#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Direction.hpp"

using namespace std;
namespace ariel 
{
    class Notebook 
    {
    private:
        map<int, vector<vector<char>>> pages;
    public:
        Notebook();
        void write(int, int, int, Direction, string);
        string read(int, int, int, Direction, int);
        void erase(int, int, int, Direction, int);
        void show(int page);
    };

    bool check_text(string);
}