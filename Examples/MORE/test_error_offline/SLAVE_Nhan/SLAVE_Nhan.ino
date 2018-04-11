
//http://arduino.vn/tutorial/5897-thu-vien-truyen-du-lieu-bat-ky-byte-long-float-double-theo-goi-tin-cho-arduino

//https://github.com/NickChungVietNam/transmit_class
// xem hướng dẫn trong tài liệu kèm theo

// test hàm nhận dữ liệu không cần Serial
// Mình đã xây dựng thư viện theo cách này ^^


#include "TRANSMIT_CLASS.h"
//Truyền 1 hàm đọc 1 byte dữ liệu 
uint8_t count=0;
// Cấu trúc của 1 gói tin đầy đủ bao gồm các phần :


uint8_t buffer_test[100]{
0,1,2,3,4,5,//LABEL NAME  (6byte)
3,// kích thước NAME PACKET (1byte)
(uint8_t)('a'),(uint8_t)('b'),(uint8_t)('c'),//tên gói tin  NAME PACKET = "abc"
8,0,// kích thước gói PAYLOAD_LENGTH = 8 (2 byte biểu diễn)
4,0,  12,0,  34,67,  67,10, // 4 số kiểu uint16_t  (8 byte biểu diễn)  (4,12,17186,2627)
194,0,0,0//CHECKSUM (kiểu uint32_t   4byte biểu diễn)
};

uint8_t RR() { 
  // đọc buffer thay cho Serial
  // hoặc bạn có thể thêm bất kỳ hàm thu thập dữ liệu nào cũng được
  uint8_t out=buffer_test[count];
  count++;
    if(count >= 99){
       count=0;
    }
  return  out;
}
// Truyền 1 hàm đọc kích thước sẵn có
uint32_t AV() {  return 1;}// luôn sẵn có
// Truyền 1 hàm gửi 1 byte dữ liệu 
void WB(uint8_t c) {c=0 ;}
 //làm gì  đó để clear buffer vào đây, ví dụ :
void CL() {;}
/////////// 













TRANSMIT_CLASS Nhan(&RR, &AV, &CL, &WB);//Nhận 
//
DATA_TRANSMIT_PACKET A("abc");// tạo 1 gói tin với tên "abc"
DATA_TRANSMIT_PACKET *PACKET_ALL[] = {&A};//TỔNG HỢP GÓI 

void setup() {
  /*
  //bạn có thể thay đổi lại tùy thích ở đây
    Nhan.ADD_READ_BYTE( &RR);
   Nhan.ADD_AVAILABLE(  &AV);
    Nhan.ADD_CLEAR( &CL);
    Nhan.ADD_WRITE_BYTE(&WB);
  */

  Serial.begin(9600);
 

}



int aa0=0, aa1=0, aa2=0,aa3=0;

       void print_(){
        Serial.print(aa0);  Serial.print(",");
        Serial.print(aa1);  Serial.print(",");
        Serial.print(aa2);  Serial.print(",");
        Serial.println(aa3);
       }
void loop() {


  uint8_t error_get = 0;
  uint8_t error_listen_label = Nhan.LISTEN_LABEL(1000, PACKET_ALL); 
  // timeout =1 giây , gói tổng hợp PACKET_ALL
   if(error_listen_label==LISTEN_OK){
    
     error_get= Nhan.GET_DATA(
                1000,A,//timeout = 1 giây, Lấy gói A
                (int)(0),//  biến kiểu (int) , 
                4,//số lượng  (3)
                 &aa0, &aa1, &aa2 , &aa3// liệt kê vào
                );

     

       if(error_get == GET_OK){

       print_();//(4,12,17186,2627) là kết quả hợp lệ

       } else {
      Serial.print("State error_get =");Serial.println(error_get);
          
     // ĐỂ CÓ THỂ THẤY Lỗi  bạn có thể thay đổi cấu trúc của mảng buffer_test phía trên

    }

   
   }else {
      Serial.print("State error_listen_label =");Serial.println(error_listen_label);
    // ĐỂ CÓ THỂ THẤY Lỗi  bạn có thể thay đổi cấu trúc của mảng buffer_test phía trên
    
    }

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