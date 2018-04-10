
//http://arduino.vn
//https://github.com/NickChungVietNam/transmit_class
// xem hướng dẫn trong tài liệu kèm theo




// 2 arduino nói chuyện với nhau (gửi và lắng nghe cùng lúc)
/// Code của 2 arduino giống hệt nhau

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













TRANSMIT_CLASS arduino(&RR, &AV, &CL, &WB);//Gửi 
//
DATA_TRANSMIT_PACKET goi_tin("hello");// 
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&goi_tin};//TỔNG HỢP GÓI 

void setup() {
 
  Serial.begin(9600);
  

}

int aa_gui=0;
int aa_nhan=0;

void loop() {

arduino.SEND_DATA(
      goi_tin, //gửi bằng gói này 
      (int)(0), //số kiểu int
       1, // gói gồm 1 biến
      aa_gui
    );
 aa_gui++;// thay đổi 
//////// Sau đó lắng nghe dữ liệu gửi trả


  uint8_t error_get = 0;
  uint8_t error_listen_label = arduino.LISTEN_LABEL(5000, PACKET_ALL); 
  // timeout =5 giây , gói tổng hợp PACKET_ALL
   if(error_listen_label==LISTEN_OK){
    //////
      error_get= arduino.GET_DATA(
                5000,
                goi_tin,//timeout = 5 giây, Lấy gói Nhan
                (int)(0),//  biến kiểu (int) , 
                1,//số lượng 1
                 &aa_nhan 
                );
       if(error_get == GET_OK){  

       Serial.print("Da nhan=");Serial.println(aa_nhan);
        }
   }
 
 Serial.print("error_listen_label= "); Serial.println(error_listen_label);
 Serial.print("error_get= "); Serial.println(error_get);
 // khi arduino đang ở chế độ gửi 
 // không nên sử dụng   Serial để xuất dữ liệu kiểm tra (nó sẽ gây chồng chéo dữ liệu gửi)
 //Serial.print() phía trên chỉ mang tính chất kiểm tra hoạt động của thư viện
      
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