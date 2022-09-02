/*
  ESP32_SD_SSD1331_Gadgets.cpp
  Beta version 1.2

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

//*********** 初期化　自作フォントファイルオープン **********************
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
void ESP32_SD_SSD1331_Gadgets::MyFont16x16ClockScroll(char VorH, uint8_t ForB, char t_chr, char *Lt_chr, bool *fnt_read, uint8_t *scl_cnt, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t (&fnt)[2][16]){
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
//***************************
void ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(File F, uint8_t ZorH, uint8_t num, uint8_t (&buf)[2][16]){
  F.seek(num * (16 * ZorH));
  F.read(buf[0], 16);
  F.read(buf[1], 16);
}
//***************************
void ESP32_SD_SSD1331_Gadgets::Scroll_1_line(uint8_t y, uint8_t num, uint8_t red, uint8_t green, uint8_t blue, uint16_t interval, uint8_t sjis_code[], uint16_t length, bool *WebGet){
  if((xTaskGetTickCount() - _xLastTime[num]) > interval){
    if(*WebGet){
      _SJ_cnt[num] = 0;
      _scl_cnt[num] = 0;
      _Zen_or_Han_cnt[num] = 0;
      _fnt_read_ok[num] = true;
      
      for(int i=0; i<16; i++){
        _Font_buf[num][0][i] = 0;
        _Font_buf[num][1][i] = 0;
      }
      
      *WebGet = false;
    }
    if(_fnt_read_ok[num] == true){
      _zen_or_han[num] = _SFR.Sjis_inc_FntRead(sjis_code, length, &_SJ_cnt[num], _Font_buf[num]);
    }
    _fnt_read_ok[num] = _ssd1331.Scroller_8x16_RtoL4line(y, num, _zen_or_han[num], &_scl_cnt[num], &_Zen_or_Han_cnt[num], _Font_buf[num], red, green, blue);
    _xLastTime[num] = xTaskGetTickCount();
  }
}
//******************** root CA SD card Get **********************
void ESP32_SD_SSD1331_Gadgets::Root_CA_SDcard_Read(const char *filename, char *Root_CA){
    //SD.begin(CS_SD, SPI, 24000000, "/sd");

    File file = SD.open(filename, FILE_READ);
    if (!file) {
      Serial.print(filename);
      Serial.print(" File not found");
      return;
    }else{
      Serial.print(filename);
      Serial.println(" File read OK!");
    }
    uint16_t i = 0;
    while(file.available()){
      Root_CA[i] = file.read();
      i++;
    }
    Root_CA[i] = '\0';
    file.close();
}

//*************気象庁天気予報用****************************
void ESP32_SD_SSD1331_Gadgets::dispJapanWeatherMyFont( uint8_t y0, String weather_str )
{
  uint8_t today_fnum[3] = {}, tomorrow_fnum[3] = {};
  uint8_t today_col[3][3] ={}, tomorrow_col[3][3] = {};
  uint8_t weather_fnt_buf[2][2][16] = {};
  String weather_code_from_key = "weatherCodes\":[\"";

  uint8_t from1 = weather_str.indexOf(weather_code_from_key, 0) + weather_code_from_key.length();
  uint8_t to1 = from1 + 3;
  uint8_t from2 = to1 + 3;
  uint8_t to2 = from2 + 3;
  String today_w_code_str = weather_str.substring(from1, to1);
  String tomorrow_w_code_str = weather_str.substring(from2, to2);
  uint16_t today_weather_code = atoi(today_w_code_str.c_str());
  uint16_t tomorrow_weather_code = atoi(tomorrow_w_code_str.c_str());
  //Serial.print(F("Today Weather Coode_str = ")); Serial.println(today_w_code_str);
  //Serial.print(F("Tomorrow Weather Code_str = ")); Serial.println(tomorrow_w_code_str);
  Serial.print(F("Today Weather Coode = ")); Serial.println(today_weather_code);
  Serial.print(F("Tomorrow Weather Code = ")); Serial.println(tomorrow_weather_code);

  ESP32_SD_SSD1331_Gadgets::JapanWeatherFontNum(today_weather_code, 0, hour(), today_fnum, today_col);
  ESP32_SD_SSD1331_Gadgets::JapanWeatherFontNum(tomorrow_weather_code, 1, hour(), tomorrow_fnum, tomorrow_col);

  //なぜか"Cannot file.seek in SD file system" というメッセージが出た
  for(int i=0; i<3; i++){
    if(today_fnum[i] != 0){
      //Serial.print("today_fnum=");
      //Serial.println(today_fnum[i]);
      ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, today_fnum[i], weather_fnt_buf[0]);
    }
    //weather_font.x0 = 64 + i * 16 * weather_font.Xsize;
    //weather_font.red = today_col[i][0], weather_font.green = today_col[i][1], weather_font.blue = today_col[i][2];
    //weather_font.bg_red = 0, weather_font.bg_green = 0, weather_font.bg_blue = 0; 
    //LCD.display8x16Font(weather_font, 2, weather_fnt_buf[0]);
    _ssd1331.SSD1331_8x16_Font_DisplayOut(2, i*16, y0, today_col[i][0], today_col[i][1], today_col[i][2], weather_fnt_buf[0]);
    delay(1);
  }

  for(int i=0; i<3; i++){
    if(tomorrow_fnum[i] != 0){
      //Serial.print("tomorrow_fnum=");
      //Serial.println(tomorrow_fnum[i]);
      ESP32_SD_SSD1331_Gadgets::MyFont_SD_Read(_MyF, 2, tomorrow_fnum[i], weather_fnt_buf[1]);
    }
    //weather_font.x0 = 224 + i * 16 * weather_font.Xsize;
    //weather_font.red = tomorrow_col[i][0], weather_font.green = tomorrow_col[i][1], weather_font.blue = tomorrow_col[i][2];
    //LCD.display8x16Font( weather_font, 2, weather_fnt_buf[1] );
    _ssd1331.SSD1331_8x16_Font_DisplayOut(2, 48+i*16, y0, tomorrow_col[i][0], tomorrow_col[i][1], tomorrow_col[i][2], weather_fnt_buf[1]);
    delay(1);
  }
}
//***************** 気象庁 Weather MyFont Number get *************************************************
void ESP32_SD_SSD1331_Gadgets::JapanWeatherFontNum(
  uint16_t weather_code,
  uint8_t wDay,
  uint8_t Htime,
  uint8_t (&Fnum)[3],
  uint8_t (&col)[3][3])
{
  uint8_t Sunny_red = 31, Sunny_green = 40, Sunny_blue = 0;
  uint8_t Cloudy_red = 29, Cloudy_green = 60, Cloudy_blue = 29;
  uint8_t Rain_red = 0, Rain_green = 0, Rain_blue = 31;
  uint8_t Snow_red = 31, Snow_green = 63, Snow_blue = 31;
  uint8_t Thunder_red = 31, Thunder_green = 63, Thunder_blue = 0;

  col[1][0] = 31; col[1][1] = 63; col[1][2] = 31; //矢印

  uint8_t sunny_fnt_num;
  if((wDay == 0) && (Htime >= 15)){ //wDay = 0 今日、wDay = 1 明日
    Sunny_red =  Thunder_red, Sunny_green = Thunder_green, Sunny_blue = Thunder_blue;
    sunny_fnt_num = 26;
  }else{
    sunny_fnt_num = 20;
  }

  uint8_t cloudy_fnt_num = 21;
  uint8_t rain_fnt_num = 22;
  uint8_t snow_fnt_num = 24;
  uint8_t storm_fnt_num = 23;
  //uint8_t thunder_fnt_num = 25;


  switch(weather_code){
    //--------Clear（晴れ）-----------------
    case 100:
    case 123:
    case 124:
    case 130:
    case 131:
      //Serial.println("晴れ");
      Fnum[0] = 0;
      col[1][0] =  Sunny_red; col[1][1] = Sunny_green; col[1][2] = Sunny_blue;
      Fnum[1] = sunny_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;

    //--------晴れ時々（一時）曇り----------------
    case 101:
    case 132:
      //Serial.println("晴れ時々曇り");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = cloudy_fnt_num;
      break;

    //--------晴れ時々（一時）雨----------------
    case 102:
    case 103:
    case 106:
    case 107:
    case 108:
    case 120:
    case 121:
    case 140:
      //Serial.println("晴れ時々雨");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;

    //--------晴れ時々（一時）雪----------------
    case 104:
    case 105:
    case 160:
    case 170:
      //Serial.println("晴れ時々雪");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;

    //--------晴れ後曇り----------------
    case 110:
    case 111:
      //Serial.println("晴れ後曇り");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = cloudy_fnt_num;
      break;

    //--------晴れ後雨----------------
    case 112:
    case 113:
    case 114:
    case 118:
    case 119:
    case 122:
    case 125:
    case 126:
    case 127:
    case 128:
      //Serial.println("晴れ後雨");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;

    //--------晴れ後雪----------------
    case 115:
    case 116:
    case 117:
    case 181:
      //Serial.println("晴れ後雪");
      col[0][0] =  Sunny_red; col[0][1] = Sunny_green; col[0][2] = Sunny_blue;
      Fnum[0] = sunny_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;
    
    //--------曇り-----------------
    case 200:
    case 209:
    case 231:
      //Serial.println("曇り");
      Fnum[0] = 0;
      col[1][0] =  Cloudy_red; col[1][1] = Cloudy_green; col[1][2] = Cloudy_blue;
      Fnum[1] = cloudy_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;

    //--------曇り時々晴れ-----------------
    case 201:
    case 223:
      //Serial.println("曇り時々晴れ");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;
    
    //--------曇り時々雨-----------------
    case 202:
    case 203:
    case 206:
    case 207:
    case 208:
    case 220:
    case 221:
    case 240:
      //Serial.println("曇り時々雨");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;
    
    //--------曇り一時雪-----------------
    case 204:
    case 205:
    case 250:
    case 260:
    case 270:
      //Serial.println("曇り一時雪");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;

    //--------曇り後晴れ-----------------
    case 210:
    case 211:
      //Serial.println("曇り後晴れ");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;

    //--------曇り後雨-----------------
    case 212:
    case 213:
    case 214:
    case 218:
    case 219:
    case 222:
    case 224:
    case 225:
    case 226:
      //Serial.println("曇り後雨");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;
    
    //--------曇り後雪-----------------
    case 215:
    case 216:
    case 217:
    case 228:
    case 229:
    case 230:
    case 281:
      //Serial.println("曇り後雪");
      col[0][0] =  Cloudy_red; col[0][1] = Cloudy_green; col[0][2] = Cloudy_blue;
      Fnum[0] = cloudy_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;
    
    //--------雨-----------------
    case 300:
    case 304:
    case 306:
    case 328:
    case 329:
    case 350:
      //Serial.println("雨");
      Fnum[0] = 0;
      col[1][0] =  Rain_red; col[1][1] = Rain_green; col[1][2] = Rain_blue;
      Fnum[1] = rain_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;

    //--------雨時々晴れ-----------------
    case 301:
      //Serial.println("雨時々晴れ");
      col[0][0] =  Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;
    
    //--------雨時々曇り-----------------
    case 302:
      //Serial.println("雨時々曇り");
      col[0][0] =  Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = cloudy_fnt_num;
      break;
    
    //--------雨時々雪-----------------
    case 303:
    case 309:
    case 322:
      //Serial.println("雨時々雪");
      col[0][0] =  Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;
    
    //--------暴風雨-----------------
    case 308:
      //Serial.println("暴風雨");
      Fnum[0] = 0;
      col[1][0] =  Rain_red; col[1][1] = Rain_green; col[1][2] = Rain_blue;
      Fnum[1] = storm_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;
    
    //--------雨後晴れ-----------------
    case 311:
    case 316:
    case 320:
    case 323:
    case 324:
    case 325:
      //Serial.println("雨後晴れ");
      col[0][0] = Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;

    //--------雨後曇り-----------------
    case 313:
    case 317:
    case 321:
      //Serial.println("雨後曇り");
      col[0][0] = Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = cloudy_fnt_num;
      break;

    //--------雨後雪-----------------
    case 314:
    case 315:
    case 326:
    case 327:
      //Serial.println("雨後雪");
      col[0][0] = Rain_red; col[0][1] = Rain_green; col[0][2] = Rain_blue;
      Fnum[0] = rain_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Snow_red; col[2][1] = Snow_green; col[2][2] = Snow_blue;
      Fnum[2] = snow_fnt_num;
      break;
    
    //--------雪-----------------
    case 340:
    case 400:
    case 405:
    case 425:
    case 426:
    case 427:
    case 450:
      //Serial.println("雪");
      Fnum[0] = 0;
      col[1][0] =  Snow_red; col[1][1] = Snow_green; col[1][2] = Snow_blue;
      Fnum[1] = storm_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;
    
    //--------雪時々晴れ-----------------
    case 401:
      //Serial.println("雪時々晴れ");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;
    
    //--------雪時々曇り-----------------
    case 402:
      //Serial.println("雪時々曇り");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = sunny_fnt_num;
      break;
    
    //--------雪時々雨-----------------
    case 403:
    case 409:
      //Serial.println("雪時々雨");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 27; //[時々（一時）]縦棒
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;
    
    //--------暴風雪-----------------
    //暴風雪アイコンはまだ無い
    case 406:
    case 407:
      //Serial.println("暴風雪");
      Fnum[0] = 0;
      col[1][0] =  Snow_red; col[1][1] = Snow_green; col[1][2] = Snow_blue;
      Fnum[1] = storm_fnt_num; //縦棒、または矢印
      Fnum[2] = 0;
      break;
    
    //--------雪後晴れ-----------------
    case 361:
    case 411:
    case 420:
      //Serial.println("雪後晴れ");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Sunny_red; col[2][1] = Sunny_green; col[2][2] = Sunny_blue;
      Fnum[2] = sunny_fnt_num;
      break;
    
    //--------雪後曇り-----------------
    case 371:
    case 413:
    case 421:
      //Serial.println("雪後曇り");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Cloudy_red; col[2][1] = Cloudy_green; col[2][2] = Cloudy_blue;
      Fnum[2] = cloudy_fnt_num;
      break;

    //--------雪後雨-----------------
    case 414:
    case 422:
    case 423:
      //Serial.println("雪後雨");
      col[0][0] =  Snow_red; col[0][1] = Snow_green; col[0][2] = Snow_blue;
      Fnum[0] = snow_fnt_num;
      Fnum[1] = 28; //[のち]矢印
      col[2][0] =  Rain_red; col[2][1] = Rain_green; col[2][2] = Rain_blue;
      Fnum[2] = rain_fnt_num;
      break;

    default:
      break;
  }
 }