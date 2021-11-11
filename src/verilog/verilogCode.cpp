#include "verilogCode.h"

namespace SimpleVerilog{
VerilogCode::VerilogCode()
{
}
VerilogCode::VerilogCode(const string &filePath)
{
    setContentPath(filePath);
}
VerilogCode::VerilogCode(const VerilogCode &other)
{
    m_content = other.m_content;
    m_contentRaw = other.m_contentRaw;
}
VerilogCode::~VerilogCode()
{
    m_content.clear();
}

void VerilogCode::setContentPath(const string &filePath)
{
    fstream  file;
    vector<string> content;
    file.open(filePath.c_str(),std::ios::in);
    if(!file.is_open())
    {
        VERILOG_ERROR("Can't open file: "+filePath)

    }
    string line;
  //  qDebug() << "RawFile:";
    while(getline(file, line)){ //read data from file object and put it into string.
      // qDebug() << line.c_str(); //print the data of the string
       content.push_back(line);
    }
    file.close(); //close the file object.
   // qDebug() << "RawFile end";

    setContent(content);
}
void VerilogCode::setContent(const vector<string> &contentList)
{
    m_contentRaw = contentList;
    m_content = getVerilogCode(contentList);
   /* qDebug() << "VERILOG VerilogCode: ";
    for(auto &a : m_content)
    {
        qDebug() << a.c_str();
    }
    qDebug() << "VERILOG: VerilogCode end ";*/
}
const vector<string> &VerilogCode::getContent() const
{
    return m_content;
}
const vector<string> &VerilogCode::getRawContent() const
{
    return m_contentRaw;
}
vector<string> VerilogCode::getVerilogCode(const vector<string> &content)
{
    vector<string> newContent;

    const string lineComment  = "//";
    const string commentBegin = "/*";
    const string commentEnd   = "*/";
    const string statementEnd = ";";
    const string newLine      = "\n";

    bool isLineComment = false; // for //
    bool isAreaComment = false; // for /*   */

    for(size_t i=0; i<content.size(); i++)
    {
        string line = content[i];
        string::size_type matchLineCommentIndex = line.find(lineComment);
        string::size_type matchAreaCommentIndex = line.find(commentBegin);
        string::size_type matchAreaEndCommentIndex = line.find(commentEnd);

        if(matchAreaCommentIndex != matchLineCommentIndex)
        {
            // Any comment found
            if(matchLineCommentIndex == string::npos)
            {
                isAreaComment = true;
                goto eraseComment;
            }

            if(matchAreaCommentIndex == string::npos)
            {
                isLineComment = true;
                goto eraseComment;
            }
            if(matchLineCommentIndex < matchAreaCommentIndex)
                isLineComment = true;
            else
                isAreaComment = true;
        }
        eraseComment:

        if(isLineComment)
        {
            isLineComment = false;
            goto skipLine;
        }
        if(isAreaComment)
        {
            if(matchAreaEndCommentIndex != string::npos)
            {
                line = line.substr(matchAreaEndCommentIndex+commentEnd.size(),line.size());
                isAreaComment = false;
            }
            else
            {
                goto skipLine;
            }
        }

        newContent.push_back(line);
        skipLine:;

    }

    // Sort Statements
    vector<string> sortedStatements;
    //size_t beginStatementLine = 0, beginStatementColumn = 0;
    string statement = "";
    for(size_t i=0; i<newContent.size(); i++)
    {
        string line = newContent[i];

        string::size_type matchStatementEnd = line.find(statementEnd);

        if(matchStatementEnd != string::npos)
        {
            statement += line.substr(0,matchStatementEnd);
            sortedStatements.push_back(statement+statementEnd);
            statement = line.substr(matchStatementEnd+statementEnd.size(),line.size());
        }
        else
        {
            statement += line;
        }
    }
    sortedStatements.push_back(statement+statementEnd);

    // remove newLin
    for(size_t i=0; i<sortedStatements.size(); i++)
    {
        remove_all(sortedStatements[i],newLine);
        replace_all(sortedStatements[i],"\t"," ");
        replace_all(sortedStatements[i],"  "," ");
    }
    return sortedStatements;
}
}
