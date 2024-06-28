#include "esphome/core/log.h"
#include "shys_m5_dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        static const char *TAG = "shys_m5_dial";

        std::string format_uid(std::vector<uint8_t> &uid) {
            char buf[32];
            int offset = 0;
            for (size_t i = 0; i < uid.size(); i++) {
                const char *format = "%02X";
                if (i + 1 < uid.size())
                format = "%02X-";
                offset += sprintf(buf + offset, format, uid[i]);
            }
            return std::string(buf);
        }

        /**
         * @brief SETUP
         *
         * Initialisierung
         */

        void ShysM5Dial::setup()
        {
            using std::placeholders::_1;
            ShysM5Dial::initDevice();
            ESP_LOGI("log", "%s", "M5 is initialized");
        }

        /**
         * @brief LOOP
         *
         * Standard Loop
         */
        void ShysM5Dial::loop()
        {
            M5.delay(1);
            M5Dial.update();
            esphome::delay(1);
            ShysM5Dial::doLoop();
            esphome::delay(1);
        }

        /**
         * @brief dump_config
         *
         * Ausgabe der aktuellen Konfiguration im Log nach Initialisierung
         */
        void ShysM5Dial::dump_config()
        {
            ESP_LOGCONFIG(TAG, "-----------------------------------");
            ESP_LOGCONFIG(TAG, "Shys M5 Dial");
            ESP_LOGCONFIG(TAG, "-----------------------------------");
        }

        void ShysM5Dial::sendScannedTag(const char* tag){
                //const uint8_t* tag1 = reinterpret_cast<const uint8_t*>(tag);
                
                for (auto *trigger : this->triggers_ontag_)
                trigger->process(tag);
            }
        
        
        void M5RC522Trigger::process(const char* &data) { this->trigger(data.c_str()); }

        
    }
}