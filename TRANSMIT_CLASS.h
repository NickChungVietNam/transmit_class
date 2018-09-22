#include <stdarg.h>

/*
// (add 2/9/2018 )Tương lai: Cần sửa lại hàm checksum sang ít nhất 1 trong 2 cách sau :
// hàm băm 1 byte đơn giản
uint8_t GetCheckSum(uint8_t* cmdBuf, int size)
{
  uint8_t checkSum = 0; 
  for(int i = 0; i < size; ++i)
  {
    checkSum ^= *(cmdBuf++);  
  } 
  return checkSum;
}

 // hàm băm 4 byte
 
int32_t hashCode (String str){
  int32_t  hash = 0;
    if (str.length() == 0) return hash;
    
    for (uint16_t i = 0; i < str.length(); i++) {
        uint8_t char_i= (uint8_t)str.charAt(i);
        hash = ((hash<<5)-hash)+char_i;
        hash = hash & hash; // Convert to 32bit integer
    }
    return hash;
}
// hoặc MD5 ,SHA256,.vvv
*/


// 
//Thư viện NÉN-GIẢI NÉN trong truyền dẫn dữ liệu
 // Người viết : Phùng Thái Sơn -phungthaison96@gmail.com
//http://github.com/NickChungVietNam/
//http://arduino.vn/users/nick-chung/
//https://github.com/NickChungVietNam/transmit_class
//http://arduino.vn/tutorial/5897-thu-vien-truyen-du-lieu-bat-ky-byte-long-float-double-theo-goi-tin-cho-arduino
// xem hướng dẫn trong tài liệu kèm theo



// hoàn thiện : ngày 10/4/2018


// Thư viện mã nguồn mở, 
//Mục đích xây dựng : 


/*



Trước kia mình cũng đã viết 1 thư viện để gửi kiểu số bất kỳ trong truyền dẫn data(UART_ARDUINO.h) 

http://arduino.vn/tutorial/1471-truyen-cac-so-kieu-long-int-float-trong-giao-tiep-serial-uart
 Khi xây dựng nó mình cũng đã nhận ra một vài điểm yếu của UART_ARDUINO.h
Cũng thật bất ngờ khi thấy bài viết giới thiệu thư viện có tới trên 11k view. 
 Nhận thấy nhu cầu không hề nhỏ từ phía cộng đồng, mặc dù bận rộn những mình cũng cố gắng 
dành thời gian phát triển lại  (từ đầu) 1 cách cẩn thận hơn với hi vọng gỡ gạc lại
 phần nào những thiếu sót của thư viện cũ.
TRANSMIT_CLASS.h có ưu điểm dễ sử dụng, trực quan, dễ nâng cấp và rẽ nhánh có thể sử dụng linh
 hoạt ở bất kỳ dạng truyền nào đòi hỏi tính đóng gói và bảo toàn dữ liệu (UART, SPI, I2C, IR ,
  sóng RF , ...).
Mình cũng rất mong thư viện sẽ được mọi người đóng góp xây dựng thêm . Mọi phản hồi các bạn có 
thể gửi qua phần bình luận hoặc qua email của mình 

//phungthaison96@gmail.com
//phungthaion2012@gmail.com
http://github.com/NickChungVietNam/
http://arduino.vn/users/nick-chung/

*/


// Quy ước Giao thức : Tự định nghĩa ! , tạm gọi là "MZ1" (minimum zip 1)
/*
// 1 gói tin đầy đủ bao gồm các phần :
// LABEL_NAME  +NamePK_Leng + NamePK + Payload_leng +PlayLoad +CheckSum

 -LABEL_NAME : tên nhãn bắt đầu gói tin (6)byte  (mặc định ={0,1,2,3,4,5}) , 
      có thể chỉnh sửa  LABEL_NAME bằng thành viên  RESET_LABEL_NAME(x,x,x,x,x,x)
 -NamePK_Leng (NamePacket_length) : độ dài tên gói tin (1 byte)
 -NamePK (Namepacket) : tên gói tin (nhiều nhất là 255 kí tự)
 -Payload_leng : kích thước của Payload  (2 byte  , 8bit thấp trước + 8 bit cao sau)
 -PayLoad: phần dữ liệu chính : là dữ liệu phân giải của kiểu dữ liệu tương ứng, xếp liên tục nhau , (bit thấp nhất viết đầu tiên)
 PayLoad chỉ được chứa 1 loại kiểu dữ liệu , PayLoad có thể lên tới 65535 byte dữ liệu
 -CheckSum : kiểm tra lần cuối (4 byte)


DATA_TRANSMIT_PACKET: // lớp chứa  NamePK và cờ lựa chọn
    */    
class DATA_TRANSMIT_PACKET{
  
  private:
     // lớp này để lưu tên gói tin NamePK và cờ lựa chọn
String NAME;
boolean the_same_name;
  public:
  static uint8_t MAX_LENGTH_STRING;/// tìm chiều dài lớn nhất có thể có của chuỗi tên
  //( dùng để kiểm tra ở hàm ...)
  static uint16_t count_value;// đếm số biến

  DATA_TRANSMIT_PACKET(String name_){
    NAME=name_;
    count_value++;
    if(NAME.length() > MAX_LENGTH_STRING){
      //cập nhật kích thước lớn nhất của tên gói tin có thể có (dùng để kiểm tra sau này) 
      MAX_LENGTH_STRING=NAME.length();
    }
    the_same_name=false;
  }
  boolean is_the_same_name(){
    return  the_same_name;
  }
  uint8_t get_length(){
    return (uint8_t )NAME.length();
  }
  String get_string(){
    return NAME;
  }
  void Reset(){
    the_same_name=false;
  }
 
  void set_true(){
    the_same_name=true;
  }
};
uint8_t DATA_TRANSMIT_PACKET::MAX_LENGTH_STRING=0;
uint16_t DATA_TRANSMIT_PACKET::count_value=0;





enum ERROR_LISTEN_LABEL{
  LISTEN_OK,// không có bất cứ lỗi nào khi tìm tên 
  ERROR_NO_FOUND_LABEL_NAME,//Không tìm được tên nhãn 
  ERROR_NO_FOUND_LENGTH_NAME,////Không tìm được  độ dài tên 
  ERROR_NOT_FIT_LENGTH_NAME,// lỗi độ dài tên không khớp với bất kì biến nào
  ERROR_NO_FOUND_FULL_NAME,///Không tìm được toàn bộ tên
  ERROR_NAME_NOT_TRUE,///Đã tìm được tên nhưng Tên không giống với bất kỳ tên nào đã khai báo
};

enum ERROR_GET_DATA{
  GET_OK,//không lỗi
  ERROR_NO_SELECT,// không được lựa chọn s
  ERROR_NO_FOUND_PAYLOAD_LEGNTH,// KO tìm được độ dài của gói dữ liệu
  ERROR_PAYLOAD_LEGNTH_BIGGER,//PAYLOAD lớn hơn kích thước phù hợp
  ERROR_PAYLOAD_LEGNTH_SMALLER,//PAYLOAD nhỏ hơn kích thước phù hợp
  ERROR_READ_NOT_FULL,//chưa nạp đủ dữ liệu
  ERROR_NO_FOUND_CHECK_SUM,// chưa tìm được checksum
  ERROR_CHECK_SUM,// lỗi checksum
  
};



class TRANSMIT_CLASS {
private:
public:

const uint8_t LABEL_NAME_length=6;
uint8_t  LABEL_NAME[7]={0,1,2,3,4,5};// tên nhãn mặc định

uint8_t BUFFER_CHECK[60];
uint8_t (*TRANSMIT_READ_BYTE)();// con trỏ hàm không có đối số
// dùng để đọc từng byte 
uint32_t (*TRANSMIT_AVAILABLE)();
// Dùng để kiểm tra kích thước sẵn có bộ nhớ đệm
void (*TRANSMIT_CLEAR)();
//xóa bộ nhớ đệm
void (*TRANSMIT_WRITE)(uint8_t);
//gửi 1 byte dữ liệu
void ADD_READ_BYTE( uint8_t (*fc)() ){
  TRANSMIT_READ_BYTE =fc;
}
  
void ADD_AVAILABLE( uint32_t (*fc)() ){
  TRANSMIT_AVAILABLE=fc;
}
  
void ADD_CLEAR( void (*fc)() ){
  TRANSMIT_CLEAR =fc;
}
void ADD_WRITE_BYTE( void(*fc)(uint8_t) ){
  TRANSMIT_WRITE =fc;
}

TRANSMIT_CLASS (){
  //hàm dựng
}

//constructor cho nhanh
   TRANSMIT_CLASS  ( uint8_t (*rb)(),   uint32_t (*av)() , void (*cl)() ,void (*wb)(uint8_t)){
    TRANSMIT_READ_BYTE= rb;
     TRANSMIT_AVAILABLE=av;
     TRANSMIT_CLEAR=cl;
     TRANSMIT_WRITE=wb;
    RESET_LABEL_NAME(0,1,2,3,4,5);
   }




void RESET_LABEL_NAME( 
  uint8_t n0, uint8_t n1,uint8_t n2,
  uint8_t n3,uint8_t n4,uint8_t n5
){
  // cài lại tên nhãn (LABEL_NAME)bắt đầu gói tin 
     LABEL_NAME[0]=n0;
     LABEL_NAME[1]=n1;
     LABEL_NAME[2]=n2;
     LABEL_NAME[3]=n3;
     LABEL_NAME[4]=n4;
     LABEL_NAME[5]=n5;
}

void RESET_BUFFER_CHECK(){
  //reset
  for(uint8_t i=0; i<60; i++){
    BUFFER_CHECK[i]=0;
  }
}

  boolean FOUND_LABEL_NAME_(uint8_t input){
    
  
  boolean the_same=true;

uint8_t i=0;
    for(i=0; i< LABEL_NAME_length-1; i++){
    
    BUFFER_CHECK[i]= BUFFER_CHECK[i+1];//ghi dịch
    
    if(BUFFER_CHECK[i] !=LABEL_NAME[i]){  the_same=false;}
    
  }
 BUFFER_CHECK[i]=input;//i đã cộng thêm 1 khi kết thúc for
  if(BUFFER_CHECK[i] !=LABEL_NAME[i]){    the_same=false;   }//kiểm tra phần tử cuối
 
    return the_same;
}

boolean check_array_bang_string(String a, uint8_t array_[],uint16_t length_array){
  
                  //kiểm tra độ dài tên có bằng nhau hay không (tiết kiệm thời gian)
                
  if(length_array!= a.length()){
    return false;
  }

  // kiểm tra mảng tên và chuỗi tên giống nhau hay không

  for(uint16_t i=0; i<length_array ; i++){
    if( (char)(array_[i]) != a.charAt(i)){
      return false;
    }
  }
  return true;
}


 
      uint8_t LISTEN_LABEL(uint32_t timeout,  DATA_TRANSMIT_PACKET **array_ );
template <class Type>  uint8_t GET_DATA(uint32_t time_out,DATA_TRANSMIT_PACKET x ,Type c, uint16_t Count, ... );

template <class Type>  uint8_t GET_DATA_ARRAY(uint32_t time_out,DATA_TRANSMIT_PACKET x ,Type c, uint16_t Count, Type *array_ );
  uint8_t GET_BINARY_ARRAY(uint32_t time_out,DATA_TRANSMIT_PACKET x , uint16_t SIZE_ARRAY, uint8_t *array_ , uint16_t *number_get);


  template <class Type>   uint8_t SEND_DATA( DATA_TRANSMIT_PACKET x , Type c, uint16_t Count , ...);
    
  template <class Type>   uint8_t SEND_DATA_ARRAY( DATA_TRANSMIT_PACKET x , Type c, uint16_t Count, Type *array_);
     
  
template <class T> int TRANSMIT_readAnything(uint8_t *buffer_ , T& value);
template <class T> uint32_t TRANSMIT_writeAnything( const T& value);


};//CLASS




template <class T> int TRANSMIT_CLASS::TRANSMIT_readAnything(uint8_t *buffer_ , T& value)
{
   byte* p = (byte*)(void*)&value;
   uint16_t i=0;
   for (i = 0; i < sizeof(value); i++)
       *p++ = *(buffer_ +i);// gộp từng byte dữ liệu
   return i;
}




uint8_t TRANSMIT_CLASS::LISTEN_LABEL(uint32_t timeout,  DATA_TRANSMIT_PACKET **array_ ){
  
  boolean FOUND_LABEL_NAME=false;// tìm chuỗi xuất pháts
  boolean FOUND_LENGTH_NAME=false;//tìm được độ dài tên
  boolean FOUND_FULL_NAME=false;// tìm được toàn bộ tên
  
   RESET_BUFFER_CHECK();
  for(uint16_t i=0; i< DATA_TRANSMIT_PACKET::count_value;i++){
    
         DATA_TRANSMIT_PACKET *vl_x=array_[i];
        (*vl_x).Reset();
  }
    
   uint8_t count_name_i=0;
uint8_t length_name=0;

uint32_t time_start_wait=millis();
  while(millis()- time_start_wait < timeout){

    
     if( TRANSMIT_AVAILABLE()){// if(//Serial.available()){

      
         
       uint8_t read_= TRANSMIT_READ_BYTE();//(uint8_t)//Serial.read();
        if(!FOUND_LABEL_NAME){
            if(FOUND_LABEL_NAME_(read_)){
              FOUND_LABEL_NAME=true;
              
       time_start_wait=millis();//có tiến triển ,reset watchdog
         //Serial.println(" FOUND_LABEL_NAME" ); 
             }
          continue;//bỏ qua phần dưới
        }


   
       if(!FOUND_LENGTH_NAME){
        
         length_name=read_;//tìm được độ dài tên
         //so sánh độ dài 
         if(length_name> DATA_TRANSMIT_PACKET::MAX_LENGTH_STRING ){
          
         //Serial.println("  ERROR_NOT_FIT_LENGTH_NAME" ); 
          return ERROR_NOT_FIT_LENGTH_NAME;// lỗi độ dài tên không khớp với bất kì biến nào
         }else{
        FOUND_LENGTH_NAME=true;
        
       time_start_wait=millis();//có tiến triển ,reset watchdog
         //Serial.println(" FOUND_LENGTH_NAME" ); 
        //Serial.print("length_name = ");  //Serial.println(length_name); 
           RESET_BUFFER_CHECK();//chuẩn bị để lưu tên
         }
        continue;
       }
   
         if(!FOUND_FULL_NAME){

           BUFFER_CHECK[count_name_i]=read_;
           count_name_i++;

           
           


            
           if(count_name_i >= length_name){
            FOUND_FULL_NAME =true;

         //Serial.println(" FOUND_FULL_NAME" ); 
             for(uint16_t i=0; i< DATA_TRANSMIT_PACKET::count_value; i++){

                //duyệt từng tên 1 
                        DATA_TRANSMIT_PACKET *vl_x=array_[i];

                          
                    String name_=(*vl_x).get_string();
                     if( check_array_bang_string(name_,BUFFER_CHECK,length_name)){
                      // kiểm tra tên có khớp không
                      (*vl_x).set_true();
                
                      //Serial.println(name_ ); 
                      return LISTEN_OK;//không có bất cứ lỗi tìm tên nào
                        }
              
             }
          return ERROR_NAME_NOT_TRUE;///Đã tìm được tên nhưng Tên không giống với bất kỳ tên nào đã khai báo

           
           }else{
          continue;
           }
         }

        
          
        
      }
    
    
  }
       if(!FOUND_LABEL_NAME){
        return ERROR_NO_FOUND_LABEL_NAME;//Không tìm thấy tên nhãn 
       }else if(!FOUND_LENGTH_NAME){
        return ERROR_NO_FOUND_LENGTH_NAME;////Không tìm được độ dài tên
       }else if(!FOUND_FULL_NAME){
        return ERROR_NO_FOUND_FULL_NAME;///Không tìm toàn bộ tên
        
       }
  return ERROR_NO_FOUND_LABEL_NAME;
}



template <class T> uint32_t TRANSMIT_CLASS::TRANSMIT_writeAnything( const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
 
    uint32_t SUM=0;
   for (uint8_t i = 0; i < sizeof(value); i++){
      uint8_t out=*p++;//tách ra để truyền
      SUM+=out;//// lấy tổng để gộp checksum
       TRANSMIT_WRITE( out);//truyền
   }
   return SUM;
}
template <class Type> void FindMax(Type c,  ...)
{
  va_list ap;
  int _count=(int)(c);
  va_start(ap, _count);
 
  Type greater = va_arg(ap, Type);
 //Serial.println(greater);
  for(int i = 1; i < _count; i++)
  {
    Type val = va_arg(ap, Type);
     //Serial.println(val );
  }
 
  va_end(ap);
  
}

  
 template <class Type>   uint8_t TRANSMIT_CLASS::SEND_DATA_ARRAY( DATA_TRANSMIT_PACKET x , Type c, uint16_t Count, Type *array_){
 
  uint16_t _count=Count;

      // b1 : gửi start name
     for(uint8_t i=0; i < LABEL_NAME_length; i++){
      TRANSMIT_WRITE(LABEL_NAME[i]);
     }
     // b2: gửi length name
     String name_=x.get_string();
      TRANSMIT_WRITE((uint8_t)(name_.length()));
      // b3 : gửi name_
       for(uint8_t i=0; i < name_.length(); i++){
        TRANSMIT_WRITE((uint8_t)name_.charAt(i));
       }
       // b4: gửi payload length
       uint16_t SIZE_OF=sizeof(c);
       uint16_t  payload_length=_count*SIZE_OF;
       TRANSMIT_writeAnything( payload_length);//gửi nén
       //b5: gửi payload data
       uint32_t SUM=0;
       for(uint16_t i=0; i< _count; i++){
         
        Type value_ =  *(array_ +i) ;//lấy dữ liệu từ mảng
        
         SUM+= TRANSMIT_writeAnything( value_);//gửi nén
       }
       //b6 : gửi checksum
       TRANSMIT_writeAnything( SUM);//gửi nén
      return 0;
   }


 template <class Type>   uint8_t TRANSMIT_CLASS::SEND_DATA( DATA_TRANSMIT_PACKET x , Type c,uint16_t Count,...){
 
  va_list ap;
  //int _count=Count;
  va_start(ap, Count);
      // b1 : gửi start name
     for(uint8_t i=0; i < LABEL_NAME_length; i++){
      TRANSMIT_WRITE(LABEL_NAME[i]);
     }
     // b2: gửi length name
     String name_=x.get_string();
      TRANSMIT_WRITE((uint8_t)(name_.length()));
      // b3 : gửi name_
       for(uint8_t i=0; i < name_.length(); i++){
        TRANSMIT_WRITE((uint8_t)name_.charAt(i));
       }
       // b4: gửi payload length
       uint16_t SIZE_OF=sizeof(c);
       uint16_t  payload_length=Count*SIZE_OF;
       TRANSMIT_writeAnything( payload_length);//gửi nén
       //b5: gửi payload data
       uint32_t SUM=0;
       for(uint16_t i=0; i< Count; i++){
        
        Type value_ = va_arg(ap, Type);
        
         SUM+= TRANSMIT_writeAnything( value_);//gửi nén
       }
       //b6 : gửi checksum
       TRANSMIT_writeAnything( SUM);//gửi nén
  va_end(ap);
  return 0;
   }
  
template <class Type>  uint8_t TRANSMIT_CLASS::GET_DATA(uint32_t time_out,DATA_TRANSMIT_PACKET x ,Type c,uint16_t Count,... ){
  
   // uint16_t _count_vl=Count;//số lượng biến lưu
  

  if(x.is_the_same_name()){//đây là tên dược được đánh dấu lựa chọn

    va_list ap;
  va_start(ap,Count);
 
 boolean END=false;

     uint32_t time_start_wait=millis();
    boolean FOUND_PAYLOAD_LEGNTH=false;// tìm được độ dài của gói dữ liệu
    
    boolean READ_FULL=false;// đã nạp đủ dữ liệu
    boolean FOUND_CHECK_SUM =false;// tìm được check sum
    uint32_t SUM =0;// check sum 
    
    
      RESET_BUFFER_CHECK();
    uint16_t PAYLOAD_LEGNTH=0;// độ dài gói dữ liệu
    uint32_t  count_read_=0;//đếm số byte đã đọc
    
    uint16_t _count_save=0;//số lượng biến đã được lưu
    uint8_t SIZE_OF=sizeof(c);// kích cỡ dữ liệu
    uint8_t buffer_[16];// 
    

 
    while(millis() -  time_start_wait <  time_out){
      
    
       
   if(TRANSMIT_AVAILABLE()){//  if(//Serial.available()){
    
      uint8_t read_=TRANSMIT_READ_BYTE();;//(uint8_t)//Serial.read();
    
      if(!FOUND_PAYLOAD_LEGNTH){
         
        if(count_read_==0){
            PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|read_;// đọc bit thấp trước
          
           count_read_++;
        }else if(count_read_==1){
          PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|(read_<< 8);// đọc bit cao sau
             uint16_t Size_save=(uint16_t)(SIZE_OF*Count);//tổng số byte càn lưu
           
           if(PAYLOAD_LEGNTH < Size_save  ){
            
           //Serial.println("ERROR_PAYLOAD_LEGNTH_SMALLER");
            if(!END){va_end(ap);END=true;}
            return ERROR_PAYLOAD_LEGNTH_SMALLER;
           }else if(PAYLOAD_LEGNTH > Size_save  ){
           //Serial.println("ERROR_PAYLOAD_LEGNTH_BIGGER");
            if(!END){va_end(ap);END=true;}
            return  ERROR_PAYLOAD_LEGNTH_BIGGER;
           }
           

           
       time_start_wait=millis();//có tiến triển ,reset watchdog
           count_read_=0;//đặt lại =0 
            
           FOUND_PAYLOAD_LEGNTH=true;
           //Serial.print("FOUND_PAYLOAD_LEGNTH=");
           //Serial.println(PAYLOAD_LEGNTH);
        }
       
        continue;
      }
       if(!READ_FULL){
         SUM += (uint32_t)read_; // lấy tổng để sau này kiểm tra 
      if(count_read_ < PAYLOAD_LEGNTH){
          
        uint8_t i=count_read_%SIZE_OF; 
        buffer_[i]=read_;
           //Serial.print(buffer_[i]);
           //Serial.print(",");
           count_read_++;
        if(_count_save < Count){
          
           
        if(i>=SIZE_OF-1){
          // số byte cần đọc mỗi lần bằng kích thước của kiểu dữ liệu lưu trữ
         Type m;//đệm
          TRANSMIT_readAnything(buffer_,m);// đổi sang số
          
         Type* in = va_arg(ap, Type*);// đọc từng biến
         (*in)=m;// sửa lại
           //Serial.print(":");
           //Serial.print((*in));
           //Serial.print("|");
           _count_save++;
         
       time_start_wait=millis();//có tiến triển ,reset watchdog
           if(_count_save >=Count){
             READ_FULL=true;
            if(!END){va_end(ap);END=true;}
           
           count_read_=0;//đặt lại =0 
           
           }
        }else{
          
        }
            
        
      }
      }
        continue;
       }
    if(!FOUND_CHECK_SUM){
      if(count_read_>=4){
        uint32_t CHECK_SUM_FOUND=0;
        FOUND_CHECK_SUM=true;
          TRANSMIT_readAnything(buffer_,CHECK_SUM_FOUND);// đổi sang số
           //Serial.println();
           //Serial.print("Checksum="); //Serial.println(CHECK_SUM_FOUND);
           //Serial.print("Sum="); //Serial.println(SUM);
          if(CHECK_SUM_FOUND == SUM){
              if(!END){va_end(ap);END=true;}
            return  GET_OK;;// lấy hoàn tất
          }else{
             if(!END){va_end(ap);END=true;}
            return  ERROR_CHECK_SUM;// lỗi checksum
          }
      }else{
          buffer_[count_read_]=read_;
           //Serial.print(buffer_[count_read_]);
           //Serial.print(",");
           count_read_++;
      }
      
      continue;
    }
       
     

     }// available
    }//while


 
   
    
    
  }else { 
    return ERROR_NO_SELECT;
  } 
   return ERROR_NO_SELECT;
}

///////////////////////////////


///////////////////////////////
template <class Type>  uint8_t TRANSMIT_CLASS::GET_DATA_ARRAY(uint32_t time_out,DATA_TRANSMIT_PACKET x ,Type c, uint16_t Count, Type *array_ ){
  if(x.is_the_same_name()){//đây là tên dược được đánh dấu lựa chọn
     uint32_t time_start_wait=millis();
    boolean FOUND_PAYLOAD_LEGNTH=false;// tìm được độ dài của gói dữ liệu
    
    boolean READ_FULL=false;// đã nạp đủ dữ liệu
    boolean FOUND_CHECK_SUM =false;// tìm được check sum
    uint32_t SUM =0;// check sum 
    
    
      RESET_BUFFER_CHECK();
    uint16_t PAYLOAD_LEGNTH=0;// độ dài gói dữ liệu
    uint32_t  count_read_=0;//đếm số byte đã đọc
    //uint16_t _count_vl=Count ;//số lượng biến lưu
    
    uint16_t _count_save=0;//số lượng biến đã được lưu
    uint8_t SIZE_OF=sizeof(c);// kích cỡ dữ liệu
    uint8_t buffer_[16];// 
    
 

 
    while(millis() -  time_start_wait <  time_out){

   if(TRANSMIT_AVAILABLE()){//  if(//Serial.available()){
     
      uint8_t read_=TRANSMIT_READ_BYTE();;//(uint8_t)//Serial.read();
    
      if(!FOUND_PAYLOAD_LEGNTH){
         
        if(count_read_==0){
            PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|read_;// đọc bit thấp trước
          
           count_read_++;
        }else if(count_read_==1){
          PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|(read_<< 8);// đọc bit cao sau
             uint16_t Size_save=(uint16_t)(SIZE_OF*Count);//tổng số byte càn lưu
           
           if(PAYLOAD_LEGNTH < Size_save  ){
            
           //Serial.println("ERROR_PAYLOAD_LEGNTH_SMALLER");
            return ERROR_PAYLOAD_LEGNTH_SMALLER;
           }else if(PAYLOAD_LEGNTH > Size_save  ){
           //Serial.println("ERROR_PAYLOAD_LEGNTH_BIGGER");
            return  ERROR_PAYLOAD_LEGNTH_BIGGER;
           }
           

           
       time_start_wait=millis();//có tiến triển ,reset watchdog
           count_read_=0;//đặt lại =0 
            
           FOUND_PAYLOAD_LEGNTH=true;
           //Serial.print("FOUND_PAYLOAD_LEGNTH=");
           //Serial.println(PAYLOAD_LEGNTH);
        }
       
        continue;
      }
       if(!READ_FULL){
         SUM += (uint32_t)read_; // lấy tổng để sau này kiểm tra 
      if(count_read_ < PAYLOAD_LEGNTH){
          
        uint8_t i=count_read_%SIZE_OF; 
        buffer_[i]=read_;
           //Serial.print(buffer_[i]);
           //Serial.print(",");
           count_read_++;
        if(_count_save <Count){
          
           
        if(i>=SIZE_OF-1){


          // số byte cần đọc mỗi lần bằng kích thước của kiểu dữ liệu lưu trữ
         Type m;//đệm
          TRANSMIT_readAnything(buffer_,m);// đổi sang số
          
         *(array_ +  _count_save)=m;// sửa lại
           //Serial.print(":");
           //Serial.print(m);
           //Serial.print("|");
           _count_save++;
         
       time_start_wait=millis();//có tiến triển ,reset watchdog
           if(_count_save >=Count){
             READ_FULL=true;
           
           count_read_=0;//đặt lại =0 
           
           }
        }else{
          
        }
            
        
      }
      }
        continue;
       }
    if(!FOUND_CHECK_SUM){
      if(count_read_>=4){
        uint32_t CHECK_SUM_FOUND=0;
        FOUND_CHECK_SUM=true;
          TRANSMIT_readAnything(buffer_,CHECK_SUM_FOUND);// đổi sang số
           //Serial.println();
           //Serial.print("Checksum="); //Serial.println(CHECK_SUM_FOUND);
           //Serial.print("Sum="); //Serial.println(SUM);
          if(CHECK_SUM_FOUND == SUM){
            return  GET_OK;;// lấy hoàn tất
          }else{
            return  ERROR_CHECK_SUM;// lỗi checksum
          }
      }else{
          buffer_[count_read_]=read_;
           //Serial.print(buffer_[count_read_]);
           //Serial.print(",");
           count_read_++;
      }
      
      continue;
    }
       
     

     }// available
    }//while


 
   
    
    
  }else { 
    return ERROR_NO_SELECT;
  } 
   return ERROR_NO_SELECT;
}




///////////////////////////////
  uint8_t TRANSMIT_CLASS::GET_BINARY_ARRAY(uint32_t time_out,DATA_TRANSMIT_PACKET x , uint16_t SIZE_ARRAY, uint8_t *array_ , uint16_t *number_get){
  if(x.is_the_same_name()){//đây là tên dược được đánh dấu lựa chọn

    //SIZE_ARRAY là kích thước của mảng lưu trữ array
    //lấy dữ liệu nhận được dưới dạng chuỗi nhị phân 
    //dữ liệu nhạn được sẽ lưu vào mảng array
    //và trả về kích thước của mảng nhận được number_get

     uint32_t time_start_wait=millis();
    boolean FOUND_PAYLOAD_LEGNTH=false;// tìm được độ dài của gói dữ liệu
    
    boolean READ_FULL=false;// đã nạp đủ dữ liệu
    boolean FOUND_CHECK_SUM =false;// tìm được check sum
    uint32_t SUM =0;// check sum 
    
    
      RESET_BUFFER_CHECK();
    uint16_t PAYLOAD_LEGNTH=0;// độ dài gói dữ liệu
    uint32_t  count_read_=0;//đếm số byte đã đọc
    
    uint8_t buffer_[16];// 
    
 

 
    while(millis() -  time_start_wait <  time_out){

   if(TRANSMIT_AVAILABLE()){//  if(//Serial.available()){
     
      uint8_t read_=TRANSMIT_READ_BYTE();;//(uint8_t)//Serial.read();
    
      if(!FOUND_PAYLOAD_LEGNTH){
         
        if(count_read_==0){
            PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|read_;// đọc bit thấp trước
          
           count_read_++;
        }else if(count_read_==1){
          PAYLOAD_LEGNTH=PAYLOAD_LEGNTH|(read_<< 8);// đọc bit cao sau

           
           //Serial.print("FOUND_PAYLOAD_LEGNTH=");
           //Serial.println(PAYLOAD_LEGNTH);

           if(PAYLOAD_LEGNTH > SIZE_ARRAY  ){
            
           //Serial.println("ERROR_PAYLOAD_LEGNTH_BIGGER");
           //kích cỡ mảng lưu trữ không đủ để lưu toàn bộ dữ liệu sắp nhận
            return  ERROR_PAYLOAD_LEGNTH_BIGGER;

           }
           

       time_start_wait=millis();//có tiến triển ,reset watchdog
           (*number_get)=PAYLOAD_LEGNTH;//tìm được kích thước 
           count_read_=0;//đặt lại =0 
            
           FOUND_PAYLOAD_LEGNTH=true;
        }
       
        continue;
      }
       if(!READ_FULL){
         SUM += (uint32_t)read_; // lấy tổng để sau này kiểm tra 
         
       time_start_wait=millis();//có tiến triển ,reset watchdog
      if(count_read_ < PAYLOAD_LEGNTH){
          
           *(array_ + count_read_) =read_;

           //Serial.print(read_);
           //Serial.print(",");
           count_read_++;
            if(count_read_ >= PAYLOAD_LEGNTH){

              READ_FULL=true;
              count_read_=0;//reset để tìm nốt checksum
            }

              
      }
        continue;
       }
    if(!FOUND_CHECK_SUM){
      if(count_read_>=4){
        uint32_t CHECK_SUM_FOUND=0;
        FOUND_CHECK_SUM=true;
          TRANSMIT_readAnything(buffer_,CHECK_SUM_FOUND);// đổi sang số
          
           //Serial.println();
           //Serial.print("Checksum="); //Serial.println(CHECK_SUM_FOUND);
           //Serial.print("Sum="); //Serial.println(SUM);
           
          if(CHECK_SUM_FOUND == SUM){
            return  GET_OK;;// lấy hoàn tất
          }else{
            return  ERROR_CHECK_SUM;// lỗi checksum
          }
      }else{
          buffer_[count_read_]=read_;
           //Serial.print(buffer_[count_read_]);
           //Serial.print(",");
           count_read_++;
      }
      
      continue;
    }
       
     

     }// available
    }//while


 
   
    
    
  }else { 
    return ERROR_NO_SELECT;
  } 
   return ERROR_NO_SELECT;
}


