#include "unF1FlexCelFormulas.h"


void TF1FlexCelFormulas::ApplyChanges() {
    short typeRC;
    int i;
    TF1Value* fv;

    _xlsF1->Recalc();

    for (auto  it = _cellValues.begin(); it != _cellValues.end(); ++it) {
        fv = *it;
        typeRC = _xlsF1->TypeRC(fv->row, fv->col);
        if (typeRC < 0) {
            if (typeRC == -2) {
                std::string wsValue = char16ToUTF8(_xlsF1->TextRC(fv->row, fv->col));
                if (fv->valueString != wsValue) {
                    _actionView->CellValue(_xlsF1->Sheet, fv->row, fv->col, 2);
                }
            } else {
                if (fv->valueDouble != _xlsF1->NumberRC(fv->row, fv->col)) {
                    _actionView->CellValue(_xlsF1->Sheet, fv->row, fv->col, 1);
                }
            }
        }
    }
}

void TF1FlexCelFormulas::RememberCellValues() {
    short typeRC;
    int r, c;
    std::string s;

    for (r = 1; r <= _xlsF1->LastRow; r++) {
        for (c = 1; c <= _xlsF1->LastCol; c++) {
            typeRC = _xlsF1->TypeRC(r,c);
            if (typeRC < 0) { // IsFormula
                TF1Value* fv = new TF1Value();
                fv->row = r;
                fv->col = c;
                if (typeRC == -3) {
                    s = "Строка " + std::to_string(fv->row) + ", столбец " + std::to_string(fv->col) +
                        ". Некорректный результат формулы: " + char16ToUTF8(_xlsF1->FormulaRC(fv->row, fv->col));
                    throw std::runtime_error(s);
                }
                if (typeRC == -2) {
                    fv->isString = true;
                    fv->valueString = char16ToUTF8(_xlsF1->FormulaResultTextRC(fv->row, fv->col));
                } else {
                    fv->isString = false;
                    fv->valueDouble = _xlsF1->FormulaResultNumberRC(fv->row, fv->col);
                }
                _cellValues.push_back(fv);
            }
        }
    }
}



void TF1FlexCelFormulas::Clear() {
    for (auto& fv : _cellValues) {
        delete fv;
    }
    _cellValues.clear();
}

bool TF1FlexCelFormulas::iaListCellValuesEmpty() {
    return _cellValues.empty();
}
