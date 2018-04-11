

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
  for( uint32_t i=0; i< Buffer_available ; i++){  RR();}//đọc hết nhũng gì chưa đọc
}

/////////// 













TRANSMIT_CLASS Slave(&RR, &AV, &CL, &WB);//Nhận 
//
DATA_TRANSMIT_PACKET A("abcd");// tạo 1 gói tin với tên "abcd"
DATA_TRANSMIT_PACKET B("efgh");// tạo 1 gói tin với tên "efgh"
DATA_TRANSMIT_PACKET C("iklm");// tạo 1 gói tin với tên "iklm"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A , &B,  &C};//TỔNG HỢP GÓI 

void setup() {
  /*
  //bạn có thể thay đổi lại tùy thích ở đây
    Slave.ADD_READ_BYTE( &RR);
    Slave.ADD_AVAILABLE(  &AV);
    Slave.ADD_CLEAR( &CL);
    Slave.ADD_WRITE_BYTE(&WB);
  */

  Serial.begin(9600);
 

}

int aa0=0,aa1=0,aa2=0;
long bb0=0,bb1=0,bb2=0,bb3=0;
double cc0=0.0,cc1=0.0,cc2=0.0,cc3=0.0, cc4=0.0;


void check_A(){
//check số đại diện thôi nhé
  Serial.print("check_A="); Serial.print(aa0);Serial.println();
 // không được sử dụng Serial để test check khi arduino ở chế độ Gửi 
}
void check_B(){
 Serial.print("check_B=");Serial.print(bb0);Serial.println();

}
void check_C(){ 
  Serial.print("check_C=");Serial.print(cc0,9);Serial.println();

}
void loop() {

  
//Kiểu float hiện không được sử dụng để khai báo trong  SEND_DATA() và GET_DATA(); nhưng có thể sử dụng bình thường trong SEND_DATA_ARRAY() và GET_DATA_ARRAY();

   
  uint8_t error_get = 0;
  uint8_t error_listen_label = Slave.LISTEN_LABEL(5000, PACKET_ALL); 
  // timeout =5 giây , gói tổng hợp PACKET_ALL
   if(error_listen_label==LISTEN_OK){
    //////
      error_get= Slave.GET_DATA(
                5000,A,//timeout = 5 giây, Lấy gói A
                (int)(0),//  biến kiểu (int) , 
                3,//số lượng  (3)
                 &aa0, &aa1, &aa2 // liệt kê vào
                );
       if(error_get == GET_OK){  check_A();  }

       ////// 
        error_get= Slave.GET_DATA(
                5000,B,//timeout = 5 giây, Lấy gói B
                (long)(0),//  biến kiểu (long) , 
                4,//số lượng  (4)
                 &bb0, &bb1, &bb2, &bb3 // liệt kê vào
                );
       if(error_get == GET_OK){  check_B();  }

        //////
        error_get= Slave.GET_DATA(
                5000,C,//timeout = 5 giây, Lấy gói C
                (double)(0),//  biến kiểu ( double) , 
                5,//số lượng  (5)
                 &cc0, &cc1, &cc2,&cc3,&cc4 // liệt kê vào
                );
       if(error_get == GET_OK){  check_C();  }

   }
Serial.print("error_listen_label= "); Serial.println(error_listen_label);
 Serial.print("error_get= "); Serial.println(error_get);
 
}

/*
// Tên gói tin giống với gói gửi ("abc")
// số lượng biến liệt kê bằng với số lượng của gói gửi (4)
// kiểu dữ liệu giống với gói gửi (int)
//timeout là thời gian đợi lâu nhất 

*/
/*
Các trạng thái trả về 

(error_listen_label) có thể bằng các trường hợp sau :

  LISTEN_OK=0,// không có bất cứ lỗi nào khi tìm tên 
  ERROR_NO_FOUND_LABEL_NAME=1,//Không tìm được tên nhãn 
  ERROR_NO_FOUND_LENGTH_NAME=2,////Không tìm được  độ dài tên 
  ERROR_NOT_FIT_LENGTH_NAME=3,// lỗi độ dài tên không khớp với bất kì biến nào
  ERROR_NO_FOUND_FULL_NAME=4,///Không tìm được toàn bộ tên
  ERROR_NAME_NOT_TRUE=5,///Đã tìm được tên nhưng Tên không giống với bất kỳ tên nào đã khai báo

(error_get ) có thể bằng : 
  GET_OK=0,//không lỗi
  ERROR_NO_SELECT=1,// không được lựa chọn 
  ERROR_NO_FOUND_PAYLOAD_LEGNTH=2,// KO tìm được độ dài của gói dữ liệu (PAYLOAD)
  ERROR_PAYLOAD_LEGNTH_BIGGER=3,//PAYLOAD lớn hơn kích thước phù hợp
  ERROR_PAYLOAD_LEGNTH_SMALLER=4,//PAYLOAD nhỏ hơn kích thước phù hợp
  ERROR_READ_NOT_FULL=4,//chưa nạp đủ dữ liệu
  ERROR_NO_FOUND_CHECK_SUM=5,// chưa tìm được checksum
  ERROR_CHECK_SUM=6,// lỗi checksum
*/