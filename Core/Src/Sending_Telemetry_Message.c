/*
 * Sending_Telemetry_Message.c
 *
 *  Created on: Apr 21, 2022
 *      Author: domin
 */


#include <stdio.h>

#include "nx_api.h"
#include "nx_azure_iot_hub_client.h"
#include "nx_azure_iot_hub_client_properties.h"
#include "nx_azure_iot_provisioning_client.h"

/* These are sample files, user can build their own certificate and ciphersuites.  */
#include "nx_azure_iot_cert.h"
#include "nx_azure_iot_ciphersuites.h"
#include "sample_config.h"

#include "stm32l4s5i_iot01.h"
#include "stm32l4s5i_iot01_accelero.h"
#include "stm32l4s5i_iot01_gyro.h"
#include "stm32l4s5i_iot01_hsensor.h"
#include "stm32l4s5i_iot01_magneto.h"
#include "stm32l4s5i_iot01_psensor.h"
#include "stm32l4s5i_iot01_tsensor.h"

#define TELEMETRY_HUMIDITY          "humidity"
#define TELEMETRY_TEMPERATURE       "temperature"
#define TELEMETRY_PRESSURE          "pressure"
#define TELEMETRY_MAGNETOMETERX     "magnetometerX"
#define TELEMETRY_MAGNETOMETERY     "magnetometerY"
#define TELEMETRY_MAGNETOMETERZ     "magnetometerZ"
#define TELEMETRY_ACCELEROMETERX    "accelerometerX"
#define TELEMETRY_ACCELEROMETERY    "accelerometerY"
#define TELEMETRY_ACCELEROMETERZ    "accelerometerZ"
#define TELEMETRY_GYROSCOPEX        "gyroscopeX"
#define TELEMETRY_GYROSCOPEY        "gyroscopeY"
#define TELEMETRY_GYROSCOPEZ        "gyroscopeZ"
#define TELEMETRY_INTERVAL_PROPERTY "telemetryInterval"
#define LED_STATE_PROPERTY          "ledState"
#define SET_LED_STATE_COMMAND       "setLedState"

static const CHAR temperature_component[] = "Temperature";
static const CHAR humidity_component[] = "Humidity";
static const CHAR pressure_component[] = "Pressure";
static const CHAR magnetometer_component[] = "Magnetometer";
static const CHAR accelerometer_component[] = "Accelerometer";
static const CHAR gyroscope_component[] = "Gyroscope";



float Humidity;
float Temp;
float Pressure;
int16_t Magnetometer[3];
int16_t Accelerometr[3];
int16_t Gyroscope[3];
bool LedState;



#ifndef SAMPLE_WAIT_OPTION
#define SAMPLE_WAIT_OPTION                                              (NX_NO_WAIT)
#endif /* SAMPLE_WAIT_OPTION */

/* Sample events.  */
#define SAMPLE_ALL_EVENTS                                               ((ULONG)0xFFFFFFFF)
#define SAMPLE_CONNECTED_EVENT                                          ((ULONG)0x00000001)
#define SAMPLE_DISCONNECT_EVENT                                         ((ULONG)0x00000002)
#define SAMPLE_PERIODIC_EVENT                                           ((ULONG)0x00000004)
#define SAMPLE_TELEMETRY_SEND_EVENT                                     ((ULONG)0x00000008)
#define SAMPLE_COMMAND_RECEIVE_EVENT                                    ((ULONG)0x00000010)
#define SAMPLE_PROPERTIES_RECEIVE_EVENT                                 ((ULONG)0x00000020)
#define SAMPLE_WRITABLE_PROPERTIES_RECEIVE_EVENT                        ((ULONG)0x00000040)
#define SAMPLE_REPORTED_PROPERTIES_SEND_EVENT                           ((ULONG)0x00000080)

#define SAMPLE_DEFAULT_START_TEMP_CELSIUS                               (22)
#define DOUBLE_DECIMAL_PLACE_DIGITS                                     (2)
#define SAMPLE_COMMAND_SUCCESS_STATUS                                   (200)
#define SAMPLE_COMMAND_ERROR_STATUS                                     (500)
#define SAMPLE_COMMAND_NOT_FOUND_STATUS                                 (404)

#define SAMPLE_PNP_MODEL_ID                                             "dtmi:testowanko512:InterfaceSensor_7i4;1"
#define SAMPLE_PNP_DPS_PAYLOAD                                          "{\"modelId\":\"" SAMPLE_PNP_MODEL_ID "\"}"

/* Generally, IoTHub Client and DPS Client do not run at the same time, user can use union as below to
   share the memory between IoTHub Client and DPS Client.

   NOTE: If user can not make sure sharing memory is safe, IoTHub Client and DPS Client must be defined seperately.  */
typedef union SAMPLE_CLIENT_UNION
{
    NX_AZURE_IOT_HUB_CLIENT                         iothub_client;

#ifdef ENABLE_DPS_SAMPLE
    NX_AZURE_IOT_PROVISIONING_CLIENT                prov_client;
#endif /* ENABLE_DPS_SAMPLE */

} SAMPLE_CLIENT;

static SAMPLE_CLIENT                                client;

#define iothub_client client.iothub_client
#ifdef ENABLE_DPS_SAMPLE
#define prov_client client.prov_client
#endif /* ENABLE_DPS_SAMPLE */

VOID sample_entry(NX_IP *ip_ptr, NX_PACKET_POOL *pool_ptr, NX_DNS *dns_ptr, UINT (*unix_time_callback)(ULONG *unix_time));

#ifdef ENABLE_DPS_SAMPLE
static UINT sample_dps_entry(NX_AZURE_IOT_PROVISIONING_CLIENT *prov_client_ptr,
                             UCHAR **iothub_hostname, UINT *iothub_hostname_length,
                             UCHAR **iothub_device_id, UINT *iothub_device_id_length);
#endif /* ENABLE_DPS_SAMPLE */

/* Define Azure RTOS TLS info.  */
static NX_SECURE_X509_CERT root_ca_cert;
static NX_SECURE_X509_CERT root_ca_cert_2;
static NX_SECURE_X509_CERT root_ca_cert_3;
static UCHAR nx_azure_iot_tls_metadata_buffer[NX_AZURE_IOT_TLS_METADATA_BUFFER_SIZE];
static ULONG nx_azure_iot_thread_stack[NX_AZURE_IOT_STACK_SIZE / sizeof(ULONG)];

/* Using X509 certificate authenticate to connect to IoT Hub,
   set the device certificate as your device.  */
#if (USE_DEVICE_CERTIFICATE == 1)
extern const UCHAR sample_device_cert_ptr[];
extern const UINT sample_device_cert_len;
extern const UCHAR sample_device_private_key_ptr[];
extern const UINT sample_device_private_key_len;
NX_SECURE_X509_CERT device_certificate;
#endif /* USE_DEVICE_CERTIFICATE */

/* Define buffer for IoTHub info.  */
#ifdef ENABLE_DPS_SAMPLE
static UCHAR sample_iothub_hostname[SAMPLE_MAX_BUFFER];
static UCHAR sample_iothub_device_id[SAMPLE_MAX_BUFFER];
#endif /* ENABLE_DPS_SAMPLE */

/* Define the prototypes for AZ IoT.  */
static NX_AZURE_IOT nx_azure_iot;

static TX_EVENT_FLAGS_GROUP sample_events;
static TX_TIMER sample_timer;
static volatile UINT sample_connection_status = NX_AZURE_IOT_NOT_INITIALIZED;
static volatile ULONG sample_periodic_counter = 0;

static UCHAR scratch_buffer[2048];

/* Include the connection monitor function from sample_azure_iot_embedded_sdk_connect.c.  */
extern VOID sample_connection_monitor(NX_IP *ip_ptr, NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr, UINT connection_status,
                                      UINT (*iothub_init)(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr));




static VOID sample_periodic_timer_entry(ULONG context)
{

    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&(sample_events), SAMPLE_PERIODIC_EVENT, TX_OR);
}




#ifdef ENABLE_DPS_SAMPLE
static UINT sample_dps_entry(NX_AZURE_IOT_PROVISIONING_CLIENT *prov_client_ptr,
                             UCHAR **iothub_hostname, UINT *iothub_hostname_length,
                             UCHAR **iothub_device_id, UINT *iothub_device_id_length)
{
UINT status;

    printf("Start Provisioning Client...\r\n");

    /* Initialize IoT provisioning client.  */
    if ((status = nx_azure_iot_provisioning_client_initialize(prov_client_ptr, &nx_azure_iot,
                                                              (UCHAR *)ENDPOINT, sizeof(ENDPOINT) - 1,
                                                              (UCHAR *)ID_SCOPE, sizeof(ID_SCOPE) - 1,
                                                              (UCHAR *)REGISTRATION_ID, sizeof(REGISTRATION_ID) - 1,
                                                              _nx_azure_iot_tls_supported_crypto,
                                                              _nx_azure_iot_tls_supported_crypto_size,
                                                              _nx_azure_iot_tls_ciphersuite_map,
                                                              _nx_azure_iot_tls_ciphersuite_map_size,
                                                              nx_azure_iot_tls_metadata_buffer,
                                                              sizeof(nx_azure_iot_tls_metadata_buffer),
                                                              &root_ca_cert)))
    {

        printf("Failed on nx_azure_iot_provisioning_client_initialize!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Initialize length of hostname and device ID.  */
    *iothub_hostname_length = sizeof(sample_iothub_hostname);
    *iothub_device_id_length = sizeof(sample_iothub_device_id);

    /* Add more CA certificates.  */
    if ((status = nx_azure_iot_provisioning_client_trusted_cert_add(prov_client_ptr, &root_ca_cert_2)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_trusted_cert_add!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_provisioning_client_trusted_cert_add(prov_client_ptr, &root_ca_cert_3)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_trusted_cert_add!: error code = 0x%08x\r\n", status);
    }

#if (USE_DEVICE_CERTIFICATE == 1)

    /* Initialize the device certificate.  */
    else if ((status = nx_secure_x509_certificate_initialize(&device_certificate, (UCHAR *)sample_device_cert_ptr, (USHORT)sample_device_cert_len, NX_NULL, 0,
                                                             (UCHAR *)sample_device_private_key_ptr, (USHORT)sample_device_private_key_len, DEVICE_KEY_TYPE)))
    {
        printf("Failed on nx_secure_x509_certificate_initialize!: error code = 0x%08x\r\n", status);
    }

    /* Set device certificate.  */
    else if ((status = nx_azure_iot_provisioning_client_device_cert_set(prov_client_ptr, &device_certificate)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_device_cert_set!: error code = 0x%08x\r\n", status);
    }
#else

    /* Set symmetric key.  */
    else if ((status = nx_azure_iot_provisioning_client_symmetric_key_set(prov_client_ptr, (UCHAR *)DEVICE_SYMMETRIC_KEY,
                                                                          sizeof(DEVICE_SYMMETRIC_KEY) - 1)))
    {



    	// Zaimplementuj tutaj mozliwosc dodania danych po NFC



        printf("Failed on nx_azure_iot_hub_client_symmetric_key_set!: error code = 0x%08x\r\n", status);
    }
#endif /* USE_DEVICE_CERTIFICATE */
    else if ((status = nx_azure_iot_provisioning_client_registration_payload_set(prov_client_ptr, (UCHAR *)SAMPLE_PNP_DPS_PAYLOAD,
                                                                                 sizeof(SAMPLE_PNP_DPS_PAYLOAD) - 1)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_registration_payload_set!: error code = 0x%08x\r\n", status);
    }

    /* Register device */
    else if ((status = nx_azure_iot_provisioning_client_register(prov_client_ptr, NX_WAIT_FOREVER)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_register!: error code = 0x%08x\r\n", status);
    }

    /* Get Device info */
    else if ((status = nx_azure_iot_provisioning_client_iothub_device_info_get(prov_client_ptr,
                                                                               sample_iothub_hostname, iothub_hostname_length,
                                                                               sample_iothub_device_id, iothub_device_id_length)))
    {
        printf("Failed on nx_azure_iot_provisioning_client_iothub_device_info_get!: error code = 0x%08x\r\n", status);
    }
    else
    {
        *iothub_hostname = sample_iothub_hostname;
        *iothub_device_id = sample_iothub_device_id;
        printf("Registered Device Successfully.\r\n");
    }

    /* Destroy Provisioning Client.  */
    nx_azure_iot_provisioning_client_deinitialize(prov_client_ptr);

    return(status);
}
#endif /* ENABLE_DPS_SAMPLE */





static VOID log_callback(az_log_classification classification, UCHAR *msg, UINT msg_len)
{
    if (classification == AZ_LOG_IOT_AZURERTOS)
    {
        printf("%.*s", msg_len, (CHAR *)msg);
    }
}




static VOID connection_status_callback(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr, UINT status)
{
    NX_PARAMETER_NOT_USED(hub_client_ptr);
    if (status)
    {
        printf("Disconnected from IoTHub!: error code = 0x%08x\r\n", status);
        tx_event_flags_set(&sample_events, SAMPLE_DISCONNECT_EVENT, TX_OR);
    }
    else
    {
        printf("Connected to IoTHub.\r\n");
        tx_event_flags_set(&sample_events, SAMPLE_CONNECTED_EVENT, TX_OR);
    }

    sample_connection_status = status;
}

static VOID message_receive_callback_properties(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr, VOID *context)
{
    NX_PARAMETER_NOT_USED(hub_client_ptr);
    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&sample_events, SAMPLE_PROPERTIES_RECEIVE_EVENT, TX_OR);
}

static VOID message_receive_callback_command(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr, VOID *context)
{
    NX_PARAMETER_NOT_USED(hub_client_ptr);
    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&(sample_events), SAMPLE_COMMAND_RECEIVE_EVENT, TX_OR);
}

static VOID message_receive_callback_writable_properties(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr, VOID *context)
{
    NX_PARAMETER_NOT_USED(hub_client_ptr);
    NX_PARAMETER_NOT_USED(context);
    tx_event_flags_set(&(sample_events), SAMPLE_WRITABLE_PROPERTIES_RECEIVE_EVENT, TX_OR);
}














static UINT sample_initialize_iothub(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
#ifdef ENABLE_DPS_SAMPLE
UCHAR *iothub_hostname = NX_NULL;
UCHAR *iothub_device_id = NX_NULL;
UINT iothub_hostname_length = 0;
UINT iothub_device_id_length = 0;
#else
UCHAR *iothub_hostname = (UCHAR *)HOST_NAME;
UCHAR *iothub_device_id = (UCHAR *)DEVICE_ID;
UINT iothub_hostname_length = sizeof(HOST_NAME) - 1;
UINT iothub_device_id_length = sizeof(DEVICE_ID) - 1;
#endif /* ENABLE_DPS_SAMPLE */

#ifdef ENABLE_DPS_SAMPLE

    /* Run DPS.  */
    if ((status = sample_dps_entry(&prov_client, &iothub_hostname, &iothub_hostname_length,
                                   &iothub_device_id, &iothub_device_id_length)))
    {
        printf("Failed on sample_dps_entry!: error code = 0x%08x\r\n", status);
        return(status);
    }
#endif /* ENABLE_DPS_SAMPLE */

    printf("IoTHub Host Name: %.*s; Device ID: %.*s.\r\n",
           iothub_hostname_length, iothub_hostname, iothub_device_id_length, iothub_device_id);

    /* Initialize IoTHub client.  */
    if ((status = nx_azure_iot_hub_client_initialize(iothub_client_ptr, &nx_azure_iot,
                                                     iothub_hostname, iothub_hostname_length,
                                                     iothub_device_id, iothub_device_id_length,
                                                     (const UCHAR *)MODULE_ID, sizeof(MODULE_ID) - 1,
                                                     _nx_azure_iot_tls_supported_crypto,
                                                     _nx_azure_iot_tls_supported_crypto_size,
                                                     _nx_azure_iot_tls_ciphersuite_map,
                                                     _nx_azure_iot_tls_ciphersuite_map_size,
                                                     nx_azure_iot_tls_metadata_buffer,
                                                     sizeof(nx_azure_iot_tls_metadata_buffer),
                                                     &root_ca_cert)))
    {
        printf("Failed on nx_azure_iot_hub_client_initialize!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Set the model id.  */
    if ((status = nx_azure_iot_hub_client_model_id_set(iothub_client_ptr,
                                                       (const UCHAR *)SAMPLE_PNP_MODEL_ID,
                                                       sizeof(SAMPLE_PNP_MODEL_ID) - 1)))
    {
        printf("Failed on nx_azure_iot_hub_client_model_id_set!: error code = 0x%08x\r\n", status);
    }

    /* Add more CA certificates.  */
    else if ((status = nx_azure_iot_hub_client_trusted_cert_add(iothub_client_ptr, &root_ca_cert_2)))
    {
        printf("Failed on nx_azure_iot_hub_client_trusted_cert_add!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_hub_client_trusted_cert_add(iothub_client_ptr, &root_ca_cert_3)))
    {
        printf("Failed on nx_azure_iot_hub_client_trusted_cert_add!: error code = 0x%08x\r\n", status);
    }

#if (USE_DEVICE_CERTIFICATE == 1)

    /* Initialize the device certificate.  */
    else if ((status = nx_secure_x509_certificate_initialize(&device_certificate,
                                                             (UCHAR *)sample_device_cert_ptr, (USHORT)sample_device_cert_len,
                                                             NX_NULL, 0,
                                                             (UCHAR *)sample_device_private_key_ptr, (USHORT)sample_device_private_key_len,
                                                             DEVICE_KEY_TYPE)))
    {
        printf("Failed on nx_secure_x509_certificate_initialize!: error code = 0x%08x\r\n", status);
    }

    /* Set device certificate.  */
    else if ((status = nx_azure_iot_hub_client_device_cert_set(iothub_client_ptr, &device_certificate)))
    {
        printf("Failed on nx_azure_iot_hub_client_device_cert_set!: error code = 0x%08x\r\n", status);
    }
#else

    /* Set symmetric key.  */
    else if ((status = nx_azure_iot_hub_client_symmetric_key_set(iothub_client_ptr,
                                                                 (UCHAR *)DEVICE_SYMMETRIC_KEY,
                                                                 sizeof(DEVICE_SYMMETRIC_KEY) - 1)))
    {
        printf("Failed on nx_azure_iot_hub_client_symmetric_key_set! error: 0x%08x\r\n", status);
    }
#endif /* USE_DEVICE_CERTIFICATE */

    /* Enable command and properties features.  */
    else if ((status = nx_azure_iot_hub_client_command_enable(iothub_client_ptr)))
    {
        printf("Failed on nx_azure_iot_hub_client_command_enable!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_hub_client_properties_enable(iothub_client_ptr)))
    {
        printf("Failed on nx_azure_iot_hub_client_properties_enable!: error code = 0x%08x\r\n", status);
    }

    /* Set connection status callback.  */
    else if ((status = nx_azure_iot_hub_client_connection_status_callback_set(iothub_client_ptr,
                                                                              connection_status_callback)))
    {
        printf("Failed on connection_status_callback!\r\n");
    }
    else if ((status = nx_azure_iot_hub_client_receive_callback_set(iothub_client_ptr,
                                                                    NX_AZURE_IOT_HUB_COMMAND,
                                                                    message_receive_callback_command,
                                                                    NX_NULL)))
    {
        printf("device command callback set!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_hub_client_receive_callback_set(iothub_client_ptr,
                                                                    NX_AZURE_IOT_HUB_PROPERTIES,
                                                                    message_receive_callback_properties,
                                                                    NX_NULL)))
    {
        printf("device properties callback set!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_hub_client_receive_callback_set(iothub_client_ptr,
                                                                    NX_AZURE_IOT_HUB_WRITABLE_PROPERTIES,
                                                                    message_receive_callback_writable_properties,
                                                                    NX_NULL)))
    {
        printf("device writable properties callback set!: error code = 0x%08x\r\n", status);
    }
    else if ((status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
                                                             (const UCHAR *)temperature_component,
                                                             sizeof(temperature_component) - 1)) ||
             (status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
                                                             (const UCHAR *)humidity_component,
                                                             sizeof(humidity_component) - 1)) ||
             (status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
                                                             (const UCHAR *)pressure_component,
                                                             sizeof(pressure_component) - 1))  ||
			(status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
															 (const UCHAR *)magnetometer_component,
															 sizeof(magnetometer_component) - 1))  ||
			(status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
															  (const UCHAR *)accelerometer_component,
															  sizeof(accelerometer_component) - 1)) ||
			(status = nx_azure_iot_hub_client_component_add(iothub_client_ptr,
															  (const UCHAR *)gyroscope_component,
															   sizeof(gyroscope_component) - 1)))
    {
        printf("Failed to add component to client!: error code = 0x%08x\r\n", status);
    }

    if (status)
    {
        nx_azure_iot_hub_client_deinitialize(iothub_client_ptr);
    }

    return(status);
}



















void sample_components_init()
{
	Humidity = 0;
	Temp = 0;
	Pressure = 0;
	Magnetometer[0] = 0;
	Magnetometer[1] = 0;
	Magnetometer[2] = 0;
	Accelerometr[0] = 0;
	Accelerometr[1] = 0;
	Accelerometr[2] = 0;
	Gyroscope[0] = 0;
	Gyroscope[1] = 0;
	Gyroscope[2] = 0;
	LedState = false;
}

/* #######################################################################################
 *
 * 						sample_connected_action section
 *
 * #######################################################################################
 */


static VOID sample_connected_action(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr)
{
UINT status;

    /* Request all properties.  */
    if ((status = nx_azure_iot_hub_client_properties_request(hub_client_ptr, NX_WAIT_FOREVER)))
    {
        printf("Properties request failed!: error code = 0x%08x\r\n", status);
    }
    else
    {
        printf("Sent properties request.\r\n");
    }
}

/* #######################################################################################
 *
 * 						sample_connected_action section END
 *
 * #######################################################################################
 */



/* #######################################################################################
 *
 * 						sample_periodic_action section
 *
 * #######################################################################################
 */


static VOID sample_periodic_action(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr)
{
    NX_PARAMETER_NOT_USED(hub_client_ptr);

    if ((sample_periodic_counter % 5) == 0)
    {

        /* Set telemetry send event and reported properties send event.  */
        tx_event_flags_set(&(sample_events), (SAMPLE_TELEMETRY_SEND_EVENT | SAMPLE_REPORTED_PROPERTIES_SEND_EVENT), TX_OR);
    }

    sample_periodic_counter++;
}

/* #######################################################################################
 *
 * 						sample_periodic_action section END
 *
 * #######################################################################################
 */




/* #######################################################################################
 *
 * 						sample_telemetry_action section
 *
 * #######################################################################################
 */

static UINT temp_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Add component name.  */

    /*
    if ((status = nx_azure_iot_hub_client_telemetry_component_set(packet_ptr,
    																(UCHAR *)temperature_component,
                                                                  (USHORT)sizeof(temperature_component),
                                                                  NX_WAIT_FOREVER)))
    {
        printf("Telemetry message failed to set component\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }
*/

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_TEMPERATURE,
                                                                 sizeof(TELEMETRY_TEMPERATURE) - 1,
																 BSP_TSENSOR_ReadTemp(), 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("Temp Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}


static UINT humidity_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_HUMIDITY,
                                                                 sizeof(TELEMETRY_HUMIDITY) - 1,
																 BSP_HSENSOR_ReadHumidity(), 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("Humidity Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}

static UINT pressure_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_PRESSURE,
                                                                 sizeof(TELEMETRY_PRESSURE) - 1,
																 BSP_PSENSOR_ReadPressure(), 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("Humidity Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}

static UINT magnetometer_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    BSP_MAGNETO_GetXYZ(Magnetometer);

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_MAGNETOMETERX,
                                                                 sizeof(TELEMETRY_MAGNETOMETERX) - 1,
																 Magnetometer[0], 2) ||
	   nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
			   	   	   	   	   	   	   	   	   	   	   	   	   	   (UCHAR *)TELEMETRY_MAGNETOMETERY,
																   sizeof(TELEMETRY_MAGNETOMETERY) - 1,
																   Magnetometer[1], 2) ||

		nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
		                                                           (UCHAR *)TELEMETRY_MAGNETOMETERZ,
		                                                           sizeof(TELEMETRY_MAGNETOMETERZ) - 1,
																   Magnetometer[2], 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("MagnetometerXYZ Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}


static UINT accelerometer_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    BSP_ACCELERO_AccGetXYZ(Accelerometr);

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_ACCELEROMETERX,
                                                                 sizeof(TELEMETRY_ACCELEROMETERX) - 1,
																 Accelerometr[0], 2) ||
	   nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
			   	   	   	   	   	   	   	   	   	   	   	   	   	   (UCHAR *)TELEMETRY_ACCELEROMETERY,
																   sizeof(TELEMETRY_ACCELEROMETERY) - 1,
																   Accelerometr[1], 2) ||

		nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
		                                                           (UCHAR *)TELEMETRY_ACCELEROMETERZ,
		                                                           sizeof(TELEMETRY_ACCELEROMETERZ) - 1,
																   Accelerometr[2], 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("AccelerometerXYZ Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}


static UINT gyroscope_telemetry_send(NX_AZURE_IOT_HUB_CLIENT *iothub_client_ptr)
{
UINT status;
NX_PACKET *packet_ptr;
NX_AZURE_IOT_JSON_WRITER json_writer;
UINT buffer_length;

    /* Create a telemetry message packet.  */
    if ((status = nx_azure_iot_hub_client_telemetry_message_create(iothub_client_ptr,
                                                                   &packet_ptr, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message create failed!: error code = 0x%08x\r\n", status);
        return(status);
    }

    /* Build telemetry JSON payload.  */
    if (nx_azure_iot_json_writer_with_buffer_init(&json_writer, scratch_buffer, sizeof(scratch_buffer)))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    BSP_GYRO_GetXYZ(Gyroscope);

    if(nx_azure_iot_json_writer_append_begin_object(&json_writer) ||
       nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
                                                                 (UCHAR *)TELEMETRY_GYROSCOPEX,
                                                                 sizeof(TELEMETRY_GYROSCOPEX) - 1,
																 Gyroscope[0], 2) ||
	   nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
			   	   	   	   	   	   	   	   	   	   	   	   	   	   (UCHAR *)TELEMETRY_GYROSCOPEY,
																   sizeof(TELEMETRY_GYROSCOPEY) - 1,
																   Gyroscope[1], 2) ||

		nx_azure_iot_json_writer_append_property_with_double_value(&json_writer,
		                                                           (UCHAR *)TELEMETRY_GYROSCOPEZ,
		                                                           sizeof(TELEMETRY_GYROSCOPEZ) - 1,
																   Gyroscope[2], 2) ||
        nx_azure_iot_json_writer_append_end_object(&json_writer))
    {
        printf("Telemetry message failed to build message\r\n");
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(NX_NOT_SUCCESSFUL);
    }

    buffer_length = nx_azure_iot_json_writer_get_bytes_used(&json_writer);
    if ((status = nx_azure_iot_hub_client_telemetry_send(iothub_client_ptr, packet_ptr,
                                                         (UCHAR *)scratch_buffer, buffer_length, NX_WAIT_FOREVER)))
    {
        printf("Telemetry message send failed!: error code = 0x%08x\r\n", status);
        nx_azure_iot_hub_client_telemetry_message_delete(packet_ptr);
        return(status);
    }

    printf("GyroscopeXYZ Telemetry message send: %.*s.\r\n", buffer_length, scratch_buffer);

    return(status);
}


static void sample_telemetry_action(NX_AZURE_IOT_HUB_CLIENT *hub_client_ptr)
{
UINT status;

    if (sample_connection_status != NX_SUCCESS)
    {
        return;
    }

    if((status = temp_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send temp_telemetry_send, error: %d", status);
    }

    if((status = humidity_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send humidity_telemetry_send, error: %d", status);
    }

    if((status = pressure_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send pressure_telemetry_send, error: %d", status);
    }

    if((status = magnetometer_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send magnetometer_telemetry_send, error: %d", status);
    }

    if((status = accelerometer_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send accelerometer_telemetry_send, error: %d", status);
    }

    if((status = gyroscope_telemetry_send(hub_client_ptr)) != NX_AZURE_IOT_SUCCESS){
    	printf("Failed to send gyroscope_telemetry_send, error: %d", status);
    }

}

/* #######################################################################################
 *
 * 						sample_telemetry_action section END
 *
 * #######################################################################################
 */



VOID sample_entry(NX_IP *ip_ptr, NX_PACKET_POOL *pool_ptr, NX_DNS *dns_ptr, UINT (*unix_time_callback)(ULONG *unix_time))
{
UINT status;
UINT loop = NX_TRUE;
ULONG app_events;

    nx_azure_iot_log_init(log_callback);

    sample_components_init();

    /* Create Azure IoT handler.  */
    if ((status = nx_azure_iot_create(&nx_azure_iot, (UCHAR *)"Azure IoT", ip_ptr, pool_ptr, dns_ptr,
                                      nx_azure_iot_thread_stack, sizeof(nx_azure_iot_thread_stack),
                                      NX_AZURE_IOT_THREAD_PRIORITY, unix_time_callback)))
    {
        printf("Failed on nx_azure_iot_create!: error code = 0x%08x\r\n", status);
        return;
    }

    /* Initialize CA certificates.  */
    if ((status = nx_secure_x509_certificate_initialize(&root_ca_cert, (UCHAR *)_nx_azure_iot_root_cert,
                                                        (USHORT)_nx_azure_iot_root_cert_size,
                                                        NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE)))
    {
        printf("Failed to initialize ROOT CA certificate!: error code = 0x%08x\r\n", status);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    if ((status = nx_secure_x509_certificate_initialize(&root_ca_cert_2, (UCHAR *)_nx_azure_iot_root_cert_2,
                                                        (USHORT)_nx_azure_iot_root_cert_size_2,
                                                        NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE)))
    {
        printf("Failed to initialize ROOT CA certificate!: error code = 0x%08x\r\n", status);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    if ((status = nx_secure_x509_certificate_initialize(&root_ca_cert_3, (UCHAR *)_nx_azure_iot_root_cert_3,
                                                        (USHORT)_nx_azure_iot_root_cert_size_3,
                                                        NX_NULL, 0, NULL, 0, NX_SECURE_X509_KEY_TYPE_NONE)))
    {
        printf("Failed to initialize ROOT CA certificate!: error code = 0x%08x\r\n", status);
        nx_azure_iot_delete(&nx_azure_iot);
        return;
    }

    tx_timer_create(&(sample_timer), (CHAR*)"sample_app_timer", sample_periodic_timer_entry, 0,
                    NX_IP_PERIODIC_RATE, NX_IP_PERIODIC_RATE, TX_AUTO_ACTIVATE);
    tx_event_flags_create(&sample_events, (CHAR*)"sample_app_event");

    while (loop)
    {

        /* Pickup sample event flags.  */
        tx_event_flags_get(&(sample_events), SAMPLE_ALL_EVENTS, TX_OR_CLEAR, &app_events, NX_WAIT_FOREVER);

        if (app_events & SAMPLE_CONNECTED_EVENT)
        {
            sample_connected_action(&iothub_client);
        }

        if (app_events & SAMPLE_PERIODIC_EVENT)
        {
            sample_periodic_action(&iothub_client);
        }

        if (app_events & SAMPLE_TELEMETRY_SEND_EVENT)
        {
            sample_telemetry_action(&iothub_client);
        }


        /* Connection monitor.  */
        sample_connection_monitor(ip_ptr, &iothub_client, sample_connection_status, sample_initialize_iothub);
    }

    /* Cleanup.  */
    tx_event_flags_delete(&sample_events);
    tx_timer_delete(&sample_timer);
    nx_azure_iot_hub_client_deinitialize(&iothub_client);
    nx_azure_iot_delete(&nx_azure_iot);
}









