#ifndef F1FLEXCEL_CPPPROJECT_TMWSPAGESETUP_H
#define F1FLEXCEL_CPPPROJECT_TMWSPAGESETUP_H

#include "Property.h"

namespace F1DevExpress::unMWSPageSetup {
    using namespace DelphiAdapt;

    class TMWSPageSetup {
    public:
        bool AutoPageNumber = false;
        int FirstPageNumber = 0;
        double LeftMargin = 0;
        double TopMargin = 0;
        double BottomMargin = 0;
        double RightMargin = 0;
        double HeaderMargin = 0;
        double FooterMargin = 0;
        bool CenterHoriz = false;
        bool CenterVert = false;
        char16_t *Header{};
        char16_t *Footer{};
        bool Landscape = false;
        bool FitPages = false;
        int PrintScale = 0;
        int PagesWide = 0;
        int PagesTall = 0;
        int PaperSize = 0;
        char16_t *PrintTitles{};
        bool GridLines = false;
        bool BlackAndWhite = false;
        bool RowHeadings = false;
        bool ColHeadings = false;
        bool LeftToRight = false;
        char16_t *PrintArea{};

    public:
        TMWSPageSetup() = default;
    };
}

#endif //F1FLEXCEL_CPPPROJECT_TMWSPAGESETUP_H
