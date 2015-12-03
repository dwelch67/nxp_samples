

all : prognxp_erase prognxp_sram prognxp_flash

prognxp_erase :prognxp_erase.c ser.c ser.h
	gcc -O2 -Wall -Wno-unused-result prognxp_erase.c ser.c ser.h -o prognxp_erase

prognxp_sram :prognxp_sram.c ser.c ser.h
	gcc -O2 -Wall -Wno-unused-result prognxp_sram.c ser.c ser.h -o prognxp_sram

prognxp_flash :prognxp_flash.c ser.c ser.h
	gcc -O2 -Wall -Wno-unused-result prognxp_flash.c ser.c ser.h -o prognxp_flash

clean :
	rm -f prognxp_erase
	rm -f prognxp_sram
	rm -f prognxp_flash

