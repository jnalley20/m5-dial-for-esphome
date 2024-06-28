#pragma once
#include "M5Dial.h"
#include "globals.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialRfid {
            protected:
                int currentLoop = 0;
                int checkOnLoop = 50;
                std::function<void(const char*)> tag_scanned;
            public:
                void on_tag_scanned(std::function<void(const char*)>){
                    ESP_LOGD("Rfid", "register on_tag_scanned Callback");
                    this->tag_scanned = callback;
                }
                void handleRfId(){
                    if(currentLoop == checkOnLoop){
                        currentLoop = 0;
                        if (M5Dial.Rfid.PICC_IsNewCardPresent() &&
                            M5Dial.Rfid.PICC_ReadCardSerial()) 
                        {
                            M5Dial.Display.clear();// This will clear the clock tick marks...

                            Serial.print(F("PICC type: "));
                            
                            uint8_t piccType = M5Dial.Rfid.PICC_GetType(M5Dial.Rfid.uid.sak);
                            Serial.println(M5Dial.Rfid.PICC_GetTypeName(piccType));
                            
                            String uid = "";
                            for (byte i = 0; i < M5Dial.Rfid.uid.size;
                                i++) {  // Output the stored UID data.  将存储的UID数据输出
                                Serial.printf("%02X ", M5Dial.Rfid.uid.uidByte[i]);
                                uid += String(M5Dial.Rfid.uid.uidByte[i], HEX);
                            }
                            //M5Dial.Speaker.tone(8000, 20);
                            M5Dial.Display.drawString(M5Dial.Rfid.PICC_GetTypeName(piccType),
                                                    M5Dial.Display.width() / 2,
                                                    M5Dial.Display.height() / 2 - 30);

                            M5Dial.Display.drawString("card id:", M5Dial.Display.width() / 2,
                                                    M5Dial.Display.height() / 2);

                            M5Dial.Display.drawString(uid, M5Dial.Display.width() / 2,
                                                    M5Dial.Display.height() / 2 + 30);
                            Serial.println();

                            //
                            this->tag_scanned(uid);
                        }
                    }
                    else{
                        currentLoop = currentLoop + 1;
                    }
                }
        };
    }
}