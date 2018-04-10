

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













TRANSMIT_CLASS Slave(&RR, &AV, &CL, &WB);//Nhận 
//
DATA_TRANSMIT_PACKET A("abc");// tạo 1 gói tin với tên "abc"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A};//TỔNG HỢP GÓI 

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


uint8_t text_array_buffer[250];//tạo mảng buffer kích thước lớn hơn hoặc bằng kích thước sẽ nhận
//bắt buộc kiểu uint8_t 

void print_(uint8_t size_){
  for(uint8_t i=0; i<  size_; i++){
    Serial.print((char)text_array_buffer[i]); 

  } 
}


void loop() {


   uint16_t length_receive=0;
  uint8_t error_get = 0;
  uint8_t error_listen_label = Slave.LISTEN_LABEL(5000, PACKET_ALL); 
  // timeout =5 giây , gói tổng hợp PACKET_ALL
   if(error_listen_label==LISTEN_OK){
    

      error_get= Slave.GET_BINARY_ARRAY(
                5000,A,//timeout = 5 giây, Lấy gói A
                250,// kích thước của mảng đệm text_array_buffer
                 text_array_buffer,// mảng cần lưu
                 &length_receive // chiều dài binary nhận được
                );

       if(error_get == GET_OK){

       print_(length_receive);

       }else{
        
       Serial.print("error_get = "); Serial.println(error_get);
       }

   
   }else{

    Serial.print("error_listen_label= "); Serial.println(error_listen_label);
   }
   
}

// Hàm .GET_DATA_ARRAY() : dành cho nhận dữ liệu mảng 
// Hàm .GET_BINARY_ARRAY() : dành cho nhận dữ liệu nhị phân bất kỳ

/*
// Tên gói tin giống với gói gửi ("abc")
// số lượng biến liệt kê bằng với số lượng của gói gửi (4)

// kiểu dữ liệu của mảng bắt buộc  uint8_t  (khi dùng để nhận dạng binary)

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