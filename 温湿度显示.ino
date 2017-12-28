#include <U8glib.h>//使用OLED需要包含这个头文件
#include <IRremote.h>
#include "font.h"
#include <Wire.h>//调用收发数据所使用的库函数
#include "I2Cdev.h"
float sensor_tem;//float定义基本数据单精度常量
//float SHT2x.readRH();
#include <SHT2x.h>
#define INTERVAL_LCD 20 //定义OLED刷新时间间隔 
int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long lcd_time = millis();
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); //设置OLED型号
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // 启动红外解码
  Serial.println("Initialisation complete.");
}
void loop() {
  /*Serial.println(value, HEX);*/
  sensor_tem = SHT2x.readT();//把获得的温度值赋给变量sensor_tem
  Serial.println(sensor_tem);//将数据从Arduino传递到PC且单独占据一行，此数据可在串口监视器中看到
  Serial.print("--");
  Serial.println(SHT2x.readRH());
  u8g.firstPage();
  do {
    u8g.drawXBMP(0, 0, 16, 16, u4);
    u8g.drawXBMP(16, 0, 16, 16, u5);
    u8g.drawXBMP(32, 0, 16, 16, u6);
    u8g.drawXBMP(0, 16, 16, 16, u7);
    u8g.drawXBMP(16, 16, 16, 16, u5);
    u8g.drawXBMP(32, 16, 16, 16, u6);
    setFont_L;
    u8g.setPrintPos(64, 16);
    u8g.print(sensor_tem);
    setFont_L;
    u8g.setPrintPos(64, 32);
    u8g.print(SHT2x.readRH());
  } while ( u8g.nextPage() );
}
