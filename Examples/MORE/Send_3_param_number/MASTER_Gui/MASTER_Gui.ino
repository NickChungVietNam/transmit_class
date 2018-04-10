

//http://arduino.vn
//https://github.com/NickChungVietNam/transmit_class
// xem hướng dẫn trong tài liệu kèm theo



#include "TRANSMIT_CLASS.h"
//Truyền 1 hàm đọc 1 byte dữ liệu 
uint8_t RR() { return (uint8_t) Serial.read();   }
// Truyền 1 hàm đọc kích thước sẵn có
uint32_t AV() {  return (uint32_t)Serial.available();}
// Truyền 1 hàm gửi 1 byte dữ liệu
uint8_t COUNT_OVER=0; // dùng để làm trễ 
void WB(uint8_t c) { Serial.write(c); COUNT_OVER ++; if(COUNT_OVER> 50){ delay(10); COUNT_OVER=0;  }}


 //làm gì  đó để clear buffer vào đây, ví dụ :
void CL() {
  uint32_t Buffer_available= AV() ;
  for( uint32_t i=0; i< Buffer_available ; i++){  RR();}//đọc hết nhũng gì chưa đọc
}

///////////













TRANSMIT_CLASS Master(&RR, &AV, &CL, &WB);//Gửi 
//
DATA_TRANSMIT_PACKET A("abcd");// tạo 1 gói tin với tên "abcd"
DATA_TRANSMIT_PACKET B("efgh");// tạo 1 gói tin với tên "efgh"
DATA_TRANSMIT_PACKET C("iklm");// tạo 1 gói tin với tên "iklm"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A , &B,  &C};//TỔNG HỢP GÓI 

void setup() {
  /*
  //bạn có thể thay đổi lại tùy thích ở đây
    Master.ADD_READ_BYTE( &RR);
    Master.ADD_AVAILABLE(  &AV);
    Master.ADD_CLEAR( &CL);
    Master.ADD_WRITE_BYTE(&WB);
  */

  Serial.begin(9600);
  

}

int aa0=0,aa1=0,aa2=0;
long bb0=0,bb1=0,bb2=0,bb3=0;
double cc0=0.0,cc1=0.0,cc2=0.0,cc3=0.0, cc4=0.0;

void TANG_(){
  aa0++;aa1++; aa2++;     bb0++;bb1++; bb2++;bb3++;    cc0++;cc1++; cc2++;cc3++;cc4++; // thay đổi
}
void loop() {
  
//Kiểu float hiện không được sử dụng để khai báo trong  SEND_DATA() và GET_DATA(); nhưng có thể sử dụng bình thường trong SEND_DATA_ARRAY() và GET_DATA_ARRAY();

Master.SEND_DATA(
      A, //gửi bằng gói này 
      (int)(0), //số kiểu int
       3, // gói gồm 3 biến
      aa0,aa1,aa2 //liệt kê vào 
    );


Master.SEND_DATA(
      B, //gửi bằng gói này 
      (long)(0), //số kiểu long
       4, // gói gồm 4 biến
      bb0,bb1,bb2,bb3 //liệt kê vào 
    );
Master.SEND_DATA(
      C, //gửi bằng gói này 
      (double)(0), //số kiểu  double
       5, // gói gồm 5 biến
      cc0,cc1,cc2,cc3,cc4 //liệt kê vào 
    );

 TANG_();
  delay(1000);

 // khi arduino đang ở chế độ gửi 
 //  thì không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
  
}
/*
// Tên gói tin không được quá 255 kí tự  [("abcd") = 4 ]
// số lượng biến liệt kê là tùy ý (4)
// kiểu dữ liệu của gói là tùy ý (int)
// mỗi gói chỉ chấp nhận loại biến có cùng kiểu đã khai báo
// kích thước gói (PAYLOAD) không quá 65535 byte 
//PAYLOAD = (số lượng biến) * (cỡ của biến ) = 4 * (sizeof(int))= 8 byte


kiểu (byte ,uint8_t , int8_t, char , unsigned char  ) sizeof=1 byte
kiểu (int ,unsigned int, uint16_t , int16_t, char16_t ) sizeof= 2 byte
kiểu (int32_t, uint32_t, long, unsigned long , double) sizeof= 4byte
// chú ý:
+kiểu float không được hỗ trợ cho list va_arg 
+riêng kiểu double trên arduino 8 bit là =4 và trên các máy nhúng khác như Arduino Due, esp32, esp8266 (32bit) thì sizeof= 8byte
*/