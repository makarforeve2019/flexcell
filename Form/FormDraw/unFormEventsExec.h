#ifndef FormEventsExecH
#define FormEventsExecH

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

class TFormEventsExec {
private:
    PFormEvent* FormEvent;
    void* FormShowF1;
    TwwStringList p;
    int nRow, nCol;
    double oldValue, newValue;

    void doInput1();
    void doInput1Handle();

    void doInput1HandleIn();
    void doInput1HandlePkz();
    void doInput1HandleObj();
    void doInput1HandleObjUp();

public:
    int plan_id;  // for calculated indicator/characteristic
    int cell_id;
    int vid_otc;
    time_t value_date;
    int objectid;  // for calculated object/indicator

    static void LoadFormEvents(TwwStringList& p);
    static void doManInputs(void* FormShowF1, TwwStringList& p, int nRow, int nCol, string EditString,
                           double oldValue, int& plan_idHandle, int& cell_idHandle, int& objectidHandle, int& vid_otcHandle, time_t& value_dateHandle);
    static void doSysInputs(void* FormShowF1, TwwStringList& p, int nRow, int nCol, string EditString,
                           double oldValue);
    // static bool checkPossibilityDelPkz(void* FormShowF1, int nRow, int PlanId);
    // static bool checkPossibilityDelObj(void* FormShowF1, int nRow, int PlanId);
};

#endif