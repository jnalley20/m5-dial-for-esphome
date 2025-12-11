#pragma once
#include "ha_device.h"
#include "ha_device_mode_lock.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceLock: public esphome::shys_m5_dial::HaDevice {
            protected:
                HaDeviceModeLock*   modeLock   = nullptr;

            public:
                HaDeviceLock(const std::string& entity_id, const std::string& name, const std::string& modes) : HaDevice(entity_id, name, modes) {}

                ~HaDeviceLock() {
                    delete modeLock;
                }

                void init() override {
                    ESP_LOGD("HA_DEVICE", "Init Lock: %s", this->getEntityId().c_str());

                    modeLock = new HaDeviceModeLock(*this);
                    if (modeLock == nullptr) {
                        ESP_LOGE("HA_DEVICE", "Failed to allocate modeLock for %s", this->getEntityId().c_str());
                        return;
                    }
                    this->addMode(modeLock);

                    if (this->modeConfig["lock_mode"].is<JsonObject>()) {
                        JsonObject lock_mode = this->modeConfig["lock_mode"];

                        if (lock_mode["rotary_step_width"].is<int>()) {
                            modeLock->setRotaryStepWidth(lock_mode["rotary_step_width"].as<int>());
                        }

                        if(lock_mode["open_on_button"].is<bool>()){
                           modeLock->setOpenOnButton(lock_mode["open_on_button"].as<bool>());
                        }
                    }
                }

        };

    }
}