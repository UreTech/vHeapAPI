/*
Copyright 2024 ÜreTech Software
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

char shitpost[] = "Balina kumanyasi icinde kitap okuyan tavsan, uzayda kosan devekuzu ve denizde yuzerek gozu acik kalmis kartal ile bulustu, sonra birden bire telefon kulubesinin etrafinda donen aslanlar ve sehirde dondurma satan kedi ile dans eden yunuslarla birlikte kosmaya basladi, sonunda ise kaykayla ulkeye giden kuzugozu ve yolda kosan koyunlarin arasinda kayboldu.";

#include <malloc.h>
#include <iostream>
#include <string>
#include <vector>

#include "vHeap/nHeap/nHeapConfig.h"
#include "vHeap/vHeap.h"
#include "vHeap/nHeap/nHeap.h"

using namespace vhp::v120;

std::string readCon() {
    std::string res;
    std::getline(std::cin, res);
    return res;
}


int main() {
    printCon("vhp.ver-->", vhp_version_str);
    printCon("vhp.vde-->", vhp_version_desc);
    printCon("nhp.ver-->", nhp_version_str);
    printCon("nhp.vde-->", nhp_version_desc);
    printCon("nhp.sys-->", "Enter mode (c/s)");

    std::string rd = readCon();



    nhp::init_nhp_runtime();
    if (rd == "s") {
        nhp::_nhp->initServer(3306);
    }
    else if (rd == "c") {
        nhp::_nhp->initClient("127.0.0.1", 3306);
        while (1) {
            if (nhp::_nhp->getMode() == nhp::n_client) {
                nhp::_nhp->sendToServer((uint8_t*)shitpost, sizeof(shitpost));
            }
            Sleep(6000);
        }
    }
    else {
        LOG_ERROR("Unknown mode!")
    }
    LOG_INFO("main end")
    while (1);//end for now
    //*******************
    init_vhp_runtime();

    bool exitCon = false;
    while (!exitCon) {
        printCon("vhp.sys()-->", "Enter console var or list (type exit for exiting vhp)");
        std::string inp = readCon();

        if (inp == "exit") {
            exitCon = true;
        }
        else if (inp == "cls") {
            system("cls");
        }
        else if (inp == "dump") {
            printCon("vhp.sys()-->", " vhp dump:\n" + _vhp->createHeapDump());
        }
        else if (inp == "add") {
            printCon("vhp.sys()-->", "Enter var type (int8 / uint8...)");
            std::string varTypeStr = readCon();
            printCon("vhp.sys()-->", "Enter var name (don't use space)");
            std::string varNameStr = readCon();
            printCon("vhp.sys()-->", "use Index flag on the name (y/n)");
            std::string useFlagStr = readCon();
            bool useFlag = true;
            if (useFlagStr == "y") {
                useFlag = true;
            }
            else {
                useFlag = false;
            }

            if (varTypeStr.empty() or varNameStr.empty()) {
                printCon("vhp.sys.arg.error-->", "arguments can't be empty!");
                continue;
            }

            try
            {
                void* ptr;
                size_t varIndex = _vhp->addNewVar(varNameStr, varType_string_to_enum(varTypeStr), ptr, useFlag);
                printCon("vhp.sys.add.info-->", "varIndex--> " + std::to_string(varIndex));
            }
            catch (std::string msg)
            {
                printCon("vhp.sys.add.error-->", msg);
                continue;
            }
        }
        else if (inp == "string") {
            printCon("vhp.sys()-->", "Enter var name (don't use space)");
            std::string varNameStr = readCon();
            printCon("vhp.sys()-->", "use Index flag on the name (y/n)");
            std::string useFlagStr = readCon();
            printCon("vhp.sys()-->", "Enter string");
            std::string varStr = readCon();
            bool useFlag = true;
            if (useFlagStr == "y") {
                useFlag = true;
            }
            else {
                useFlag = false;
            }

            if (varNameStr.empty()) {
                printCon("vhp.sys.arg.error-->", "arguments can't be empty!");
                continue;
            }

            try
            {
                void* ptr;
                size_t varIndex = _vhp->addNewStringVar(varNameStr, varStr, ptr, useFlag);
                printCon("vhp.sys.add.info-->", "varIndex--> " + std::to_string(varIndex));
            }
            catch (std::string msg)
            {
                printCon("vhp.sys.add.error-->", msg);
                continue;
            }
        }
        else if (inp == "getstr") {
            printCon("vhp.sys()-->", "Enter var name");
            std::string varNameStr = readCon();
            try
            {
                vHeap* _vh = _vhp->getVarByName(varNameStr);
                if (_vh->getVarType() != t_flex) {
                    printCon("vhp.sys.getstr.error-->", "var type is not flex");
                }
                else {
                    printCon("vhp.sys.set.info-->", " varData:" + std::string((char*)_vh->getVar(), _vh->getVarSize()));
                    //std::cout << " f " << (char*)_vh->getVar();
                }
            }
            catch (std::string msg)
            {
                printCon("vhp.sys.getstr.error-->", msg);
                continue;
            }
        }
        else if (inp == "set") {
            printCon("vhp.sys()-->", "Enter var name");
            std::string varNameStr = readCon();
            try
            {
                vHeap* _vh = _vhp->getVarByName(varNameStr);
                printCon("vhp.sys.set.info-->", " varType:" + _vh->getVarTypeName());
                printCon("vhp.sys.set.info-->", " varData:" + vHeap_to_string(_vh));

                printCon("vhp.sys()-->", "Enter set value");
                std::string varValueStr = readCon();
                vHeap_setValueByString(varValueStr, _vh);
                printCon("vhp.sys.set.info-->", " set to: varData:" + vHeap_to_string(_vh));
            }
            catch (std::string msg)
            {
                printCon("vhp.sys.set.error-->", msg);
                continue;
            }
        }
        else {
            printCon("vhp.sys()-->", "unknown command");
        }
    }

    printCon("MemX: ", "Reading 'memRefAddr'...");
    Sleep(3000);
    vHeap* v_ = nullptr;
    try
    {
        v_ = _vhp->getVarByName("memRefAddr");
    }
    catch (std::string err)
    {
        printCon("vhp.sys.get.error-->", err);
    }

    if (v_ == nullptr) {
        printCon("MemX: ", "'memRefAddr' not found! Please add with vhpAPI!");
        while (1);
    }
    if (v_->getVarType() != t_uint8) {
        printCon("MemX: ", "'memRefAddr' is not uint8! varType->" + v_->getVarTypeName());
        while (1);
    }
    printCon("MemX: ", "memRefAddr-VALUE->" + vHeap_to_string(v_));
    while (1);
}
