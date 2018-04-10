

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













TRANSMIT_CLASS Master(&RR, &AV, &CL, &WB);// 
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

int aa[4]={0,0,0,0};
long bb[4]={0,0,0,0};
double cc[4]={0,0,0,0};

void TANG_(){
	for(uint8_t i=0; i< 4; i++){
		aa[i]++;
		bb[i]++;
		cc[i]++;
	}
 }
void loop() {

Master.SEND_DATA_ARRAY(
      A, //gửi bằng gói này 
      (int)(0), //số kiểu int
       4, // gói gồm 3 biến
      aa  // mảng 
    );


Master.SEND_DATA_ARRAY(
      B, //gửi bằng gói này 
      (long)(0), //số kiểu long
       4, // gói gồm 4 biến
      bb//mảng
    );
Master.SEND_DATA_ARRAY(
      C, //gửi bằng gói này 
      (double)(0), //số kiểu double
       4, // gói gồm 4 biến
      cc// mảng
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
// số lượng mảng cho mỗi gói tin phải = 1 
// kích thước gói (PAYLOAD) không quá 65535 byte 
//PAYLOAD = (số lượng biến) * (cỡ của biến ) = 4 * (sizeof(int))= 8 byte


kiểu (byte ,uint8_t , int8_t, char , unsigned char  ) sizeof=1 byte
kiểu (int ,unsigned int, uint16_t , int16_t, char16_t ) sizeof= 2 byte
kiểu (int32_t, uint32_t, long, unsigned long , double) sizeof= 4byte
// chú ý:
+kiểu float không được hỗ trợ cho list va_arg 
+riêng kiểu double trên arduino 8 bit là =4 và trên các máy nhúng khác như Arduino Due, esp32, esp8266 (32bit) thì sizeof= 8byte
*/