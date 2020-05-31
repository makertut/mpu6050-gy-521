#include "U8glib.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

U8GLIB_ST7920_128X64 u8g( 13, 11, 10 , U8G_PIN_NONE );


class Ball{
  public:
    int x=64;
    int y=32;
    int mass =1;
};
Ball *ball;

void draw(void) {

  enum {BufSize=9};
  char buf[BufSize];
  snprintf (buf, BufSize, "%d", ax);

  // graphic commands to redraw the complete screen should be placed here 
  u8g.setFont(u8g_font_helvB08);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 31, buf);

   u8g.drawLine(0,32,127,32);
   u8g.drawLine(64,0,64,63);

   ball->x = ay;
   ball->y = ax;

   u8g.drawDisc( ball->x,ball->y,10,U8G_DRAW_ALL);

}

void setup(void) {
  // flip screen, if required
   u8g.setRot180();

  Serial.begin(9600);

  Wire.begin();
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
 
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
 
  pinMode(8, OUTPUT);
}

void loop(void) {

   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

ax = map(ax, -18000, 18000, 63, 0);
ay = map(ay, -18000, 18000, 127, 0);
az = map(az, -18000, 18000, 0, 180);
gx = map(gx, -18000, 18000, 0, 180);
gy = map(gy, -18000, 18000, 0, 180);
gz = map(gz, -18000, 18000, 0, 180);

Serial.print("Axyz \t");
Serial.print(ax); Serial.print("\t");
Serial.print(ay); Serial.print("\t");
Serial.print(az); Serial.print("\t");

Serial.print("Gxyz \t");
Serial.print(gx); Serial.print("\t");
Serial.print(gy); Serial.print("\t");
Serial.println(gz);

delay(50);

 
  // picture loop
  u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() );
 
  // rebuild the picture after some delay
  //delay(50);
}
