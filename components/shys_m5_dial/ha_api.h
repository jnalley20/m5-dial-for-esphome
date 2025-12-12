#pragma once
#include "esphome.h"
#include "esphome/components/api/api_pb2.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaApi {
            protected:
                std::string colorValue;
                
                bool startsWith(const char *pre, const char *str){
                    return strncmp(pre, str, strlen(pre)) == 0;
                }                
            public:

                void updateEntity(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("homeassistant.update_entity"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "update Entity: %s", entity.c_str());
                }



                void turnLightOn(const std::string& entity, int brightness = -1, int colorValue = -1){
                    ESP_LOGI("HA_API", "ENTRY turnLightOn");
                    ESP_LOGI("HA_API", "brightness=%i", brightness);
                    ESP_LOGI("HA_API", "About to check server");
                    if(esphome::api::global_api_server == nullptr){
                        ESP_LOGE("HA_API", "Server is NULL");
                        return;
                    }
                    ESP_LOGI("HA_API", "Server valid");
                    
                    ESP_LOGI("HA_API", "Stack ptr: %p", (void*)&entity);
                    ESP_LOGI("HA_API", "Creating request");
                    esphome::api::HomeassistantActionRequest resp{};
                    ESP_LOGI("HA_API", "Request created");
                    
                    ESP_LOGI("HA_API", "Setting service");
                    resp.set_service(esphome::StringRef("light.turn_on"));
                    ESP_LOGI("HA_API", "Service set");
                    
                    ESP_LOGI("HA_API", "resp.data size=%u", resp.data.size());
                    ESP_LOGI("HA_API", "resp.data addr=%p", &resp.data);
                    ESP_LOGI("HA_API", "Calling emplace_back");
                    auto &kv1 = resp.data.emplace_back();
                    ESP_LOGI("HA_API", "emplace_back done");
                    
                    kv1.set_key(esphome::StringRef("entity_id"));
                    ESP_LOGV("HA_API", "Key set to entity_id");
                    
                    kv1.value = entity;
                    ESP_LOGV("HA_API", "Entity ID set to %s", entity.c_str());

                    std::string brightness_str;
                    if(brightness >= 0){
                        ESP_LOGV("HA_API", "Converting brightness to string");
                        brightness_str = std::to_string(brightness);
                        ESP_LOGV("HA_API", "Brightness string created: %s", brightness_str.c_str());
                        
                        ESP_LOGV("HA_API", "About to emplace_back for brightness_pct");
                        auto &kv2 = resp.data.emplace_back();
                        ESP_LOGV("HA_API", "emplace_back for brightness completed");
                        
                        kv2.set_key(esphome::StringRef("brightness_pct"));
                        ESP_LOGV("HA_API", "Key set to brightness_pct");
                        
                        kv2.value = brightness_str;
                        ESP_LOGD("HA_API", "Turn ON %s with brightness: %i (str=%s)", entity.c_str(), brightness, brightness_str.c_str());
                    }

                    if(colorValue >= 0){
                        // Verwende data_template mit Home Assistant Template-Syntax
                        auto &kv3 = resp.data_template.emplace_back();
                        kv3.set_key(esphome::StringRef("hs_color"));
                        // Template: {{(hue,saturation)|list}} wird zu echtem Array
                        char colorTemplate[32];
                        snprintf(colorTemplate, sizeof(colorTemplate), "{{(%d,100)|list}}", colorValue);
                        kv3.value = std::string(colorTemplate);
                        ESP_LOGI("HA_API", "Turn ON %s with color template: %s", entity.c_str(), colorTemplate);
                    }

                    ESP_LOGI("HA_API", "About to send light.turn_on to %s", entity.c_str());
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGI("HA_API", "Successfully sent light.turn_on to %s", entity.c_str());
                }

                void turnLightOnWhite(const std::string& entity, int kelvin = -1){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("light.turn_on"));
                    
                    int data_count = (kelvin >= 0) ? 2 : 1;
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string kelvin_str;
                    if(kelvin >= 0){
                        kelvin_str = std::to_string(kelvin);
                        auto &kv2 = resp.data.emplace_back();
                        kv2.set_key(esphome::StringRef("kelvin"));
                        kv2.value = kelvin_str;
                        ESP_LOGD("HA_API", "Turn ON %s with kelvin: %i", entity.c_str(), kelvin);
                    }

                    api::global_api_server->send_homeassistant_action(resp);
                }



                void turnLightOff(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("light.turn_off"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn OFF Light: %s", entity.c_str());
                }

                void toggleLight(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("light.toggle"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "toggle Light: %s", entity.c_str());
                }






                void turnClimateOn(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("climate.turn_on"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn ON Climate: %s", entity.c_str());
                }

                void turnClimateOff(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("climate.turn_off"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn OFF Climate: %s", entity.c_str());
                }

                void setClimateTemperature(const std::string& entity, int value){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("climate.set_temperature"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string value_str = std::to_string(value);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("temperature"));
                    kv2.value = value_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Climate set Temperature: %i on %s", value, entity.c_str());
                }

                void setClimateFanMode(const std::string& entity, const std::string& mode){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("climate.set_fan_mode"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("fan_mode"));
                    kv2.value = mode;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Climate set Fan-Mode: %s on %s", mode.c_str(), entity.c_str());
                }



                void setCoverPosition(const std::string& entity, int value){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("cover.set_cover_position"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string value_str = std::to_string(value);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("position"));
                    kv2.value = value_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Cover set Position: %i on %s", value, entity.c_str());
                }






                void turnSwitchOn(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("switch.turn_on"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn ON Switch: %s", entity.c_str());
                }

                void turnSwitchOff(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("switch.turn_off"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn OFF Switch: %s", entity.c_str());
                }

                void toggleSwitch(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("switch.toggle"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "toggle Switch: %s", entity.c_str());
                }






                void turnFanOn(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("fan.turn_on"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn ON Fan: %s", entity.c_str());
                }

                void turnFanOff(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("fan.turn_off"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "turn OFF Fan: %s", entity.c_str());
                }

                void toggleFan(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("fan.toggle"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "toggle Fan: %s", entity.c_str());
                }

                void setFanDirection(const std::string& entity, const char* direction){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("fan.set_direction"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("direction"));
                    kv2.value = direction;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Fan set Direction: %s on %s", direction, entity.c_str());
                }

                void setFanSpeed(const std::string& entity, int value){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("fan.set_percentage"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string value_str = std::to_string(value);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("percentage"));
                    kv2.value = value_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Fan set Speed: %i on %s", value, entity.c_str());
                }







                void setMediaPlayerVolume(const std::string& entity, int value){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.volume_set"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string value_str = std::to_string((float)value/100);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("volume_level"));
                    kv2.value = value_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer set Volume: %i on %s", value, entity.c_str());
                }

                void stopMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.media_stop"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer Stop: %s", entity.c_str());
                }

                void setNextTrackOnMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.media_next_track"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer next Track: %s", entity.c_str());
                }

                void setPreviousTrackOnMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.media_previous_track"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer previous Track: %s", entity.c_str());
                }

                void playPauseMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.media_play_pause"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer play/pause: %s", entity.c_str());
                }

                void playMediaOnMediaPlayer(const std::string& entity, const std::string& media_content_id, const std::string& media_content_type){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("media_player.play_media"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("media_content_id"));
                    kv2.value = media_content_id;

                    auto &kv3 = resp.data.emplace_back();
                    kv3.set_key(esphome::StringRef("media_content_type"));
                    kv3.value = media_content_type;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer play_media: %s on %s", media_content_id.c_str(), entity.c_str());
                }

                void refreshMediaPlayer(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("homeassistant.update_entity"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "MediaPlayer refresh: %s", entity.c_str());
                }







                void openLock(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("lock.open"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Lock open: %s", entity.c_str());
                }

                void lockLock(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("lock.lock"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Lock lock: %s", entity.c_str());
                }

                void unlockLock(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("lock.unlock"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Lock unlock: %s", entity.c_str());
                }





                void setNumberValue(const std::string& entity, int value){
                    std::string service = this->startsWith("input_number.", entity.c_str()) ? 
                                         "input_number.set_value" : "number.set_value";
                    
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef(service));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string value_str = std::to_string(value);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("value"));
                    kv2.value = value_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Number set Value: %i on %s", value, entity.c_str());
                }







                // Timer start without duration (uses timer's default duration)
                void timerStart(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.start"));
                    
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer start (default duration) on %s", entity.c_str());
                }

                // Timer start with specific duration
                void timerStart(const std::string& entity, int duration){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.start"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string duration_str = std::to_string(duration);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("duration"));
                    kv2.value = duration_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer start: %i on %s", duration, entity.c_str());
                }

                void timerPause(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.pause"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer pause: %s", entity.c_str());
                }

                void timerCancle(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.cancle"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer cancle: %s", entity.c_str());
                }

                void timerFinish(const std::string& entity){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.finish"));
                    auto &kv = resp.data.emplace_back();
                    kv.set_key(esphome::StringRef("entity_id"));
                    kv.value = entity;
                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer finish: %s", entity.c_str());
                }

                void timerChange(const std::string& entity, int duration){
                    esphome::api::HomeassistantActionRequest resp;
                    resp.set_service(esphome::StringRef("timer.change"));
                    
                    auto &kv1 = resp.data.emplace_back();
                    kv1.set_key(esphome::StringRef("entity_id"));
                    kv1.value = entity;

                    std::string duration_str = std::to_string(duration);
                    auto &kv2 = resp.data.emplace_back();
                    kv2.set_key(esphome::StringRef("duration"));
                    kv2.value = duration_str;

                    api::global_api_server->send_homeassistant_action(resp);
                    ESP_LOGD("HA_API", "Timer change: %i on %s", duration, entity.c_str());
                }
        };
    }
}
