server:
	gcc -o PC_main main.c Data_Delivery_Cloud.c Retriever_collection.c -lcurl

arm:
	arm-linux-gnueabihf-gcc -static -o arm_main main.c Data_Delivery_Cloud.c Retriever_collection.c -L. libcurl.a -pthread -lrt

