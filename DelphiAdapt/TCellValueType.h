#ifndef F1FLEXCEL_CPPPROJECT_TCELLVALUETYPE_H
#define F1FLEXCEL_CPPPROJECT_TCELLVALUETYPE_H

namespace DelphiAdapt {
    /**
     * Источник: FlexCelVCLNT/Source/Core/__UCellValue.pas
     */
    enum class TCellValueType {
        Empty,
        Number,
        DateTime,
        StringValue,
        Boolean,
        Error,
        Formula
    };
}

#endif //F1FLEXCEL_CPPPROJECT_TCELLVALUETYPE_H
