#ifndef _AZERO_PLAYER_API_H_
#define _AZERO_PLAYER_API_H_

#ifndef SAI_API
    #if defined(_WIN32) && defined(DLL_EXPORT)
        #define SAI_API __declspec(dllexport)
    #elif defined(_WIN32)
        #define SAI_API __declspec(dllimport)
    #elif defined(__GNUC__) && defined(DLL_EXPORT)
        #define SAI_API __attribute__((visibility("default")))
    #else
        #define SAI_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
* Enum of media player comman commands.
*/
typedef enum{

    APLAYER_PLAY = 100,
    APLAYER_PAUSE,
    APLAYER_STOP,
    APLAYER_RESUME,

} eAzeroPlayerCMD;

/**
* Enum of media player IO control commands.
*/
typedef enum{

    APLAYER_SETLOOPMODE = 200,
	APLAYER_GETLOOPMODE,
	APLAYER_SETPOSTION,                     /* seek and play */
	APLAYER_GETPOSTION,
	APLAYER_SETMUTE,
	APLAYER_GETMUTE,
    APLAYER_SETVOLUME,
	APLAYER_GETVOLUME,
    APLAYER_ADJVOLUME,
} eAzeroPlayerIOctrlCMD;

/**
* Enum of media player status.
*/
typedef enum{

    PLAYERSTATE_OPENING = 300,             /* Optional */
    PLAYERSTATE_BUFFERING,                 /* Optional */
    PLAYERSTATE_PLAYING,
    PLAYERSTATE_PAUSED,
    PLAYERSTATE_FINISHED,
    PLAYERSTATE_STOPPED,
    PLAYERSTATE_SEEKING,                   /* Optional */
	PLAYERSTATE_ERROR_NETWORK,
    PLAYERSTATE_ERROR_FORMAT,
    PLAYERSTATE_ERROR,                     /* ERROR except network & unsupported format  */
    PLAYERSTATE_IDLE,
    PLAYERSTATE_GETFAILED,                 /* For get player status failed */  
} eAzeroPlayerStatus;


/**
* Enum of media player types.
*   APLAYER_TTSPCM is optional.
*/
typedef enum{

//    APLAYER_LOCALFS = 1,                       /* local file: wav & mp3 */
    APLAYER_LOCALMP3 = 1,                      /* local mp3 file */
    APLAYER_URL,                               /* http or https */
    APLAYER_TTSMP3,                            /* mp3 stream: 24kbps  */
    APLAYER_TTSPCM,                            /* pcm stream: 16khz 16bit mono LE */
} eAzeroSourceType;

/**
* Enum of media player types.
*   APLAYER_TTSPCM is optional.
*/
typedef enum{
    azeroDATA_TYPE_MP3,
    azeroDATA_TYPE_AAC,
    azeroDATA_TYPE_M4A,
    azeroDATA_TYPE_WAV,
    azeroDATA_TYPE_TS,
    azeroDATA_TYPE_AMR,
    azeroDATA_TYPE_SPX,
    azeroDATA_TYPE_SBC,
    azeroDATA_TYPE_FLAC,
    azeroDATA_TYPE_NULL,
}eDataFormat;

/**
* Enum of media player types.
*   APLAYER_TTSPCM is optional.
*/
typedef struct {
    char    url[512];
    int     totalLen;
} tDataSourceInfo;


typedef struct sAzeroPlayerHandler  tAzeroPlayerHandler;

/**
* Structure of a media player instance.
*   Instance_NUM :               the player Instance NO.
*   azero_player_status_evt_cb : report player states callback function, called by player monitor.
*   azero_player_TTS_feed :      feed mp3/pcm TTS data to player, called by Azero SDK.  
*                                return -1 incase error;
*   azero_player_cmd :           send eAzeroPlayerCMD to player, pSource is valid for APLAYER_PLAY.  
*                                return -1 incase error;
*   azero_player_ioctrl :        ioctrol for player, return result for get cmd.
*                                return -1 incase error;
*   azero_player_get_cur_play_status :    get player status.
*                                         return PLAYERSTATE_GETFAILED incase get failed.
*   azero_player_get_music_format :       get music format.
*                                         return DATA_TYPE_NULL incase get failed.
*/
struct sAzeroPlayerHandler
{
  uint8_t            InstanceNUM;                                /* The player Instance NO. [1~5], instead of name */
  eDataFormat        MusicFormat;                                /* auto set by player*/
  int                (*azero_player_status_evt_cb)(tAzeroPlayerHandler*  pPlayer, eAzeroPlayerStatus status_event);
  int                (*azero_player_TTS_feed)(tAzeroPlayerHandler*  pPlayer, const void *data,int dataLen);
  int                (*azero_player_cmd)( tAzeroPlayerHandler *  pPlayerInstance,  eAzeroPlayerCMD eCMD,  tDataSourceInfo* pSource);
  int			     (*azero_player_ioctrl)( tAzeroPlayerHandler *  pPlayerInstance,  eAzeroPlayerIOctrlCMD eIOCTRLCMD,  int param);
  eAzeroPlayerStatus (*azero_player_get_cur_play_status)(tAzeroPlayerHandler*  pPlayer);
  eDataFormat        (*azero_player_get_music_format)(tAzeroPlayerHandler*  pPlayer);            /* optional */
};


/**
* Called after hw init, creat one player instance which supports the specified music type;
* Init & implement callback functions in structure tAzeroPlayerHandler
*
* @param [in] pPlayerType Specified music type which this player must support.
*
* @return A pointer of tAzeroPlayerHandler instance.
*/
SAI_API tAzeroPlayerHandler* azero_player_init( eAzeroSourceType pPlayerType );
 

#ifdef __cplusplus
}
#endif
#endif
