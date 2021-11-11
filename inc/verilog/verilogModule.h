#ifndef VERILOGBLOCK_H
#define VERILOGBLOCK_H

#include "core.h"
#include "verilogPin.h"
#include "verilogCode.h"


namespace SimpleVerilog{
class Module
{
    public:
        Module(const string &name,const vector<Pin> &pinList);
        Module(const Module &other);
        ~Module();


        string toString(size_t tabCount = 0) const;
        const VerilogCode &getCode() const;

        static Module* parseModule(const string &filePath);
        //static VerilogCode getModuleCode(const VerilogCode &code);
    private:
        string m_name;
        vector<Pin> m_pins;
        VerilogCode m_code;
};
}
#endif
