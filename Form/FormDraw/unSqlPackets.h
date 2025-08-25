#ifndef unSqlPacketsH
#define unSqlPacketsH

#include <DB_Common.h>
#include <unFormLogicF1.h>
#include <FormTypes.h>

class TSel {
public:
    std::string Orders;
    std::string Objects;
    std::string Cells1;
    std::string Cells2;
    std::string Plans;
    std::string VidOtcs;
    std::string Dates1; // для первых 4 байтов TDateTime
    std::string Dates2; // для последних 4 байтов TDateTime
    std::string pokaz_types;

    int CurrNumb;
    TQueryUni* sp;
    int NBlock;
    TSel() {
        CurrNumb = 0;
        NBlock = 0;
        sp = new TQueryUni(DefDBconnect.Conn);
    }
    ~TSel() {
        delete sp;
    }
    virtual void BeginAdd();
    virtual void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) = 0;
    virtual void AddCell(int Order, PCellValueAtr vd);
    virtual void Execute();
    static std::string getListPlanIds(int album_id, int& CountPlanIds, int accessalbum_idSelBlockSubj);
};

class TSelCells : public TSel {
public:
    TSelCells() : TSel() {}
    ~TSelCells() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};

class TSelCellsUslPost : public TSel {
public:
    TSelCellsUslPost() : TSel() {}
    ~TSelCellsUslPost() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};

class TSelCellsSum : public TSel {
private:
    std::string PokazIds;
    std::string LinkPokazIds;
    std::string Cells;
    std::string ist_fin_ids;
    std::string spaces;
    std::string modes;
    int objekt_class_id;
    std::string statusdo_cell_id1, statusdo_cell_id2;
    std::string enum_id, enum_id2;
public:
    TSelCellsSum() : TSel() {}
    ~TSelCellsSum() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(unsigned char mode, int Order, PCellValueAtr vd);
    void Execute() override;
};

class TAgregat : public TSel {
private:
    std::string years;
    std::string rootpokazids;
    std::string kbkids;
    std::string Dates1;
    std::string Dates2;
    std::string istfinids;
    std::string dlins;
public:
    TAgregat() : TSel() {}
    ~TAgregat() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, TFormLogicF1* FormLogicF1, PCellValueAtr vd);
    void Execute() override;
};

class TAgregatSum : public TSel {
private:
    std::string rootpokazids;
    std::string kbkids;
    std::string Dates1;
    std::string Dates2;
    std::string istfinids;
    std::string dlins;
    std::string spaces;
    std::string modes;
    std::string nacals;
    int objekt_class_id = 0;
public:
    TAgregatSum() : TSel() {}
    ~TAgregatSum() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(unsigned char mode, int Order, TFormLogicF1* FormLogicF1, PCellValueAtr vd);
    void Execute() override;
};

class TComNP : public TSel {
private:
    std::string Spokaz_id;
    std::string Scomnp_id;
    std::string Scell_period;
    std::string Splan_id;
    std::string Sgod;
    std::string Skvrtl;
    std::string Smesjc;
    std::string Sden;
public:
    TComNP() : TSel() {}
    ~TComNP() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};

class TSelCellsSumObAlb : public TSel {
private:
    std::string PlanIds;
    int CountPlanIds;
public:
    TSelCellsSumObAlb() : TSel() {}
    ~TSelCellsSumObAlb() {}
    void BeginAdd() override;
    void AddListPlanIds(int album_id, bool isSrednee, int accessalbum_idSelBlockSubj);
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};

class TComNpSumObAlb : public TSel {
private:
    std::string SPlanIds;
    int CountPlanIds = 0;
    std::string Spokaz_id;
    std::string Scomnp_id;
    std::string Scell_period;
    std::string Sgod;
    std::string Skvrtl;
    std::string Smesjc;
    std::string Sden;
public:
    TComNpSumObAlb() : TSel() {}
    ~TComNpSumObAlb() {}
    void BeginAdd() override;
    void AddListPlanIds(int album_id, bool isSrednee, int accessalbum_idSelBlockSubj);
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};

class TSelCellsHistory : public TSel {
public:
    TSelCellsHistory() : TSel() {}
    ~TSelCellsHistory() {}
    void BeginAdd() override;
    void GetItems(TFormLogicF1* FormLogicF1, TwwStringList* p) override;
    void AddCell(int Order, PCellValueAtr vd) override;
    void Execute() override;
};
#endif
