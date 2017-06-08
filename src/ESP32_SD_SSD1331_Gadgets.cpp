/*
  ESP32_SD_SSD1331_Gadgets.cpp
  Beta version 1.0

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

#include "ESP32_SD_SSD1331_Gadgets.h"

static uint8_t _colon_fnt[2][16] = {
    0,
    0,
    0b00111000,
    0b01111100,
    0b01111100,
    0b01111100,
    0b00111000,
    0,
    0,
    0b00111000,
    0b01111100,
    0b01111100,
    0b01111100,
    0b00111000,
    0,
    0
  };

ESP32_SD_SSD1331_Gadgets::ESP32_SD_SSD1331_Gadgets(){}

//*********** 半角　東雲フォント　年月日表示 **********************
void ESP32_SD_SSD1331_Gadgets::Gadgets_MyFont_Init(const char* my_fontfile){
  SD.begin(5, SPI, 24000000, "/sd");
  _MyF = SD.open(my_fontfile, FILE_READ);
  if (!_MyF) {
    Serial.print(my_fontfile);
    Serial.println(" File not found");
    return;
  }else{
    Serial.print(my_fontfile);
    Serial.println(" File read OK!");
  }
}
//*********** 半角　東雲フォント　年月日表示 **********************
void ESP32_SD_SSD1331_Gadgets::ShinonomeClock_YMD(uint8_t x0, uint8_t y0, uint8_t Red, uint8_t Green, uint8_t Blue){
  if(day() != _day_t){
    String ymd_str = "";
    char month_chr[3], day_chr[3];
    uint8_t fnt_buf[10][16];

    sprintf(month_chr, "%2d", month());
    sprintf(day_chr, "%02d", day());
    ymd_str = String(year()) + '/' + String(month_chr) + '/' + String(day_chr);

    _SFR.StrDirect_ShinoFNT_readALL(ymd_str, fnt_buf);

    //年月日表示
    _ssd1331.SSD1331_8x16_Font_DisplayOut(10, x0, y0, Red, Green, Blue, fnt_buf);
    _day_t = day();
  }
}
//*********** 自作フォント 曜日　表示 **********************
void ESP32_SD_SSD1331_Gadgets::MyFontClock_Weekday(uint8_t x0, uint8_t y0, uint8_t Red, uint8_t Green, uint8_t Blue){
  if(weekday() != _weekday_t){
    uint8_t week_num = 10 + weekday() - 1;
/*
    if(week_num == 10){
      Red = 7; Green = 0; Blue = 0; //日曜日の場合
    }else if(week_num == 16){
      Red = 1; Green = 1; Blue = 3; //土曜日の場合
    }
*/    
    uint8_t fntBuf[2][16];

    ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, week_num, fntBuf);
    _ssd1331.SSD1331_8x16_Font_DisplayOut(2, x0, y0, Red, Green, Blue, fntBuf);
    _weekday_t = weekday();
  }  
}

//*********** 半角東雲フォント　秒　表示 **********************
void ESP32_SD_SSD1331_Gadgets::Shinonome_Sec_Clock(uint8_t x0, uint8_t y0, char s_HorV1, char s_HorV2, uint8_t s_direction1, uint8_t s_direction2, uint16_t scl_interval, uint8_t Red, uint8_t Green, uint8_t Blue){
  if(millis() - _last_sec_time1 > scl_interval){
    char s_chr[3];
    sprintf(s_chr, "%02d", second());

    Shinonome8x16ClockScroll(s_HorV1, s_direction1, s_chr[0], &_sec_c1, &_sec_fnt_read1, &_sec_scl_cnt1, x0, y0, Red, Green, Blue, _sec_fnt[0]);
    Shinonome8x16ClockScroll(s_HorV2, s_direction2, s_chr[1], &_sec_c2, &_sec_fnt_read2, &_sec_scl_cnt2, x0+8, y0, Red, Green, Blue, _sec_fnt[1]);

    _last_sec_time1 = millis();
  }
}
//*********** 自作フォント時刻表示 **********************
void ESP32_SD_SSD1331_Gadgets::MyFont_HM_Clock(uint8_t x0, uint8_t y0, char h_HorV1, char h_HorV2, char m_HorV1, char m_HorV2, uint8_t h_direction1, uint8_t h_direction2, uint8_t m_direction1, uint8_t m_direction2, uint16_t scl_interval, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t y1 = y0 + 15;

  if(millis() - _last_Hmin_time > scl_interval){ //時間、分表示設定
    char h_chr[3], m_chr[3];

    sprintf(h_chr, "%2d", hour());//ゼロを空白で埋める場合は%2dとすれば良い
    sprintf(m_chr, "%02d", minute());

    MyFont16x16ClockScroll(h_HorV1, h_direction1, h_chr[0], &_hour_c1, &_hour_fnt_read1, &_hour_scl_cnt1, 0, y0, Red, Green, Blue, _hour_fnt[0]);
    MyFont16x16ClockScroll(h_HorV2, h_direction2, h_chr[1], &_hour_c2, &_hour_fnt_read2, &_hour_scl_cnt2, 16, y0, Red, Green, Blue, _hour_fnt[1]);

    MyFont16x16ClockScroll(m_HorV1, m_direction1, m_chr[0], &_min_c1, &_min_fnt_read1, &_min_scl_cnt1, 40, y0, Red, Green, Blue, _min_fnt[0]);
    MyFont16x16ClockScroll(m_HorV2, m_direction1, m_chr[1], &_min_c2, &_min_fnt_read2, &_min_scl_cnt2, 56, y0, Red, Green, Blue, _min_fnt[1]);

    _last_Hmin_time = millis();
  }
  //-----------------------
  if(_colon_t != second()){ //コロン表示
    _ssd1331.SSD1331_8x16_Font_DisplayOut(1, x0+32, y0, Red, Green, Blue, _colon_fnt);

    _colon_last_time = millis();
    _colon_t = second();
    _colon_in = true;
  }
  if(_colon_in == true){
    if((millis() - _colon_last_time) >= 500){
      _ssd1331.Display_Clear(x0+32, y0, x0+39, y1);
      _colon_in = false;
    }
  }
}
//*********** 半角8x16 東雲フォント１文字時刻スクロール **********************
void ESP32_SD_SSD1331_Gadgets::Shinonome8x16ClockScroll(char VorH, uint8_t ForB, char t_chr, char *Lt_chr, bool *fnt_read, uint8_t *scl_cnt, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t fnt[2][16]){
  uint8_t y1 = y0 + 15;
  uint8_t Hscl_cnt = 16;

  if(VorH == 'H'){ //半角で水平スクロールならばスクロールカウントは８
    Hscl_cnt = 8;
  }

  if(t_chr != *Lt_chr){
    if(*fnt_read == true && *scl_cnt == 0){
      _SFR.StrDirect_ShinoFNT_readALL(String(t_chr), fnt);
      *fnt_read = false;
    }
    if(VorH == 'V'){
      _ssd1331.Time_Copy_V_Scroll(ForB, 1, fnt, scl_cnt, x0, y0, x0+7, y1, red, green, blue);
    }else if(VorH == 'H'){
      _ssd1331.Time_Copy_H_Scroll(ForB, 1, fnt, scl_cnt, x0, y0, x0+7, y1, red, green, blue);
    }
  }

  if(*scl_cnt == Hscl_cnt){
    *Lt_chr = t_chr;
    *scl_cnt = 0;
    *fnt_read = true;
  }
}

//*********** 16x16自作フォント時刻スクロール **********************
void ESP32_SD_SSD1331_Gadgets::MyFont16x16ClockScroll(char VorH, uint8_t ForB, char t_chr, char *Lt_chr, bool *fnt_read, uint8_t *scl_cnt, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t fnt[2][16]){
  uint8_t y1 = y0 + 15;
  uint8_t Hscl_cnt = 16;

  if(t_chr != *Lt_chr){
    if(*fnt_read == true && *scl_cnt == 0){
      if(t_chr == ' '){ //時間表示の十の位が空白の場合、フォントにゼロを代入
        for(int i=0; i<16; i++){
          for(int j= 0; j<16; j++){
            fnt[i][j] = 0;
          }
        }
      }else{
        uint8_t num = t_chr - 0x30;
        ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, num, fnt);
      }
      *fnt_read = false;
    }
    if(VorH == 'V'){
      _ssd1331.Time_Copy_V_Scroll(ForB, 2, fnt, scl_cnt, x0, y0, x0+15, y1, red, green, blue);
    }else if(VorH == 'H'){
      _ssd1331.Time_Copy_H_Scroll(ForB, 2, fnt, scl_cnt, x0, y0, x0+15, y1, red, green, blue);
    }
  }

  if(*scl_cnt == Hscl_cnt){
    *Lt_chr = t_chr;
    *scl_cnt = 0;
    *fnt_read = true;
  }
}
//*********** 天気予報ロゴ表示 **********************
void ESP32_SD_SSD1331_Gadgets::YahooJ_Weather_TodayTomorrow(uint8_t y0, String str){
  uint8_t today_fnum[3] = {0}, tomorrow_fnum[3] = {0};
  uint8_t today_col[3][3] ={0}, tomorrow_col[3][3] = {0};
  uint8_t weather_fnt[2][2][16] = {0}; //全てゼロで初期化(C++)

  uint8_t from1 = str.indexOf('|', 0);
  uint8_t from2 = str.indexOf('|', 2);
  uint8_t to1 = str.indexOf('|', 2);
  String w_str1 = str.substring(from1, to1);
  String dummy1 = str.substring(from2);
  String w_str2 = dummy1.substring(0, dummy1.indexOf('|', 2));
  Serial.print("Weather Today = "); Serial.println(w_str1);
  Serial.print("Weather Tomorrow = "); Serial.println(w_str2);

  _EWG.WeatherJ_font_num(w_str1, 0, hour(), today_fnum, today_col);
  _EWG.WeatherJ_font_num(w_str2, 1, hour(), tomorrow_fnum, tomorrow_col);

  for(int i=0; i<3; i++){
    if(today_fnum[i] != 0){
      ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, today_fnum[i], weather_fnt[0]);
    }
    if(tomorrow_fnum[i] != 0){
      ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, tomorrow_fnum[i], weather_fnt[1]);
    }
    _ssd1331.SSD1331_8x16_Font_DisplayOut(2, i*16, y0, today_col[i][0], today_col[i][1], today_col[i][2], weather_fnt[0]);
    _ssd1331.SSD1331_8x16_Font_DisplayOut(2, 48+i*16, y0, tomorrow_col[i][0], tomorrow_col[i][1], tomorrow_col[i][2], weather_fnt[1]);
  }
}
//***************************
void ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(File F, uint8_t ZorH, uint8_t num, uint8_t buf[2][16]){
  F.seek(num * (16 * ZorH));
  F.read(buf[0], 16);
  F.read(buf[1], 16);
}