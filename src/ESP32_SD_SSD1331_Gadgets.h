/*
  ESP32_SD_SSD1331_Gadgets.h
  Beta version 1.1

Copyright (c) 2017 Mgo-tec

This library is used by the Arduino IDE(Tested in ver1.8.2).

Reference Blog --> https://www.mgo-tec.com

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef _ESP32_SD_SSD1331_GADGETS_H_INCLUDED
#define _ESP32_SD_SSD1331_GADGETS_H_INCLUDED

#include <Arduino.h>
#include "ESP32_SD_ShinonomeFNT.h"
#include "ESP32_SD_UTF8toSJIS.h"
#include "TimeLib.h" //Arduino time library ver1.5
#include "ESP32_SSD1331.h"
#include "ESP32_WebGet.h"
#include <SD.h>

extern ESP32_SSD1331 _ssd1331;
extern ESP32_SD_ShinonomeFNT _SFR;
extern ESP32_WebGet _EWG;

class ESP32_SD_SSD1331_Gadgets
{
private:

  const char *_ssid;
  const char *_password;

  File _MyF;

  uint8_t _day_t = 0;
  uint8_t _weekday_t = 0;

  uint8_t _week_scl_cnt1 = 0;
  bool _week_fnt_read1 = true;
  uint8_t _week_fnt[2][16] = {};

  char _sec_c1 = ' ', _sec_c2 = ' ';
  uint8_t _sec_scl_cnt1 = 0, _sec_scl_cnt2 = 0;
  bool _sec_fnt_read1 = true, _sec_fnt_read2 = true;
  uint8_t _sec_fnt[2][2][16] = {};

  char _min_c1 = ' ', _min_c2 = ' ';
  uint8_t _min_scl_cnt1 = 0, _min_scl_cnt2 = 0;
  bool _min_fnt_read1 = true, _min_fnt_read2 = true;
  uint8_t _min_fnt[2][2][16] = {};

  char _hour_c1 = ' ', _hour_c2 = ' ';
  uint8_t _hour_scl_cnt1 = 0, _hour_scl_cnt2 = 0;
  bool _hour_fnt_read1 = true, _hour_fnt_read2 = true;
  uint8_t _hour_fnt[2][2][16] = {};

  bool _colon_in = false;
  uint8_t _colon_t = 60;

  uint32_t _last_week_time = 0;
  uint32_t _last_sec_time1 = 0;
  uint32_t _last_Hmin_time = 0;
  uint32_t _colon_last_time = 0;
  
  uint32_t _xLastTime[4] = {};
  bool _fnt_read_ok[4] = {true, true, true, true};
  uint8_t _scl_cnt[4] = {}; //pixelのスクロールカウント数
  uint16_t _SJ_cnt[4] = {}; //Shift_JISコードのカウント数
  uint8_t _zen_or_han[4] = {}; //全角の場合2, 半角の場合1
  uint8_t _Zen_or_Han_cnt[4] = {};
  uint8_t _Font_buf[4][2][16] = {}; //フォントを格納するためのもの

public:
  ESP32_SD_SSD1331_Gadgets();

  void Gadgets_Init();
  void Gadgets_MyFont_Init(const char* my_fontfile);
  void ShinonomeClock_YMD(uint8_t x0, uint8_t y0, uint8_t Red, uint8_t Green, uint8_t Blue);
  void MyFontClock_Weekday(uint8_t x0, uint8_t y0, uint8_t Red, uint8_t Green, uint8_t Blue);
  void Shinonome_Sec_Clock(uint8_t x0, uint8_t y0, char s_HorV1, char s_HorV2, uint8_t s_direction1, uint8_t s_direction2, uint16_t scl_interval, uint8_t Red, uint8_t Green, uint8_t Blue);
  void MyFont_HM_Clock(uint8_t x0, uint8_t y0, char h_HorV1, char h_HorV2, char m_HorV1, char m_HorV2, uint8_t h_direction1, uint8_t h_direction2, uint8_t m_direction1, uint8_t m_direction2, uint16_t scl_interval, uint8_t Red, uint8_t Green, uint8_t Blue);
  void Shinonome8x16ClockScroll(char VorH, uint8_t ForB, char t_chr, char *Lt_chr, bool *fnt_read, uint8_t *scl_cnt, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t fnt[2][16]);
  void MyFont16x16ClockScroll(char VorH, uint8_t ForB, char t_chr, char *Lt_chr, bool *fnt_read, uint8_t *scl_cnt, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t fnt[2][16]);
  void YahooJ_Weather_TodayTomorrow(uint8_t y0, String str);
  void MyFont_SD_Read(File F, uint8_t ZorH, uint8_t num, uint8_t buf[2][16]);
  bool YahooNewsGadget(uint8_t y0, uint8_t num, const char *Host, String t_p, bool GetNow, uint32_t *NewsGetLastTime, uint32_t get_interval, uint16_t Scrolle_interval, uint8_t news_txt1[], uint8_t Red, uint8_t Green, uint8_t Blue);
  bool YahooWeatherGadget(uint8_t y0, uint8_t num, const char *Host, String t_p, bool GetNow, uint32_t *WeatherGetLastTime, uint32_t get_interval, uint16_t Scrolle_interval, uint8_t weather_txt1[], uint8_t Red, uint8_t Green, uint8_t Blue);
  void Scroll_1_line(uint8_t y, uint8_t num, uint8_t red, uint8_t green, uint8_t blue, uint16_t interval, uint8_t sjis_code[], uint16_t length, bool *WebGet);
  void Root_CA_SDcard_Read(const char *filename, char Root_CA[]);

};

#endif
