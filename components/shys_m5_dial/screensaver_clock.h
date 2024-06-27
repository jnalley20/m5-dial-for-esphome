#pragma once
#include "screensaver.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ScreensaverClock: public esphome::shys_m5_dial::Screensaver {
            protected:
                esphome::time::RealTimeClock* localTime;

                uint8_t lastHr  = 255;
                uint8_t lastMin = 255;
                uint8_t lastSec = 255;

                void showClock(M5DialDisplay& display){
                    display.clear(BLACK);

                    for(int i=1;i<=12;i++){
                        int degree = i*30;
                        coord c1 = display.getColorCoord(i%3?117:112, degree);
                        coord c2 = display.getColorCoord(120, degree-(i%3?1:4));
                        coord c3 = display.getColorCoord(120, degree+(i%3?1:4));

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, WHITE);
                    }
                }

                void refreshClock(M5DialDisplay& display){
                    this->paintHourHand(display);
                    this->paintMinuteHand(display);
                    this->paintSecondHand(display);

                    int x = display.getWidth()/2;
                    int y = display.getHeight()/2;
                    display.getGfx()->fillCircle(x, y, 5, WHITE);
                    
                    static constexpr const char* const wd[7] = {"Sun", "Mon", "Tue", "Wed",
                                                "Thr", "Fri", "Sat"};
                    auto dt = M5Dial.Rtc.getDateTime();
                    Serial.printf("RTC   UTC  :%04d/%02d/%02d (%s)  %02d:%02d:%02d\r\n",
                                dt.date.year, dt.date.month, dt.date.date,
                                wd[dt.date.weekDay], dt.time.hours, dt.time.minutes,
                                dt.time.seconds);
                    M5Dial.Display.setCursor(0, 0);
                    M5Dial.Display.printf("RTC   UTC  :%04d/%02d/%02d (%s)  %02d:%02d:%02d",
                                        dt.date.year, dt.date.month, dt.date.date,
                                        wd[dt.date.weekDay], dt.time.hours, dt.time.minutes,
                                        dt.time.seconds);

                    /// ESP32 internal timer
                    auto t = time(nullptr);
                    {
                        auto tm = gmtime(&t);  // for UTC.
                        Serial.printf("UTC :%04d/%02d/%02d (%s)  %02d:%02d:%02d\r\n",
                                    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                                    wd[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec);
                        M5Dial.Display.setCursor(0, 20);
                        M5Dial.Display.printf("UTC  :%04d/%02d/%02d (%s)  %02d:%02d:%02d",
                                            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                                            wd[tm->tm_wday], tm->tm_hour, tm->tm_min,
                                            tm->tm_sec);
                    }

                    {
                        auto tm = localtime(&t);  // for local timezone.
                        Serial.printf("CT :%s:%04d/%02d/%02d (%s)  %02d:%02d:%02d\r\n",
                                    "UTC-5", tm->tm_year + 1900, tm->tm_mon + 1,
                                    tm->tm_mday, wd[tm->tm_wday], tm->tm_hour, tm->tm_min,
                                    tm->tm_sec);
                        M5Dial.Display.setCursor(0, 40);
                        M5Dial.Display.printf("CT: %s:%04d/%02d/%02d (%s)  %02d:%02d:%02d",
                                            "UTC-5", tm->tm_year + 1900, tm->tm_mon + 1,
                                            tm->tm_mday, wd[tm->tm_wday], tm->tm_hour,
                                            tm->tm_min, tm->tm_sec);
                    }
                }

                void paintHourHand(M5DialDisplay& display){
                    uint8_t hour = this->localTime->now().hour;
                    uint8_t minute = this->localTime->now().minute;

                    if(lastHr != hour || lastMin != minute){
                        int degree = 180 - (lastHr * 360 / 12);
                        degree = degree - (round(lastMin/12) * 360 / 60);
                        paintHand(display, degree, 30, 70, BLACK);
                    }

                    int degree = 180 - (hour * 360 / 12);
                    degree = degree - (round(minute/12) * 360 / 60);
                    paintHand(display, degree, 30, 70, WHITE);

                    lastHr = hour;
                }

                void paintMinuteHand(M5DialDisplay& display){
                    uint8_t minute = this->localTime->now().minute;

                    if(lastMin != minute){
                        int degree = 180 - (lastMin * 360 / 60);
                        paintHand(display, degree, 20, 95, BLACK);
                    }

                    int degree = 180 - (minute * 360 / 60);
                    paintHand(display, degree, 20, 95, WHITE);

                    lastMin = minute;
                }

                void paintSecondHand(M5DialDisplay& display){
                    uint8_t second = this->localTime->now().second;

                    if(lastSec != second){
                        int degree = 180 - (lastSec * 360 / 60);
                        paintHand(display, degree, 2, 105, BLACK);
                    }

                    int degree = 180 - (second * 360 / 60);
                    paintHand(display, degree, 1, 105, RED);

                    lastSec = second;
                }

                void paintHand(M5DialDisplay& display, int degree, int width, int length, uint16_t color){
                        coord c1 = display.getColorCoord(length, degree);
                        coord c2 = display.getColorCoord(4, degree - width);
                        coord c3 = display.getColorCoord(4, degree + width);

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

            public:
                void show(M5DialDisplay& display, bool init) override {
                    if(init){
                        showClock(display);
                    } else {
                        refreshClock(display);
                    }
                }

                void setLocalTime(esphome::time::RealTimeClock* time_comp) {
                    this->localTime = time_comp;
                }

        };

    }
}