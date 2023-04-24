#ifndef ACQCONFIG_H
#define ACQCONFIG_H

#include "SapClassBasic.h"

class AcqConfig
{
public:
    AcqConfig();
    bool GetCorAcqDeviceOptionsFromQuestions(char *acqServerName, char *acqDeviceName,int *pAcqDeviceIndex, bool showGigEOnly = false);

protected:
    //SapAcqDevice *m_AcqDevice;
    SapAcquisition m_Acq;		// Internal SapAcquisition object


private:
    SapManager *mSapManager;
    char m_serverName[256];
    int m_resourceIndex;


    bool UpdateAcquisition(char *m_configFile);

};

#endif // ACQCONFIG_H
