#ifndef F1FLEXCEL_CPPPROJECT_SORTKEYS_H
#define F1FLEXCEL_CPPPROJECT_SORTKEYS_H

#include <cstdint>
#include <map>
#include <string>

namespace F1DevExpress::unF1FlexCel {
    using namespace std;

    /**
     * Тип SortKeys имитирует добавление значений в OleVariant.
     * <br/>Например, создаем объект: SortKeys keys = SortKeys(); Конструктор без аргументов.
     * <br/>И добавляем значения как в Delphi: keys[1] = 9; keys[2] = 10 и т.д.
     * <br/>Внутри SortKeys ключи хранятся в map'е, поэтому номера ключей значения не имеют. Они могут быть любыми int. Важен порядок.
     * <br/>В каком порядке добавим, в таком и выстроится итоговая строка и в таком порядке будут использоваться ключи для сортировки.
     */
    class SortKeys {
    protected:
        u16string str;
        map<int, int> keyItems;

    public:
        int &operator[](int keyIndex) {
            return keyItems[keyIndex];
        }

    protected:
        u16string intToChar16(const int i) {
            auto s = std::to_string(i);
            return {s.begin(), s.end()};
        }

    public:
        const char16_t *GetKeysString() {
            str = u"";
            for (const auto &[key, value]: keyItems) {
                if (!str.empty())
                    str += u",";
                str += intToChar16(value);
            }
            return str.c_str();
        }
    };
}

#endif //F1FLEXCEL_CPPPROJECT_SORTKEYS_H
