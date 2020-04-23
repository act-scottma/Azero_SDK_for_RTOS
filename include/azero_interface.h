#ifndef _azeros_interface_
#define _azeros_interface_

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

#include "time.h"
#include "stdint.h"
#include "stdio.h"

/**
* Get network time callback.
*
* @param [out] *t  time result.
* @return EXIT_SUCCESS: successful, EXIT_FAILURE: failed.
*/
typedef int (*cbAzeroGetNetworkTime)(time_t *t);

/**
* Get IP address callback.
*
* @param [out] *ipAddress  ipAddress strings pointer.
* @return EXIT_SUCCESS: successful, EXIT_FAILURE: failed.
*/
typedef int (*cbAzeroGetIPAddress)(char *ipAddress);

/**
* Get MAC address callback.
*
* @param [out] *mac  mac address: mac[0] ... mac[5].
* @return EXIT_SUCCESS: successful, EXIT_FAILURE: failed.
*/
typedef int (*cbAzeroGetMACAddress)(uint8_t *mac);

/**
* Log print function callback.
*
* @param [in] *string  string to be printed.
*/
typedef void (*cbAzeroLogPrintCallBack)(const char* string);

/**
* Set sys volume callback.
*
* @param [in] vol  volume to be set [0~100].
*/
typedef void (*cbAzeroSetVolume)(uint8_t vol);

/**
* Get sys volume callback.
*
* @param [out] vol  volume returned by sys/application [0~100].
*/
typedef void (*cbAzeroGetVolume)(uint8_t *vol);

/**
* callback types.
*/
typedef uint8_t (*cbAzeroCharNoargs)();

typedef struct tAzeroCallback
{
    cbAzeroGetNetworkTime getNetworkTimeCallback;
    cbAzeroGetIPAddress   getIPAddressCallback;
    cbAzeroGetMACAddress  getMACAddressCallback;
    cbAzeroLogPrintCallBack logPrintCallback;
    cbAzeroSetVolume setVolumeCallback;
    cbAzeroGetVolume getVolumeCallback;
    cbAzeroCharNoargs isInIrqCallBack;
    cbAzeroCharNoargs isInHandlerModeCallBack;
    cbAzeroCharNoargs forceNVICSysresetCallBack;
}tAzeroCallback;

typedef enum eAzeroDeviceInfo
{
    AZERO_CLIENT_ID,
    AZERO_PRODUCT_ID,
    AZERO_SERIAL_NUMBER
} eAzeroDeviceInfo;

/**
* Init the customer device info.
* Must be set before azero_service_init()
* type include clientId, productId, serialNumber
*
* @param [type] the information type to be set.
* @param [value] th information value to be set.
* @return -1: failed.
*/
int azero_set_device_info(eAzeroDeviceInfo type, const char* value);

/**
* Init the sdk service.
*
* @param [in] cbs The callback functions that implement by application.
* @return -1: failed.
*/
int azero_service_init(tAzeroCallback cbs);

typedef enum eAzeroWakeupType
{
    AZERO_WAKEUP_STOP,
    AZERO_WAKEUP_START
} eAzeroWakeupType;

/**
* Set SDK in wakeup status then ready to capture audio data for ASR.
*
* @param [in] status  set 1 to wakeup the SDK and 0 to unwakeup it.
* @return EXIT_SUCCESS: successful, EXIT_FAILURE: failed.
*/
int azero_set_wakeup_status(eAzeroWakeupType type);


/**
* Input audio data to Azero SDK, currently only 16k/16bit/mono raw data supported.
* 
* @param [in] buf The data buffer bear audio data.
* @param [in] size The date size once feed, 512 bytes is recomend.
* @return -1: if failed, the others is successfully. 
*/
size_t azero_audio_data_input_mono(const char* buf, size_t size);


#ifdef __cplusplus
}
#endif
#endif
