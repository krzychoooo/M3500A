/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   M3500A.cpp
 * Author: user
 *
 * Created on 1 kwietnia 2017, 16:50
 */

#include "m3500a.h"
//#include "StdAfx.h"
#include "visa.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

#define far

using namespace std;

std::string float_to_string( float liczba )
{
    std::stringstream floatToStr;
    std::string str;

    floatToStr << liczba;
    floatToStr >> str;
    floatToStr.clear();

    return str;
}

M3500A::M3500A() {
}


M3500A::M3500A(std::string miernikId) {
    //typedef int32_t ViStatus;
    //HINSTANCE hUSBTMCLIB;

    //hUSBTMCLIB = LoadLibrary("NIvisa64.dll");
    hUSBTMCLIB = LoadLibrary("visa32.dll");


    signed long (__stdcall * PviOpenDefaultRM_usb) (unsigned long *vi);
    signed long (__stdcall * PviFindRsrc_usb) (unsigned long sesn, char *expr, unsigned long *vi,
            unsigned long *retCnt, char far desc[]);
    signed long (__stdcall * PviOpen_usb) (unsigned long sesn, char *name, unsigned long mode,
            unsigned long timeout, unsigned long *vi);
    signed long (__stdcall * PviClose_usb) (unsigned long vi);
    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviSetAttribute_usb) (unsigned long vi, unsigned long viAttr, unsigned long attrstat);


    PviOpenDefaultRM_usb = (signed long (__stdcall*) (unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpenDefaultRM");
    PviFindRsrc_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long*,
            unsigned long*, char[]))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viFindRsrc");
    PviClose_usb = (signed long (__stdcall*) (unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viClose");
    PviOpen_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long,
            unsigned long, unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpen");
    PviWrite_usb	      = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viWrite");
    PviRead_usb			  = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viRead");
    PviSetAttribute_usb = (signed long (__stdcall*) (unsigned long, unsigned long, unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viSetAttribute");


    if (PviOpenDefaultRM_usb == NULL ||
            PviFindRsrc_usb == NULL ||
            PviClose_usb == NULL ||
            PviOpen_usb == NULL ||
            PviWrite_usb == NULL ||
            PviRead_usb == NULL ||
            PviSetAttribute_usb == NULL
            ) {
        //FreeLibrary(hUSBTMCLIB);
        //printf("%x,%x,%x,%x,%x,%x,%x,",PviOpenDefaultRM_usb, PviFindRsrc_usb, PviClose_usb, PviOpen_usb, PviWrite_usb, PviRead_usb, PviSetAttribute_usb);
        //hUSBTMCLIB = NULL;
        //MessageBox(NULL, "NIVISA for USBTMC library not ready.", "Multimeter device test", MB_OK);
        printf("NIVISA for USBTMC library not ready.");
    }


    // Open Device -- Resource Manager
    status = PviOpenDefaultRM_usb(&m_defaultRM_usbtmc);
    if (status < 0L) {
        PviClose_usb(m_defaultRM_usbtmc);
        hUSBTMCLIB = NULL;
        m_defaultRM_usbtmc = 0;
        //		MessageBox(NULL, "USBTMC resource not found.", "Multimeter device test", MB_OK);
        printf("USBTMC resource not found.");
        //Sleep(5000);
    } else {
        //char id1[]="USB[0-9]*::0x164E::0x0DAD::?*INSTR";
        //char id1[]="USB[0-9]*::"+miernikId+"::?*INSTR";

        //printf("string-%s",miernikId.c_str());
        char * idUSB = new char[miernikId.size()+1];
        strcpy(idUSB, miernikId.c_str());
        status = PviFindRsrc_usb(m_defaultRM_usbtmc, idUSB, &m_findList_usbtmc, &m_nCount, instrDescriptor);

        if (status < 0L) {
            char id2[]="USB[0-9]*::5710::3501::?*INSTR";
            status = PviFindRsrc_usb(m_defaultRM_usbtmc, id2, &m_findList_usbtmc, &m_nCount, instrDescriptor);
            if (status < 0L) {
                PviClose_usb(m_defaultRM_usbtmc);
                hUSBTMCLIB = NULL;
                m_defaultRM_usbtmc = 0;
            } else {
                PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
                status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            }
        } else {
            PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
            status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            //Sleep(5000);
            //printf("miernik OK\n\r");
        }
    }

    if (!hUSBTMCLIB) {
        printf("Multimeter device connect failed.\n");
        Sleep(5000);
    }

}

M3500A::~M3500A() {
}

float M3500A::read(){
    len = 64;
    pStrout = new char[len];

    memset(pStrout,0, len);

    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);

    PviWrite_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viWrite");
    PviRead_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viRead");

    // Send read command
    strcpy(pStrout, "READ?\0");
    status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) pStrout, 6, &nWritten);
    Sleep(30);
    //printf(" output : READ?\n");

    status = PviRead_usb(m_instr_usbtmc, pStrin, 64, &nRead);
    if (nRead > 0) {
        for (len = 0; len < (long) nRead; len++) {
            buffer[len] = pStrin[len];
        }
    }
    buffer[nRead] = '\0';
    float pomiar;
    sscanf(buffer,"%f",&pomiar);
    //printf(" input : %s\n\n", buffer);
    return pomiar;
}

void M3500A::sendCommand(char* command,unsigned char* requestBuffer , int commandSize, bool readDevice){

    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);

    PviWrite_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viWrite");
    PviRead_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viRead");

    // Send read command

    status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) command, commandSize, &nWritten);
    Sleep(30);
    //printf(" output : READ?\n");

    if(readDevice){
        status = PviRead_usb(m_instr_usbtmc, requestBuffer, 64, &nRead);
        requestBuffer[nRead] = '\0';
    }
//    if (nRead > 0) {
//        for (int len = 0; len < (long) nRead; len++) {
//            requestBuffer[len] = pStrin[len];
//        }
//    }

    //printf(" input : %s\n\n", buffer);
}

void M3500A::setText(std::string displayText){
    len = 64;
    pStrout = new char[len];
    displayText = "DISPlay:TEXT \"" + displayText + "\"\n\0";
    //printf("%s",displayText.c_str());
    sprintf(pStrout, "%s", displayText.c_str());
    //strcpy(pStrout, displayText.c_str());
    sendCommand(pStrout, pStrin, displayText.length(), false);
    delete pStrout;
}

void M3500A::setMultimetr(float range, float digit, meterMode mMode){
    len = 64;
    pStrout = new char[len];
    std::string command;
    switch (mMode){
    case VOLTage_DC:
        command = "CONFigure:VOLTage:DC";
        break;
    case VOLTage_AC:
        command = "CONFigure:VOLTage:AC";
        break;
    case CURRent_AC:
        command = "CONFigure:CURRent:AC";
        break;
    case CURRent_DC:
        command = "CONFigure:CURRent:DC";
        break;
    case RESistance:
        command = "CONFigure:RESistance";
        break;
    case FRESistance:
        command = "CONFigure:FRESistance";
        break;
    case FREQuency:
        command = "CONFigure:FREQuency";
        break;
     case PERiod:
        command = "CONFigure:PERiod";
        break;
     case CONTinuity:
        command = "CONFigure:CONTinuity";
        break;
     case DIODe:
        command = "CONFigure:DIODe";
        break;
     case TCOuple:
        command = "CONFigure:TCOuple";
        break;
     case TEMPerature:
        command = "CONFigure:TEMPerature";
        break;
    }

    std::string sRange = float_to_string(range);
    std::string sDigit = float_to_string(digit);
    command = command + " " + sRange + "," + sDigit;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);
    delete pStrout;
}

