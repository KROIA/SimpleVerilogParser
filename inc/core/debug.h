#ifndef DEBUG_H
#define DEBUG_H


#define VERILOG_DEBUG_ENABLE
  #define DEBUG_WITH_METADATA
#define QT_APP



#ifdef VERILOG_DEBUG_ENABLE
    #ifdef QT_APP
        #include <QDebug>
        #include <regex>
    #else
        #include <iostream>
        #include <stdlib.h>
        #include <windows.h>
    #endif
#endif

#ifdef VERILOG_DEBUG_ENABLE
#ifdef QT_APP
// Console color management
#define GET_CONSOLE_HANDLE
#define TEXT_COL(color)
#define TEXT_COLOR_NORMAL

// Console text management
#define REMOVE_NEWLINE(text) std::regex_replace(text, std::regex("\n+"), "")
//#define WRITE_TO_CONSOLE(text) qDebug() << REMOVE_NEWLINE(text).c_str();
#define WRITE_TO_CONSOLE(text) qDebug() << std::string(text).c_str();
#define PRINT_NEW_LINE WRITE_TO_CONSOLE("\n")

// Console print management
#define WRITE_COLORED(color,text)  TEXT_COL(color) WRITE_TO_CONSOLE(text) TEXT_COLOR_NORMAL



// Debug management

#define PRINT_FILE WRITE_TO_CONSOLE( __FILE__)
#define PRINT_FUNCTION_NAME WRITE_TO_CONSOLE(__PRETTY_FUNCTION__)
#define PRINT_LINE_INDEX WRITE_TO_CONSOLE(std::to_string( __LINE__ ))

#define DEBUG_METADATA

#ifdef DEBUG_WITH_METADATA
#define DEBUG_GENERAL(type,typeColor,color,message) \
WRITE_TO_CONSOLE(std::string(type)+" | "+std::string(__FILE__)+" | "+std::string(__PRETTY_FUNCTION__)+" | line "+std::to_string(__LINE__)+" | "+message)
#else
#define DEBUG_GENERAL(type,typeColor,color,message) \
WRITE_TO_CONSOLE(type+" | "+message)
#endif

#else

// Console color management
#define GET_CONSOLE_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)
#define TEXT_COL(color) SetConsoleTextAttribute(GET_CONSOLE_HANDLE, color);
#define TEXT_COLOR_NORMAL TEXT_COL(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED)

// Console text management
#define WRITE_TO_CONSOLE(text) std::cout << text;
#define PRINT_NEW_LINE WRITE_TO_CONSOLE("\n")


// Console print management
#define WRITE_COLORED(color,text)  TEXT_COL(color) WRITE_TO_CONSOLE(text) TEXT_COLOR_NORMAL



// Debug management

#define PRINT_FILE WRITE_TO_CONSOLE( __FILE__)
#define PRINT_FUNCTION_NAME WRITE_TO_CONSOLE(__PRETTY_FUNCTION__)
#define PRINT_LINE_INDEX WRITE_TO_CONSOLE(std::to_string( __LINE__ ))

#define DEBUG_METADATA

#ifdef DEBUG_WITH_METADATA
#define DEBUG_GENERAL(type,typeColor,color,message) \
TEXT_COL(typeColor) \
WRITE_TO_CONSOLE(type) \
TEXT_COL(color) \
WRITE_TO_CONSOLE(" | ") \
PRINT_FILE \
WRITE_TO_CONSOLE(" | ") \
PRINT_FUNCTION_NAME \
WRITE_TO_CONSOLE(" | line ") \
PRINT_LINE_INDEX \
WRITE_TO_CONSOLE(" | ") \
TEXT_COLOR_NORMAL \
WRITE_TO_CONSOLE(message)
#else
#define DEBUG_GENERAL(type,typeColor,color,message) \
TEXT_COL(typeColor) \
WRITE_TO_CONSOLE(type) \
TEXT_COL(color) \
WRITE_TO_CONSOLE(" | ") \
TEXT_COLOR_NORMAL \
WRITE_TO_CONSOLE(message)
#endif









#endif

#define NL std::string("\n")
#define VERILOG_DEBUG(message)   DEBUG_GENERAL("DEBUG   ",FOREGROUND_INTENSITY,FOREGROUND_INTENSITY,message);
#define VERILOG_WARNING(message) DEBUG_GENERAL("WARNING ",FOREGROUND_RED|FOREGROUND_GREEN,FOREGROUND_INTENSITY,message+NL);
#define VERILOG_ERROR(message)   DEBUG_GENERAL("ERROR   ",FOREGROUND_RED,FOREGROUND_INTENSITY,message+NL);

#define VERILOG_ERROR_OUT_OF_RANGE(paramName,min,max) VERILOG_ERROR( std::string(#paramName)+" = "+std::to_string(paramName)+", is out of range. Range: ["+std::to_string(min)+" - "+std::to_string(max)+"]"+NL)


// Verilog errors
#define VERILOG_SYNTAX_ERROR(message) VERILOG_ERROR("Verilog syntax error: "+std::string(message)+NL)

#else
#define NL
#define VERILOG_DEBUG(message)
#define VERILOG_WARNING(message)
#define VERILOG_ERROR(message)

#define VERILOG_ERROR_OUT_OF_RANGE(paramName,min,max)

#define VERILOG_SYNTAX_ERROR(message)
#endif

#endif // DEBUG_H
