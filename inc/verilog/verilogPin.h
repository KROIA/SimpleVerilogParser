#ifndef VERILOGPIN_H
#define VERILOGPIN_H

#include "core.h"

namespace SimpleVerilog{
class Pin
{
    public:
        enum Type
        {
            wire,
            reg,
            t__count
        };
        enum Direction
        {
            input,
            output,
            inout,
            d__count
        };

        Pin();
        Pin(Direction dir,Type type,const string &name,size_t dimensionMSB = 0,size_t dimensionLSB = 0);
        Pin(const Pin &other);
        ~Pin();

        const string &name() const;
        const Type &type() const;
        const Direction &direction() const;
        size_t dimensionMSB() const;
        size_t dimensionLSB() const;
        size_t dimension() const;

        string toString(size_t tabCount = 0) const;

        static Type stringToType(const string &str);
        static string typeToString(Type t);

        static Direction stringToDirection(const string &str);
        static string directionToString(Direction d);

    private:
        string m_name;
        Type m_type;
        Direction m_direction;
        size_t m_dimensionMSB; // Size of the array of pins
        size_t m_dimensionLSB;
};
}
#endif
