CXX = g++
CXXFLAGS = -Wall -L$(PWD) -ggdb
LIBS = -lexpat -ldl -rdynamic
L_LIBS = -lconfig -lcore -lnumeration -lrecogn -lsig -lnet -lhard -llog
L_ARCH = libhard.a libnet.a libconfig.a liblog.a libsig.a libnumeration.a libcore.a librecogn.a

MIR_INSTALL_DIR = /usr/share/mir/

export 

.PHONY: clean dumb restore backup global_clean \
	module insert_module insert_emulation run_emulation \
	plugins clients \
	install install_plugins

dumb:
	# 
	# mir_station_2		-  компилирует станцию
	# intstall		-  инсталирует станцию
	# install_plugins	-  инсталирует плагины
	# install_player	-  инсталирует проигрыватель музыки
	# module 		-  компилирует модуль ядра
	# insert_module 	-  вставляет модуль ядра
	# insert_emulation	-  вставляет модуль эмулятора
	# run_emulation		-  запускает программу управления эмулятором
	# 

restore:
	tar -xzf ../backup.tgz .

backup: global_clean
	if [ -f ../backup.tgz ]; then mv -f -- ../backup.tgz ../backup_old.tgz ;fi
	tar -czf ../backup.tgz .
	wc -l *.h */*.h *.c* */*.c* | sort -n | tail -n 1

global_clean: clean
	$(MAKE) -C plugins global_clean
	$(MAKE) -C net global_clean
	$(MAKE) -C log global_clean
	$(MAKE) -C driver global_clean
	$(MAKE) -C tools global_clean

clean:
	rm -f -- *.o *.a 
	cd clients; rm -f -- `ls -l | grep -e '^-[rw-]*x' | awk '{print $9;}'`; cd ..
	cd test; rm -f -- `ls -l | grep -e '^-[rw-]*x' | awk '{print $9}'`; cd ..
	rm -f -- `ls -l | grep -e '^-[rw-]*x' | awk '{print $9}'`;
###########################################################################
#
#	Installation
#
####################################

install: mir_station_2 mir.conf.xml install_plugins install_player
	cp mir_station_2 $(MIR_INSTALL_DIR)
	cp mir.conf.xml $(MIR_INSTALL_DIR)

install_plugins:
	$(MAKE) -C plugins install

install_player:
	$(MAKE) -C tools m_player
	cp tools/m_player $(MIR_INSTALL_DIR)

#########################################################################
#
#	Plugins
#
#############################################

plugins:  
	$(MAKE) -C plugins

#############################################################################
#
#	Clients
#
###########################################

clients:
	$(MAKE) -C clients
	

##############################################################################
#
#	Driver
#
##################################################

module:
	$(MAKE) -C driver

insert_module:
	$(MAKE) -C driver module

insert_emulation:
	$(MAKE) -C driver emulation_module

run_emulation:
	$(MAKE) -C driver run_control_emu



##########################################################################
#
#   	Station_2 
#
##############################################

mir_station_2: mir_station_2.cpp $(L_ARCH)
	$(CXX) $(CXXFLAGS) $(LIBS) -o mir_station_2 mir_station_2.cpp $(L_LIBS)



##############################################################################
#
#	Core
#
#################################################

libcore.a: connection.o line.o inner_line.o outer_line.o station.o  
	ar ru libcore.a connection.o line.o inner_line.o outer_line.o station.o 

connection.o: connection.cpp connection.h

line.o: line.cpp line.h

inner_line.o: inner_line.cpp inner_line.h

outer_line.o: outer_line.cpp outer_line.h

station.o: station.cpp station.h

##########################################################################
#
#	Нумерация. Пучки. Маски.
#
##############################################
test/mask: test/mask.cpp $(L_ARCH)
	$(CXX) $(CXXFLAGS) -o test/mask test/mask.cpp $(LIBS) $(L_LIBS)


libnumeration.a: mask.o masks.o bus.o busses.o mask_actions.o connection_mask_actions.o \
connection_mask.o connection_masks.o
	ar ru libnumeration.a mask.o masks.o bus.o busses.o mask_actions.o connection_mask_actions.o \
		connection_mask.o connection_masks.o

connection_mask_actions.o: connection_mask_actions.h connection_mask_actions.cpp

connection_mask.o: connection_mask.cpp connection_mask.h 

connection_masks.o: connection_masks.cpp connection_masks.h

masks.o: masks.h masks.cpp

mask_actions.o: mask_actions.h mask_actions.cpp

mask.o: mask.h mask.cpp

bus.o: bus.h bus.cpp

busses.o: busses.h busses.cpp

##########################################################################
#
#	Сигнализация
#
##############################################

libsig.a: melodies.o melody.o inner_player.o outer_player.o switcher.o player.o
	ar ru libsig.a melody.o melodies.o inner_player.o outer_player.o switcher.o player.o

switcher.o: switcher.h switcher.cpp

player.o: player.h player.cpp

inner_player.o: inner_player.h inner_player.cpp 

outer_player.o: outer_player.h outer_player.cpp 

melodies.o: melodies.h melodies.cpp 

melody.o: melody.h melody.cpp

##########################################################################
#
#	Распознование набора
#
##############################################

librecogn.a: pulse_recognizer.o
	ar ru librecogn.a pulse_recognizer.o

test/pulse_recognizer: test/pulse_recognizer.cpp pulse_recognizer.o

pulse_recognizer.o: pulse_recognizer.cpp pulse_recognizer.h 

##########################################################################
#
#	Кофигурация / начальная инициализация
#
##############################################

test/config: test/config.cpp $(L_ARCH)
	$(CXX) $(CXXFLAGS) $(LIBS) -o test/config test/config.cpp $(L_LIBS)

libconfig.a: config.o
	ar ru libconfig.a config.o

config.o: config.h config.cpp 

##########################################################################
#
#	Взаимодействие с аппаратурой
#
##############################################

test/hard_write: test/hard_write.cpp $(L_ARCH)
	$(CXX) $(CXXFLAGS) $(LIBS) test/hard_write.cpp $(L_LIBS) -o test/hard_write 

test/hard_read: test/hard_read.cpp $(L_ARCH)
	$(CXX) $(CXXFLAGS) pulse_recognizer.o test/hard_read.cpp $(LIBS) $(L_LIBS) -o test/hard_read 

libhard.a: hard.o
	ar ru libhard.a hard.o

hard.o: interface/driver.h hard.cpp 

##########################################################################
#
#	Поддержка сети
#
##############################################

libnet.a: net/server.cpp net/service.cpp net/socket.cpp
	$(MAKE) -C net

clients/test: clients/test.cpp net/socket.o net/service.o

test/plugin: test/plugin.cpp liblog.a net/server.o plugins/test.so net/service.o net/socket.o clients/test
	$(CXX) $(CXXFLAGS) $(LIBS) -rdynamic -o test/plugin test/plugin.cpp net/server.o net/service.o net/socket.o -llog

plugins/test.so:
	$(MAKE) -C plugins/test

test/socket: test/socket_client test/socket_server

test/socket_client: net/socket.o test/socket_client.cpp

test/socket_server: net/socket.o test/socket_server.cpp

##########################################################################
#
#	Журналирование событий
#
###############################################

liblog.a: 
	$(MAKE) -C log

test/log: log/log.o test/log.cpp

##########################################################################
#
#	Вспомогательные структуры
#	ring_	- двусвязный кольцевой список
#	array_	- динамический массив
#
###############################################

test/array_: test/array_.cpp aux/array_.h

test/ring_: aux/ring_.h test/ring_.cpp 

