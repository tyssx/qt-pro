#include "acqconfig.h"
#include <QDebug>

AcqConfig::AcqConfig()
{
    mSapManager = new SapManager;
}

bool AcqConfig::GetCorAcqDeviceOptionsFromQuestions(char *acqServerName, char *acqDeviceName,int *pAcqDeviceIndex, bool showGigEOnly)
{
    //获取服务的数量
    int serverCount = SapManager::GetServerCount();
    int acqDeviceCount = 0;
    int deviceCount = 0;
    int GenieIndex = 0;
    int deviceIndex;
    std::vector<std::string> v_ServerNames;
    if(serverCount == 0) //没有服务
    {
        qDebug() << "No device found!";
        return false;
    }
    char serverName[CORSERVER_MAX_STRLEN];
    for(int serverAcqIndex = 0;serverAcqIndex < serverCount;serverAcqIndex++)
    {
        if(SapManager::GetResourceCount(serverAcqIndex,SapManager::ResourceAcqDevice) != 0)
        {
            SapManager::GetServerName(serverAcqIndex,serverName,sizeof (serverName));
            acqDeviceCount++;

            if(showGigEOnly)
            {
                if(strstr(serverName,"CameraLink_") != 0 || SapManager::GetResourceCount(serverAcqIndex,SapManager::ResourceAcq) != 0)
                    acqDeviceCount--;
            }
        }
    }

    //使用的这里
    bool serverFound = false;
    for(int serverIndex = 0; serverIndex < serverCount; serverIndex++)
    {
        if(SapManager::GetResourceCount(serverIndex,SapManager::ResourceAcqDevice) != 0)
        {
            SapManager::GetServerName(serverIndex,serverName,sizeof (serverName));
            if(showGigEOnly)
            {
                if(strstr(serverName,"CameraLink_") != 0 || SapManager::GetResourceCount(serverIndex,SapManager::ResourceAcq) != 0)
                    continue;
            }
            qDebug() << GenieIndex+1 << ": " << serverName;  //Linea_M4096-7um_1
            GenieIndex++;
            serverFound = true;
            deviceCount = GenieIndex;

            char deviceName[CORPRM_GETSIZE(CORACQ_PRM_LABEL)];
            deviceIndex = 0;
            SapManager::GetResourceName(serverName,SapManager::ResourceAcqDevice,deviceIndex,deviceName,sizeof (deviceName));
            qDebug() << "User defined Name : " << deviceName;
            CorStrncpy(acqDeviceName,deviceName,sizeof (deviceName));  //12205585
            v_ServerNames.push_back(serverName);
        }
    }

    if(!serverFound)
    {
        qDebug() << "No camera found!";
        return false;
    }

    //目前只给一个服务及设备
    std::string sServerName = v_ServerNames[0];
    CorStrncpy(acqServerName,sServerName.c_str(),sServerName.size()+1);
    *pAcqDeviceIndex = 0; //第几个设备

    return true;
}

