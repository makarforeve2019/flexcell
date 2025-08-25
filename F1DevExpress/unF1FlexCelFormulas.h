#ifndef unF1FlexCelFormulasH
#define unF1FlexCelFormulasH

#include "unActionView.h"
#include <string>
#include <vector>
#include "TF1FlexCel.h"

using namespace F1DevExpress::unF1FlexCel;

class TF1Value
{
public:
    int row;
    int col;
    double valueDouble;
    std::string valueString;
    bool isString;
};

class TF1FlexCelFormulas
{
public:
    TF1FlexCelFormulas(TActionView* actionView, TF1FlexCel* xls){
        _actionView = actionView;
        _xlsF1 = xls;
    };
    ~TF1FlexCelFormulas(){
        //delete _cellValues;
    };
    void RememberCellValues();
    void ApplyChanges();
    void Clear();
    bool iaListCellValuesEmpty();

private:
    TActionView* _actionView;
    TF1FlexCel* _xlsF1;
    std::vector<TF1Value*> _cellValues;
};

#endif
