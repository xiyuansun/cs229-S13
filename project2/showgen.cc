#include <string>
#include <iostream>
#include "lib/io/aut.h"
#include <vector>

using namespace std;

int main()
{
    string file("file.aut");
    AutFile* a = new AutFile(file);
    
    /*vector<string> vs = a->get();
    
    for(unsigned int i = 0; i < vs.size(); ++i)
    {
        cout << vs[i] << endl;
    }*/

    return 0;
}
