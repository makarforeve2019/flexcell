#include "unMWSPageSetup.h"

// Getter and Setter methods
bool TMWSPageSetup::getAutoPageNumber() const {
    return FAutoPageNumber;
}

void TMWSPageSetup::setAutoPageNumber(bool value) {
    FAutoPageNumber = value;
}

int TMWSPageSetup::getFirstPageNumber() const {
    return FFirstPageNumber;
}

void TMWSPageSetup::setFirstPageNumber(int value) {
    FFirstPageNumber = value;
}

double TMWSPageSetup::getLeftMargin() const {
    return FLeftMargin;
}

void TMWSPageSetup::setLeftMargin(double value) {
    FLeftMargin = value;
}

double TMWSPageSetup::getTopMargin() const {
    return FTopMargin;
}

void TMWSPageSetup::setTopMargin(double value) {
    FTopMargin = value;
}

double TMWSPageSetup::getBottomMargin() const {
    return FBottomMargin;
}

void TMWSPageSetup::setBottomMargin(double value) {
    FBottomMargin = value;
}

double TMWSPageSetup::getRightMargin() const {
    return FRightMargin;
}

void TMWSPageSetup::setRightMargin(double value) {
    FRightMargin = value;
}

double TMWSPageSetup::getHeaderMargin() const {
    return FHeaderMargin;
}

void TMWSPageSetup::setHeaderMargin(double value) {
    FHeaderMargin = value;
}

double TMWSPageSetup::getFooterMargin() const {
    return FFooterMargin;
}

void TMWSPageSetup::setFooterMargin(double value) {
    FFooterMargin = value;
}

bool TMWSPageSetup::getCenterHoriz() const {
    return FCenterHoriz;
}

void TMWSPageSetup::setCenterHoriz(bool value) {
    FCenterHoriz = value;
}

bool TMWSPageSetup::getCenterVert() const {
    return FCenterVert;
}

void TMWSPageSetup::setCenterVert(bool value) {
    FCenterVert = value;
}

std::string TMWSPageSetup::getHeader() const {
    return FHeader;
}

void TMWSPageSetup::setHeader(const std::string& value) {
    FHeader = value;
}

std::string TMWSPageSetup::getFooter() const {
    return FFooter;
}

void TMWSPageSetup::setFooter(const std::string& value) {
    FFooter = value;
}

bool TMWSPageSetup::getLandscape() const {
    return FLandscape;
}

void TMWSPageSetup::setLandscape(bool value) {
    FLandscape = value;
}

bool TMWSPageSetup::getFitPages() const {
    return FFitPages;
}

void TMWSPageSetup::setFitPages(bool value) {
    FFitPages = value;
}

int TMWSPageSetup::getPrintScale() const {
    return FPrintScale;
}

void TMWSPageSetup::setPrintScale(int value) {
    FPrintScale = value;
}

int TMWSPageSetup::getPagesWide() const {
    return FPagesWide;
}

void TMWSPageSetup::setPagesWide(int value) {
    FPagesWide = value;
}

int TMWSPageSetup::getPagesTall() const {
    return FPagesTall;
}

void TMWSPageSetup::setPagesTall(int value) {
    FPagesTall = value;
}

int TMWSPageSetup::getPaperSize() const {
    return FPaperSize;
}

void TMWSPageSetup::setPaperSize(int value) {
    FPaperSize = value;
}

std::wstring TMWSPageSetup::getPrintTitles() const {
    return FPrintTitles;
}

void TMWSPageSetup::setPrintTitles(const std::wstring& value) {
    FPrintTitles = value;
}

bool TMWSPageSetup::getGridLines() const {
    return FGridLines;
}

void TMWSPageSetup::setGridLines(bool value) {
    FGridLines = value;
}

bool TMWSPageSetup::getBlackAndWhite() const {
    return FBlackAndWhite;
}

void TMWSPageSetup::setBlackAndWhite(bool value) {
    FBlackAndWhite = value;
}

bool TMWSPageSetup::getRowHeadings() const {
    return FRowHeadings;
}

void TMWSPageSetup::setRowHeadings(bool value) {
    FRowHeadings = value;
}

bool TMWSPageSetup::getColHeadings() const {
    return FColHeadings;
}

void TMWSPageSetup::setColHeadings(bool value) {
    FColHeadings = value;
}

bool TMWSPageSetup::getLeftToRight() const {
    return FLeftToRight;
}

void TMWSPageSetup::setLeftToRight(bool value) {
    FLeftToRight = value;
}

std::string TMWSPageSetup::getPrintArea() const {
    return FPrintArea;
}

void TMWSPageSetup::setPrintArea(const std::string& value) {
    FPrintArea = value;
}
