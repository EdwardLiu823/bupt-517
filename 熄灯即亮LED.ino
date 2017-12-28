#include <Adafruit_NeoPixel.h>//引用头文件
#include <IRremote.h>
#include <U8glib.h>
#define INTERVAL_LCD             20             //定义OLED刷新时间间隔  
#define PIN 6   /*定义了控制LED的引脚，6表示Microduino的D6引脚，可通过Hub转接出来，用户可以更改 */
#define PIN_NUM 2 //定义允许接的led灯的个数
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM, PIN, NEO_GRB + NEO_KHZ800); //该函数第一个参数控制串联灯的个数，第二个是控制用哪个pin脚输出，第三个显示颜色和变化闪烁频率
int lightPin = A6;
int RECV_PIN = 10;   //红外线接收器OUTPUT端接在pin 10
IRrecv irrecv(RECV_PIN);  //定义IRrecv对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号
                         //-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)

void setup() {
  Serial.begin(9600);  //设置波特率
  irrecv.enableIRIn(); // 启动红外解码
  pinMode(D6, OUTPUT);  //设置输出口
}
int a = 600;
int hour = 22;    //初始时间
int minute = 59;
int second = 300;
int maxhour = 23;   //初始工作时间
int minhour = 20;
void loop()
{
  int n = analogRead(A6);   //读取模拟口A3，获取光强
  Serial.println(n);               // 用于IDE串口观察窗
  second = second + 1;              //简易时钟    把1s分为600份，每100ms加一
  if (second >590)
  {
    minute = minute + 1;
    second = 0;
  }
  if (minute > 59)
  {
    hour = hour + 1;
    minute = 0;
  }
  if (n < a && hour > minhour && hour < maxhour)     //判断是否符合工作条件
  {
    strip.setPixelColor(0, strip.Color(255, 255, 255));//LED灯工作
    strip.show();
  }
  else                  //对光强进行判断
  {
    strip.setPixelColor(0, strip.Color(0, 0, 0));//LED灯关闭
    strip.show();
  }
  delay(100);       //防止串口写入速度过快
  if (irrecv.decode(&results)) {                       //通过不同按键调整当前时间以及工作时间
    if ( results.value == 0x1FE807F) {
      minhour++;
    }
    else if ( results.value == 0x1fe20df) {
      minhour--;
    }
    else if ( results.value == 0x1fec03f) {
      maxhour++;
    }
    else if (results.value == 0x1fe609f) {
      maxhour--;
    }
      else if (results.value == 0x1fe50af) {
      hour++;
    }
      else if (results.value == 0x1fe30cf) {
      hour--;
    }
      else if (results.value == 0x1fef807) {
      minute++;
    } 
      else if (results.value == 0x1fe708f) {
      minute--;
    }
    
    if (minhour < 0) {              //控制时间上下限
      minhour = 0;
    }
    if(minhour>60){
      minhour=0;
    }
    if(maxhour<0){
      maxhour=0;
    }
    if (maxhour > 24) {
      maxhour = 0;
    }
    if(minute<0){
      minute=0;
    }
    if (minute>60) {
      maxhour = 0;
    }
    if(hour<0){
       hour=0;
    }
   if (hour>24) {
      hour=0;
    }
    u8g.firstPage();         //显示数据   limit表示工作时间单位均为h，time表示当前时间单位分别为h、min
    do {
      setFont_L;
      u8g.setPrintPos(4, 16);
      u8g.print("limit");
      u8g.setPrintPos(4, 32);
      u8g.print(minhour);
      u8g.print(':');
      u8g.print(maxhour);
      u8g.setPrintPos(4, 48);
      u8g.print("time");
      u8g.setPrintPos(4, 64);
      u8g.print(hour);
      u8g.print(':');
      u8g.print(minute);
    } while (u8g.nextPage());


    irrecv.resume(); //  接收下一个值
  }
}
