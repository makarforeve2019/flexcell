#ifndef DB_OKDP_H
#define DB_OKDP_H

#include <string>

#include "DB_Common.h"

class TDB_OKDP {
public:
    static int GetId();
    static int AddItem(int ownerId, const std::string& codeOKDP, const std::string& nameOKDP);
    static void GetAllItemsOwner(TQueryUni* qIn, int ownerId);
    static void GetItemsBySubstr(TQueryUni* qIn, const std::string& substr);
    static void GetItemsByCode(TQueryUni* qIn, const std::string& Code);
    static void GetItemsBySubstrInUzel(TQueryUni* qIn, const std::string& substr, const std::string& inUzel);
    static std::string GetOKDPCode(int okdpId);
    static bool isItemsOwner(int ownerId);
    static void GetAllItems(TQueryUni* qIn);
    static bool GetOKDP(TQueryUni* qIn, int okdpId);
    static std::string GetOKDPCodeName(int okdpId);
    static std::string GetOKDPName(int okdpId);
};

#endif // DB_OKDP_H
