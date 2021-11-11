#include "verilogPin.h"

namespace SimpleVerilog{
Pin::Pin()
{

}
Pin::Pin(Direction dir,Type type,const string &name,size_t dimensionMSB ,size_t dimensionLSB )
{
    m_direction = dir;
    m_type = type;
    m_name = name;
    m_dimensionLSB = dimensionLSB;
    m_dimensionMSB = dimensionMSB;
    if(dimensionLSB > dimensionMSB)
    {
        VERILOG_SYNTAX_ERROR(typeToString(type)+"["+to_string(dimensionMSB)+":"+to_string(dimensionLSB)+"] | Part-select direction is opposite from prefix index direction")
        m_dimensionLSB = dimensionMSB;
        m_dimensionMSB = dimensionLSB;
    }

}
Pin::Pin(const Pin &other)
{
    m_name = other.m_name;
    m_type = other.m_type;
    m_direction = other.m_direction;
    m_dimensionMSB = other.m_dimensionMSB;
    m_dimensionLSB = other.m_dimensionLSB;
}
Pin::~Pin()
{

}

const string &Pin::name() const
{
    return m_name;
}
const Pin::Type &Pin::type() const
{
    return m_type;
}
const Pin::Direction &Pin::direction() const
{
    return m_direction;
}
size_t Pin::dimensionMSB() const
{
    return m_dimensionMSB;
}
size_t Pin::dimensionLSB() const
{
    return m_dimensionLSB;
}
size_t Pin::dimension() const
{
    return m_dimensionMSB-m_dimensionLSB+1;
}

string Pin::toString(size_t tabCount) const
{
    string tabs;
    for(size_t i=0; i<tabCount; i++)
        tabs+="  ";
    string str =
            string(tabs+"Pin\n"+tabs+"{\n")+
            string(tabs+"  Name:        "+m_name)+"\n"+
            string(tabs+"  Type:        "+typeToString(m_type))+"\n"+
            string(tabs+"  Direction:   "+directionToString(m_direction))+"\n"+
            string(tabs+"  Dimensions:  ["+to_string(m_dimensionMSB)+":"+to_string(m_dimensionLSB)+"]\n")+
            string(tabs+"}\n");
    return str;
}

Pin::Type Pin::stringToType(const string &str)
{
    if(str.find("wire") != string::npos)
        return Type::wire;
    if(str.find("reg") != string::npos)
        return Type::reg;
    return Type::t__count;
}
string Pin::typeToString(Type t)
{
    switch(t)
    {
        case Type::wire: return "wire";
        case Type::reg:  return "reg";
        default:
            break;
    }
    return "notAType";
}

Pin::Direction Pin::stringToDirection(const string &str)
{
    if(str.find("input") != string::npos)
        return Direction::input;
    if(str.find("output") != string::npos)
        return Direction::output;
    if(str.find("inout") != string::npos)
        return Direction::inout;
    return Direction::d__count;
}
string Pin::directionToString(Direction d)
{
    switch(d)
    {
        case Direction::input: return "input";
        case Direction::output:return "output";
        case Direction::inout: return "inout";
        default:
            break;
    }
    return "notADirection";
}

}
