/* Copyright (c) 2011-2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LOC_API_V_0_2_H
#define LOC_API_V_0_2_H

#include <stdint.h>
#include <stdbool.h>
#include "ds_client.h"
#include <LocApiBase.h>
#include <loc_api_v02_client.h>

using namespace loc_core;

/* This class derives from the LocApiBase class.
   The members of this class are responsible for converting
   the Loc API V02 data structures into Loc Adapter data structures.
   This class also implements some of the virtual functions that
   handle the requests from loc engine. */
class LocApiV02 : public LocApiBase {
protected:
  /* loc api v02 handle*/
  locClientHandleType clientHandle;

private:
  /*ds client handle*/
  dsClientHandleType dsClientHandle;

  /* Convert event mask from loc eng to loc_api_v02 format */
  static locClientEventMaskType convertMask(LOC_API_ADAPTER_EVENT_MASK_T mask);

  /* Convert error from loc_api_v02 to loc eng format*/
  static enum loc_api_adapter_err convertErr(locClientStatusEnumType status);

  /* convert Ni Encoding type from QMI_LOC to loc eng format */
  static GpsNiEncodingType convertNiEncoding(
    qmiLocNiDataCodingSchemeEnumT_v02 loc_encoding);

  /*convert NI notify verify type from QMI LOC to loc eng format*/
  static bool convertNiNotifyVerifyType (GpsNiNotification *notif,
      qmiLocNiNotifyVerifyEnumT_v02 notif_priv);

  /* convert position report to loc eng format and send the converted
     position to loc eng */
  void reportPosition
    (const qmiLocEventPositionReportIndMsgT_v02 *location_report_ptr);

  /* convert satellite report to loc eng format and  send the converted
     report to loc eng */
  void reportSv (const qmiLocEventGnssSvInfoIndMsgT_v02 *gnss_report_ptr);

  /* convert engine state report to loc eng format and send the converted
     report to loc eng */
  void reportEngineState (
    const qmiLocEventEngineStateIndMsgT_v02 *engine_state_ptr);

  /* convert fix session report to loc eng format and send the converted
     report to loc eng */
  void reportFixSessionState (
    const qmiLocEventFixSessionStateIndMsgT_v02 *fix_session_state_ptr);

  /* convert NMEA report to loc eng format and send the converted
     report to loc eng */
  void reportNmea (const qmiLocEventNmeaIndMsgT_v02 *nmea_report_ptr);

  /* convert and report an ATL request to loc engine */
  void reportAtlRequest(
    const qmiLocEventLocationServerConnectionReqIndMsgT_v02
    *server_request_ptr);

  /* convert and report NI request to loc eng */
  void reportNiRequest(
    const qmiLocEventNiNotifyVerifyReqIndMsgT_v02 *ni_req_ptr);

  /* report the xtra server info */
  void reportXtraServerUrl(
    const qmiLocEventInjectPredictedOrbitsReqIndMsgT_v02* server_request_ptr);

protected:
  virtual enum loc_api_adapter_err
    open(LOC_API_ADAPTER_EVENT_MASK_T mask);
  virtual enum loc_api_adapter_err
    close();

public:
  LocApiV02(const MsgTask* msgTask,
            LOC_API_ADAPTER_EVENT_MASK_T exMask);
  ~LocApiV02();

  /* event callback registered with the loc_api v02 interface */
  virtual void eventCb(locClientHandleType client_handle,
               uint32_t loc_event_id,
               locClientEventIndUnionType loc_event_payload);

  /* error callback, this function handles the  service unavailable
     error */
  void errorCb(locClientHandleType handle,
               locClientErrorEnumType errorId);

  void ds_client_event_cb(ds_client_status_enum_type result);

  virtual enum loc_api_adapter_err startFix(const LocPosMode& posMode);

  virtual enum loc_api_adapter_err stopFix();

  virtual enum loc_api_adapter_err
    setPositionMode(const LocPosMode& mode);

  virtual enum loc_api_adapter_err
    setTime(GpsUtcTime time, int64_t timeReference, int uncertainty);

  virtual enum loc_api_adapter_err
    injectPosition(double latitude, double longitude, float accuracy);

  virtual enum loc_api_adapter_err
    deleteAidingData(GpsAidingData f);

  virtual enum loc_api_adapter_err
    informNiResponse(GpsUserResponseType userResponse,
                     const void* passThroughData);

  virtual enum loc_api_adapter_err
    setServer(const char* url, int len);
  virtual enum loc_api_adapter_err
    setServer(unsigned int ip, int port, LocServerType type);
  virtual enum loc_api_adapter_err
    setXtraData(char* data, int length);
  virtual enum loc_api_adapter_err
    requestXtraServer();
  virtual enum loc_api_adapter_err
    atlOpenStatus(int handle, int is_succ, char* apn, AGpsBearerType bear,
                   AGpsType agpsType);
  virtual enum loc_api_adapter_err atlCloseStatus(int handle, int is_succ);
  virtual enum loc_api_adapter_err setSUPLVersion(uint32_t version);
  virtual enum loc_api_adapter_err setLPPConfig(uint32_t profile);

  virtual enum loc_api_adapter_err
    setSensorControlConfig(int sensorUsage, int sensorProvider);

  virtual enum loc_api_adapter_err
    setSensorProperties(bool gyroBiasVarianceRandomWalk_valid, float gyroBiasVarianceRandomWalk,
                            bool accelBiasVarianceRandomWalk_valid, float accelBiasVarianceRandomWalk,
                            bool angleBiasVarianceRandomWalk_valid, float angleBiasVarianceRandomWalk,
                            bool rateBiasVarianceRandomWalk_valid, float rateBiasVarianceRandomWalk,
                            bool velocityBiasVarianceRandomWalk_valid, float velocityBiasVarianceRandomWalk);

  virtual enum loc_api_adapter_err
    setSensorPerfControlConfig(int controlMode, int accelSamplesPerBatch, int accelBatchesPerSec,
                               int gyroSamplesPerBatch, int gyroBatchesPerSec,
                               int accelSamplesPerBatchHigh, int accelBatchesPerSecHigh,
                               int gyroSamplesPerBatchHigh, int gyroBatchesPerSecHigh, int algorithmConfig);
  virtual enum loc_api_adapter_err setExtPowerConfig(int isBatteryCharging);
  virtual enum loc_api_adapter_err setAGLONASSProtocol(unsigned long aGlonassProtocol);
  virtual enum loc_api_adapter_err getZppFix(GpsLocation & zppLoc);
  virtual enum loc_api_adapter_err getZppFix(GpsLocation & zppLocLoc, LocPosTechMask &tech_mask);
  virtual int initDataServiceClient();
  virtual int openAndStartDataCall();
  virtual void stopDataCall();
  virtual void closeDataCall();
  /*Values for lock
    1 = Do not lock any position sessions
    2 = Lock MI position sessions
    3 = Lock MT position sessions
    4 = Lock all position sessions
  */
  virtual int setGpsLock(unsigned int lock);

  /*
    Returns
    Current value of GPS Lock on success
    -1 on failure
  */
  virtual int getGpsLock(void);

private:
  locClientEventMaskType mQmiMask = 0;
  bool mInSession = false;
  bool mEngineOn = false;

  bool registerEventMask(locClientEventMaskType qmiMask);
  locClientEventMaskType adjustMaskForNoSession(locClientEventMaskType qmiMask);
};

#endif //LOC_API_V_0_2_H
