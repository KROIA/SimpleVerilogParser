#include "verilog/verilogModule.h"

namespace SimpleVerilog{
/*Module::Module(const string &filePath)
{
    Module *m = parseModule(filePath);
    if(m==nullptr)
        throw std::runtime_error("No Module");
    *this = *m;
    delete m;
}*/
Module::Module(const string &name,const vector<Pin> &pinList)
{
    m_name = name;
    m_pins = pinList;
}
Module::Module(const Module &other)
{
    *this = other;
}
Module::~Module()
{
    m_pins.clear();
}

const Module &Module::operator=(const Module &other)
{
    m_name = other.m_name;
    m_pins = other.m_pins;
    m_code = other.m_code;
    return *this;
}
string Module::toString(size_t tabCount) const
{
    string tabs;
    for(size_t i=0; i<tabCount; i++)
        tabs+="  ";

    string pinsString="";
    for(size_t i=0; i<m_pins.size(); i++)
        pinsString += m_pins[i].toString(tabCount+2);

    string codeString="";
    vector<string> codeList = m_code.getRawContent();
    for(size_t i=0; i<codeList.size(); i++)
    {
        codeString+=tabs+"    "+codeList[i]+"\n";
    }

    string str =
            string("\n"+tabs+"Module\n"+tabs+"{\n")+
            string(tabs+"  Name:     "+m_name)+"\n"+
            string(tabs+"  Pincount: "+to_string(m_pins.size()))+"\n"+
            string(tabs+"  Pins:\n")+
            pinsString+
            string(tabs+"  Code:\n"+tabs+"<VERILOG>\n")+
            codeString+
            string(tabs+"  <VERILOG\\>\n"+tabs+"}\n");
    return str;
}
const VerilogCode &Module::getCode() const
{
    return m_code;
}

const string &Module::name() const
{
    return m_name;
}

size_t Module::getPinCount() const
{
    return m_pins.size();
}

size_t Module::getPinCount(Pin::Type type) const
{
    size_t count = 0;
    for(size_t i=0; i<m_pins.size(); i++)
    {
        if(m_pins[i].type() == type)
            count += m_pins[i].dimension();
    }
    return count;
}

size_t Module::getPinCount(Pin::Direction dir) const
{
    size_t count = 0;
    for(size_t i=0; i<m_pins.size(); i++)
    {
        if(m_pins[i].direction() == dir)
            count += m_pins[i].dimension();
    }
    return count;
}
const vector<Pin> Module::pinList() const
{
    return m_pins;
}

Module* Module::parseModule(const string &filePath)
{
    VerilogCode code(filePath);
    vector<string> codeList = code.getContent();

    /*qDebug() << "VERILOG MODULE: ";
    for(auto &a : codeList)
    {
        qDebug() << a.c_str();
    }
    qDebug() << "VERILOG: MODULE end ";*/

    string name = "NotDefined";
    vector<Pin> pinList;

    for(size_t i=0; i<codeList.size(); i++)
    {
        string::size_type modulePos = codeList[i].find("module");
        if(modulePos != string::npos)
        {
            string codeLine = codeList[i];

            regex modulNameExp("module[ ]+([_a-zA-Z][a-zA-Z0-9_]*)");
            smatch modulNameMatch;
            regex_search(codeLine,modulNameMatch,modulNameExp);
            if(modulNameMatch[1] == "")
            {
                VERILOG_SYNTAX_ERROR("name of module not found")
                goto err;
            }
            name = modulNameMatch[1].str();
            //qDebug() << "modulname: "<<name.c_str();

            // Find all params
            regex modulParamExp("[[,|\\(][ ]*([_a-zA-Z][_a-zA-Z0-9 \\[\\]:]*)[,|\\)]*");
            smatch modulParamMatch;
            vector<string> pinListStr;
            while(regex_search(codeLine,modulParamMatch,modulParamExp))
            {
                //for(size_t b=0; b<modulParamMatch.size(); b++)
                //    qDebug() <<b<<" AAA  "<< modulParamMatch[b].str().c_str();
                if(modulParamMatch.size() != 2)
                {
                    codeLine = modulParamMatch.suffix();
                    continue;
                }
                //qDebug() << "PinXX: "<<modulParamMatch.str(1).c_str();
                pinListStr.push_back(modulParamMatch.str(1));
                //qDebug() << "modulParamMatch.suffix()" << modulParamMatch.suffix().str().c_str();

                codeLine = ","+modulParamMatch.suffix().str();
            }


            for(size_t a=0; a<pinListStr.size(); a++)
            {
                Pin::Direction pinDir = Pin::Direction::d__count;
                Pin::Type pinType = Pin::Type::wire;
                size_t msb = 0;
                size_t lsb = 0;
                string pinName = "";

                {
                    string dirExpStr="";
                    for(int t=0; t<(int)Pin::Direction::d__count;t++)
                    {
                        dirExpStr+=Pin::directionToString(Pin::Direction(t));
                        if(t<(int)Pin::Direction::d__count-1)
                            dirExpStr+="|";
                    }
                    regex dirExp("("+dirExpStr+")");
                    smatch dirMatch;
                    regex_search(pinListStr[a],dirMatch,dirExp);
                    if(dirMatch.size() != 2)
                    {
                        VERILOG_SYNTAX_ERROR("Pin direction is not defined")
                    }
                    else
                    {
                        pinDir = Pin::stringToDirection(dirMatch[1]);

                        pinListStr[a] = pinListStr[a].substr(pinListStr[a].find(dirMatch[1])+dirMatch[1].str().size(),pinListStr[a].size());
                    }
                }
                {
                    string typeExpStr="";
                    for(int t=0; t<(int)Pin::Type::t__count;t++)
                    {
                        typeExpStr+=Pin::typeToString(Pin::Type(t));
                        if(t<(int)Pin::Type::t__count-1)
                            typeExpStr+="|";
                    }
                    regex typeExp("("+typeExpStr+")");
                    smatch typeMatch;
                    regex_search(pinListStr[a],typeMatch,typeExp);
                    if(typeMatch.size() != 2)
                    {
                        //VERILOG_SYNTAX_ERROR("Pin direction is not defined")
                    }
                    else
                    {
                        pinType = Pin::stringToType(typeMatch[1]);
                        pinListStr[a] = pinListStr[a].substr(pinListStr[a].find(typeMatch[1])+typeMatch[1].str().size(),pinListStr[a].size());
                    }
                }
                {
                    regex dimExp("[\\[][ ]*([0-9]*)[ ]*[:][ ]*([0-9]*)[ ]*[\\]]");
                    smatch dimMatch;
                    regex_search(pinListStr[a],dimMatch,dimExp);
                    if(dimMatch.size() == 3)
                    {
                        msb = atoi(dimMatch[1].str().c_str());
                        lsb = atoi(dimMatch[2].str().c_str());
                        pinListStr[a] = pinListStr[a].substr(pinListStr[a].find("]")+1,pinListStr[a].size());

                    }
                }
                {
                    regex nameExp("([_a-zA-Z][_a-zA-Z0-9 ]*)");
                    smatch nameMatch;
                    regex_search(pinListStr[a],nameMatch,nameExp);
                    if(nameMatch.size() == 2)
                    {
                        pinName = nameMatch[1];
                    }
                }


               /* qDebug() << "name: "<<pinName.c_str();
                qDebug() << "dir:  "<<Pin::directionToString(pinDir).c_str();
                qDebug() << "type: "<<Pin::typeToString(pinType).c_str();
                qDebug() << "msb:  "<<msb;
                qDebug() << "lsb:  "<<lsb;*/

                Pin newPin(pinDir,pinType,pinName,msb,lsb);
                pinList.push_back(newPin);
                //qDebug() << "PinCount: "<<pinList.size();
            }

            goto createModule;
        }
    }
    err:
    VERILOG_ERROR("Can't parse module")
    return nullptr;

    createModule:
    //qDebug() << "PinCount: "<<pinList.size();
    Module *module = new Module(name,pinList);
    module->m_code = code;
    VERILOG_DEBUG(module->toString())
    return module;
}


}
