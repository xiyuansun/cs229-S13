#ifndef __AUT__
#define __AUT__

#include <string>
#include <vector>

class AutFile
{
    public:
        AutFile(std::string &in);
        //Board const get();
        //void update(Board &b);
        std::string const to_string();
    private:
        void parse(std::string &s);
        std::vector<std::string> statements;
};
#endif
