

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
void WB(uint8_t c) { Serial.write(c); COUNT_OVER ++; if(COUNT_OVER> 50){ delay(5); COUNT_OVER=0;  }}

 //làm gì  đó để clear buffer vào đây, ví dụ :
void CL() {
  uint32_t Buffer_available= AV() ;
  for( uint32_t i=0; i< Buffer_available ; i++){  RR();}//đọc hết nhũng gì chưa đọc
}

///////////













TRANSMIT_CLASS Master(&RR, &AV, &CL, &WB);//Gửi 
//
DATA_TRANSMIT_PACKET A("abc");// tạo 1 gói tin với tên "abc"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A};//TỔNG HỢP GÓI 

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

uint8_t text_array_buffer[100];//tạo mảng buffer lớn hơn kích thước String 
//bắt buộc kiểu uint8_t
 uint16_t  from_string_to_array(String t){
   // phân giải String về mảng
  for(uint16_t i=0; i<t.length() ; i++){
     text_array_buffer[i]=(uint8_t)t.charAt(i);
  }
  return t.length();//trả về độ dài (kích thước ) String
}
void loop() {
String text_st="Hello I'm master, Can i talk with you ? ";
 
 
 uint16_t size_=from_string_to_array(text_st);

Master.SEND_DATA_ARRAY(
      A, //gửi gói tin A ("abc")
      (uint8_t)(0), // mảng kiểu (uint8_t)  
      size_,// kích thước cần gửi
      text_array_buffer // mảng cần gửi
    );

 // khi arduino đang ở chế độ gửi 
 //  thì không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
 
  delay(500);
}
/*
// Tên gói tin không được quá 255 kí tự ("abc")
// kích thước mảng là tùy ý
// kiểu dữ liệu của mảng uin8_t 
// mỗi gói chỉ chấp nhận loại biến có cùng kiểu đã khai báo
// số lượng mảng = 1 
// kích thước gói (PAYLOAD) không quá 65535 byte 
//PAYLOAD = (số lượng phần tử) * (cỡ của biến ) = 4 * (sizeof(int))= 8 byte


kiểu (byte ,uint8_t , int8_t, char , unsigned char  ) sizeof=1 byte
kiểu (int ,unsigned int, uint16_t , int16_t, char16_t ) sizeof= 2 byte
kiểu (int32_t, uint32_t, long, unsigned long , float, double) sizeof= 4byte

riêng kiểu double trên arduino 8 bit là =4 và trên các máy nhúng khác như Arduino Due, esp32, esp8266 (32bit) thì sizeof= 8byte
*/