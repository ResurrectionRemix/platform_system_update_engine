// Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_PLATFORM_UPDATE_ENGINE_REAL_SYSTEM_STATE_H_
#define CHROMEOS_PLATFORM_UPDATE_ENGINE_REAL_SYSTEM_STATE_H_

#include "update_engine/system_state.h"

#include "update_engine/clock.h"
#include "update_engine/connection_manager.h"
#include "update_engine/gpio_handler.h"
#include "update_engine/hardware.h"
#include "update_engine/payload_state.h"
#include "update_engine/prefs.h"
#include "update_engine/real_dbus_wrapper.h"
#include "update_engine/update_attempter.h"
#include "update_engine/p2p_manager.h"

namespace chromeos_update_engine {

// A real implementation of the SystemStateInterface which is
// used by the actual product code.
class RealSystemState : public SystemState {
public:
  // Constructors and destructors.
  RealSystemState();
  virtual ~RealSystemState() {}

  virtual bool IsOOBEComplete(base::Time* out_time_of_oobe);

  virtual inline void set_device_policy(
      const policy::DevicePolicy* device_policy) {
    device_policy_ = device_policy;
  }

  virtual inline const policy::DevicePolicy* device_policy() const {
    return device_policy_;
  }

  virtual inline ClockInterface* clock() {
    return &clock_;
  }

  virtual inline ConnectionManager* connection_manager() {
    return &connection_manager_;
  }

  virtual inline HardwareInterface* hardware() {
    return &hardware_;
  }

  virtual inline MetricsLibraryInterface* metrics_lib() {
    return &metrics_lib_;
  }

  virtual inline PrefsInterface* prefs() {
    return &prefs_;
  }

  virtual inline PrefsInterface* powerwash_safe_prefs() {
      return &powerwash_safe_prefs_;
    }

  virtual inline PayloadStateInterface* payload_state() {
    return &payload_state_;
  }

  virtual inline GpioHandler* gpio_handler() const {
    return gpio_handler_.get();
  }

  virtual inline UpdateAttempter* update_attempter() {
    return update_attempter_.get();
  }

  // Returns a pointer to the object that stores the parameters that are
  // common to all Omaha requests.
  virtual inline OmahaRequestParams* request_params() {
    return &request_params_;
  }

  virtual inline P2PManager* p2p_manager() {
    return p2p_manager_.get();
  }

  virtual inline bool system_rebooted(){
    return system_rebooted_;
  }

  // Initializes this concrete object. Other methods should be invoked only
  // if the object has been initialized successfully.
  bool Initialize(bool enable_gpio);

private:
  // Interface for the clock.
  Clock clock_;

  // The latest device policy object from the policy provider.
  const policy::DevicePolicy* device_policy_;

  // The connection manager object that makes download
  // decisions depending on the current type of connection.
  ConnectionManager connection_manager_;

  // Interface for the hardware functions.
  Hardware hardware_;

  // The Metrics Library interface for reporting UMA stats.
  MetricsLibrary metrics_lib_;

  // Interface for persisted store.
  Prefs prefs_;

  // Interface for persisted store that persists across powerwashes.
  Prefs powerwash_safe_prefs_;

  // All state pertaining to payload state such as
  // response, URL, backoff states.
  PayloadState payload_state_;

  // Pointer to a GPIO handler and other needed modules (note that the order of
  // declaration significant for destruction, as the latter depends on the
  // former).
  scoped_ptr<UdevInterface> udev_iface_;
  scoped_ptr<FileDescriptor> file_descriptor_;
  scoped_ptr<GpioHandler> gpio_handler_;

  // The dbus object used to initialize the update attempter.
  RealDBusWrapper dbus_;

  // Pointer to the update attempter object.
  scoped_ptr<UpdateAttempter> update_attempter_;

  // Common parameters for all Omaha requests.
  OmahaRequestParams request_params_;

  scoped_ptr<P2PManager> p2p_manager_;

  // If true, this is the first instance of the update engine since the system
  // rebooted. Important for tracking whether you are running instance of the
  // update engine on first boot or due to a crash/restart.
  bool system_rebooted_;
};

}  // namespace chromeos_update_engine

#endif  // CHROMEOS_PLATFORM_UPDATE_ENGINE_REAL_SYSTEM_STATE_H_
