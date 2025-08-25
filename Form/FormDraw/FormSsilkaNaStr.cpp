#include "FormSsilkaNaStr.h"
#include "dmGlobalVars.h"  

void TFormSsilkaNaStr::CreateSL()
{
    if (GlobalVars->slSsilkaNaStr == nullptr){
        GlobalVars->slSsilkaNaStr = std::make_unique<TStringList>();
    }



    //GlobalVars->slSsilkaNaStr->Sorted = true;
}

void TFormSsilkaNaStr::FreeSL()
{
    if (GlobalVars->slSsilkaNaStr != nullptr)
    {
        GlobalVars->slSsilkaNaStr = nullptr;
    }
}

void TFormSsilkaNaStr::AddSsilkaNaStr(string aSsilkaNaStr, int aRow)
{
    TracerManager::Instance().StartFunctionTrace("TFormSsilkaNaStr::AddSsilkaNaStr");
    int Index;
    if (GlobalVars->slSsilkaNaStr->IndexOf(aSsilkaNaStr) != -1)
    {
        throw "Условный номер строки " + aSsilkaNaStr + " для строки таблицы " + IntToStr(aRow) + " уже определен. Дублирование недопустимо";
    }
    GlobalVars->slSsilkaNaStr->AddObject(aSsilkaNaStr, aRow);
    TracerManager::Instance().EndFunctionTrace();
}
//
//bool TFormSsilkaNaStr::findSsilkaNaStr(string aSsilkaNaStr, int &aRow)
//{
//    if (GlobalVars->slSsilkaNaStr == nullptr)
//        return false;
//
//    int Index = GlobalVars->slSsilkaNaStr->IndexOf(aSsilkaNaStr);
//    if (Index != -1)
//    {
//        aRow = *std::get<std::shared_ptr<int>>(GlobalVars->slSsilkaNaStr->Objects[Index]);
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
bool TFormSsilkaNaStr::findSsilkaNaStr(string aSsilkaNaStr, int &aRow)
{
    if (GlobalVars->slSsilkaNaStr == nullptr)
        return false;

    for (auto it = GlobalVars->slSsilkaNaStr->items.begin(); it != GlobalVars->slSsilkaNaStr->items.end(); ++it) {
        if (it->first == aSsilkaNaStr) {
            aRow = *std::get<std::shared_ptr<int>>(it->second);
            return true;
        }
    }
    return false;
}

