# ESP32_SD_SSD1331_Gadgets Beta ver 1.1
This library is for the Arduino core for the ESP32.  
https://github.com/espressif/arduino-esp32  
This library is used by the Arduino IDE(Tested in ver1.8.2).  
  
Use micro SDHC card.  
Use OLED SSD1331 ( SPI ).  
  
The following library is required.  
- ESP32_SD_ShinonomeFNT  
- ESP32_SD_UTF8toSJIS  
- TimeLib (Arduino time library ver1.5)  
- ESP32_SSD1331  
- ESP32_WebGet  
  
My Blog: https://www.mgo-tec.com  

# Change log
(1.1)  
In the function of one line, add the following function to scroll one line of text to OLED SSD 1331 display.  
- Scroll_1_line  
  
Added functions to read the root certificate of SSL page saved on SD card.  

【更新履歴】(Japanese)  
(1.1)  
１行の関数で、OLED SSD1331 ディスプレイに１行の文字列をスクロールする以下の関数を追加  
- Scroll_1_line  
  
SDカードに保存した、SSLページのルート証明書を読み込む関数を追加  
  
  
# Credits and license
*Licensed under the MIT  
  
Copyright (c) 2017 Mgo-tec  
  
Reference Blog --> https://www.mgo-tec.com  
  
# My Blog: 
Other usage is Japanese, please visit my blog.  
https://www.mgo-tec.com  