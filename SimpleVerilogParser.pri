QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


verilogParserPath = $$PWD
verilogParserInc = $$verilogParserPath/inc
verilogParserSrc = $$verilogParserPath/src

INCLUDEPATH += $$verilogParserInc\
               $$verilogParserInc/core \
               $$verilogParserInc/verilog

HEADERS += \
    $$verilogParserInc/verilog/verilogCode.h \
    $$verilogParserInc/core/core.h \
    $$verilogParserInc/core/debug.h \
    $$verilogParserInc/verilog/verilogModule.h \
    $$verilogParserInc/verilog/verilogPin.h



SOURCES += \
    $$verilogParserSrc/verilog/verilogCode.cpp \
    $$verilogParserSrc/core/core.cpp \
    $$verilogParserSrc/verilog/verilogModule.cpp \
    $$verilogParserSrc/verilog/verilogPin.cpp






# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
