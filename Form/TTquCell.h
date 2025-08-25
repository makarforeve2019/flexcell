#ifndef TTQU_CELL_H
#define TTQU_CELL_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ClassesUtils.h"
#include "unDefDBconnect.h"

class TquCell {
private:
    int Params0_;
    int Params1_;
    int Params2_;
    std::string Params3_;
    class TQueryUni* qu;
    TStringList* sl;
    bool qu_Eof;

public:
    int Params0;
    int Params1;
    int Params2;
    std::string Params3;
    int cell_id;
    int cell_period;
    int cell_is_eval;
    int ist_fin_id;


    TquCell() {
        qu = new TQueryUni(DefDBconnect.Conn);
        qu->SQL->Add("SELECT * FROM Cell");
        qu->SQL->Add("WHERE pokaz_id = :PokazId AND cell_period = :PeriodType");
        qu->SQL->Add("AND ist_fin_id = :IstFinID AND cell_din_izm = :DinIzm");
        sl = new TStringList();
//        sl->sorted = true;
    }

    ~TquCell() {
        delete qu;
        // ����������� ��� ������� ���� TrcrdFM, ����������� � TStringList
        if (sl) {
            for (int i = 0; i < sl->Count(); ++i) {
                // ������� ������� ������ ���� TrcrdFM* �� std::variant
                if (auto obj = std::get_if<TrcrdFM*>(&sl->GetObject(i))) {
                    delete *obj;  // ����������� ����������� ���������� ������
                }
            }
            sl->Clear();  // ������� TStringList ����� ���������
            delete sl;     // ����������� ���� TStringList
            sl = nullptr;
        }
    }

    void Open() {
        std::string sKey = IntToStr(Params0) + "_" + IntToStr(Params1) + "_" + IntToStr(Params2) + "_" + Trim(Params3);
        int Index = 0;
        PrcrdFM rcrdFM = nullptr;

        if (sl->Find(sKey, Index)) {
            // �������� ��������� �� variant, ���������� ������
            auto it = std::next(sl->items.begin(), Index);
            MyVariantSL& variant = it->second;

            // ���������� std::get_if ��� ����������� ��������� ��������� �� PrcrdFM
            PrcrdFM* ptrToPrcrdFM = std::get_if<PrcrdFM>(&variant);
            rcrdFM = *ptrToPrcrdFM;
            cell_id = rcrdFM->cell_id;
            cell_period = rcrdFM->cell_period;
            cell_is_eval = rcrdFM->cell_is_eval;
            ist_fin_id = rcrdFM->ist_fin_id;
            qu_Eof = false;
            return;
        }

        qu->Close();
        qu->ParamByName("PokazId")->AsInteger() = Params0;
        Params0_ = Params0;
        qu->ParamByName("PeriodType")->AsInteger() = Params1;
        Params1_ = Params1;
        qu->ParamByName("IstFinID")->AsInteger() = Params2;
        Params2_ = Params2;
        qu->ParamByName("DinIzm")->AsString() = Params3;
        Params3_ = Params3;
        qu->Open();

        if (qu->EoF()) {
            cell_id = 0;
            cell_period = 0;
            cell_is_eval = 0;
            ist_fin_id = 0;
            qu_Eof = true;
        } else {
            cell_id = qu->FieldByName("cell_id").AsInteger();
            cell_period = qu->FieldByName("cell_period").AsInteger();
            cell_is_eval = qu->FieldByName("cell_is_eval").AsInteger();
            ist_fin_id = qu->FieldByName("ist_fin_id").AsInteger();
            qu_Eof = false;

            rcrdFM = new TrcrdFM{cell_id, cell_period, cell_is_eval, ist_fin_id};
            sl->AddObject(sKey, rcrdFM);
        }
    }




    bool EoF() {
        return qu_Eof;
    }

    void Close() {
        // Do nothing
    }
};
#endif // TTQU_CELL_H