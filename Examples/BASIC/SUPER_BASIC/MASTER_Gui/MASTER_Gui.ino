

//http://arduino.vn/tutorial/5897-thu-vien-truyen-du-lieu-bat-ky-byte-long-float-double-theo-goi-tin-cho-arduino

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
	for( uint32_t i=0; i< Buffer_available ; i++){	RR();}//đọc hết nhũng gì chưa đọc
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

double i0=0.123;//gửi số này đi
void loop() {

//Kiểu float hiện không được sử dụng để khai báo trong  SEND_DATA() và GET_DATA(); nhưng có thể sử dụng bình thường trong SEND_DATA_ARRAY() và GET_DATA_ARRAY();

Master.SEND_DATA(
       A, //gửi gói tin A ("abc")
       (double)(0), //số kiểu double
       1, // gói gồm 1 biến
       i0 //liệt kê vào 
       );
i0+=1; // thay đổi cho sinh động

  delay(1000);
   // khi arduino đang ở chế độ gửi 
 //  thì không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
 
}
/*

// Tên gói tin không được quá 255 kí tự ("abc")
// số lượng biến liệt kê là tùy ý (1)

// kiểu dữ liệu của gói là tùy ý (double)
// mỗi gói chỉ chấp nhận loại biến có cùng kiểu đã khai báo
// kích thước gói (PAYLOAD) không quá 65535 byte 
//PAYLOAD = (số lượng biến) * (cỡ của biến ) = 1 * (sizeof(int))= 4 byte


kiểu (byte ,uint8_t , int8_t, char , unsigned char  ) sizeof=1 byte
kiểu (int ,unsigned int, uint16_t , int16_t, char16_t ) sizeof= 2 byte
kiểu (int32_t, uint32_t, long, unsigned long , float, double) sizeof= 4byte

riêng kiểu double trên arduino 8 bit là =4 và trên các máy nhúng khác như Arduino Due, esp32, esp8266 (32bit) thì sizeof= 8byte
*/
