server:
	gcc -o WuHan_main WuHan_main.c Retriever.c PLC_serial.c Client_UDP.c

arm:
	arm-linux-gnueabihf-gcc -o arm-WuHan_main WuHan_main.c Client_UDP.c Retriever.c PLC_serial.c

clean:
	rm WuHan_main
