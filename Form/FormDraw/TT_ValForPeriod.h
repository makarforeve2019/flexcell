#ifndef TT_VALFORPERIOD_H
#define TT_VALFORPERIOD_H

#include "DB_Common.h"
#include "dmGlobalVars.h"

class TValForPeriod {
public:
    static TStringList* sl;
    static void CreateObjValForPeriod(); // ������� ������ ������ TValForPeriod
    static void FreeObjValForPeriod(); // ����������� ������ ������ TValForPeriod
    static std::string CreateKey(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);
    // �������� ������
    void Addrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                 int cell_id, double value);
    // ���������, �������� ������ ��� ��������� ������ � ������� �������� - ���������� ����
    double Val_Addrcrd_Det4(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                            int cell_id_Mes);
    // ���������, �������� ������ ��� ��������� ������ � ������� �������� - ����
    double Val_Addrcrd_Det5(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                            int cell_id);
    // ���������, �������� ������ ��� ��������� ������ "� ��� �����" � ������� ��������
    double Val_Addrcrd_VtomCisle(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                                 int cell_id);
    // ���������, �������� ������ ��� ������ "����� �� �������" � ������� ��������
    double Val_Addrcrd_Itogo(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                             int cell_id);
    // ������������ ���������� ����������� � ������� ��������
    double Val_forOwner(int plan_id, int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);
    // ������� ��������� �������� ������
    double Getrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id,
                   int pokaz_is_razd,  int pokaz_vert);
    // ���������� ��� ������� �������������� - ������� ����� � ������ �������
    static double valFactToBegPeriod(int plan_id, int pokaz_id, int god, int nMesjc, int day);
    // ���������� ��� ������� �������������� - ������� ����� �� ������� �� ����
    static double valFactKvNaDatu(int plan_id, int pokaz_id, int god, int nMesjc, int day, int kv);
};

#endif
