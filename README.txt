//http://arduino.vn/tutorial/5897-thu-vien-truyen-du-lieu-bat-ky-byte-long-float-double-theo-goi-tin-cho-arduino


Nhiệm vụ của thư viện này gồm 2 phần cơ bản là Nén dữ liệu gửi và Giải nén dữ liệu nhận.
Mình quy ước định dạng gói tin cho thư viện của mình như sau: 

LABEL_NAME  +NamePK_Leng + NamePK + Payload_leng +PlayLoad +CheckSum

 
•	 -LABEL_NAME : tên nhãn bắt đầu gói tin (6)byte  (mặc định ={0,1,2,3,4,5}) , 
•	      có thể chỉnh sửa  LABEL_NAME bằng thành viên  RESET_LABEL_NAME(x,x,x,x,x,x)
•	 -NamePK_Leng (NamePacket_length) : độ dài tên gói tin (1 byte)
•	 -NamePK (Namepacket) : tên gói tin (nhiều nhất là 255 kí tự)
•	 -Payload_leng : kích thước của Payload  (2 byte  , 8bit thấp trước + 8 bit cao sau)
•	 -PayLoad: phần dữ liệu chính : là dữ liệu phân giải của kiểu dữ liệu tương ứng, xếp liên tục nhau , (bit thấp nhất viết đầu tiên)
•	 PayLoad chỉ được chứa 1 loại kiểu dữ liệu , PayLoad có thể lên tới 65535 byte dữ liệu
•	 -CheckSum : kiểm tra lần cuối (4 byte)
Cách hoạt động:
 Khi gửi :
Đơn giản là gộp dữ liệu và xuất ra đường truyền theo định dạng.
Riêng phần Payload sẽ được thêm vào theo ý định của người dùng.
Khi nhận: 
 Bước 1 LISTEN_LABEL:  Phát hiện gói tin bằng cách kiểm tra 6 byte  LABEL_NAME, sau đó lấy chiều dài tên gói tin NamePK_Leng  và lọc lấy tên của gói NamePK. Cuối cùng sẽ kiểm tra lỗi và quyết định sẽ đổ dữ liệu PayLoad vào gói nào bằng câu lệnh rẽ nhánh.
Bước 2 GET_DATA :  Lấy chiều dài Payload_leng và nạp vào biến lưu trữ (mảng hoặc con trỏ địa chỉ) cho đến khi đủ Payload_leng thì dừng lại, Cuối cùng kiểm tra CheckSum để kiểm tra tính toàn vẹn của gói tin.
Việc nhận gói tin thành công chỉ xác nhận khi vượt qua tất cả các bước này.
Một lưu ý: Thư viện không dùng bộ nhớ đệm để lưu trữ dữ liệu PayLoad nhận được,  dữ liệu nhận đến đâu sẽ ghi đè vào mảng hoặc biến đến đó ngay cả khi gói tin đó bị lỗi. 
Tuy nhiên cả 2 hàm đều có  khả năng trả về lỗi mà nó phát hiện, ta sẽ dựa vào đó đế quyết định xem có nên dùng dữ liệu đó hay không.

