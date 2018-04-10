

//http://arduino.vn
//https://github.com/NickChungVietNam/transmit_class
// xem hướng dẫn trong tài liệu kèm theo



#include "TRANSMIT_CLASS.h"

////////// CODE NÀY DÀNH CHO ARDUINO MEGA , xuất 2 gói tin ra 2 cổng TX0 và TX1
// Hãy chọn lại board là arduino mega trước khi biên dịch

// Sử dụng cổng Serial0 của arduino mega
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


// Sử dụng cổng Serial1 của arduino mega

//Truyền 1 hàm đọc 1 byte dữ liệu 
uint8_t RR1() { return (uint8_t) Serial1.read();   }
// Truyền 1 hàm đọc kích thước sẵn có
uint32_t AV1() {  return (uint32_t)Serial1.available();}
// Truyền 1 hàm gửi 1 byte dữ liệu
uint8_t COUNT_OVER1=0; // dùng để làm trễ 
void WB1(uint8_t c) { Serial1.write(c); COUNT_OVER1 ++; if(COUNT_OVER1> 50){ delay(5); COUNT_OVER=0;  }}

 //làm gì  đó để clear buffer vào đây, ví dụ :
void CL1() {
  uint32_t Buffer_available= AV1() ;
  for( uint32_t i=0; i< Buffer_available ; i++){  RR1();}//đọc hết nhũng gì chưa đọc
}

///////////



////////// CODE NÀY DÀNH CHO ARDUINO MEGA , xuất 2 gói tin ra 2 cổng TX0 và TX1

// Tạo 2 thiết bị Transmit riêng biệt cho 2 cổng
TRANSMIT_CLASS Master0(&RR, &AV, &CL, &WB);//
TRANSMIT_CLASS Master1(&RR1, &AV1, &CL1, &WB1);//






DATA_TRANSMIT_PACKET A("test");// tạo 1 gói tin với tên "test"

DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A};//TỔNG HỢP GÓI 

void setup() {
  

  Serial.begin(9600);// mở cổng TX-RX 0
  Serial1.begin(9600); // mở cổng TX-RX 1
  

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
String text_st="I'm master 0 ";
 
 
 uint16_t size_=from_string_to_array(text_st);

//Xuất ra cổng TX0 trên arduino mega
   Master0.SEND_DATA_ARRAY(
      A, //gửi gói tin A 
      (uint8_t)(0), // mảng kiểu (char)  
      size_,// kích thước cần gửi
      text_array_buffer // mảng cần gửi
    );


   text_st="I'm master 1";
   size_=from_string_to_array(text_st);

//Xuất ra cổng TX1 trên arduino mega
   Master1.SEND_DATA_ARRAY(
      A, //gửi gói tin A 
      (uint8_t)(0), // mảng kiểu (char)  
      size_,// kích thước cần gửi
      text_array_buffer // mảng cần gửi
    );
  delay(500);


 // khi arduino đang ở chế độ gửi 
 //  thì không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
 
}
/*
// Tên gói tin không được quá 255 kí tự ("abc")
// kích thước mảng là tùy ý
// kiểu dữ liệu của mảng uin8_t (khi dùng để gửi dạng binary)
// số lượng mảng cho mỗi gói tin phải = 1 
// kích thước gói (PAYLOAD) không quá 65535 byte 
//PAYLOAD = (số lượng phần tử) * (cỡ của biến ) = 4 * (sizeof(int))= 8 byte


kiểu (byte ,uint8_t , int8_t, char , unsigned char  ) sizeof=1 byte
kiểu (int ,unsigned int, uint16_t , int16_t, char16_t ) sizeof= 2 byte
kiểu (int32_t, uint32_t, long, unsigned long , float, double) sizeof= 4byte

riêng kiểu double trên arduino 8 bit là =4 và trên các máy nhúng khác như Arduino Due, esp32, esp8266 (32bit) thì sizeof= 8byte
*/