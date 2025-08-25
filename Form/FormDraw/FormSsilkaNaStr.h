#ifndef FormSsilkaNaStrH
#define FormSsilkaNaStrH


#include <string>

class TFormSsilkaNaStr
{
public:
    static void CreateSL();
    static void FreeSL();
    static void AddSsilkaNaStr(std::string aSsilkaNaStr, int aRow);
    static bool findSsilkaNaStr(std::string aSsilkaNaStr, int &aRow);
};

#endif
