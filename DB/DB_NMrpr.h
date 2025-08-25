#ifndef DB_NMRPR_H
#define DB_NMRPR_H

#include <string>
#include "DB_Common.h"


struct DBGodN0 {
    std::string DBName;
    int God;
    int N0;
};

const int KolElemNMRPR = 62;

static DBGodN0 tblDBGodN0[KolElemNMRPR] = {
    {"vao", 2024, 162422},
    {"vao", 2023, 158220},
    {"vao", 2022, 152370},
    {"vao", 2021, 145946},
    {"vao", 2020, 140650},
    {"vao", 2019, 133070},
    {"uao", 2019, 143690},
    {"vao", 2018, 127490},
    {"uao", 2018, 136175},
    {"vao", 2017, 121844},
    {"uao", 2017, 131580},
    {"szao", 2016, 103685},
    {"vao", 2016, 116871},
    {"vaoOtl", 2016, 116871},
    {"uao", 2016, 125278},
    {"vao", 2015, 110011},
    {"vaoOtl", 2015, 110011},
    {"uao", 2015, 115910},
    {"ugzao", 2015, 125743},
    {"economicsdb", 2015, 111677},
    {"vao", 2014, 101006},
    {"vaoOtl", 2014, 101006},
    {"uao", 2014, 101638},
    {"ugzao", 2014, 119534},
    {"economicsdb", 2014, 102370},
    {"vao", 2013, 93830},
    {"vaoOtl", 2013, 93830},
    {"uao", 2013, 94275},
    {"ugzao", 2013, 110667},
    {"economicsdb", 2013, 97512},
    {"uvao", 2006, 25000},
    {"vao", 2006, 25000},
    {"vaoOtl", 2006, 25000},
    {"ugzao", 2006, 54000},
    {"vao", 2007, 53400},
    {"ugzao", 2007, 54000},
    {"uvao", 2007, 54500},
    {"ugzao", 2008, 76800},
    {"vao", 2008, 64605},
    {"vaoOtl", 2008, 64605},
    {"uvao", 2008, 67350},
    {"vao", 2009, 72900},
    {"vaoOtl", 2009, 72900},
    {"ugzao", 2009, 87700},
    {"uvao", 2009, 74760},
    {"uao", 2009, 76530},
    {"vao", 2010, 78560},
    {"vaoOtl", 2010, 78560},
    {"ugzao", 2010, 97220},
    {"uvao", 2010, 83355},
    {"uao", 2010, 76715},
    {"uao", 2011, 84020},
    {"vao", 2011, 83265},
    {"ugzao", 2011, 100818},
    {"uvao", 2011, 88260},
    {"vao", 2012, 90510},
    {"ugzao", 2012, 108505},
    {"uao", 2012, 89469},
    {"uvao", 2012, 91697},
    {"svao_ECO", 2012, 93451},
    {"economicsdb", 2012, 92833},
    {" ", 0, 0} // Last element
};

static std::string CurDBName = "";

class TDB_NMrpr {
public:
    // выборка имени текущей БД из самой базы данных
    static std::string GetCurDBNameFromDB();

    // установка имени текущей БД
    static void SetCurDBName(std::string aCurDBName);

    // чтение имени текущей БД
    static std::string GetCurDBName();

    // перевод pokaz_id в N МП
    static std::string PokazidToNMP(int pokaz_id, int God, int& NMP);

    // перевод N МП в pokaz_id
    static int NMPToPokazid(int NMP, int God);
};

#endif
