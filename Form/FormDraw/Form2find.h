
#ifndef Form2findH
#define Form2findH

#include <string>
#include "ClassesUtils.h"


extern TStringList StmntList;
class TForm2find
{
public:
    static void createStmntList();
    static int findByStmnt(std::string aStmnt);

    TForm2find(){

//        StmntList.sorted = true;
//        createStmntList();
    };

    ~TForm2find(){
        //mak???delete StmntList;
    };
private:

};

#endif
