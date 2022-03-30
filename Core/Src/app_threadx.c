/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_azure_rtos.h"
#include "nx_api.h"
#ifndef SAMPLE_DHCP_DISABLE
#include "nxd_dhcp_client.h"
#endif /* SAMPLE_DHCP_DISABLE */
#include "nxd_dns.h"
#include "nxd_sntp_client.h"
#include "nx_secure_tls_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

extern NX_PACKET_POOL   pool_0;
extern NX_IP            ip_0;
static NX_DNS           dns_0;
#ifndef SAMPLE_DHCP_DISABLE
static NX_DHCP          dhcp_0;
#endif /* SAMPLE_DHCP_DISABLE  */
static NX_SNTP_CLIENT   sntp_client;

/* System clock time for UTC.  */
static ULONG            unix_time_base;

static const CHAR *sntp_servers[] =
{
    "0.pool.ntp.org",
    "1.pool.ntp.org",
    "2.pool.ntp.org",
    "3.pool.ntp.org",
};
static UINT sntp_server_index;



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Include the sample.  */
extern VOID sample_entry(NX_IP* ip_ptr, NX_PACKET_POOL* pool_ptr, NX_DNS* dns_ptr, UINT (*unix_time_callback)(ULONG *unix_time));
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
#ifdef SAMPLE_BOARD_SETUP
    SAMPLE_BOARD_SETUP();
#endif /* SAMPLE_BOARD_SETUP */
  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/* Define sample helper thread entry.  */
void sample_helper_thread_entry(ULONG parameter)
{

	UINT    status;
	ULONG   ip_address = 0;
	ULONG   network_mask = 0;
	ULONG   gateway_address = 0;
	UINT    unix_time;
	ULONG   dns_server_address[3];
	#ifndef SAMPLE_DHCP_DISABLE
	UINT    dns_server_address_size = sizeof(dns_server_address);
	#endif

	    NX_PARAMETER_NOT_USED(parameter);

	#ifndef SAMPLE_DHCP_DISABLE
	    if (dhcp_wait())
	    {
	        printf("Failed to get the IP address!\r\n");
	        return;
	    }
	#elif defined(SAMPLE_NETWORK_CONFIGURE)
	    SAMPLE_NETWORK_CONFIGURE(&ip_0, &dns_server_address[0]);
	#else
	    nx_ip_gateway_address_set(&ip_0, SAMPLE_GATEWAY_ADDRESS);
	#endif /* SAMPLE_DHCP_DISABLE  */

	    /* Get IP address and gateway address.  */
	    nx_ip_address_get(&ip_0, &ip_address, &network_mask);
	    nx_ip_gateway_address_get(&ip_0, &gateway_address);

	    /* Output IP address and gateway address.  */
	    printf("IP address: %lu.%lu.%lu.%lu\r\n",
	           (ip_address >> 24),
	           (ip_address >> 16 & 0xFF),
	           (ip_address >> 8 & 0xFF),
	           (ip_address & 0xFF));
	    printf("Mask: %lu.%lu.%lu.%lu\r\n",
	           (network_mask >> 24),
	           (network_mask >> 16 & 0xFF),
	           (network_mask >> 8 & 0xFF),
	           (network_mask & 0xFF));
	    printf("Gateway: %lu.%lu.%lu.%lu\r\n",
	           (gateway_address >> 24),
	           (gateway_address >> 16 & 0xFF),
	           (gateway_address >> 8 & 0xFF),
	           (gateway_address & 0xFF));

	#ifndef SAMPLE_DHCP_DISABLE
	    /* Retrieve DNS server address.  */
	    nx_dhcp_interface_user_option_retrieve(&dhcp_0, 0, NX_DHCP_OPTION_DNS_SVR, (UCHAR *)(dns_server_address),
	                                           &dns_server_address_size);
	#elif !defined(SAMPLE_NETWORK_CONFIGURE)
	    dns_server_address[0] = SAMPLE_DNS_SERVER_ADDRESS;
	#endif /* SAMPLE_DHCP_DISABLE */

	    /* Create DNS.  */
	    status = dns_create(dns_server_address[0]);

	    /* Check for DNS create errors.  */
	    if (status)
	    {
	        printf("dns_create fail: %u\r\n", status);
	        return;
	    }

	    /* Sync up time by SNTP at start up.  */
	    status = sntp_time_sync();

	    /* Check status.  */
	    if (status)
	    {
	        printf("SNTP Time Sync failed.\r\n");
	        printf("Set Time to default value: SAMPLE_SYSTEM_TIME.");
	        unix_time_base = SAMPLE_SYSTEM_TIME;
	    }
	    else
	    {
	        printf("SNTP Time Sync successfully.\r\n");
	    }

	    unix_time_get((ULONG *)&unix_time);
	    srand(unix_time);

	    /* Start sample.  */
	    sample_entry(&ip_0, &pool_0, &dns_0, unix_time_get);
}

#ifndef SAMPLE_DHCP_DISABLE
static UINT dhcp_wait()
{
UINT    status;
ULONG   actual_status;

    printf("DHCP In Progress...\r\n");

    /* Create the DHCP instance.  */
    status = nx_dhcp_create(&dhcp_0, &ip_0, "DHCP Client");

    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Request NTP server.  */
    status = nx_dhcp_user_option_request(&dhcp_0, NX_DHCP_OPTION_NTP_SVR);

    /* Check status.  */
    if (status)
    {
        nx_dhcp_delete(&dhcp_0);
        return(status);
    }

    /* Start the DHCP Client.  */
    status = nx_dhcp_start(&dhcp_0);

    /* Check status.  */
    if (status)
    {
        nx_dhcp_delete(&dhcp_0);
        return(status);
    }

    /* Wait util address is solved.  */
    status = nx_ip_status_check(&ip_0, NX_IP_ADDRESS_RESOLVED, &actual_status, SAMPLE_DHCP_WAIT_OPTION);

    /* Check status.  */
    if (status)
    {
        nx_dhcp_delete(&dhcp_0);
        return(status);
    }

    return(NX_SUCCESS);
}
#endif /* SAMPLE_DHCP_DISABLE  */


 UINT dns_create(ULONG dns_server_address)
{
UINT    status;

    /* Create a DNS instance for the Client.  Note this function will create
       the DNS Client packet pool for creating DNS message packets intended
       for querying its DNS server.  */
    status = nx_dns_create(&dns_0, &ip_0, (UCHAR *)"DNS Client");
    if (status)
    {
        return(status);
    }

    /* Is the DNS client configured for the host application to create the packet pool?  */
#ifdef NX_DNS_CLIENT_USER_CREATE_PACKET_POOL

    /* Yes, use the packet pool created above which has appropriate payload size
       for DNS messages.  */
    status = nx_dns_packet_pool_set(&dns_0, ip_0.nx_ip_default_packet_pool);
    if (status)
    {
        nx_dns_delete(&dns_0);
        return(status);
    }
#endif /* NX_DNS_CLIENT_USER_CREATE_PACKET_POOL */

    /* Add an IPv4 server address to the Client list.  */
    status = nx_dns_server_add(&dns_0, dns_server_address);
    if (status)
    {
        nx_dns_delete(&dns_0);
        return(status);
    }

    /* Output DNS Server address.  */
    printf("DNS Server address: %lu.%lu.%lu.%lu\r\n",
           (dns_server_address >> 24),
           (dns_server_address >> 16 & 0xFF),
           (dns_server_address >> 8 & 0xFF),
           (dns_server_address & 0xFF));

    return(NX_SUCCESS);
}

/* Sync up the local time.  */
static UINT sntp_time_sync_internal(ULONG sntp_server_address)
{
UINT    status;
UINT    server_status;
UINT    i;

    /* Create the SNTP Client to run in broadcast mode.. */
    status =  nx_sntp_client_create(&sntp_client, &ip_0, 0, &pool_0,
                                    NX_NULL,
                                    NX_NULL,
                                    NX_NULL /* no random_number_generator callback */);

    /* Check status.  */
    if (status)
    {
        return(status);
    }

    /* Use the IPv4 service to initialize the Client and set the IPv4 SNTP server. */
    status = nx_sntp_client_initialize_unicast(&sntp_client, sntp_server_address);

    /* Check status.  */
    if (status)
    {
        nx_sntp_client_delete(&sntp_client);
        return(status);
    }

    /* Set local time to 0 */
    status = nx_sntp_client_set_local_time(&sntp_client, 0, 0);

    /* Check status.  */
    if (status)
    {
        nx_sntp_client_delete(&sntp_client);
        return(status);
    }

    /* Run Unicast client */
    status = nx_sntp_client_run_unicast(&sntp_client);

    /* Check status.  */
    if (status)
    {
        nx_sntp_client_stop(&sntp_client);
        nx_sntp_client_delete(&sntp_client);
        return(status);
    }

    /* Wait till updates are received */
    for (i = 0; i < SAMPLE_SNTP_UPDATE_MAX; i++)
    {

        /* First verify we have a valid SNTP service running. */
        status = nx_sntp_client_receiving_updates(&sntp_client, &server_status);

        /* Check status.  */
        if ((status == NX_SUCCESS) && (server_status == NX_TRUE))
        {

            /* Server status is good. Now get the Client local time. */
            ULONG sntp_seconds, sntp_fraction;
            ULONG system_time_in_second;

            /* Get the local time.  */
            status = nx_sntp_client_get_local_time(&sntp_client, &sntp_seconds, &sntp_fraction, NX_NULL);

            /* Check status.  */
            if (status != NX_SUCCESS)
            {
                continue;
            }

            /* Get the system time in second.  */
            system_time_in_second = tx_time_get() / TX_TIMER_TICKS_PER_SECOND;

            /* Convert to Unix epoch and minus the current system time.  */
            unix_time_base = (sntp_seconds - (system_time_in_second + SAMPLE_UNIX_TO_NTP_EPOCH_SECOND));

            /* Time sync successfully.  */

            /* Stop and delete SNTP.  */
            nx_sntp_client_stop(&sntp_client);
            nx_sntp_client_delete(&sntp_client);

            return(NX_SUCCESS);
        }

        /* Sleep.  */
        tx_thread_sleep(SAMPLE_SNTP_UPDATE_INTERVAL);
    }

    /* Time sync failed.  */

    /* Stop and delete SNTP.  */
    nx_sntp_client_stop(&sntp_client);
    nx_sntp_client_delete(&sntp_client);

    /* Return success.  */
    return(NX_NOT_SUCCESSFUL);
}

 UINT sntp_time_sync()
{
UINT status;
ULONG gateway_address;
ULONG sntp_server_address[3];
#ifndef SAMPLE_DHCP_DISABLE
UINT  sntp_server_address_size = sizeof(sntp_server_address);
#endif

#ifndef SAMPLE_DHCP_DISABLE
    /* Retrieve NTP server address.  */
    status = nx_dhcp_interface_user_option_retrieve(&dhcp_0, 0, NX_DHCP_OPTION_NTP_SVR, (UCHAR *)(sntp_server_address),
                                                    &sntp_server_address_size);

    /* Check status.  */
    if (status == NX_SUCCESS)
    {
        for (UINT i = 0; (i * 4) < sntp_server_address_size; i++)
        {
            printf("SNTP Time Sync...%lu.%lu.%lu.%lu (DHCP)\r\n",
                   (sntp_server_address[i] >> 24),
                   (sntp_server_address[i] >> 16 & 0xFF),
                   (sntp_server_address[i] >> 8 & 0xFF),
                   (sntp_server_address[i] & 0xFF));

            /* Start SNTP to sync the local time.  */
            status = sntp_time_sync_internal(sntp_server_address[i]);

            /* Check status.  */
            if(status == NX_SUCCESS)
            {
                return(NX_SUCCESS);
            }
        }
    }
#endif /* SAMPLE_DHCP_DISABLE */

    /* Sync time by NTP server array.  */
    for (UINT i = 0; i < SAMPLE_SNTP_SYNC_MAX; i++)
    {
        printf("SNTP Time Sync...%s\r\n", sntp_servers[sntp_server_index]);

        /* Make sure the network is still valid.  */
        while (nx_ip_gateway_address_get(&ip_0, &gateway_address))
        {
            tx_thread_sleep(NX_IP_PERIODIC_RATE);
        }

        /* Look up SNTP Server address. */
        status = nx_dns_host_by_name_get(&dns_0, (UCHAR *)sntp_servers[sntp_server_index], &sntp_server_address[0], 5 * NX_IP_PERIODIC_RATE);

        /* Check status.  */
        if (status == NX_SUCCESS)
        {

            /* Start SNTP to sync the local time.  */
            status = sntp_time_sync_internal(sntp_server_address[0]);

            /* Check status.  */
            if(status == NX_SUCCESS)
            {
                return(NX_SUCCESS);
            }
        }

        /* Switch SNTP server every time.  */
        sntp_server_index = (sntp_server_index + 1) % (sizeof(sntp_servers) / sizeof(sntp_servers[0]));
    }

    return(NX_NOT_SUCCESSFUL);
}

 UINT unix_time_get(ULONG *unix_time)
{

    /* Return number of seconds since Unix Epoch (1/1/1970 00:00:00).  */
    *unix_time =  unix_time_base + (tx_time_get() / TX_TIMER_TICKS_PER_SECOND);

    return(NX_SUCCESS);
}

/* USER CODE END 1 */
