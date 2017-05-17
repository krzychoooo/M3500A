/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   miernikUSB.h
 * Author: user
 *
 * Created on 1 kwietnia 2017, 16:50
 */

#ifndef M3500A_H
#define M3500A_H

//#include "./../StdAfx.h"
#include "visa.h"
#include <windows.h>
#include <string>
//using namespace std;


//extern HINSTANCE hUSBTMCLIB;
class M3500A {

public:
    HINSTANCE hUSBTMCLIB;
    unsigned long m_defaultRM_usbtmc, m_instr_usbtmc;
    unsigned long m_findList_usbtmc;
    unsigned long m_nCount;
    ViStatus status;
    const static int m_Timeout = 7000;
    char *pStrout; // Write out data buffer
    unsigned char pStrin[64]; // Read in data buffer
    int len;
    unsigned long nWritten;
    unsigned long  nRead;
    char buffer[256];
    char instrDescriptor[256];
    enum meterMode{
                    VOLTage_DC,
                    VOLTage_AC,
                    CURRent_DC,
                    CURRent_AC,
                    RESistance,
                    FRESistance,
                    FREQuency,
                    PERiod,
                    CONTinuity,
                    DIODe,
                    TCOuple,
                    TEMPerature
    };
    M3500A();
    M3500A(std::string miernikId);
    virtual ~M3500A();
    void sendCommand(char*, unsigned char*, int, bool);
    float read();
    void setMultimetr(float, float,meterMode);
    void setText(std::string);
private:

};

#endif /* M3500A_H */

