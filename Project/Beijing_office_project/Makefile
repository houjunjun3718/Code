server:
	arm-linux-gnueabihf-gcc -o arm_main main.c IOT_Communication.c DATA_Retriever.c EC20.c Log.c Shared.c Spectrum_Data_Process.c -L. libiot_sdk.a libiot_hal.a libiot_tls.a libsqlite3.so.0.8.6 -pthread -lrt
