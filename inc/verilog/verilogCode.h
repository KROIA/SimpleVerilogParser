#ifndef VERILOGCODE_H
#define VERILOGCODE_H

#include "core.h"
#include <fstream>
using std::fstream;

namespace SimpleVerilog{
class VerilogCode
{
    public:
        VerilogCode();
        VerilogCode(const string &filePath);
        VerilogCode(const VerilogCode &other);
        ~VerilogCode();

        void setContentPath(const string &filePath);
        void setContent(const vector<string> &contentList);
        const vector<string> &getContent() const;
        const vector<string> &getRawContent() const;

    private:
        vector<string> getVerilogCode(const vector<string> &content);
        vector<string> m_content;
        vector<string> m_contentRaw;
};
}
#endif // VERILOGCODE_H
