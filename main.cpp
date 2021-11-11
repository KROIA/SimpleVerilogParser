#include <QCoreApplication>
#include "core.h"
#include "VerilogParser.h"
#include "verilogModule.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SimpleVerilog::Module *module;
    module = SimpleVerilog::Module::parseModule("E:\\Dokumente\\QT\\Projects\\FPGA_Editor\\Blocks\\AND.verilog");


    return a.exec();
}
