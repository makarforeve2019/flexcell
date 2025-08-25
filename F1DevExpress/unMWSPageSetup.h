#ifndef UNMWSPAGESETUP_H
#define UNMWSPAGESETUP_H

#include <string>

class TMWSPageSetup {
private:
    bool FAutoPageNumber;
    int FFirstPageNumber;
    double FLeftMargin;
    double FTopMargin;
    double FBottomMargin;
    double FRightMargin;
    double FHeaderMargin;
    double FFooterMargin;
    bool FCenterHoriz;
    bool FCenterVert;
    std::string FHeader;
    std::string FFooter;
    bool FLandscape;
    bool FFitPages;
    int FPrintScale;
    int FPagesWide;
    int FPagesTall;
    int FPaperSize;
    std::wstring FPrintTitles;
    bool FGridLines;
    bool FBlackAndWhite;
    bool FRowHeadings;
    bool FColHeadings;
    bool FLeftToRight;
    std::string FPrintArea;

public:
    // Getter and Setter methods
    bool getAutoPageNumber() const;
    void setAutoPageNumber(bool value);

    int getFirstPageNumber() const;
    void setFirstPageNumber(int value);

    double getLeftMargin() const;
    void setLeftMargin(double value);

    double getTopMargin() const;
    void setTopMargin(double value);

    double getBottomMargin() const;
    void setBottomMargin(double value);

    double getRightMargin() const;
    void setRightMargin(double value);

    double getHeaderMargin() const;
    void setHeaderMargin(double value);

    double getFooterMargin() const;
    void setFooterMargin(double value);

    bool getCenterHoriz() const;
    void setCenterHoriz(bool value);

    bool getCenterVert() const;
    void setCenterVert(bool value);

    std::string getHeader() const;
    void setHeader(const std::string& value);

    std::string getFooter() const;
    void setFooter(const std::string& value);

    bool getLandscape() const;
    void setLandscape(bool value);

    bool getFitPages() const;
    void setFitPages(bool value);

    int getPrintScale() const;
    void setPrintScale(int value);

    int getPagesWide() const;
    void setPagesWide(int value);

    int getPagesTall() const;
    void setPagesTall(int value);

    int getPaperSize() const;
    void setPaperSize(int value);

    std::wstring getPrintTitles() const;
    void setPrintTitles(const std::wstring& value);

    bool getGridLines() const;
    void setGridLines(bool value);

    bool getBlackAndWhite() const;
    void setBlackAndWhite(bool value);

    bool getRowHeadings() const;
    void setRowHeadings(bool value);

    bool getColHeadings() const;
    void setColHeadings(bool value);

    bool getLeftToRight() const;
    void setLeftToRight(bool value);

    std::string getPrintArea() const;
    void setPrintArea(const std::string& value);
};

#endif // UNMWSPAGESETUP_H
