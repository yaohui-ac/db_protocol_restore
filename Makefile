sniffer:tcp.cpp
	g++ -o tcp tcp.cpp -I/usr/local/mysql/include -L/usr/local/mysql/lib -I/opt/homebrew/include -L/opt/homebrew/lib  -lmysqlclient -lpcap -lnet -lnids -lglib-2.0 -lgthread-2.0 
mysql:mysql.cpp
	g++ -g mysql.cpp -o mysql -lpcap -lnet -lnids  -lglib-2.0 -lgthread-2.0