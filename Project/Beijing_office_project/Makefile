server:
	gcc -o PC_main main.c Data_Delivery_Cloud.c Retriever_collection.c -lcurl

arm:
	arm-linux-gnueabihf-gcc -o arm_main main.c Data_Delivery_Cloud.c Retriever_collection.c -L. libcurl.a libsqlite3.so.0.8.6 -pthread -lrt

