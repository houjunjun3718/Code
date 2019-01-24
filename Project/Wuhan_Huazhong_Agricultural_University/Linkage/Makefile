server:
	arm-linux-gnueabihf-gcc -o arm_main main.c Data_Delivery_Cloud.c PLC_communication.c Reteiever_Processing.c Retriever_collection.c ReuseFunction.c Watchdog.c -L. libcurl.a -pthread -lrt

2475:
	arm-linux-gnueabihf-gcc -o 2475-arm 2475-main.c watchdog_2475.c Data_Delivery_Cloud.c ReuseFunction.c -L. libcurl.a -pthread -lrt
