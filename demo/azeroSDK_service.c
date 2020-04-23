#include <stdlib.h>
#include <string.h>
#include "azeroSDK_service.h"
#include "azero_interface.h"
#include "azero_player_interface.h"

#include "reboot.h"
#include "osal.h"
#include "uplayer.h"

#include "ff_headers.h"
#include "ff_stdio.h"

static int net_time_get = 1; //0
extern TaskHandle_t azeroSDK_service;
extern TaskHandle_t azeroSDK_feeddata;

int azero_get_network_time_cb(time_t *t)
{
    time_t ptime;
    time(&ptime);
    *t = ptime;
    if(net_time_get == 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int azero_get_ip_address_cb(char *ipAddress)
{
    if(net_time_get == 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        char ip[] = "192.168.1.1";
        strcpy(ipAddress,ip);
        return EXIT_SUCCESS;
    }
}

int azero_get_mac_address_cb(uint8_t *mac)
{
    mac[0] = 0xAA;
    mac[1] = 0xEE;
    mac[2] = 0xEE;
    mac[3] = 0xEE;
    mac[4] = 0xEE;
    mac[5] = 0xEE;
    return EXIT_SUCCESS;
}

void azero_log_print_cb(const char* string)
{
	printf("%s",string);
}

void azero_setVolume_cb(unsigned char vol)
{
	// todo:
}

void azero_getVolume_cb(unsigned char vol)
{
	// todo:
	vol = 100;
}

unsigned char azero_isInIrq_cb()
{
	// user interface :
	//printf("%s--%d\n",__FUNCTION__,__LINE__);
    // return (!OSAL_InThreadMode());
	return 0;
}

unsigned char azero_isInHandlerMode_cb()
{
	// user interface :
	//printf("%s--%d\n",__FUNCTION__,__LINE__);
    // return OSAL_InThreadMode();
	return 1;
}

unsigned char azero_forceNVICSysreset_cb()
{
	// user interface :
	  printf("%s--%d\n",__FUNCTION__,__LINE__);
      //cmd_reset(0,NULL); //reboot
    return 1;
}

void azeroSDK_start(void)
{
	  printf("%s--%d\n",__FUNCTION__,__LINE__);
    tAzeroCallback cbs = 
    {
        .getNetworkTimeCallback = azero_get_network_time_cb,\
        .getIPAddressCallback = azero_get_ip_address_cb,\
        .getMACAddressCallback = azero_get_mac_address_cb,\
        .logPrintCallback = azero_log_print_cb,\
		.setVolumeCallback = azero_setVolume_cb,\
        .getVolumeCallback = azero_getVolume_cb,\
		.isInIrqCallBack = azero_isInIrq_cb,\
		.isInHandlerModeCallBack = azero_isInHandlerMode_cb,\
		.forceNVICSysresetCallBack = azero_forceNVICSysreset_cb
    };

		vTaskDelay(pdMS_TO_TICKS(10000)); //sleep(10);
    azero_set_device_info(AZERO_CLIENT_ID, "XXXXXXXXXXXXXXXX");
    azero_set_device_info(AZERO_PRODUCT_ID, "XXXXXXX");
    azero_set_device_info(AZERO_SERIAL_NUMBER, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    printf("%s--%d\n",__FUNCTION__,__LINE__);
    azero_service_init(cbs);
    printf("%s--%d\n",__FUNCTION__,__LINE__);
		
	vTaskDelete(azeroSDK_service);
}


/******************************* feed data ***********************/

void azeroSDK_feed_data(void)
{
	/* todo */
	while(1);
}


/******************************* player ***********************/

tAzeroPlayerHandler *gFSFileFakePlayer;
tAzeroPlayerHandler *gURLFakePlayer;
tAzeroPlayerHandler *gTTSFakePlayer;

static void uplayer_URL_notifier(DataSourceInfo * audioInfo){
    eAzeroPlayerStatus cbstatus;
    printf("status:%d\n\r",audioInfo->status);
    printf("type:%d\n\r",audioInfo->type);
    switch(audioInfo->status)
    {
        case UPLAYER_PLAYING:
            cbstatus = PLAYERSTATE_PLAYING;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        case UPLAYER_PAUSED:
            cbstatus = PLAYERSTATE_PAUSED;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        case UPLAYER_FINISHED:
            cbstatus = PLAYERSTATE_FINISHED;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        case UPLAYER_STOPPED:
            cbstatus = PLAYERSTATE_STOPPED;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        case UPLAYER_ERROR:
            cbstatus = PLAYERSTATE_ERROR;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        case UPLAYER_IDLE:
            cbstatus = PLAYERSTATE_IDLE;
            gURLFakePlayer->azero_player_status_evt_cb(gURLFakePlayer, cbstatus);
            break;
        default:
            break;
    }
}

int extnel_player_cmd( tAzeroPlayerHandler *  pPlayerInstance,  eAzeroPlayerCMD eCMD,  tDataSourceInfo* pSource)
{
	printf("player=%d,cmd=%d.\n",pPlayerInstance->InstanceNUM, eCMD);
    DataSourceInfo info;
    memset(&info,0,sizeof(DataSourceInfo));
    if(pPlayerInstance->InstanceNUM == 1)  // local mp3
    {
        switch(eCMD)
        {
            case APLAYER_PLAY:
                if(pSource == NULL)
                {
                    printf("error:source is NULL! %s--%d\n", __FUNCTION__,__LINE__);
                    break;
                }
								printf("url= %s\n", (pSource->url));
            	  uplayer_prompt_audio_play(pSource->url);
		        //uplayer_prompt_audio_play("/sai_config/wakeup/wakeup_1.mp3");
                break;
            case APLAYER_PAUSE:
                uplayer_audio_pause();
                break;
            case APLAYER_STOP:
                uplayer_audio_stop();
                break;  
            case APLAYER_RESUME:
                uplayer_audio_resume();
                break;
            default:
                break;
        }

    } else if(pPlayerInstance->InstanceNUM == 2)  // URL
    {
        switch(eCMD)
        {
            case APLAYER_PLAY:
                if(pSource == NULL)
                {
                    printf("error:source is NULL! %s--%d\n", __FUNCTION__,__LINE__);
                    break;
                }
                sprintf(info.url,"%s",(pSource->url));
                info.notify = uplayer_URL_notifier;
            	uplayer_audio_play(&info);
                break;
            case APLAYER_PAUSE:
                uplayer_audio_pause();
                break;
            case APLAYER_STOP:
                uplayer_audio_stop();
                break;  
            case APLAYER_RESUME:
                uplayer_audio_resume();
                break;
            default:
                break;
        }
    } else if(pPlayerInstance->InstanceNUM == 3)  // mp3 stream
    {
        switch(eCMD)
        {
            case APLAYER_PLAY:
                uplayer_buffer_audio_start(DATA_TYPE_MP3);
                break;
            case APLAYER_PAUSE:
                uplayer_audio_pause();
                break;
            case APLAYER_STOP:
                uplayer_audio_stop();
                break;
            case APLAYER_RESUME:
                uplayer_audio_resume();
                break;
            defalut:
                break;
        }
    }
}

int extnel_player_TTS_feed(tAzeroPlayerHandler*  pPlayer, const void *data,int dataLen)
{
    int ret = 0;
	
    static int audio_record_pxFile = 0;

    if (audio_record_pxFile == 0)
    {
        audio_record_pxFile = ff_fopen("TTStest.mp3", "w" ); //open("TTStest.mp3", 2);
        printf("pxFile open ret %x, %p, %d.\n", audio_record_pxFile, data, dataLen);
    } else {
				printf("data ptr=%p,len=%d.\n", data, dataLen);
		} 

    if (dataLen > 0)
    {
        //ff_fwrite(data, 1, dataLen, (FF_FILE *)audio_record_pxFile);//write(audio_record_pxFile, data, dataLen);
        ret = uplayer_buffer_audio_put((unsigned char*)data,dataLen);  // dataLen by bytes
        if (ret == -1){
            printf("failed to put tts data to uplayer.\n");
        }
    } else 
    {
        //ff_fclose((FF_FILE *)audio_record_pxFile); //close(audio_record_pxFile);
        uplayer_buffer_audio_put(DATA_TYPE_NULL,0);
        uplayer_buffer_audio_until_empty();  // blocking ?
        printf("no more data \n");
    }
    return ret;
}


tAzeroPlayerHandler* azero_player_init( eAzeroSourceType pPlayerType )
{
    tAzeroPlayerHandler *FakePlayer;
	printf("Azero SDK:creat player type=%d,%s--%d\n",pPlayerType,__FUNCTION__,__LINE__);
    switch(pPlayerType)
    {
        case APLAYER_LOCALMP3:
                gFSFileFakePlayer = malloc(sizeof(tAzeroPlayerHandler));
                if (gFSFileFakePlayer == NULL) {
                    printf("init error: %s--%d\n",__FUNCTION__,__LINE__);
                }
                FakePlayer = gFSFileFakePlayer;
                gFSFileFakePlayer->InstanceNUM = 1;  // 1 for APLAYER_LOCALMP3
                gFSFileFakePlayer->azero_player_cmd = extnel_player_cmd;
            break;
        case APLAYER_URL:
                gURLFakePlayer = malloc(sizeof(tAzeroPlayerHandler));
                if (gURLFakePlayer == NULL) {
                    printf("init error: %s--%d\n",__FUNCTION__,__LINE__);
                }
                FakePlayer = gURLFakePlayer;
                gURLFakePlayer->InstanceNUM = 2;  // 2 for APLAYER_URL
                gURLFakePlayer->azero_player_cmd = extnel_player_cmd;
            break;
        case APLAYER_TTSMP3:
                gTTSFakePlayer = malloc(sizeof(tAzeroPlayerHandler));
                if (gTTSFakePlayer == NULL) {
                    printf("init error: %s--%d\n",__FUNCTION__,__LINE__);
                }
                FakePlayer = gTTSFakePlayer;
                gTTSFakePlayer->InstanceNUM = 3;  // 3 for APLAYER_TTSMP3
                gTTSFakePlayer->azero_player_cmd = extnel_player_cmd;
                gTTSFakePlayer->azero_player_TTS_feed = extnel_player_TTS_feed;
            break;

        default:
                printf("init error: %s--%d\n",__FUNCTION__,__LINE__);
            break;
    }
		//uplayer_audio_set_volume(15);  // 0~15

    return FakePlayer;
}