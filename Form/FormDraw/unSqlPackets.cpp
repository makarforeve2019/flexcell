#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "FormDopInp.h"
#include "DB_CellValue.h"
#include "DB_BPoluc.h"
#include "DB_OKDP.h"
#include "Form1.h"
#include "DB_Pokaztl.h"
#include "Form2Plus.h"
#include "unIDsClassFieldCell.h"
#include "DB_AccessAlbum.h"
#include "DB_bkMetaDef.h"
#include "DB_Enum.h"
#include "unDBType.h"
#include "DB_Plan.h"
#include "DB_CNP_value.h"
#include "unSqlPackets.h"
#include "DB_SystSettings.h"
#include "Base91Converter.h"
#include <cctype>
#include <regex>


void TSel::BeginAdd() {
    sp->SQL->Clear();
    sp->params.clear();
    CurrNumb = 0;
    Orders = "";
    Objects = "";
    Cells1 = "";
    Cells2 = "";
    Plans = "";
    VidOtcs = "";
    Dates1 = "";
    Dates2 = "";
    pokaz_types = "";
}


void TSel::AddCell(int Order, PCellValueAtr vd) {
    string s; // длина 4
    string s2;
    string ws;
    string sd; // длина 10
    int nA = 'A';

    Order -= NBlock;
    //s = IntToHex(Order, 1) + "    ";
    s = Base91Converter::IntToBase91String(Order, 1) + "    ";
//    std::cout << s << endl;
    Orders += s.substr(0, 4);

    sd = IntToStr(vd->ObjectId) + "          ";
    Objects += sd.substr(0, 8);

    sd = IntToStr(vd->CellId) + "          ";
    Cells1 += sd.substr(0, 5);
    std::string ws2 = sd.substr(0, 5);
    Cells2 += sd.substr(5, 5);

//    std::regex Pattern("[0-9]+$");
//    string pat = "[0-9]+$";
//    std::regex Pattern{pat};
    for (char ch: ws) {
        if ((ch < '0' || ch > '9') || ch == ':') {
            bool res = false;
        }
    }

    s = IntToStr(vd->PlanId) + "    ";
    Plans += s.substr(0, 4);

    ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9)
        ws = Chr(nA + vd->VidOtch - 10);
    VidOtcs += ws.substr(0, 1);

    ws = IntToStr(vd->pokaz_type);
    if (vd->pokaz_type > 9)
        ws = Chr(nA + vd->pokaz_type - 10);
    if (vd->pokaz_type == 11)
        ws = IntToStr(1);//{как число, но это будет objectId}
    else if (vd->pokaz_type == 12) // {Строка-список из}
        ws = IntToStr(2); // как строка}

    pokaz_types += ws.substr(0, 1);
    CurrNumb++;
}

void TSel::Execute() {
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add("DECLARE @i integer,");
            sp->SQL->Add("  @Order char(4),");
            sp->SQL->Add("  @Object char(8),");
            sp->SQL->Add("  @Cell char(10),");
            sp->SQL->Add("  @Plan char(4),");
            sp->SQL->Add("  @VidOtc varchar(2),");
            sp->SQL->Add("  @DateC char(10),");
            sp->SQL->Add("  @Date datetime,");
            sp->SQL->Add("  @pokaz_type varchar(2),");
            sp->SQL->Add("  @Value float,");
            sp->SQL->Add("  @isFound tinyint,");
            sp->SQL->Add("  @StrValue varchar(5000),");
            sp->SQL->Add("  @valueEnum varchar(6000),");
            sp->SQL->Add("  @ti tinyint,");
            sp->SQL->Add("  @Note varchar(1000);");
            break;
        }
    }

}

// формирует параметр - список plan_id из описания альбома
std::string TSel::getListPlanIds(int album_id, int &CountPlanIds, int accessalbum_idSelBlockSubj) {
    const std::string msg = "Для операции суммирования оказалось, что список субъектов в альбоме пустой";
    std::string PlanIds = "";
    CountPlanIds = 0;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (accessalbum_idSelBlockSubj == 0) {
        TDB_AccessAlbum::GetAccessAlbum(q, album_id);
    } else {
        TDB_AccessAlbum::GetAccessAlbumById(q, accessalbum_idSelBlockSubj);
    }

    if (q->EoF()) {
        throw std::runtime_error(msg);
    }

    // проверяем, может быть ссылка на другой альбом, т.е. plan_id < 0
    if (q->FieldByName("plan_id")->AsInteger() < 0) {
        album_id = -q->FieldByName("plan_id")->AsInteger();
        TDB_AccessAlbum::GetAccessAlbum(q, album_id);
        if (q->EoF()) {
            throw std::runtime_error(msg);
        }
    }

    while (!q->EoF()) {
        if (PlanIds.empty()) {
            PlanIds = q->FieldByName("plan_id")->AsString();
        } else {
            PlanIds += "," + q->FieldByName("plan_id")->AsString();
        }
        CountPlanIds++;
        q->Next();
    }

    return PlanIds;
}

//**********************************************//
//          TSelCells = class (TSel)
//**********************************************//


void TSelCells::BeginAdd() {
    TSel::BeginAdd();
}

void TSelCells::AddCell(int Order, PCellValueAtr vd) {
    TSel::AddCell(Order, vd);
    string sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);
}

void TSelCells::Execute() {
    TSel::Execute();
    switch (GetDBType()) {
        case TDBType::Postgre: {
            sp->SQL->Add(
                    "SELECT ordern1 as ordern, value1 as value, str_value1 as str_value, isFound1 as isFound, valueEnumname1 as valueEnumname");
            sp->SQL->Add(
                    "FROM public.sel_cells_proc(:CurrNumb, :Orders, :Objects, :Cells1, :Cells2, :Plans, :VidOtcs, :Dates1, :Dates2, :pokaz_types);");
            break;
        }
    }
    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Objects")->AsString() = Objects;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("Plans")->AsString() = Plans;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("pokaz_types")->AsString() = pokaz_types;


}

void TSelCells::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value, OrdS, stmnt;
    double ValueD;
    int OrdN, Id_Znac;
    PCellValueAtr vd, vd1;
    TFormatStyle FormatStyle;
    // цикл выборки записей
    sp->First();
    int ind = 0;

    while (!sp->EoF()) {
        ind = ind + 1;
//        std::cout << ind << endl;
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS; // в начало добавляется доллар - означает что строка состоит из шестнадцатеричных цифр 
        //MAK  OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN = OrdN + NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        Value = sp->FieldByName("str_value")->AsString();
        ValueD = sp->FieldByName("value")->AsFloat();

        if (p->work_with == 2) { // Объектная форма
            if (ValueD != 0)
                Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));

            // переписал с учетом того, значение найдено или нет
            if (sp->FieldByName("isFound")->AsInteger() == 1)
                TForm1::SetCellValue_Obnovl(p, FormLogicF1, Value, vd->DataType, vd->Row, vd->Col);
            else if (sp->FieldByName("isFound")->AsInteger() == 2) {
                TForm1::SetCellValue_Obnovl(p, FormLogicF1, Value, vd->DataType, vd->Row, vd->Col);
                FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                FormLogicF1->f1Forms->Protection_Set(true, true);
                //mak???  SystFuncsFlex::SetRangeBlock(FormLogicF1->f1Forms, vd->Row, vd->Col, vd->Row, vd->Col);
                vd->is_block = true;
            } else
                // в случае, если не найдено значение посылаем отрицательный тип значения              
                TForm1::SetCellValue_Obnovl(p, FormLogicF1, Value, -vd->DataType, vd->Row, vd->Col);
        } else {
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);
            PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
            switch (AtrRow->CurRowVid) {
                case 2:
                case 3:
                case 4:
                case 5:
                case 13:
                    Value = "";
                    ValueD = 0;
                    stmnt = p->ListStmnt[vd->Col].Stmnt;
                    if (stmnt == "ЗНАЧЕНИЕСВ") {// {or (stmnt = 'ЗНАЧЕНИЕНВ') был заменен на ИСТЗНАЧВВОДПК}
                        switch (vd->pokaz_type) {
                            case 1: // число
                                ValueD = sp->FieldByName("value")->AsFloat();
                                if (FormatStyle.Align_vert == 0)
                                    FormatStyle.Align_vert = 2;
                                if (FormatStyle.align_gor == 0)
                                    FormatStyle.align_gor = 3;
                                break;
                            case 2:
                            case 12: // строка, строка-список из
                                Value = sp->FieldByName("str_value")->AsString();
                                if (Value.length() > 4950)
                                    Value = TDB_CellValue::GetStrValue(vd->ObjectId, vd->CellId, vd->PlanId,
                                                                       vd->VidOtch, vd->ValueDate);
                                // Бывают строки длиной, больше 5000. Но хранимая процедура возвращает только 5000
                                // Поэтому проверяем Length(Value) > 4950 и читаем напрямую из БД всю строку
                                break;
                            case 3: // перечисление
                                Value = sp->FieldByName("valueEnumname")->AsString();
                                break;
                            case 4: // из множества
                                vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row, vd->Col);
                                switch (vd->EnumId) {
                                    case 1:
                                        vd1->BPId = sp->FieldByName("value")->AsInteger();
                                        Value = TDB_BPoluc::GetNameBP(vd1->BPId);
                                        p->nColForBP = vd->Col;
                                        break;
                                    case 2:
                                        vd1->BPId = sp->FieldByName("value")->AsInteger();
                                        //mak ??? Value = TDB_VidProdukcii::GetVidProdKodName(vd1->BPId);
                                        break;
                                    case 3:
                                        vd1->BPId = sp->FieldByName("value")->AsInteger();
                                        Value = TDB_OKDP::GetOKDPCode(vd1->BPId);
                                        break;
                                }
                                break;
                            case 5:
                            case 6: // дата, гиперссылка
                                Value = sp->FieldByName("str_value")->AsString();
                                break;
                            case 7: // связь
                                Id_Znac = sp->FieldByName("value")->AsInteger();
                                vd->BPId = Id_Znac;
                                if (Id_Znac > 0)
                                    Value = TDB_Pokaztl::GetPokazName(Id_Znac);
                                else
                                    Value = "";
                                TForm2Plus::SetXarakterSvjzObjecta(FormLogicF1, vd->Row, vd->EnumId, Id_Znac);
                                break;
                            case 8: // рубрика НТИ
                                // Value = sp->FieldByName("str_value")->AsString() + " - " + TDB_GRNTI.GetGRNTI_name(sp->FieldByName("str_value")->AsString());
                                break;
                            case 9: // КБК, программируемый
                                Value = sp->FieldByName("str_value")->AsString();
                                break;
                            case 10: // документ
                                if (sp->FieldByName("isFound")->AsInteger() == 1)
                                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, true);
                                else
                                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, false);
                                goto LNext;
                        }
                    }
                    if (ValueD != 0)
                        Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));

                    // переписал с учетом того, значение найдено или нет
                    if (sp->FieldByName("isFound")->AsInteger() == 1)
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                             vd->Col, Value, vd->DataType, &FormatStyle);
                    else if (sp->FieldByName("isFound")->AsInteger() == 2) {
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                             vd->Col, Value, vd->DataType, &FormatStyle);
                        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
                        FormLogicF1->f1Forms->Protection_Set(true, true);
                        //mak ??? SystFuncsFlex::SetRangeBlock(FormLogicF1->f1Forms, vd->Row, vd->Col, vd->Row, vd->Col);
                        vd->is_block = true;
                    } else
                        // в случае, если не найдено значение посылаем отрицательный тип значения
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                             vd->Col, Value, -vd->DataType, &FormatStyle);
                    break;
            }
        }
        LNext:
        sp->Next();
    }
    // End of цикл выборки записей
}

//**********************************************//
//        TSelCellsUslPost = class (TSel)
//**********************************************//

void TSelCellsUslPost::BeginAdd() {
    TSel::BeginAdd();
}

void TSelCellsUslPost::AddCell(int Order, PCellValueAtr vd) {
    TSel::AddCell(Order, vd);
}

void TSelCellsUslPost::Execute() {
    TSel::Execute();
    switch (GetDBType()) {
        /*case TDBType::MSSQL: {
            sp->SQL->Add("DECLARE @valueDate datetime ");
            sp->SQL->Add("create table #D (ordern varchar(20) not NULL, value float NULL, str_value varchar(5000) NULL, value_date datetime NULL, isFound tinyint not NULL, valueEnumname varchar(1000) NULL)" +
            "set @i = 0");
            sp->SQL->Add("while (@i < :CurrNumb) begin " +
                "set @Order = substring (:Orders,@i*4+1,4) " +
                "set @Object = substring (:Objects,@i*8+1,8) " +
                "set @Cell = substring (:Cells1,@i*5+1,5)+substring (:Cells2,@i*5+1,5) " +
                "set @Plan = substring (:Plans,@i*4+1,4) " +
                "set @VidOtc = substring (:VidOtcs,@i+1,1) " +
                //"if @VidOtc > '9' " +
                //"set @VidOtc = cast(ASCII(@VidOtc)-ASCII('A')+10 as varchar(2)) " +
                "if @VidOtc BETWEEN '0' AND '9' begin " +
                    "set @ti = cast(@VidOtc as tinyint) " +
                "end else begin "+
                    "set @ti = ASCII(@VidOtc)-ASCII('A')+10 " +
                "end; "+
                "set @pokaz_type = substring (:pokaz_types,@i+1,1) " +
                "set @StrValue = NULL " +
                "set @isFound = 1 " +
                "set @Note = '' " +
                "select @Value = value, @StrValue = str_value, @valueDate = value_Date(), @Note = note from Cell_value " +
                //"where objectid = @Object and cell_id = @Cell and plan_id = @Plan and vid_otc = @VidOtc " +
                "where objectid = @Object and cell_id = @Cell and plan_id = @Plan and vid_otc = @ti " +
                "order by value_date DESC " +
                "if @@ROWCOUNT = 0 begin " +
                    "set @Value = 0 " +
                    "set @isFound = 0 " +
                "end else begin " +
                    "if CHARINDEX('[2]', @Note) > 0 " +
                        "set @isFound = 2; "+
                "end; "+
                "if @pokaz_type = '3' begin "+
                    "if @isFound = 0 begin "+
                        "SET @valueEnum = ''; "+
                    "end else begin "+
                        "SELECT @valueEnum = enum_name FROM enum WHERE enum_id = CAST(@value as INT) " +
                        "if @@ROWCOUNT = 0 SET @valueEnum = '???'; "+
                    "end; "+
                "end; " +

                "insert into #D values(@Order, @Value, @StrValue, @valueDate(), @isFound, @valueEnum)" +
                "set @i = @i + 1 " +
            "end");
            sp->SQL->Add("select ordern, value, str_value, value_Date(), isFound, valueEnumname from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            break;
        }*/
        case TDBType::Postgre: {
            sp->SQL->Add(
                    "SELECT ordern1 as ordern, value1 as value, str_value1 as str_value, value_date1 as value_date, isFound1 as isFound, valueEnumname1 as valueEnumname ");
            sp->SQL->Add(
                    "FROM public.sel_cellsuslpost_proc(:CurrNumb, :Orders, :Objects, :Cells1, :Cells2, :Plans, :VidOtcs, :pokaz_types);");
            break;
        }
    }
    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString()/*Value*/ = Orders;
    sp->ParamByName("Objects")->AsString()/*Value*/ = Objects;
    sp->ParamByName("Cells1")->AsString()/*Value*/ = Cells1;
    sp->ParamByName("Cells2")->AsString()/*Value*/ = Cells2;
    sp->ParamByName("Plans")->AsString()/*Value*/ = Plans;
    sp->ParamByName("VidOtcs")->AsString()/*Value*/ = VidOtcs;
    sp->ParamByName("pokaz_types")->AsString()/*Value*/ = pokaz_types;
}

void TSelCellsUslPost::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value;
    double ValueD;
    int Id_Znac;
    TDateTime wValueDate;

    std::string OrdS;
    int OrdN;
    PCellValueAtr vd;
    PCellValueAtr vd1;
    TFormatStyle FormatStyle;

    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = '$' + OrdS;
        //OrdN = HexStringToInt(OrdS); // шестнадцатеричные цифры
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);
        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row, vd->Col);
        wValueDate = sp->FieldByName("value_date")->AsDateTime();
        Value = "";
        ValueD = 0;
        switch (vd->pokaz_type) {
            case 1: { // number
                ValueD = sp->FieldByName("value")->AsFloat();
                Value = FloatToStr(PreobrValue(ValueD, vd->DataType,
                                               vd->coef_Pokaz, vd->coef_Forma));
                if (FormatStyle.Align_vert == 0)
                    FormatStyle.Align_vert = 2;
                if (FormatStyle.align_gor == 0)
                    FormatStyle.align_gor = 3; // 29.04.2022
                break;
            }
            case 2:
            case 12: { // string
                Value = sp->FieldByName("str_value")->AsString();
                if (Value.length() > 4950)
                    Value = TDB_CellValue::GetStrValue(vd->ObjectId, vd->CellId,
                                                       vd->PlanId, vd->VidOtch,
                                                       sp->FieldByName("value_date")->AsDateTime());
                // Бывают строки длиной, больше 5000. Но хранимая процедура возвращает только 5000
                // Поэтому проверяем Length(Value) > 4950 и читаем напрямую из БД всю строку
                break;
            }
            case 3: { // enumeration
                Value = sp->FieldByName("valueEnumname")->AsString();
                break;
            }
            case 4: { // from set
                Value = TDB_BPoluc::GetNameBP(sp->FieldByName("value")->AsInteger());
                Id_Znac = sp->FieldByName("value")->AsInteger();
                switch (vd->EnumId) {
                    case 1: {
                        vd1->BPId = Id_Znac;
                        Value = TDB_BPoluc::GetNameBP(vd1->BPId);
                        TFormDopInp::SetAttribByBP(FormLogicF1, vd->Row, vd->Col);
                        p->nColForBP = vd->Col;
                        break;
                    }
                    case 2: {
                        vd1->BPId = Id_Znac;
                        //mak ??? Value = TDB_VidProdukcii::GetVidProdKodName(vd1->BPId);
                        break;
                    }
                    case 3: {
                        vd1->BPId = Id_Znac;
                        Value = TDB_OKDP::GetOKDPCode(vd1->BPId);
                        break;
                    }
                    case 6: {
                        vd1->BPId = Id_Znac;
                        Value = TDB_Plan::GetPlanName(vd1->BPId);
                        break;
                    }
                }
                break;
            }
            case 5:
            case 6: // Date(), hyperlink
                Value = sp->FieldByName("str_value")->AsString();
                break;
            case 7: { // link
                Id_Znac = sp->FieldByName("value")->AsInteger();
                vd->BPId = Id_Znac;
                if (Id_Znac > 0)
                    Value = TDB_Pokaztl::GetPokazName(Id_Znac);
                else
                    Value = "";
                TForm2Plus::SetXarakterSvjzObjecta(FormLogicF1, vd->Row, vd->EnumId, Id_Znac);
                break;
            }
            case 10: { // document
                if (sp->FieldByName("isFound")->AsInteger() == 1)
                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, true);
                else
                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, false);
                goto LNext;
            }
            case 11: { // case 11
                Id_Znac = sp->FieldByName("value")->AsInteger();
                vd->BPId = Id_Znac;
                if (Id_Znac > 0)
                    Value = TDB_Pokaztl::GetPokazName(Id_Znac);
                else
                    Value = "";
                // вычислить Характеристики связанного объекта и вставить необходимые в форму
                TForm2Plus::SetXarakterSvjzObjecta(FormLogicF1, vd->Row,
                                                   vd->EnumId,//{ id связанного Класса } ,
                                                   Id_Znac);// { id связанного объекта } );
                break;
            }
        }

        if (Value != "" && sp->FieldByName("isFound")->AsInteger() == 1)
            vd1->ValueDate = wValueDate; // здесь надо присваивать через vd1
        // переписал с учетом того, значение найдено или нет
        if (sp->FieldByName("isFound")->AsInteger() == 1)
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                                 Value, vd->DataType, &FormatStyle);
        else if (sp->FieldByName("isFound")->AsInteger() == 2) {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                                 Value, vd->DataType, &FormatStyle);
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            //mak ????SystFuncsFlex::SetRangeBlock(FormLogicF1->f1Forms, vd->Row, vd->Col, vd->Row, vd->Col);
            vd->is_block = true;
        } else
            // в случае, если не найдено значение посылаем отрицательный тип значения
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                                 Value, -vd->DataType, &FormatStyle);

        LNext:
        sp->Next();
    }
}

//**********************************************//
//        TSelCellsSum = class (TSel)
//**********************************************//



void TSelCellsSum::BeginAdd() {
    TSel::BeginAdd();
    ist_fin_ids = "";
    spaces = "";
    PokazIds = "";
    LinkPokazIds = "";
    Cells = "";
    modes = "";
    statusdo_cell_id1 = "";
    statusdo_cell_id2 = "";
    enum_id = "";
    enum_id2 = "";
}

void TSelCellsSum::AddCell(unsigned char mode, int Order, PCellValueAtr vd) {
    std::string s;
    std::string ws;
    std::string sd;
    int nA;
    int class_id=0;
    int field_Finansir_id;
    int field_StatusDO;
    int LinkPokazId =0;
    int Field_Finansir_cellid=0;
    int StatusDO_cell_id = 0;
    int enum_idStatus, enumGrad_id, enumGrad_id2;

    enumGrad_id = 0;
    enumGrad_id2 = 0;

    switch (mode) {
        case 0:
        case 1: {
            if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования",
                                                     Date(), true, field_Finansir_id))
                return;
            Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                       vd->ist_fin_id);
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Мероприятие",
                                                     Date(), true, LinkPokazId))
                return;
            break;
        }
        case 2: {
            if (TDB_SystSettings::GetAppSpace() == "Контроль ГБУ") {
                LDenObj:;
                spaces += '1';
                if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_id)) return;
                if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования",
                                                         Date(), true, field_Finansir_id))
                    return;
                Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                           vd->ist_fin_id);
            } else {
                if (TIDsClassFieldCell::getClassId("Денежное обязательство", 1, false, class_id) && vd->VidOtch == 2) {
                    goto LDenObj;
                }
                spaces += '0';
                if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
                if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования",
                                                         Date(), true, field_Finansir_id))
                    return;
                Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                           vd->ist_fin_id);
            }
            break;
        }
        case 3: {
            if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования",
                                                     Date(), true, field_Finansir_id))
                return;
            Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                       vd->ist_fin_id);
            break;
        }
        case 4: {
            if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_id)) return;
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования",
                                                     Date(), true, field_Finansir_id))
                return;
            Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                       vd->ist_fin_id);
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Статус ДО", Date(), true, field_StatusDO)) return;
            StatusDO_cell_id = TIDsClassFieldCell::getPokazCellId(field_StatusDO,
                                                                  TDB_Pokaztl::getPeriodicn(field_StatusDO), 0);
            enum_idStatus = TDB_Pokaztl::GetEnumId(field_StatusDO);
            enumGrad_id = TDB_Enum::GetEnumGradId(enum_idStatus, "Заявка на закупку Одобрена");
            enumGrad_id2 = TDB_Enum::GetEnumGradId(enum_idStatus, "Заявка на закупку");
            break;
        }
        case 5: {
            spaces += '1';
            if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_id)) return;
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем кредиторской задолженности",
                                                     Date(), true, field_Finansir_id))
                return;
            Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                       vd->ist_fin_id);
            mode = 2;
            break;
        }
    }

    modes += IntToStr(mode);
    Order -= NBlock;
    nA = 'A';
    //s = IntToHex(Order, 1) + "    ";
    s = Base91Converter::IntToBase91String(Order, 1) + "    ";
    Orders += s.substr(0, 4);
    sd = IntToStr(vd->PokazId) + "          ";
    PokazIds += sd.substr(0, 8);
    sd = IntToStr(LinkPokazId) + "          ";
    LinkPokazIds += sd.substr(0, 8);
    sd = IntToStr(Field_Finansir_cellid) + "          ";
    Cells1 += sd.substr(0, 5);
    Cells2 += sd.substr(5, 5);
    s = IntToStr(vd->PlanId) + "    ";
    Plans += s.substr(0, 4);
    ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9) {
        ws = Chr(nA + vd->VidOtch - 10);
    }
    VidOtcs += ws.substr(0, 1);
    sd = IntToStr(vd->ist_fin_id) + "          ";
    ist_fin_ids += sd.substr(0, 8);

    sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);
    CurrNumb++;
    objekt_class_id = class_id;

    sd = IntToStr(StatusDO_cell_id) + "          ";
    statusdo_cell_id1 += sd.substr(0, 5);
    statusdo_cell_id2 += sd.substr(5, 5);

    s = IntToStr(enumGrad_id) + "    ";
    enum_id += s.substr(0, 4);

    s = IntToStr(enumGrad_id2) + "    ";
    enum_id2 += s.substr(0, 4);
}

void TSelCellsSum::Execute() {
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            std::string SQLString = "DECLARE @PokazId char(8),"
                                    "@LinkPokazId char(8),"
                                    "@istfin char(8),"
                                    "@space char(1),"
                                    "@L int,"
                                    "@mode char(1),"
                                    "@objekt_class_id int, "
                                    "@enum_id char(4), "
                                    "@enum_id2 char(4), "
                                    "@pokaz_bk_code char(20), "
                                    "@statusdo_cell_id char(20) ";
            SQLString += "create table #D (ordern varchar(20) not NULL, value float NULL) ";
            SQLString += "set @i = 0 ";
            SQLString += "set @objekt_class_id = " + std::to_string(objekt_class_id) + ";";
            sp->SQL->Add(SQLString);
            break;
        }
    }

    switch (GetDBType()) {
        /* case TDBType::MSSQL: {
             std::string SQLString = "while (@i < " + std::to_string(CurrNumb) + ") begin "
                 "set @Order = substring (:Orders,@i*4+1,4) "
                 "set @PokazId = substring (:PokazIds,@i*8+1,8) "
                 "set @LinkPokazId = substring (:LinkPokazIds,@i*8+1,8) "
                 "set @Cell = substring (:Cells1,@i*5+1,5)+substring (:Cells2,@i*5+1,5) "
                 "set @Plan = substring (:Plans,@i*4+1,4) "
                 "set @space = substring (:spaces,@i+1,1) "
                 "set @mode = substring (:modes,@i+1,1) "
                 "set @VidOtc = substring (:VidOtcs,@i+1,1) "
                 "if @VidOtc BETWEEN '0' AND '9' begin "
                 "set @ti = cast(@VidOtc as tinyint) "
                 "end else begin "
                 "set @ti = ASCII(@VidOtc)-ASCII('A')+10 "
                 "end "
                 "set @DateC = substring (:Dates1,@i*5+1,5)+substring (:Dates2,@i*5+1,5) "
                 "set @Date = CONVERT (datetime, @DateC, 121) "
                 "set @enum_id = substring (:enum_id,@i*4+1,4) "
                 "set @enum_id2 = substring (:enum_id2,@i*4+1,4) "
                 "set @statusdo_cell_id = substring (:statusdo_cell_id1,@i*5+1,5)+substring (:statusdo_cell_id2,@i*5+1,5) "
                 "set @istfin = substring (:istfins,@i*8+1,8)";
             SQLString += "IF @mode = 0 BEGIN "
                 "select @Value = SUM(value) FROM LinksObjects, Cell_value, KeyOfObjects "
                 "where LinksObjects.plan_id = @Plan and LinksObjects.pokaz_id = @PokazId and LinksObjects.link_pokaz_id = @LinkPokazId "
                 "and KeyOfObjects.objectid = LinksObjects.objectid and Cell_value.objectid = LinksObjects.objectid "
                 "and Cell_value.cell_id = @Cell and Cell_value.plan_id = @Plan and vid_otc = @ti and Cell_value.value_date = @Date "
                 "END ";
             SQLString += "IF @mode = 1 BEGIN "
                 "select @Value = SUM(value) FROM LinksObjects, Cell_value "
                 "where LinksObjects.plan_id = @Plan and LinksObjects.pokaz_id = @PokazId and LinksObjects.link_pokaz_id = @LinkPokazId "
                 " and Cell_value.objectid = LinksObjects.objectid "
                 "and Cell_value.cell_id = @Cell and Cell_value.plan_id = @Plan and vid_otc = @ti and Cell_value.value_date = @Date "
                 "END ";
             SQLString += "IF @mode = 2 BEGIN "
                 "if @space = '1' begin "
                 "SELECT @L = LEN(LTRIM(RTRIM(pokaz_bk_code))) FROM Pokaztl WHERE pokaz_id = @PokazID "
                 "SELECT @Value = SUM(value) FROM Pokaztl b, Pokaztl d, Cell_value "
                 "WHERE b.pokaz_id = @PokazId AND SUBSTRING(d.pokaz_bk_code, 1, @L) =SUBSTRING(b.pokaz_bk_code, 1, @L) "
                 "AND d.objekt_class_id > 0 "
                 "AND @Date BETWEEN d.pokaz_beg_date AND d.pokaz_end_date "
                 "AND Cell_value.objectid = d.pokaz_id "
                 "AND Cell_value.cell_id = @Cell AND Cell_value.plan_id = @Plan AND Cell_value.vid_otc = @ti "
                 "AND Cell_value.value_date = @Date "
                 "end else begin "
                 "select @Value = SUM(value) FROM Pokaztl b, Pokaztl d, Cell_value "
                 "WHERE b.pokaz_id = @PokazId AND d.pokaz_bk_code = b.pokaz_bk_code "
                 "AND d.objekt_class_id > 0 "
                 "AND @Date BETWEEN d.pokaz_beg_date AND d.pokaz_end_date "
                 "AND Cell_value.objectid = d.pokaz_id "
                 "AND Cell_value.cell_id = @Cell AND Cell_value.plan_id = @Plan AND Cell_value.vid_otc = @ti "
                 "AND Cell_value.value_date = @Date "
                 "end "
                 "END ";
             SQLString += "IF @mode = 3 BEGIN "
                 "SELECT @Value = SUM(value) FROM Pokaztl b, Pokaztl d, Cell_value "
                 "WHERE b.pokaz_id = @PokazId AND d.pokaz_bk_code = b.pokaz_bk_code "
                 "AND d.objekt_class_id > 0 "
                 "AND @Date BETWEEN d.pokaz_beg_date AND d.pokaz_end_date "
                 "AND Cell_value.objectid = d.pokaz_id "
                 "AND Cell_value.cell_id = @Cell AND Cell_value.plan_id = @Plan AND Cell_value.vid_otc = @ti "
                 "AND Cell_value.value_date = @Date "
                 "END ";
             SQLString += "IF @mode = 4 BEGIN "
                 "SELECT @pokaz_bk_code = pokaz_bk_code FROM Pokaztl WHERE pokaz_id = @PokazID "
                 "SELECT @Value = SUM(C.value) FROM Pokaztl P, Cell_value C, Cell_value S "
                 "WHERE C.objectid > 0 AND C.objectid = P.pokaz_id AND P.pokaz_bk_code = @pokaz_bk_code "
                 "AND C.plan_id = @Plan AND C.cell_id = @Cell AND C.vid_otc = 2 "
                 "AND C.value_date = @Date AND S.objectid = C.objectid AND S.plan_id = @Plan "
                 "AND S.cell_id = @statusdo_cell_id AND S.vid_otc = 4 AND ((S.value = @enum_id) OR (S.value = @enum_id2)) "
                 "END ";
             SQLString += "if ((@value is not null) and (@value <> 0))  "
                 "insert into #D values(@Order, @Value) "
                 "set @i = @i + 1 "
                 "end ";
             SQLString += "select ordern, value from #D order by ordern ";
             SQLString += "DROP TABLE #D";
             sp->SQL->Add(SQLString);
             break;
         }*/
        case TDBType::Postgre: {
            sp->SQL->Add("SELECT ordern1 as ordern, value1 as value ");
            sp->SQL->Add(
                    "FROM public.sel_Cellssum_proc(:CurrNumb, :Orders, :PokazIds, :LinkPokazIds, :Cells1, :Cells2, ");
            sp->SQL->Add(":Plans, :VidOtcs, :Dates1, :Dates2, :istfins, :spaces, :modes, :objekt_class_id,");
            sp->SQL->Add(":statusdo_cell_id1, :statusdo_cell_id2, :enum_id, :enum_id2);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("PokazIds")->AsString() = PokazIds;
    sp->ParamByName("LinkPokazIds")->AsString() = LinkPokazIds;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("Plans")->AsString() = Plans;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("istfins")->AsString() = ist_fin_ids;
    sp->ParamByName("spaces")->AsString() = spaces;
    sp->ParamByName("modes")->AsString() = modes;
    sp->ParamByName("objekt_class_id")->AsInteger() = objekt_class_id;
    sp->ParamByName("statusdo_cell_id1")->AsString() = statusdo_cell_id1;
    sp->ParamByName("statusdo_cell_id2")->AsString() = statusdo_cell_id2;
    sp->ParamByName("enum_id")->AsString() = enum_id;
    sp->ParamByName("enum_id2")->AsString() = enum_id2;
}

void TSelCellsSum::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    double ValueD;
    std::string Value;
    std::string OrdS;
    int OrdN = 0;
    PCellValueAtr vd;
    TFormatStyle FormatStyle;

    sp->First();
    Value = "";
    ValueD = 0;
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS; // шестнадцатеричные цифры
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, vd->Col, FormatStyle);
        ValueD = sp->FieldByName("value")->AsFloat();
        if (ValueD != 0)
            Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col, Value,
                             vd->DataType, &FormatStyle);
        sp->Next();
    }
}

//**********************************************//
//          TAgregat = class(TSel)
//**********************************************//


void TAgregat::BeginAdd() {
    TSel::BeginAdd();
    years = "";
    rootpokazids = "";
    istfinids = "";
    kbkids = "";
    dlins = "";
}

void TAgregat::AddCell(int order, TFormLogicF1 *FormLogicF1, PCellValueAtr vd) {
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
    if (AtrRow == nullptr)
        return;

    int kbk_id = vd->EnumId;
    if (kbk_id == 0) {
        throw std::runtime_error(
                "Вычисление агрегата.Для строки=" + std::to_string(vd->Row) + ", столбца=" + std::to_string(vd->Col) +
                ". kbk_id = 0. Неправильно");
        return;
    }

    TDateTime ToDate;
    int Year, Month, Day;
    DecodeDate(vd->ValueDate, Year, Month, Day);
    ToDate = EncodeDate(Year, 12, 30);
    int root_Pokaz_id = vd->OwnerId;

    int nA = Ord('A');

    // Order
    order = order - NBlock;
    //string s = IntToHex(order, 1) + "    ";
    string s = Base91Converter::IntToBase91String(order, 1) + "    ";
    Orders += s.substr(0, 4);

    // Plan_id
    s = IntToStr(vd->PlanId) + "    ";
    Plans += s.substr(0, 4);

    // Year
    std::string sd = IntToStr(Year) + "          ";
    years += sd.substr(0, 8);

    // Cell
    sd = IntToStr(vd->cell_period) + "          ";
    Cells1 += sd.substr(0, 5);
    Cells2 += sd.substr(5, 5);

    // VidOtch
    std::string ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9)
        ws = Chr(nA + vd->VidOtch - 10);
    VidOtcs += ws.substr(0, 1);

    // Data
    sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);

    // Root Pokaz ID
    sd = IntToStr(root_Pokaz_id) + "          ";
    rootpokazids += sd.substr(0, 8);

    // ist fin id
    sd = IntToStr(vd->ist_fin_id) + "          ";
    istfinids += sd.substr(0, 8);

    // KBKid
    sd = IntToStr(kbk_id) + "          ";
    kbkids += sd.substr(0, 8);

    // DLINA
    int vid = TDB_bkMetaDef::GetVidById(kbk_id);
    int dlina = 0;
    switch (vid) {
        case 0:
            dlina = 4;
            break;
        case 1:
            dlina = TDB_bkMetaDef::GetDl_R_CS(Year);
            break;
        case 2:
            dlina = TDB_bkMetaDef::GetDl_R_CS_VR(Year);
            break;
        case 3:
            dlina = TDB_bkMetaDef::GetDl_All(Year);
            break;
    }
    s = IntToStr(dlina) + "    ";
    dlins += s.substr(0, 4);

    CurrNumb += 1;
}

void TAgregat::Execute() {
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add(
                    "DECLARE @God char(8), "
                    "@God2 char(8), "
                    "@rootpokazid char(8),"
                    "@rootpokazid2 char(8),"
                    "@kbkid char(8), "
                    "@Dlina char(4), "
                    "@istfin char(8),"
                    "@codeBK2 varchar(50), "
                    "@Dlina2 char(4), "
                    "@vid int, ");
            sp->SQL->Add("@ToDate datetime, ");
            sp->SQL->Add("@codeBK varchar(50), ");
            sp->SQL->Add("@pokaz_id int,");
            sp->SQL->Add("@plan_id_okrug int");
            sp->SQL->Add(
                    "create table #D (ordern varchar(20) not NULL, value float NULL)");
            sp->SQL->Add(
                    "set @i = 0 set @rootpokazid2 = '''' 	set @codeBK2 = '''' set @Dlina2 = '''' set @God2 = ''''");
            //--SQL->Add('');
            sp->SQL->Add(
                    "create table #List (pokaz_id int not NULL)");
            sp->SQL->Add(
                    "CREATE TABLE #PokaztlTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");
            sp->SQL->Add(
                    " pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code char(20) NULL, is_obrab tinyint NULL)");
            break;
        }
    }

    switch (GetDBType()) {
        /*case TDBType::MSSQL: {
            sp->SQL->Add(
                "while (@i < :CurrNumb) begin " +
                "set @Order = substring (:Orders,@i*4+1,4) " +
                "set @God = substring (:Gods,@i*8+1,8) " +
                "set @kbkid = substring (:kbkids,@i*8+1,8) " +
                "set @rootpokazid = substring (:rootpokazids,@i*8+1,8)" +
                "set @Cell = substring (:Cells1,@i*5+1,5)+substring (:Cells2,@i*5+1,5) " +
                "set @Plan = substring (:Plans,@i*4+1,4) " +
                "set @Dlina = substring (:Dlins,@i*4+1,4) " +
                "set @VidOtc = substring (:VidOtcs,@i+1,1) " +
                //--'if @VidOtc > ''9'' ' +
                //--  'set @VidOtc = cast(ASCII(@VidOtc)-ASCII(''A'')+10 as varchar(2)) ' +
                "if @VidOtc BETWEEN ''0'' AND ''9'' begin " +
                "set @ti = cast(@VidOtc as tinyint) " +
                "end else begin "+
                "set @ti = ASCII(@VidOtc)-ASCII(''A'')+10 " +
                "end; "+
                "set @DateC = substring (:Dates1,@i*5+1,5)+substring (:Dates2,@i*5+1,5) " +
                "set @Date = CONVERT (datetime, @DateC, 121) " +
                "set @istfin = substring (:istfins,@i*8+1,8)");
            //Создание временной таблицы PokaztlTB и выполнение для нее хранимой процедуры CreateTmpPokaztlTB
            sp->SQL->Add("IF @rootpokazid <> @rootpokazid2 OR @God <> @God2 BEGIN");
            sp->SQL->Add("SET @ToDate = convert(datetime,''12/30/''+convert(char(4), @God),101)");
            sp->SQL->Add("TRUNCATE table #PokaztlTB ");
            sp->SQL->Add("EXECUTE dbo.CreateTmpPokaztlTB @rootpokazid, @ToDate ");
            sp->SQL->Add("END");
            sp->SQL->Add("SET @God2 = @God");
            //Определение вида
            sp->SQL->Add("SET @vid = 0");
            sp->SQL->Add("IF (@kbkid >= 1 and @kbkid <= 999999) BEGIN SET @vid = 0 END");
            sp->SQL->Add("IF (@kbkid >= 1000000 and @kbkid <= 1999999) BEGIN SET @vid = 1 END");
            sp->SQL->Add("IF (@kbkid >= 2000000 and @kbkid <= 2999999) BEGIN SET @vid = 2 END");
            sp->SQL->Add("IF (@kbkid >= 3000000 and @kbkid <= 3999999) BEGIN SET @vid = 3 END");
            //Определение codeKBK
            sp->SQL->Add("SELECT @codeBK = codeKBK FROM bkKBK WHERE kbk_id = @kbkid");
            sp->SQL->Add("IF @@ROWCOUNT = 0 SET @codeBK = ''''");
            //Вставка во временную таблицу #LIST pokaz_id из #PokaztlTB где длина pokaz_bk_code совпадает с @dlina и pokaz_bk_code заканчивается на @codeBK
            sp->SQL->Add("IF (@rootpokazid <> @rootpokazid2 OR @dlina2 <> @dlina OR @codeBK2 <> @codeBK) BEGIN TRUNCATE TABLE #LIST " +
                    "INSERT INTO #LIST SELECT pokaz_id FROM #PokaztlTB WHERE LEN(LTRIM(RTRIM(pokaz_bk_code))) = @dlina and RIGHT(LTRIM(RTRIM(pokaz_bk_code)), LEN(@codeBK)) = @codeBK END");
            sp->SQL->Add("SET @dlina2 = @dlina 	SET @codeBK2 = @codeBK");
            //Выполнение хранимой процедуры
            sp->SQL->Add("IF @Plan > 1 BEGIN");
            sp->SQL->Add("   SET @Value = 0");
            sp->SQL->Add("   SELECT @value = SUM(cv.value) FROM #LIST as ls");
            sp->SQL->Add("       INNER JOIN cell as c ON c.pokaz_id = ls.pokaz_id");
            sp->SQL->Add("       INNER JOIN Cell_value as cv ON cv.cell_id = c.cell_id");
            //--SQL->Add("   WHERE c.cell_period = @Cell AND c.ist_fin_id = @istfin AND cv.objectid = 0 AND cv.plan_id = @plan and cv.vid_otc = @VidOtc AND cv.value_date = @Date");
            sp->SQL->Add("   WHERE c.cell_period = @Cell AND c.ist_fin_id = @istfin AND cv.objectid = 0 AND cv.plan_id = @plan and cv.vid_otc = @ti AND cv.value_date = @Date");
            sp->SQL->Add("END ELSE BEGIN");
            sp->SQL->Add("   SELECT @plan_id_okrug = plan_id FROM Plans WHERE plan_owner_id = 0 AND plan_id > 1");
            sp->SQL->Add("   create table #LISTplan (plan_id int not NULL)");
            sp->SQL->Add("   INSERT INTO #LISTplan SELECT plan_id FROM Plans WHERE plan_is_rajon <> 0 AND plan_is_rajon <> 4 AND plan_id <> @plan_id_okrug AND YEAR(@Date) > god_beg AND YEAR(@Date) < god_end");
            sp->SQL->Add("   SET @Value = 0");
            sp->SQL->Add("   SELECT @value = SUM(cv.value) FROM #LIST as ls");
            sp->SQL->Add("       INNER JOIN cell as c ON c.pokaz_id = ls.pokaz_id");
            sp->SQL->Add("       INNER JOIN Cell_value as cv ON cv.cell_id = c.cell_id");
            sp->SQL->Add("		    INNER JOIN #LISTPlan as lp ON lp.plan_id = cv.plan_id");
            //--SQL->Add("   WHERE c.cell_period = @Cell AND c.ist_fin_id = @istfin AND cv.objectid = 0 AND cv.plan_id = @plan and cv.vid_otc = @VidOtc AND cv.value_date = @Date");
            sp->SQL->Add("   WHERE c.cell_period = @Cell AND c.ist_fin_id = @istfin AND cv.objectid = 0 AND cv.plan_id = @plan and cv.vid_otc = @ti AND cv.value_date = @Date");
            sp->SQL->Add("   DROP TABLE #LISTplan");
            sp->SQL->Add("END");
            sp->SQL->Add("SET @rootpokazid2 = @rootpokazid");
            //Конец
            sp->SQL->Add(
                "if ((@value is not null) and (@value <> 0))  " +
                "insert into #D values(@Order, @Value) " +
                "set @i = @i + 1 " +
                "end");
            sp->SQL->Add("select ordern, value from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            sp->SQL->Add("DROP TABLE #List");
            sp->SQL->Add("DROP TABLE #PokaztlTB");
            break;
        }*/
        case TDBType::Postgre: {
            sp->SQL->Add("SELECT ordern1 as ordern, value1 as value ");
            sp->SQL->Add("FROM public.agregat_proc(:CurrNumb, :Orders, :Cells1, :Cells2, ");
            sp->SQL->Add(":Plans, :VidOtcs, :Dates1, :Dates2, :istfins, :rootpokazids, :kbkids, :gods, :dlins);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("Plans")->AsString() = Plans;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("istfins")->AsString() = istfinids;
    sp->ParamByName("rootpokazids")->AsString() = rootpokazids;
    sp->ParamByName("kbkids")->AsString() = kbkids;
    sp->ParamByName("gods")->AsString() = years;
    sp->ParamByName("dlins")->AsString() = dlins;
}

void TAgregat::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    double ValueD = 0;
    std::string Value;
    std::string OrdS;
    int OrdN = 0;
    PCellValueAtr vd;
    TFormatStyle FormatStyle;

    sp->First();
    Value = "";
    ValueD = 0;
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern").AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS; // шестнадцатеричные цифры
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);
        OrdN = OrdN + NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, vd->Col, FormatStyle);
        ValueD = sp->FieldByName("value")->AsFloat();
        if (ValueD != 0) {
            Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
        }
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                             Value, vd->DataType, &FormatStyle);
        sp->Next();
    }
}

//**********************************************//
//          TAgregatSum = class(TSel)
//**********************************************//


void TAgregatSum::BeginAdd() {
    TSel::BeginAdd();
    rootpokazids = "";
    istfinids = "";
    kbkids = "";
    dlins = "";
    spaces = "";
    modes = "";
    nacals = "";
}

void TAgregatSum::AddCell(unsigned char mode, int Order, TFormLogicF1 *FormLogicF1, PCellValueAtr vd) {
    std::string s;
    std::string ws;
    std::string sd;
    int nA;
    int class_id;
    int field_Finansir_id;
    int Field_Finansir_cellid;
    int Year, Month, Day;
    TDateTime ToDate;
    int root_Pokaz_id, kbk_id;
    PAtrRow AtrRow;
    int vid;
    int dlina;
    int nacalo;

    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
    if (AtrRow == nullptr) return;
    // здесь внутри vd менять НЕЛЬЗЯ, т.к. это копия    
    kbk_id = vd->EnumId;
    if (kbk_id == 0) {
        throw std::runtime_error(
                "Вычисление агрегата. Для строки=" + std::to_string(vd->Row) + ", столбца=" + std::to_string(vd->Col) +
                ". kbk_id = 0. Неправильно");
        return;
    }
    DecodeDate(vd->ValueDate, Year, Month, Day);
    ToDate = EncodeDate(Year, 12, 30);
    root_Pokaz_id = vd->OwnerId;
    switch (mode) {
        case 2:
            if (TDB_SystSettings::GetAppSpace() == "Контроль ГБУ") {
                spaces += "1";
                if (!TIDsClassFieldCell::getClassId("Денежное обязательство", 1, true, class_id)) return;
                if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования", Date(), true,
                                                         field_Finansir_id))
                    return;
                Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                           vd->ist_fin_id);
            } else {
                spaces += "0";
                if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
                if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования", Date(), true,
                                                         field_Finansir_id))
                    return;
                Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                           vd->ist_fin_id);
            }
            break;
        case 3:
            if (!TIDsClassFieldCell::getClassId("Договор", 1, true, class_id)) return;
            if (!TIDsClassFieldCell::getClassFieldId(class_id, "Объем финансирования", Date(), true,
                                                     field_Finansir_id))
                return;
            Field_Finansir_cellid = TIDsClassFieldCell::getPokazCellId(field_Finansir_id, vd->cell_period,
                                                                       vd->ist_fin_id);
            break;
    }

    modes += IntToStr(mode);
    //{Order}
    Order = Order - NBlock;
    nA = 'A';
    std::ostringstream oss;
    //s = IntToHex(Order, 1) + "    ";
    s = Base91Converter::IntToBase91String(Order, 1) + "    ";
    Orders += s.substr(0, 4);
    //{Cell}
    sd = IntToStr(Field_Finansir_cellid) + "          ";
    Cells1 += sd.substr(0, 5);
    Cells2 += sd.substr(5, 5);

    s = IntToStr(vd->PlanId) + "    ";
    Plans += s.substr(0, 4);
    //{VidOtc}
    ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9)
        ws = Chr(nA + vd->VidOtch - 10);
    VidOtcs += ws.substr(0, 1);
    // {ist fin id}
    sd = IntToStr(vd->ist_fin_id) + "          ";
    istfinids += sd.substr(0, 8);
    //{Date}
    sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);
    //{Root Pokaz ID}
    sd = IntToStr(root_Pokaz_id) + "          ";
    rootpokazids += sd.substr(0, 8);
    // {KBKid}
    sd = IntToStr(kbk_id) + "          ";
    kbkids += sd.substr(0, 8);

    vid = TDB_bkMetaDef::GetVidById(kbk_id);
    TDB_bkMetaDef::GetVidInfo(vid, nacalo, dlina, Year);
    s = IntToStr(dlina) + "    ";
    dlins += s.substr(0, 4);

    s = IntToStr(nacalo) + "    ";
    nacals += s.substr(0, 4);

    CurrNumb++;

    objekt_class_id = class_id;
}

void TAgregatSum::Execute() {
    TSel::Execute();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add("DECLARE @rootpokazid char(8),"
                         "@kbkid char(8), "
                         "@Dlina char(4), "
                         "@nacalo char(4), "
                         "@codeBK varchar(50), "
                         "@ROOTcodeBK varchar(50), "
                         "@objekt_class_id int");
            sp->SQL->Add("create table #D (ordern varchar(20) not NULL, value float NULL) ");
            sp->SQL->Add("set @i = 0");
            sp->SQL->Add("set @objekt_class_id = " + std::to_string(objekt_class_id));
            sp->SQL->Add("while (@i < :CurrNumb) begin ");
            sp->SQL->Add("set @Order = substring (:Orders,@i*4+1,4) ");
            sp->SQL->Add("set @kbkid = substring (:kbkids,@i*8+1,8) ");
            sp->SQL->Add("set @rootpokazid = substring (:rootpokazids,@i*8+1,8) ");
            sp->SQL->Add("set @Cell = substring (:Cells1,@i*5+1,5)+substring (:Cells2,@i*5+1,5) ");
            sp->SQL->Add("set @Plan = substring (:Plans,@i*4+1,4) ");
            sp->SQL->Add("set @Dlina = substring (:Dlins,@i*4+1,4) ");
            sp->SQL->Add("set @nacalo = substring (:nacals,@i*4+1,4) ");
            sp->SQL->Add("set @VidOtc = substring (:VidOtcs,@i+1,1) ");
            sp->SQL->Add("if @VidOtc BETWEEN '0' AND '9' begin ");
            sp->SQL->Add("set @ti = cast(@VidOtc as tinyint) ");
            sp->SQL->Add("end else begin ");
            sp->SQL->Add("set @ti = ASCII(@VidOtc)-ASCII('A')+10 ");
            sp->SQL->Add("end; ");
            sp->SQL->Add("set @DateC = substring (:Dates1,@i*5+1,5)+substring (:Dates2,@i*5+1,5) ");
            sp->SQL->Add("set @Date = CONVERT(datetime, @DateC, 121)");

            sp->SQL->Add("SELECT @codeBK = LTRIM(RTRIM(codeKBK)) FROM bkKBK WHERE kbk_id = @kbkid");
            sp->SQL->Add("IF @@ROWCOUNT = 0 SET @codeBK = ''");
            sp->SQL->Add("SELECT @ROOTcodeBK = LTRIM(RTRIM(pokaz_bk_code)) FROM Pokaztl WHERE pokaz_id = @rootpokazid");
            sp->SQL->Add("IF (@@ROWCOUNT = 0 or @ROOTcodeBK is NULL) SET @ROOTcodeBK = ''");
            sp->SQL->Add("SELECT @Value = SUM(value) FROM Pokaztl b, Cell_value "
                         "WHERE b.objekt_class_id = @objekt_class_id "
                         "AND @Date BETWEEN b.pokaz_beg_date AND b.pokaz_end_date "
                         "AND Cell_value.objectid = b.pokaz_id "
                         "AND Cell_value.cell_id = @Cell AND Cell_value.plan_id = @Plan AND Cell_value.vid_otc = @ti "
                         "AND Cell_value.value_date = @Date "
                         "AND substring(RTRIM(LTRIM(b.pokaz_bk_code)), CAST(@nacalo as int), CAST(@dlina as int)) = @codeBK "
                         "AND (LEFT(LTRIM(RTRIM(b.pokaz_bk_code)), LEN(@ROOTcodeBK)) = @ROOTcodeBk or @ROOTcodeBK <> '')");
            sp->SQL->Add("if ((@value is not null) and (@value <> 0))  "
                         "insert into #D values(@Order, @Value) "
                         "set @i = @i + 1 ");
            sp->SQL->Add("select ordern, value from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            break;
        }
        case TDBType::Postgre: {
            sp->SQL->Add("SELECT ordern1 as ordern, value1 as value "
                         "FROM public.agregatsum_proc(:CurrNumb, :Orders, :Cells1, :Cells2, "
                         ":Plans, :VidOtcs, :Dates1, :Dates2, :rootpokazids, :kbkids, :dlins, :nacals, :objekt_class_id);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("Plans")->AsString() = Plans;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("rootpokazids")->AsString() = rootpokazids;
    sp->ParamByName("kbkids")->AsString() = kbkids;
    sp->ParamByName("dlins")->AsString() = dlins;
    sp->ParamByName("nacals")->AsString() = nacals;
    sp->ParamByName("objekt_class_id")->AsInteger() = objekt_class_id;
}

void TAgregatSum::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value;
    double ValueD = 0.0;
    std::string OrdS;
    int OrdN = 0;
    PCellValueAtr vd;
    TFormatStyle FormatStyle;

    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS; // шестнадцатеричные цифры
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, vd->Col, FormatStyle);
        ValueD = sp->FieldByName("value")->AsFloat();
        if (ValueD != 0.0) {
            Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
        }
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                             Value.c_str(), vd->DataType, &FormatStyle);
        sp->Next();
    }
}

//**********************************************//
//          TComNP = class(TSel)
//**********************************************//



void TComNP::BeginAdd() {
    TSel::BeginAdd();
    Spokaz_id = "";
    Scomnp_id = "";
    Scell_period = "";
    Splan_id = "";
    Sgod = "";
    Skvrtl = "";
    Smesjc = "";
    Sden = "";
}

void TComNP::AddCell(int Order, PCellValueAtr vd) {
    // Order
    Order = Order - NBlock;
    int nA = 'A';
    //std::string s = IntToHex(Order, 1) + "    ";
    std::string s = Base91Converter::IntToBase91String(Order, 1) + "    ";
    Orders += s.substr(0, 4);
    // pokaz_id
    std::string sd;
    if (vd->ObjectId == 0)
        sd = IntToStr(vd->PokazId) + "          ";
    else
        sd = IntToStr(vd->ObjectId) + "          ";
    Spokaz_id += sd.substr(0, 8);
    // comnp_id
    sd = IntToStr(vd->comnp_id) + "          ";
    Scomnp_id += sd.substr(0, 8);
    // cell_period
    std::string ws = IntToStr(vd->cell_period);
    Scell_period += ws.substr(0, 1);
    // Plan_id
    s = IntToStr(vd->PlanId) + "    ";
    Splan_id += s.substr(0, 4);
    // VidOtch
    ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9)
        ws = Chr(nA + vd->VidOtch - 10);
    VidOtcs += ws.substr(0, 1);
    // god
    s = IntToStr(vd->god) + "    ";
    Sgod += s.substr(0, 4);
    // kvrtl
    ws = IntToStr(vd->kvrtl);
    Skvrtl += ws.substr(0, 1);
    // mesjc
    s = IntToStr(vd->mesjc) + "    ";
    Smesjc += s.substr(0, 2);
    // den
    s = IntToStr(vd->den) + "    ";
    Sden += s.substr(0, 2);
    // pokaz_types
    ws = IntToStr(vd->pokaz_type);
    if (vd->pokaz_type > 9)
        ws = Chr(nA + vd->pokaz_type - 10);
    pokaz_types += ws.substr(0, 1);

    CurrNumb++;
}

void TComNP::Execute() {
    TSel::Execute();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add("DECLARE @pokaz_id char(8), "
                         "@comnp_id char(8), "
                         "@cell_period char(1), "
                         "@Plan_id char(4), "
                         "@god char(4), "
                         "@kvrtl char(1), "
                         "@mesjc char(2), "
                         "@den char(2), "
                         "@spaces char(1)");
            sp->SQL->Add(
                    "create table #D (ordern varchar(20) not NULL, value float NULL, str_value varchar(5000) NULL, isFound tinyint not NULL, valueEnumname varchar(1000) NULL)");
            sp->SQL->Add("set @i = 0");
            sp->SQL->Add("while (@i < :CurrNumb) begin "
                         "set @Order = substring (:Orders,@i*4+1,4) "
                         "set @pokaz_id = substring (:Spokaz_id,@i*8+1,8) "
                         "set @comnp_id = substring (:Scomnp_id,@i*8+1,8) "
                         "set @cell_period = substring (:Scell_period,@i+1,1) "
                         "set @Plan_id = substring (:SPlan_id,@i*4+1,4) "
                         "set @god = substring (:Sgod,@i*4+1,4) "
                         "set @kvrtl = substring (:Skvrtl,@i+1,1) "
                         "set @mesjc = substring (:Smesjc,@i*2+1,2) "
                         "set @den = substring (:Sden,@i*2+1,2) "
                         "set @pokaz_type = substring (:pokaz_types,@i+1,1) "
                         "set @VidOtc = substring (:VidOtcs,@i+1,1) "
                         "if @VidOtc BETWEEN '0' AND '9' begin "
                         "set @ti = cast(@VidOtc as tinyint) "
                         "end else begin "
                         "set @ti = ASCII(@VidOtc)-ASCII('A')+10 "
                         "end; "
                         "set @StrValue = NULL "
                         "set @isFound = 1 "
                         "set @Note = '' "
                         "SELECT @Value = value, @StrValue = str_value, @Note = note from CNP_value "
                         "WHERE pokaz_id = @pokaz_id AND comnp_id = @comnp_id AND vid_period = @cell_period AND plan_id = @Plan_id "
                         "AND vid_otc = @ti AND god = @god AND kvrtl = @kvrtl AND mesjc = @mesjc AND den = @den "
                         "if @@ROWCOUNT = 0 begin "
                         "set @Value = 0 "
                         "set @isFound = 0 "
                         "end else begin "
                         "if CHARINDEX('[2]', @Note) > 0 "
                         "set @isFound = 2; "
                         "end; "
                         "if @pokaz_type = '3' begin "
                         "if @isFound = 0 begin "
                         "SET @valueEnum = ''; "
                         "end else begin "
                         "SELECT @valueEnum = enum_name FROM enum WHERE enum_id = CAST(@value as INT) "
                         "if @@ROWCOUNT = 0 SET @valueEnum = '???'; "
                         "end; "
                         "end; "
                         "insert into #D values(@Order, @Value, @StrValue, @isFound, @valueEnum) "
                         "set @i = @i + 1 "
                         "end ");
            sp->SQL->Add("select ordern, value, str_value, isFound, valueEnumname from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            break;
        }
        case TDBType::Postgre: {
            sp->SQL->Add(
                    "SELECT ordern1 as ordern, value1 as value, str_value1 as str_value, isfound1 as isFound, valueEnumname1 as valueEnumname ");
            sp->SQL->Add("FROM public.npokaztl_proc(:CurrNumb, :Orders, :Spokaz_id, :Scomnp_id, ");
            sp->SQL->Add(":Scell_period, :SPlan_id, :Sgod, :Skvrtl, :Smesjc, :Sden, :pokaz_types, :VidOtcs);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Spokaz_id")->AsString() = Spokaz_id;
    sp->ParamByName("Scomnp_id")->AsString() = Scomnp_id;
    sp->ParamByName("Scell_period")->AsString() = Scell_period;
    sp->ParamByName("SPlan_id")->AsString() = Splan_id;
    sp->ParamByName("Sgod")->AsString() = Sgod;
    sp->ParamByName("Skvrtl")->AsString() = Skvrtl;
    sp->ParamByName("Smesjc")->AsString() = Smesjc;
    sp->ParamByName("Sden")->AsString() = Sden;
    sp->ParamByName("pokaz_types")->AsString() = pokaz_types;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
}

void TComNP::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value;
    double ValueD;
    int Id_Znac;
    TDateTime wValueDate;

    std::string OrdS;
    int OrdN;
    PCellValueAtr vd;
    PCellValueAtr vd1;
    TFormatStyle FormatStyle;
    std::string wValue;
    std::string wDocumId;

    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS; // шестнадцат цифры
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN = OrdN + NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);
        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row, vd->Col);
        if (vd->Row == 8 && vd->Col == 3) {
            int ol = 0;
        }
        Value = "";
        ValueD = 0;
        switch (vd->pokaz_type) {
            case 1: { // number
                ValueD = sp->FieldByName("value")->AsFloat();
                Value = std::to_string(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
                if (FormatStyle.Align_vert == 0) FormatStyle.Align_vert = 2;
                if (FormatStyle.align_gor == 0) FormatStyle.align_gor = 2;
                break;
            }
            case 2: { // string
                Value = sp->FieldByName("str_value")->AsString();
                if (Value.length() > 4950) {
                    Value = TDB_CellValue::GetStrValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch,
                                                       vd->ValueDate);
                    // Бывают строки длиной, больше 5000. Но хранимая процедура возвращает только 5000
                    // Поэтому проверяем Length(Value) > 4950 и читаем напрямую из БД всю строку
                }
                break;
            }
            case 3: { // enumeration
                Value = sp->FieldByName("valueEnumname")->AsString();
                break;
            }
            case 5: {  // дата
                Value = sp->FieldByName("str_value").AsString();
                break;
            }
            case 6: { // гиперссылка
                Value = sp->FieldByName("str_value").AsString();
                break;
            }
            case 7: { // link
                Id_Znac = sp->FieldByName("value")->AsInteger();
                vd->BPId = Id_Znac;
                if (Id_Znac > 0) {
                    Value = TDB_Pokaztl::GetPokazName(Id_Znac);
                } else {
                    Value = "";
                }
                // вычислить Характеристики связанного объекта и вставить необходимые в форму
                TForm2Plus::SetXarakterSvjzObjecta(FormLogicF1, vd->Row,
                                                   vd->EnumId,// { id связанного Класса } ,
                                                   Id_Znac);//{ id связанного объекта } );
                break;
            }
            case 10: { // document
                wDocumId = "";
                wValue = "";
                TDB_CNP_value::getStringItem(vd1->PokazId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
                                             vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, wValue);
                TDB_CNP_value::getDocumId(vd1->PokazId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
                                          vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, wDocumId);
                if (!wValue.empty() && !wDocumId.empty()) {
                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, true);
                } else {
                    TForm2Plus::SetTextDocument(FormLogicF1, vd->Row, vd->Col, false);
                }
                goto LNext;
            }
            default:
                break;
        }

        if (!Value.empty()) {
            vd1->ValueDate = wValueDate; // здесь надо присваивать через vd1
        }
        // переписал с учетом того, значение найдено или нет
        if (sp->FieldByName("isFound")->AsInteger() == 1) {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col,
                                 vd->Row, vd->Col, Value, vd->pokaz_type, &FormatStyle);
        } else if (sp->FieldByName("isFound")->AsInteger() == 2) {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col,
                                 vd->Row, vd->Col, Value, vd->pokaz_type, &FormatStyle);
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            //mak ??? SystFuncsFlex::SetRangeBlock(FormLogicF1->f1Forms, vd->Row, vd->Col, vd->Row, vd->Col);
            vd->is_block = true;
        } else {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col,
                                 vd->Row, vd->Col, Value, -vd->pokaz_type, &FormatStyle);
        }

        LNext:
        sp->Next();
    }
}

////////////////////////////////////////////////////////////
///   Выборка вычисляемых значений - сумм значений показателей
///   по списку субъектов, заданных в описанияя альбома
////////////////////////////////////////////////////////////

void TSelCellsSumObAlb::BeginAdd() {
    PlanIds = "";
    TSel::BeginAdd();
};

// формирует параметр - список plan_id из описания альбома
void TSelCellsSumObAlb::AddListPlanIds(int album_id, bool isSrednee, int accessalbum_idSelBlockSubj) {
    PlanIds = TSel::getListPlanIds(album_id, CountPlanIds, accessalbum_idSelBlockSubj);
    if (!isSrednee) {
        CountPlanIds = 1;
    }
}

void TSelCellsSumObAlb::AddCell(int Order, PCellValueAtr vd) {
    TSel::AddCell(Order, vd);
    std::string sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);
}

void TSelCellsSumObAlb::Execute() {
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add("DECLARE @i integer,"
                         " @Order char(4),"
                         " @Cell char(10),"
                         " @PlanIds varchar(2000),"
                         " @VidOtc varchar(2),"
                         " @ti tinyint,"
                         " @DateC char(10),"
                         " @Date datetime,"
                         " @Value float,"
                         " @CountPlanIds integer");

            sp->SQL->Add("CREATE TABLE #D (ordern varchar(20) not NULL, value float NULL);");
            sp->SQL->Add("SET @i = 0;");
            sp->SQL->Add("SET @PlanIds = :PlanIds;");
            sp->SQL->Add("SET @CountPlanIds = :CountPlanIds;");

            sp->SQL->Add("WHILE (@i < :CurrNumb) BEGIN");
            sp->SQL->Add("SET @Order = SUBSTRING (:Orders,@i*4+1,4);");
            sp->SQL->Add("SET @Cell = SUBSTRING (:Cells1,@i*5+1,5) + SUBSTRING (:Cells2,@i*5+1,5);");
            sp->SQL->Add("SET @VidOtc = SUBSTRING (:VidOtcs,@i+1,1);");
            sp->SQL->Add("IF @VidOtc BETWEEN '0' AND '9' BEGIN");
            sp->SQL->Add("SET @ti = cast(@VidOtc as tinyint);");
            sp->SQL->Add("END ELSE BEGIN");
            sp->SQL->Add("SET @ti = ASCII(@VidOtc) - ASCII('A') + 10;");
            sp->SQL->Add("END;");
            sp->SQL->Add("SET @DateC = SUBSTRING (:Dates1,@i*5+1,5) + SUBSTRING (:Dates2,@i*5+1,5);");
            sp->SQL->Add("SET @Date = CONVERT(datetime, @DateC, 121);");

            sp->SQL->Add("SELECT @Value = SUM(value) FROM Cell_value");
            sp->SQL->Add(
                    "WHERE objectid = 0 AND plan_id IN (@PlanIds) AND cell_id = @Cell AND vid_otc = @ti AND value_date = @Date;");
            sp->SQL->Add("IF @@ROWCOUNT = 0 BEGIN");
            sp->SQL->Add("SET @Value = 0;");
            sp->SQL->Add("END");

            sp->SQL->Add("SET @Value = @Value / @CountPlanIds;");

            sp->SQL->Add("INSERT INTO #D VALUES(@Order, @Value);");
            sp->SQL->Add("SET @i = @i + 1;");
            sp->SQL->Add("END");

            sp->SQL->Add("SELECT ordern, value FROM #D ORDER BY ordern;");
            sp->SQL->Add("DROP TABLE #D;");
            break;
        }
        case TDBType::Postgre: {
            sp->SQL->Add("SELECT ordern1 as ordern, value1 as value "
                         "FROM public.sel_CellsSumObAlb_proc(:CurrNumb, :Orders, :Cells1, :Cells2, :PlanIds, :VidOtcs, :Dates1, :Dates2, :CountPlanIds);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("PlanIds")->AsString() = PlanIds;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("CountPlanIds")->AsInteger() = CountPlanIds;
}

void TSelCellsSumObAlb::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value;
    double ValueD;
    std::string OrdS;
    int OrdN;
    PCellValueAtr vd;
    PCellValueAtr vd1;
    TFormatStyle FormatStyle;

    // цикл выборки записей
    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS;
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);
        ValueD = sp->FieldByName("value")->AsFloat();
        if (FormatStyle.Align_vert == 0) {
            FormatStyle.Align_vert = 2;
        }
        if (FormatStyle.align_gor == 0) {
            FormatStyle.align_gor = 3;
        }

        Value = "0";
        if (ValueD != 0) {
            Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
        }
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col,
                             Value, vd->DataType, &FormatStyle);
        sp->Next();
    }
    // End of цикл выборки записей
}

////////////////////////////////////////////////////////////
///   Выборка вычисляемых значений - сумм общих натуральных показателей
///   по списку субъектов, заданных в описанияя альбома
////////////////////////////////////////////////////////////



void TComNpSumObAlb::BeginAdd() {
    TSel::BeginAdd();
    Spokaz_id = "";
    Scomnp_id = "";
    Scell_period = "";
    SPlanIds = "";
    Sgod = "";
    Skvrtl = "";
    Smesjc = "";
    Sden = "";
}

void TComNpSumObAlb::AddCell(int Order, PCellValueAtr vd) {
    // Order
    Order = Order - NBlock;
    int nA = Ord('A');
    //std::string s = IntToHex(Order, 1) + "    ";
    std::string s = Base91Converter::IntToBase91String(Order, 1) + "    ";
    Orders += s.substr(0, 4);
    // pokaz_id
    std::string sd;
    if (vd->ObjectId == 0)
        sd = IntToStr(vd->PokazId) + "          ";
    else
        sd = IntToStr(vd->ObjectId) + "          ";
    Spokaz_id += sd.substr(0, 8);
    // comnp_id
    sd = IntToStr(vd->comnp_id) + "          ";
    Scomnp_id += sd.substr(0, 8);
    // cell_period
    std::string ws = IntToStr(vd->cell_period);
    Scell_period += ws.substr(0, 1);
    // VidOtch
    ws = IntToStr(vd->VidOtch);
    if (vd->VidOtch > 9)
        ws = Chr(nA + vd->VidOtch - 10);
    VidOtcs += ws.substr(0, 1);
    // god
    s = IntToStr(vd->god) + "    ";
    Sgod += s.substr(0, 4);
    // kvrtl
    ws = IntToStr(vd->kvrtl);
    Skvrtl += ws.substr(0, 1);
    // mesjc
    s = IntToStr(vd->mesjc) + "    ";
    Smesjc += s.substr(0, 2);
    // den
    s = IntToStr(vd->den) + "    ";
    Sden += s.substr(0, 2);

    CurrNumb += 1;
}

void TComNpSumObAlb::AddListPlanIds(int album_id, bool isSrednee, int accessalbum_idSelBlockSubj) {
    SPlanIds = TSel::getListPlanIds(album_id, CountPlanIds, accessalbum_idSelBlockSubj);
    if (!isSrednee)
        CountPlanIds = 1;
}

void TComNpSumObAlb::Execute() {
    TSel::Execute();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add(
                    "DECLARE @pokaz_id char(8),"
                    "@comnp_id char(8), "
                    "@cell_period char(1),"
                    "@PlanIds varchar(2000),"
                    "@god char(4),"
                    "@kvrtl char(1),"
                    "@mesjc char(2),"
                    "@den char(2),"
                    "@spaces char(1),"
                    "@CountPlanIds integer");
            sp->SQL->Add(
                    "create table #D (ordern varchar(20) not NULL, value float NULL)"
                    "set @i = 0 "
                    "set @PlanIds = :PlanIds "
                    "set @CountPlanIds = :CountPlanIds ");
            break;
        }
    }
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add(
                    "while (@i < :CurrNumb) begin "
                    "set @Order = substring (:Orders,@i*4+1,4) "
                    "set @pokaz_id = substring (:Spokaz_id,@i*8+1,8) "
                    "set @comnp_id = substring (:Scomnp_id,@i*8+1,8)"
                    "set @cell_period = substring (:Scell_period,@i+1,1) "
                    "set @god = substring (:Sgod,@i*4+1,4) "
                    "set @kvrtl = substring (:Skvrtl,@i+1,1) "
                    "set @mesjc = substring (:Smesjc,@i*2+1,2) "
                    "set @den = substring (:Sden,@i*2+1,2) "
                    "set @VidOtc = substring (:VidOtcs,@i+1,1) "
                    //"if @VidOtc > ''9'' "
                    //"set @VidOtc = cast(ASCII(@VidOtc)-ASCII(''A'')+10 as varchar(2)) ");
                    "if @VidOtc BETWEEN '0' AND '9' begin "
                    "set @ti = cast(@VidOtc as tinyint) "
                    "end else begin "
                    "set @ti = ASCII(@VidOtc)-ASCII('A')+10 "
                    "end; ");
            sp->SQL->Add(
                    "SELECT @Value = SUM(value) from CNP_value "
                    "WHERE pokaz_id = @pokaz_id AND comnp_id = @comnp_id AND vid_period = @cell_period AND plan_id IN (@PlanIds) "
                    //"AND vid_otc = @VidOtc AND god = @god AND kvrtl = @kvrtl AND mesjc = @mesjc AND den = @den "
                    "AND vid_otc = @ti AND god = @god AND kvrtl = @kvrtl AND mesjc = @mesjc AND den = @den "
                    "if @@ROWCOUNT = 0 begin "
                    "set @Value = 0 "
                    "end "
                    "set @Value = @Value / @CountPlanIds "
                    "insert into #D values(@Order, @Value) "
                    "set @i = @i + 1 "
                    "end ");
            sp->SQL->Add(
                    "select ordern, value from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            break;
        }
        case TDBType::Postgre: {

            sp->SQL->Add("SELECT ordern1 as ordern, value1 as value ");
            sp->SQL->Add("FROM public.cnpSumObAlb_proc(:CurrNumb, :Orders, :Spokaz_id, :Scomnp_id, ");
            sp->SQL->Add(":Scell_period, :SPlanIds, :Sgod, :Skvrtl, :Smesjc, :Sden, :VidOtcs, :CountPlanIds);");
            break;
        }
    }
    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Spokaz_id")->AsString() = Spokaz_id;
    sp->ParamByName("Scomnp_id")->AsString() = Scomnp_id;
    sp->ParamByName("Scell_period")->AsString() = Scell_period;
    sp->ParamByName("SPlanIds")->AsString() = SPlanIds;
    sp->ParamByName("Sgod")->AsString() = Sgod;
    sp->ParamByName("Skvrtl")->AsString() = Skvrtl;
    sp->ParamByName("Smesjc")->AsString() = Smesjc;
    sp->ParamByName("Sden")->AsString() = Sden;
    sp->ParamByName("Sden")->AsString() = Sden;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("CountPlanIds")->AsInteger() = CountPlanIds;
}

void TComNpSumObAlb::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    // Your method implementation
    std::string Value;
    double ValueD;
    int Id_Znac;
    TDateTime wValueDate;

    std::string OrdS;
    int OrdN;
    PCellValueAtr vd;
    TFormatStyle FormatStyle;
    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS;
        //OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);
        Value = "";
        ValueD = 0;
        switch (vd->pokaz_type) {
            case 1: {
                ValueD = sp->FieldByName("value").AsFloat();
                Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
                if (FormatStyle.Align_vert == 0) {
                    FormatStyle.Align_vert = 2;
                }
                if (FormatStyle.align_gor == 0) {
                    FormatStyle.align_gor = 2;
                }
                break;
            }
        }
        TForm1::SetCellValue(FormLogicF1, 0, 0, vd->Row, vd->Col, vd->Row, vd->Col,
                             Value, vd->DataType, &FormatStyle);
        LNext:;
        sp->Next();
    }
}

//**********************************************//
//          TSelCellsHistory = class (TSel)
//**********************************************//



// BeginAdd method
void TSelCellsHistory::BeginAdd() {
    TSel::BeginAdd();
}

void TSelCellsHistory::AddCell(int Order, PCellValueAtr vd) {
    TSel::AddCell(Order, vd);
    std::ostringstream oss;
    std::string sd = FormatDateTime("%Y-%m-%d", vd->ValueDate);
    Dates1 += sd.substr(0, 5);
    Dates2 += sd.substr(5, 5);
}

void TSelCellsHistory::Execute() {
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->SQL->Add(
                    "CREATE TABLE #D (ordern varchar(20) not NULL, value float NULL, note varchar(1000) NULL, isFound tinyint not NULL, valueEnumname varchar(1000) NULL) ");
            sp->SQL->Add("set @i = 0 ");
            sp->SQL->Add("WHILE (@i < :CurrNumb) BEGIN ");
            sp->SQL->Add("set @Order = substring (:Orders,@i*4+1,4) ");
            sp->SQL->Add("set @Object = substring (:Objects,@i*8+1,8) ");
            sp->SQL->Add("set @Cell = substring (:Cells1,@i*5+1,5)+substring (:Cells2,@i*5+1,5) ");
            sp->SQL->Add("set @Plan = substring (:Plans,@i*4+1,4) ");
            sp->SQL->Add("set @VidOtc = substring (:VidOtcs,@i+1,1) ");
            sp->SQL->Add("if @VidOtc BETWEEN '0' AND '9' begin ");
            sp->SQL->Add("set @ti = cast(@VidOtc as tinyint) ");
            sp->SQL->Add("end else begin ");
            sp->SQL->Add("set @ti = ASCII(@VidOtc)-ASCII('A')+10 ");
            sp->SQL->Add("end; ");
            sp->SQL->Add("set @pokaz_type = substring (:pokaz_types,@i+1,1) ");
            sp->SQL->Add("set @DateC = substring (:Dates1,@i*5+1,5)+substring (:Dates2,@i*5+1,5) ");
            sp->SQL->Add("set @Date = CONVERT (datetime, @DateC, 121) ");
            sp->SQL->Add("set @StrValue = NULL ");
            sp->SQL->Add("set @isFound = 1 ");
            sp->SQL->Add("set @Note = '' ");
            sp->SQL->Add("SELECT @Value = value, @StrValue = str_value, @Note = note from Cell_value ");
            sp->SQL->Add(
                    "WHERE objectid = @Object and cell_id = @Cell and plan_id = @Plan and vid_otc = @ti and value_date = @Date ");
            sp->SQL->Add("if @@ROWCOUNT = 0 begin ");
            sp->SQL->Add("set @Value = 0 ");
            sp->SQL->Add("set @isFound = 0 ");
            sp->SQL->Add("end else begin ");
            sp->SQL->Add("if CHARINDEX('[2]', @Note) > 0 ");
            sp->SQL->Add("set @isFound = 2; ");
            sp->SQL->Add("end; ");
            sp->SQL->Add("if @pokaz_type = '3' begin ");
            sp->SQL->Add("if @isFound = 0 begin ");
            sp->SQL->Add("SET @valueEnum = ''; ");
            sp->SQL->Add("end else begin ");
            sp->SQL->Add("SELECT @valueEnum = enum_name FROM enum WHERE enum_id = CAST(@value as INT) ");
            sp->SQL->Add("if @@ROWCOUNT = 0 SET @valueEnum = '???'; ");
            sp->SQL->Add("end; ");
            sp->SQL->Add("end; ");

            sp->SQL->Add("insert into #D values(@Order, @Value, @Note, @isFound, @valueEnum) ");
            sp->SQL->Add("set @i = @i + 1 ");
            sp->SQL->Add("end ");
            sp->SQL->Add(
                    "select ordern, value, note, isFound, valueEnumname from #D order by ordern ");
            sp->SQL->Add("DROP TABLE #D");
            break;
        }
        case TDBType::Postgre: {
            sp->SQL->Add(
                    "SELECT ordern1 as ordern, isFound1 as isFound, notehistory as note, valueEnumname1 as valueEnumname ");
            sp->SQL->Add(
                    "FROM public.sel_cells_proc_history(:CurrNumb, :Orders, :Objects, :Cells1, :Cells2, :Plans, :VidOtcs, :Dates1, :Dates2, :pokaz_types);");
            break;
        }
    }

    sp->ParamByName("CurrNumb")->AsInteger() = CurrNumb;
    sp->ParamByName("Orders")->AsString() = Orders;
    sp->ParamByName("Objects")->AsString() = Objects;
    sp->ParamByName("Cells1")->AsString() = Cells1;
    sp->ParamByName("Cells2")->AsString() = Cells2;
    sp->ParamByName("Plans")->AsString() = Plans;
    sp->ParamByName("VidOtcs")->AsString() = VidOtcs;
    sp->ParamByName("Dates1")->AsString() = Dates1;
    sp->ParamByName("Dates2")->AsString() = Dates2;
    sp->ParamByName("pokaz_types")->AsString() = pokaz_types;
}

void TSelCellsHistory::GetItems(TFormLogicF1 *FormLogicF1, TwwStringList *p) {
    std::string Value;
    double ValueD;
    std::string OrdS;
    int OrdN, Id_Znac;
    PCellValueAtr vd;
    PCellValueAtr vd1;
    TFormatStyle FormatStyle;
    std::string stmnt;
    std::string wsnote;
    int wsOut;
    int wsIn;
    std::string wsJSONString;
    json patternJ;
    int i;
    TFormatSettings fs;
    TDateTime dtLast;
    std::string ws;
    PAtrRow AtrRow;
    int tocnost;
    // цикл выборки записей
    sp->First();
    while (!sp->EoF()) {
        OrdS = sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS;
        //MAK OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);;
        OrdN += NBlock;
        //MAK vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
        union {
            struct {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);
        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, FormatStyle);

        switch (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid) {
            case 2:
            case 3:
            case 4:
            case 5:
            case 13: {
                Value = "";
                ValueD = 0;
                ws = sp->FieldByName("note")->AsString();
                wsIn = Pos("[{", ws);
                wsOut = Pos("}]", ws) + 1;
                if (wsIn < 0) goto LNext;

                switch (vd->pokaz_type) {
                    case 1: { // число
                        wsJSONString = ws.substr(wsIn, wsOut - wsIn + 1);
                        patternJ = nlohmann::json::parse(wsJSONString);
                        for (i = patternJ.size() - 1; i >= 0; --i) {
                            string wsDate = patternJ[i]["D"].get<std::string>();
                            TDateTime wdDate;
                            wdDate.FromFormattedDateString(wsDate);

                            dtLast = ConvertToDateBgn(wdDate);
                            if (p->selectedDate < dtLast) continue;
                            ValueD = StrToFloat(patternJ[i]["V"].get<std::string>());

                            tocnost = 0;
                            AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
                            if (AtrRow != nullptr) tocnost = AtrRow->tocnost;
                            if (tocnost == 0) tocnost = p->ListStmnt[vd->Col].tocnost;
                            if (tocnost == 0) tocnost = p->tocnost;
                            FormLogicF1->f1Forms->NumberFormat = utf8_to_utf16(
                                    TForm2Plus::CreateNumberFormat(tocnost, p->ListStmnt[vd->Col].CoefPlus1)).c_str();

                            if (FormatStyle.Align_vert == 0) FormatStyle.Align_vert = 2;
                            if (FormatStyle.align_gor == 0) FormatStyle.align_gor = 3;
                            break;
                        }
                        break;
                    }
                    case 2:
                    case 3: { // строка
                        switch (p->modeNumber) {
                            case 2: {
                                //ws = Copy(ws,wsIn+1,wsOut-wsIn+1);
                                ws = ws.substr(wsIn, wsOut - wsIn + 1);
                                ws = AnsiReplaceStr(ws, "[", "");
                                ws = AnsiReplaceStr(ws, "]", "");
                                ws = AnsiReplaceStr(ws, "\"D\"", "Дата");
                                ws = AnsiReplaceStr(ws, "\"V\"", "Значение");
                                ws = AnsiReplaceStr(ws, "\"U\"", "Исполнитель");
                                Value = ws;
                                break;
                            }
                            case 3: {
                                wsJSONString = ws.substr(wsIn, wsOut - wsIn + 1);
                                patternJ = nlohmann::json::parse(wsJSONString);
                                for (i = patternJ.size() - 1; i >= 0; --i) {
//                                    fs.DateSeparator = '.';
//                                    fs.ShortDateFormat = "dd.mm.yyyy";
//                                    fs.TimeSeparator = ":";
//                                    fs.ShortTimeFormat = "hh:nn";
//                                    fs.LongTimeFormat = "hh:nn";
                                    string wsDate = patternJ[i]["D"].get<std::string>();
                                    TDateTime wdDate;
                                    wdDate.FromFormattedDateString(wsDate);

                                    dtLast = ConvertToDateBgn(wdDate);
//                                    dtLast = ConvertToDateBgn(StrToDateTime(patternJ[i]["D"].get<std::string>()));
                                    if (p->selectedDate < dtLast) continue;
                                    Value = patternJ[i].dump();
                                    Value = AnsiReplaceStr(Value, "{", "");
                                    Value = AnsiReplaceStr(Value, "}", "");
                                    Value = AnsiReplaceStr(Value, "\"D\"", "Дата");
                                    Value = AnsiReplaceStr(Value, "\"V\"", "\r\nЗначение");
                                    Value = AnsiReplaceStr(Value, "\"U\"", "\r\nИсполнитель");
                                    break;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }

                if (ValueD != 0) {
                    Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
                }

                // переписал с учетом того, значение найдено или нет
                if (sp->FieldByName("isFound")->AsInteger() == 1) {
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                         vd->Col,
                                         Value, vd->DataType, &FormatStyle);
                } else if (sp->FieldByName("isFound")->AsInteger() == 2) {
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                         vd->Col,
                                         Value, vd->DataType, &FormatStyle);
                    //mak ??? SystFuncsFlex::SetRangeBlock(FormLogicF1->f1Forms, vd->Row, vd->Col, vd->Row, vd->Col);
                    vd->is_block = true;
                } else {
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row,
                                         vd->Col,
                                         Value, -(vd->DataType),
                                         &FormatStyle);  // в случае, если не найдено значение посылаем отрицательный тип значения
                }
                break;
            }
        }
        LNext:;
        sp->Next();
    }
    // End of цикл выборки записей
}
