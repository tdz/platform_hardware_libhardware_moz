/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* Copyright 2015 Mozilla Foundation and Mozilla contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HARDWARE_MOZ_GEOLOCATION_H
#define HARDWARE_MOZ_GEOLOCATION_H

#include <pthread.h>
#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

/* The maximum API version that this header file defines. */
#define MOZ_GPS_API_VERSION (1)

/*
 * MozMobileCellInfo.webidl
 */

#define MOZ_GPS_MOBILE_CELL_INFO_HAS_GSM_LOCATION_AREA_CODE       (1<<0)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_GSM_CELL_ID                  (1<<1)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_CDMA_BASE_STATION_ID         (1<<2)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_CDMA_BASE_STATION_LATITUDE   (1<<3)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_CDMA_BASE_STATION_LONGITUDE  (1<<4)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_CDMA_SYSTEM_ID               (1<<5)
#define MOZ_GPS_MOBILE_CELL_INFO_HAS_CDMA_NETWORK_ID              (1<<6)

/* taken from |interface MozMobileNetworkInfo| */
typedef struct {

  /* API v1 */

  uint16_t valid_fields; /* bitmask of MOZ_GPS_MOBILE_CELL_INFO_HAS_* */

  uint16_t gsm_location_area_code;
  uint32_t gsm_cell_id;

  uint32_t cdma_base_station_latitude;
  uint32_t cdma_base_station_longitude;
  uint16_t cdma_base_station_id;
  uint16_t cdma_network_id;
  uint16_t cdma_system_id;

} MozGpsMobileCellInfo;

/*
 * MozMobileNetworkInfo.webidl
 */

/* taken from |enum MobileNetworkState| */
#define MOZ_GPS_MOBILE_NETWORK_STATE_AVAILABLE (0x0)
#define MOZ_GPS_MOBILE_NETWORK_STATE_CONNECTED (0x1)
#define MOZ_GPS_MOBILE_NETWORK_STATE_FORBIDDEN (0x2)

/* taken from |interface MozMobileNetworkInfo| */
typedef struct {

  /* API v1 */

  /* TODO: What are the optimal string sizes? */

  uint8_t state; /* one of MOZ_GPS_MOBILE_NETWORK_STATE_* */

  char short_name[32]; /* \0-terminated string */
  char long_name[128]; /* \0-terminated string */
  char mcc[16]; /* \0-terminated string */
  char mnc[16]; /* \0-terminated string */

} MozGpsMobileNetworkInfo;

/*
 * MozMobileConnectionInfo.webidl
 */

/* taken from |enum MobileConnectionState| */
#define MOZ_GPS_MOBILE_CONNECTION_STATE_NOT_SEARCHING (0x0)
#define MOZ_GPS_MOBILE_CONNECTION_STATE_SEARCHING     (0x1)
#define MOZ_GPS_MOBILE_CONNECTION_STATE_DENIED        (0x2)
#define MOZ_GPS_MOBILE_CONNECTION_STATE_REGISTERED    (0x3)

/* taken from |enum MobileConnectionType| */
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_GPS        (0x00)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_GPRS       (0x01)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_EDGE       (0x02)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_UMTS       (0x03)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_HSDPA      (0x04)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_HSUPA      (0x05)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_HSPA       (0x06)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_HSPA_PLUS  (0x07)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_IS95A      (0x08)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_IS95B      (0x09)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_1XRTT      (0x0a)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_EVDO0      (0x0b)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_EVDOA      (0x0c)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_EVDOB      (0x0d)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_EHRPD      (0x0e)
#define MOZ_GPS_MOBILE_CONNECTION_TYPE_LTE        (0x0f)

/* taken from |interface MozMobileConnectionInfo| */
typedef struct {

  /* API v1 */

  uint8_t state; /* one of MOZ_GPS_MOBILE_CONNECTION_STATE_* */
  uint8_t connected;
  uint8_t emergency_calls_only;
  uint8_t roaming;
  uint8_t type; /* one of MOZ_GPS_MOBILE_CONNECTION_TYPE_* */
  int32_t signal_strength; /* in dBm, or -1 if unknown */
  int32_t rel_signal_strength; /* in %, or -1 if unknown */

  const MozGpsMobileCellInfo* cell;
  const MozGpsMobileNetworkInfo* network;

} MozGpsMobileConnectionInfo;

/*
 * nsIGeoPositionCoords.idl
 */

typedef struct {

  /* API v1 */

  double latitude;
  double longitude;
  double altitude;
  double accuracy;
  double altitudeAccuracy;
  double heading;
  double speed;
} MozGpsPositionCoords;

/*
 * nsIGeoPosition.idl
 */

typedef struct {

  /* API v1 */

  struct timespec timestamp;
  const MozGpsPositionCoords* position;
} MozGpsPosition;

/*
 * nsINetworkManager.idl
 */

/* taken from |nsINetworkInterface::NETWORK_STATE_*| */
#define MOZ_GPS_NETWORK_STATE_UNKNOWN       (0)
#define MOZ_GPS_NETWORK_STATE_CONNECTING    (1)
#define MOZ_GPS_NETWORK_STATE_CONNECTED     (2)
#define MOZ_GPS_NETWORK_STATE_DISCONNECTING (3)
#define MOZ_GPS_NETWORK_STATE_DISCONNECTED  (4)

/* taken from |nsINetworkInterface::NETWORK_TYPE_MOBILE*| */
#define MOZ_GPS_NETWORK_TYPE_MOBILE       (0)
#define MOZ_GPS_NETWORK_TYPE_MOBILE_MMS   (1)
#define MOZ_GPS_NETWORK_TYPE_MOBILE_SUPL  (2)

/*
 * nsIPrefBranch.idl
 */

/* TODO: Which preferences are relevant? */
#define MOZ_GPS_PREFERENCE_

#define MOZ_GPS_PREFERENCE_TYPE_BOOL    (0) /* int8_t */
#define MOZ_GPS_PREFERENCE_TYPE_STRING  (1) /* \0-terminated string */
#define MOZ_GPS_PREFERENCE_TYPE_LONG    (2) /* int64_t */

/*
 * nsITelephonyCallInfo.idl
 */

typedef struct {

  /* TODO: can we remove some of the fields in this structure? */

  /* API v1 */

  uint32_t client_id;
  uint32_t call_index;

  uint8_t call_state; /* one of MOZ_GPS_CALL_STATE_* */

  char disconnect_reason[64]; /* TODO: maybe use enumerator instead of string? */

  char number[64]; /* \0-terminated string */

  uint16_t number_presentation; /* one of MOZ_GPS_CALL_PRESENTATION_* */

  uint8_t outgoing;
  uint8_t emergency;
  uint8_t conference;
  uint8_t switchable;
  uint8_t mergeable;

} MozGpsTelephonyCallInfo;

/*
 * nsITelephonyService.idl
 */

/* taken from |interface nsITelephonyService::CALL_PRESENTATION_*| */
#define MOZ_GPS_CALL_PRESENTATION_ALLOWED     (0)
#define MOZ_GPS_CALL_PRESENTATION_RESTRICTED  (1)
#define MOZ_GPS_CALL_PRESENTATION_UNKNOWN     (2)
#define MOZ_GPS_CALL_PRESENTATION_PAYPHONE    (3)

/* taken from |interface nsITelephonyService::CALL_STATE_*| */
#define MOZ_GPS_CALL_STATE_UNKNOWN      (0)
#define MOZ_GPS_CALL_STATE_DIALING      (1)
#define MOZ_GPS_CALL_STATE_ALERTING     (2)
#define MOZ_GPS_CALL_STATE_CONNECTED    (3)
#define MOZ_GPS_CALL_STATE_HELD         (4)
#define MOZ_GPS_CALL_STATE_DISCONNECTED (5)
#define MOZ_GPS_CALL_STATE_INCOMING     (6)

#define MOZ_GPS_NOTIFICATION_REMOTE_HELD    (0)
#define MOZ_GPS_NOTIFICATION_REMOTE_RESUMED (1)

/*
 * nsITimer.idl
 */

#define MOZ_GPS_TIMER_TYPE_ONE_SHOT                   (0)
#define MOZ_GPS_TIMER_TYPE_REPEATING_SLACK            (1)
#define MOZ_GPS_TIMER_TYPE_REPEATING_PRECISE          (2)
#define MOZ_GPS_TIMER_TYPE_REPEATING_PRECISE_CAN_SKIP (3)

typedef uintptr_t MozGpsTimer;

/*
 * Callbacks
 */

/* nsIGeolocationProvider */
/* nsIDomGeoPosition */
/* nsIGeolocationUpdate */

typedef void (*moz_gps_update)(const MozGpsPosition* position);
typedef void (*moz_gps_update_pending)(void);
typedef void (*moz_gps_error)(int error);

  // Should already be supported by Android's HAL interfaces

/* nsIObserver */
/* nsISettingsService */
/* nsIPrefBranch */

  // Settings are managed by Gecko

// TODO: Which preferences are relevant?
typedef void (*moz_gps_request_preference_value)(int preference);

/* nsIThread */

typedef pthread_t (*moz_gps_create_thread)(const char* name, void* (*start)(void*), void* arg);

/* nsIRadioInterfaceLayer */

typedef void (*moz_gps_setup_data_call_by_network_type)(int type);
typedef void (*moz_gps_deactivate_data_call_by_network_type)(int type);
typedef void (*moz_gps_request_data_call_state_by_network_type)(int type);

typedef void (*moz_gps_update_ril_network_interface)(void);

/* nsIMobileConnectionInfo */

  /* TODO: how do we handle multiple networks? */

typedef void (*moz_gps_request_mobile_connection_info)(void);

/* nsITimer */

typedef void (*moz_gps_start_timer)(uint32_t id, uint32_t delay, uint32_t type);
typedef void (*moz_gps_cancel_timer)(uint32_t id);

/* nsINetworkManager */
/* nsIInputStreamCallback */
/* nsITransportEventSink */
/* nsISocketTransport */
/* nsIAsyncInputStream */
/* nsIOutputStream */

  // TODO: Which interfaces are relevant? What's the point of the
  //       socket I/O? Can we put that completely into Gecko?

/* nsAString */
/* nsCString */
/* NS_ConvertUTF8toUTF16 */
/* NS_ConvertASCIItoUTF16 */
/* NS_LITERAL_STRING */
/* NS_NAMED_LITERAL_STRING */

  // Use C strings instead

typedef struct {

  size_t size;

  /* API v1 */

  /* nsIGeolocationProvider */
  /* nsIDomGeoPosition */
  /* nsIGeolocationUpdate */

  moz_gps_update update_cb;
  moz_gps_update_pending update_pending_cb;
  moz_gps_error error_cb;

  /* nsIObserver */
  /* nsISettingsService */
  /* nsIPrefBranch */

    // Settings are managed by Gecko

  moz_gps_request_preference_value request_preference_value_cb;

  /* nsIThread */

  moz_gps_create_thread create_thread_cb;

  /* nsIRadioInterfaceLayer */

  moz_gps_setup_data_call_by_network_type setup_data_call_by_network_type_cb;
  moz_gps_deactivate_data_call_by_network_type
    deactivate_data_call_by_network_type_cb;
  moz_gps_request_data_call_state_by_network_type
    request_data_call_state_by_network_type_cb;

  moz_gps_update_ril_network_interface update_ril_network_interface_cb;

  /* nsIMobileConnectionInfo */

  moz_gps_request_mobile_connection_info request_mobile_connection_info_cb;

  /* nsITimer */

  moz_gps_start_timer start_timer_cb;
  moz_gps_cancel_timer cancel_timer_cb;

  /* nsINetworkManager */
  /* nsIInputStreamCallback */
  /* nsITransportEventSink */
  /* nsISocketTransport */
  /* nsIAsyncInputStream */
  /* nsIOutputStream */

} MozGpsCallbacks;

/*
 * Interfaces
 */

typedef struct {

  size_t size;

  /* API v1 */

  /* nsIGeolocationProvider */
  /* nsIDomGeoPosition */
  /* nsIGeolocationUpdate */

  int (*init)(MozGpsCallbacks* callbacks);
  void (*cleanup)(void);

  int (*start)(void);
  void (*stop)(void);

  /* nsIObserver */
  /* nsISettingsService */
  /* nsIPrefBranch */

  /* Response to |MozGpsCallback->request_preference_value_cb| */
  void (*set_preference_value)(int preference, int type, const void* value);

  /* nsITimer
   */

  void (*execute_timer)(uint32_t id);

  /* nsIRadioInterfaceLayer
   */

  /* Response to |MozGpsCallback->request_data_call_state_by_network_type_cb| */
  void (*set_data_call_state_by_network_type)(int type, unsigned long state);

  /* nsIMobileConnectionInfo
   */

  /* Response to |MozGpsCallback->request_mobile_conncetion_info_cb| */
  void (*set_mobile_connection_info)(const MozGpsMobileConnectionInfo* info);

  /* nsITelephonyListener
   */

  void (*begin_enumerate_call_state)(const MozGpsTelephonyCallInfo* call);
  void (*completed_enumerate_call_state)(void);

  void (*call_state_changed)(size_t ncalls,
                             const MozGpsTelephonyCallInfo* calls);

  void (*conference_call_state_changed)(unsigned long state);

  void (*supplimentaty_service_notification)(unsigned long client_id,
                                             unsigned long call_index,
                                             unsigned long notification);

  void (*notify_cdma_call_waiting)(unsigned long client_id,
                                   const char* number,
                                   unsigned long number_presentation,
                                   const char* name,
                                   unsigned long name_presentation);

  void (*notify_conference_error)(const char* name, const char* message);

  /* nsINetworkManager */
  /* nsIInputStreamCallback */
  /* nsITransportEventSink */
  /* nsISocketTransport */
  /* nsIAsyncInputStream */
  /* nsIOutputStream */

    // TODO: Which interfaces are relevant? What's the point of the
    //       socket I/O? Can we put that completely into Gecko?

} MozGpsInterface;

const MozGpsInterface*
moz_gps_get_interface(uint32_t* version, uint64_t* features);

__END_DECLS

#endif /* MOZGPS_H */
