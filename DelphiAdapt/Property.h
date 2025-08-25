#ifndef F1FLEXCELASPOSE_PROPERTY_H
#define F1FLEXCELASPOSE_PROPERTY_H

/**
 * Для имитации Property используется перегрузка операторов [] и ().
 * <br/>Перегрузка оператора [] с более чем 1 аргументом доступна только с версии C++23.
 * <br/>Поэтому для Property типа массив с более чем одним измерением используется только перегрузка оператора ().
 * <br/>
 * <br/>Определены следующие классы:
 * <br/>1) Property - для простых Property;
 * <br/>1.1) PropertyRO - Read Only. Только с геттером;
 * <br/>1.2) PropertyWO - Write Only. Только с сеттером;
 * <br/>2) Property_Arr_1D - для Property типа одномерный массив;
 * <br/>2.1) PropertyRO_Arr_1D;
 * <br/>2.2) PropertyWO_Arr_1D;
 * <br/>3) Property_Arr_2D - для Property типа двумерный массив;
 * <br/>3.1) PropertyRO_Arr_2D;
 * <br/>3.2) PropertyWO_Arr_2D.
 * <br/>4) Property_Arr_3D - для Property типа трехмерный массив;
 * <br/>4.1) PropertyRO_Arr_3D;
 * <br/>4.2) PropertyWO_Arr_3D.
 * <br/>
 * <br/>Для определения Property в классе, нужно определить свойство соответствующего класса Property(например, Property_Arr_1D) и тут же инициализировать его.
 * <br/>
 * <br/>Примеры:
 * <br/>Нам нужно Property Age класса Human на запись и на чтение.
 * <br/>Определяем и инициализируем Property в определении класса:
 * <br/>Property<int, Human> Age = Property<int, Human>(this, &Human::GetAge, &Human::SetAge);
 * <br/>Соответственно, методы GetAge и SetAge должны быть определены в классе Human: int GetAge() и void SetAge(int value).
 * <br/>
 * <br/>Нам нужно Property Name[номер] класса HumanQueue только на чтение:
 * <br/>Property<const char16_t *, int, HumanQueue> Name = Property<const char16_t *, int, HumanQueue>(this, &HumanQueue::GetName);
 * <br/>Метод геттер: const char16_t * GetName(int queueNumber);
 * <br/>
 * <br/>Подробнее про типы в угловых скобках для каждого варианта в комментарии к определениям шаблонов(template) типов ниже.
 */

#include "map"

namespace DelphiAdapt {

    using namespace std;

    /**
     *
     * @tparam Data Тип Property(тип данных, с которым в итоге работаем)
     * @tparam Owner Класс-владелец Property
     */
    template<typename Data, typename Owner>
    class Property {
    protected:
        typedef Data (Owner::*getter)();

        typedef void (Owner::*setter)(Data);

        Owner *_owner;
        getter _getter;
        setter _setter;

    public:
        operator Data() {
            return (_owner->*_getter)();
        }

        void operator=(Data value) {
            (_owner->*_setter)(value);
        }

    public:
        Property(Owner *const owner, getter getMethod, setter setMethod) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
        }
    };

    template<typename Data, typename Owner>
    class PropertyRO {
    protected:
        typedef Data (Owner::*getter)();

        Owner *_owner;
        getter _getter;

    public:
        operator Data() {
            return (_owner->*_getter)();
        }

    public:
        PropertyRO(Owner *const owner, getter getMethod) {
            _owner = owner;
            _getter = getMethod;
        }
    };

    template<typename Data, typename Owner>
    class PropertyWO {
    protected:
        typedef Data (Owner::*getter)();

        typedef void (Owner::*setter)(Data);

        Owner *_owner;
        setter _setter;

    public:
        void operator=(Data value) {
            (_owner->*_setter)(value);
        }

    public:
        PropertyWO(Owner *const owner, setter setMethod) {
            _owner = owner;
            _setter = setMethod;
        }
    };

    template<typename Data, typename Index, typename Owner>
    class Property_Arr_1D_Value {
    protected:
        typedef Data (Owner::*getter)(Index);

        typedef void (Owner::*setter)(Index, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;
        Index _index;

    public:
        operator Data() {
            return (_owner->*_getter)(_index);
        }

        void operator=(Data value) {
            (_owner->*_setter)(_index, value);
        }

    public:
        Property_Arr_1D_Value(Owner *const owner, getter getMethod, setter setMethod, Index indexValue) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
            _index = indexValue;
        }
    };

    template<typename Data, typename Index, typename Owner>
    class PropertyRO_Arr_1D_Value {
    protected:
        typedef Data (Owner::*getter)(Index);

        Owner *_owner;
        getter _getter;
        Index _index;

    public:
        operator Data() {
            return (_owner->*_getter)(_index);
        }

    public:
        PropertyRO_Arr_1D_Value(Owner *const owner, getter getMethod, Index indexValue) {
            _owner = owner;
            _getter = getMethod;
            _index = indexValue;
        }
    };

    template<typename Data, typename Index, typename Owner>
    class PropertyWO_Arr_1D_Value {
    protected:
        typedef void (Owner::*setter)(Index, Data);

        Owner *_owner;
        setter _setter;
        Index _index;

    public:
        void operator=(Data value) {
            (_owner->*_setter)(_index, value);
        }

    public:
        PropertyWO_Arr_1D_Value(Owner *const owner, setter setMethod, Index indexValue) {
            _owner = owner;
            _setter = setMethod;
            _index = indexValue;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Owner>
    class Property_Arr_2D_Value {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2);

        typedef void (Owner::*setter)(Index1, Index2, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;
        Index1 _index1;
        Index2 _index2;

    public:
        operator Data() {
            return (_owner->*_getter)(_index1, _index2);
        }

        void operator=(Data value) {
            (_owner->*_setter)(_index1, _index2, value);
        }

    public:
        Property_Arr_2D_Value(Owner *const owner, getter getMethod, setter setMethod, Index1 index1Value, Index2 index2Value) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
            _index1 = index1Value;
            _index2 = index2Value;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Owner>
    class PropertyRO_Arr_2D_Value {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2);

        Owner *_owner;
        getter _getter;
        Index1 _index1;
        Index2 _index2;

    public:
        operator Data() {
            return (_owner->*_getter)(_index1, _index2);
        }

    public:
        PropertyRO_Arr_2D_Value(Owner *const owner, getter getMethod, Index1 index1Value, Index2 index2Value) {
            _owner = owner;
            _getter = getMethod;
            _index1 = index1Value;
            _index2 = index2Value;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Owner>
    class PropertyWO_Arr_2D_Value {
    protected:
        typedef void (Owner::*setter)(Index1, Index2, Data);

        Owner *_owner;
        setter _setter;
        Index1 _index1;
        Index2 _index2;

    public:
        void operator=(Data value) {
            (_owner->*_setter)(_index1, _index2, value);
        }

    public:
        PropertyWO_Arr_2D_Value(Owner *const owner, setter setMethod, Index1 index1Value, Index2 index2Value) {
            _owner = owner;
            _setter = setMethod;
            _index1 = index1Value;
            _index2 = index2Value;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class Property_Arr_3D_Value {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2, Index3);

        typedef void (Owner::*setter)(Index1, Index2, Index3, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;
        Index1 _index1;
        Index2 _index2;
        Index3 _index3;

    public:
        operator Data() {
            return (_owner->*_getter)(_index1, _index2, _index3);
        }

        void operator=(Data value) {
            (_owner->*_setter)(_index1, _index2, _index3, value);
        }

    public:
        Property_Arr_3D_Value(Owner *const owner, getter getMethod, setter setMethod, Index1 index1Value, Index2 index2Value, Index3 index3Value) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
            _index1 = index1Value;
            _index2 = index2Value;
            _index3 = index3Value;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class PropertyRO_Arr_3D_Value {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2, Index3);

        Owner *_owner;
        getter _getter;
        Index1 _index1;
        Index2 _index2;
        Index3 _index3;

    public:
        operator Data() {
            return (_owner->*_getter)(_index1, _index2, _index3);
        }

    public:
        PropertyRO_Arr_3D_Value(Owner *const owner, getter getMethod, Index1 index1Value, Index2 index2Value, Index3 index3Value) {
            _owner = owner;
            _getter = getMethod;
            _index1 = index1Value;
            _index2 = index2Value;
            _index3 = index3Value;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class PropertyWO_Arr_3D_Value {
    protected:
        typedef void (Owner::*setter)(Index1, Index2, Index3, Data);

        Owner *_owner;
        setter _setter;
        Index1 _index1;
        Index2 _index2;
        Index3 _index3;

    public:
        void operator=(Data value) {
            (_owner->*_setter)(_index1, _index2, _index3, value);
        }

    public:
        PropertyWO_Arr_3D_Value(Owner *const owner, setter setMethod, Index1 index1Value, Index2 index2Value, Index3 index3Value) {
            _owner = owner;
            _setter = setMethod;
            _index1 = index1Value;
            _index2 = index2Value;
            _index3 = index3Value;
        }
    };

    /**
     *
     * @tparam Data Тип Property(тип данных, с которым в итоге работаем)
     * @tparam Index Тип индекса
     * @tparam Owner Класс-владелец Property
     */
    template<typename Data, typename Index, typename Owner>
    class Property_Arr_1D {
    protected:
        typedef Data (Owner::*getter)(Index);

        typedef void (Owner::*setter)(Index, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;

    public:
        Property_Arr_1D_Value<Data, Index, Owner> operator[](Index value) {
            return Property_Arr_1D_Value<Data, Index, Owner>(_owner, _getter, _setter, value);
        }

        Property_Arr_1D_Value<Data, Index, Owner> operator()(Index value) {
            return Property_Arr_1D_Value<Data, Index, Owner>(_owner, _getter, _setter, value);
        }

    public:
        Property_Arr_1D(Owner *const owner, getter getMethod, setter setMethod) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
        }
    };

    template<typename Data, typename Index, typename Owner>
    class PropertyRO_Arr_1D {
    protected:
        typedef Data (Owner::*getter)(Index);

        Owner *_owner;
        getter _getter;

    public:
        PropertyRO_Arr_1D_Value<Data, Index, Owner> operator[](Index value) {
            return PropertyRO_Arr_1D_Value<Data, Index, Owner>(_owner, _getter, value);
        }

        PropertyRO_Arr_1D_Value<Data, Index, Owner> operator()(Index value) {
            return PropertyRO_Arr_1D_Value<Data, Index, Owner>(_owner, _getter, value);
        }

    public:
        PropertyRO_Arr_1D(Owner *const owner, getter getMethod) {
            _owner = owner;
            _getter = getMethod;
        }
    };

    template<typename Data, typename Index, typename Owner>
    class PropertyWO_Arr_1D {
    protected:
        typedef void (Owner::*setter)(Index, Data);

        Owner *_owner;
        setter _setter;

    public:
        PropertyWO_Arr_1D_Value<Data, Index, Owner> operator[](Index value) {
            return PropertyWO_Arr_1D_Value<Data, Index, Owner>(_owner, _setter, value);
        }

        PropertyWO_Arr_1D_Value<Data, Index, Owner> operator()(Index value) {
            return PropertyWO_Arr_1D_Value<Data, Index, Owner>(_owner, _setter, value);
        }

    public:
        PropertyWO_Arr_1D(Owner *const owner, setter setMethod) {
            _owner = owner;
            _setter = setMethod;
        }
    };

    /**
     *
     * @tparam Data Тип Property(тип данных, с которым в итоге работаем)
     * @tparam Index1 Тип первого индекса
     * @tparam Index2 Тип второго индекса
     * @tparam Owner Класс-владелец Property
     */
    template<typename Data, typename Index1, typename Index2, typename Owner>
    class Property_Arr_2D {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2);

        typedef void (Owner::*setter)(Index1, Index2, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;

    public:
        Property_Arr_2D_Value<Data, Index1, Index2, Owner> operator()(Index1 value1, Index2 value2) {
            return Property_Arr_2D_Value<Data, Index1, Index2, Owner>(_owner, _getter, _setter, value1, value2);
        }

    public:
        Property_Arr_2D(Owner *const owner, getter getMethod, setter setMethod) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Owner>
    class PropertyRO_Arr_2D {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2);

        Owner *_owner;
        getter _getter;

    public:
        PropertyRO_Arr_2D_Value<Data, Index1, Index2, Owner> operator()(Index1 value1, Index2 value2) {
            return PropertyRO_Arr_2D_Value<Data, Index1, Index2, Owner>(_owner, _getter, value1, value2);
        }

    public:
        PropertyRO_Arr_2D(Owner *const owner, getter getMethod) {
            _owner = owner;
            _getter = getMethod;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Owner>
    class PropertyWO_Arr_2D {
    protected:
        typedef void (Owner::*setter)(Index1, Index2, Data);

        Owner *_owner;
        setter _setter;

    public:
        PropertyWO_Arr_2D_Value<Data, Index1, Index2, Owner> operator()(Index1 value1, Index2 value2) {
            return PropertyWO_Arr_2D_Value<Data, Index1, Index2, Owner>(_owner, _setter, value1, value2);
        }

    public:
        PropertyWO_Arr_2D(Owner *const owner, setter setMethod) {
            _owner = owner;
            _setter = setMethod;
        }
    };

    /**
     *
     * @tparam Data Тип Property(тип данных, с которым в итоге работаем)
     * @tparam Index1 Тип первого индекса
     * @tparam Index2 Тип второго индекса
     * @tparam Index3 Тип третьего индекса
     * @tparam Owner Класс-владелец Property
     */
    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class Property_Arr_3D {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2, Index3);

        typedef void (Owner::*setter)(Index1, Index2, Index3, Data);

        Owner *_owner;
        getter _getter;
        setter _setter;

    public:
        Property_Arr_3D_Value<Data, Index1, Index2, Index3, Owner> operator()(Index1 value1, Index2 value2, Index3 value3) {
            return Property_Arr_3D_Value<Data, Index1, Index2, Index3, Owner>(_owner, _getter, _setter, value1, value2, value3);
        }

    public:
        Property_Arr_3D(Owner *const owner, getter getMethod, setter setMethod) {
            _owner = owner;
            _getter = getMethod;
            _setter = setMethod;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class PropertyRO_Arr_3D {
    protected:
        typedef Data (Owner::*getter)(Index1, Index2, Index3);

        Owner *_owner;
        getter _getter;

    public:
        PropertyRO_Arr_3D_Value<Data, Index1, Index2, Index3, Owner> operator()(Index1 value1, Index2 value2, Index3 value3) {
            return PropertyRO_Arr_3D_Value<Data, Index1, Index2, Index3, Owner>(_owner, _getter, value1, value2, value3);
        }

    public:
        PropertyRO_Arr_3D(Owner *const owner, getter getMethod) {
            _owner = owner;
            _getter = getMethod;
        }
    };

    template<typename Data, typename Index1, typename Index2, typename Index3, typename Owner>
    class PropertyWO_Arr_3D {
    protected:
        typedef void (Owner::*setter)(Index1, Index2, Index3, Data);

        Owner *_owner;
        setter _setter;

    public:
        PropertyWO_Arr_3D_Value<Data, Index1, Index2, Index3, Owner> operator()(Index1 value1, Index2 value2, Index3 value3) {
            return PropertyWO_Arr_3D_Value<Data, Index1, Index2, Index3, Owner>(_owner, _setter, value1, value2, value3);
        }

    public:
        PropertyWO_Arr_3D(Owner *const owner, setter setMethod) {
            _owner = owner;
            _setter = setMethod;
        }
    };
}

#endif //F1FLEXCELASPOSE_PROPERTY_H
