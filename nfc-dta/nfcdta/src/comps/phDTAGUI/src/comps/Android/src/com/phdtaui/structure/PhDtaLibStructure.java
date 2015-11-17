/*
*                    Copyright (c), NXP Semiconductors
*
*                       (C)NXP Semiconductors B.V.2014
*         All rights are reserved. Reproduction in whole or in part is
*        prohibited without the written consent of the copyright owner.
*    NXP reserves the right to make changes without notice at any time.
*   NXP makes no warranty, expressed, implied or statutory, including but
*   not limited to any implied warranty of merchantability or fitness for any
*  particular purpose, or that the use will not infringe any third party patent,
*   copyright or trademark. NXP must not be liable for any loss or damage
*                            arising from its use.
*
*/

package com.phdtaui.structure;
import android.util.Log;
import com.phdtaui.helper.PhDTAJniEvent;
import com.phdtaui.utils.PhUtilities;

public class PhDtaLibStructure
{
    /*Version information for Underlying components*/
    public int dtaLibVerMajor, dtaLibVerMinor;
    public int mwVerMajor,mwVerMinor;
    public int fwVerMajor,fwVerMinor;
    /* P2P in LLCP & SNEP selecions*/
    public boolean enableLlcp;
    public boolean enableParamsInLlcpConnectPdu;
    public boolean enableSnep;
    /* pattern number*/
    public int patternNum;
    /*Enable Polling modes*/
    public boolean pollP2P;
    public boolean pollRdWt;
    /*Enable listen modes*/
    public boolean listenP2P;
    public boolean listenUicc;
    public boolean listenHce;
    public boolean listenEse;

    public void initializeRfTechObjs()
    {
        if(pollP2pRfTech == null)
        {
            pollP2pRfTech   = new PhRfTechnology();
            pollRdWtRfTech  = new PhRfTechnology();
            listenP2pRfTech = new PhRfTechnology();
            listenUiccRfTech= new PhRfTechnology();
            listenHceRfTech = new PhRfTechnology();
            listenEseRfTech = new PhRfTechnology();
        }
    }
    public int getDtaLibVerMajor() {
        return dtaLibVerMajor;
    }
    public void setDtaLibVerMajor(int dtaLib_VerMajor) {
        dtaLibVerMajor = dtaLib_VerMajor;
    }
    public int getDtaLibVerMinor() {
        return dtaLibVerMinor;
    }
    public void setDtaLibVerMinor(int dtaLib_VerMinor) {
        dtaLibVerMinor = dtaLib_VerMinor;
    }
    public int getLibNfcVerMajor() {
        return mwVerMajor;
    }
    public void setLibNfcVerMajor(int libNfc_VerMajor) {
        mwVerMajor = libNfc_VerMajor;
    }
    public int getLibNfcVerMinor() {
        return mwVerMinor;
    }
    public void setLibNfcVerMinor(int libNfc_VerMinor) {
    mwVerMinor = libNfc_VerMinor;
    }
    public int getFwVerMajor() {
        return fwVerMajor;
    }
    public void setFwVerMajor(int fw_VerMajor) {
        fwVerMajor = fw_VerMajor;
    }
    public int getFwVerMinor() {
        return fwVerMinor;
    }
    public void setFwVerMinor(int fw_VerMinor) {
        fwVerMinor = fw_VerMinor;
    }
    public int getPatternNum() {
        return patternNum;
    }
    public void setPatternNum(int patternNum) {
        this.patternNum = patternNum;
    }
    public void phDtaUIi_EvtCb(int Event, int EventData)
    {
        PhDTAJniEvent eEvent= new PhDTAJniEvent(PhDTAJniEvent.EventType.values()[Event]);
        switch(eEvent.evtType)
        {
            case PHDTALIB_TEST_CASE_EXEC_STARTED:
                PhUtilities.STOP_BUTTON_HANDLE=PhDTAJniEvent.EventType.PHDTALIB_TEST_CASE_EXEC_STARTED.ordinal();
                Log.d("UIDTA", "PHDTALIB_TEST_CASE_EXEC_STARTED phDtaUIi_EvtCb");
            break;
            case PHDTALIB_TEST_CASE_EXEC_COMPLETED:
                PhUtilities.STOP_BUTTON_HANDLE=PhDTAJniEvent.EventType.PHDTALIB_TEST_CASE_EXEC_COMPLETED.ordinal();
                Log.d("UIDTA", "PHDTALIB_TEST_CASE_EXEC_COMPLETED phDtaUIi_EvtCb");
            break;
            default:
                Log.d("UIDTA", "Invalid event phDtaUIi_EvtCb");
            break;
        }
    }
    /*RF technology types to enable for each poll/listen configurations*/
    public class PhRfTechnology{
        public boolean technologyA;
        public boolean technologyB;
        public boolean technologyF;
    }

    public PhRfTechnology pollP2pRfTech;
    public PhRfTechnology pollRdWtRfTech;
    public PhRfTechnology listenP2pRfTech;
    public PhRfTechnology listenUiccRfTech;
    public PhRfTechnology listenHceRfTech;
    public PhRfTechnology listenEseRfTech;
}
