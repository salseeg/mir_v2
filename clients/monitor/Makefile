.PHONY : mir_monitor clean global_clean

mir_monitor: ../mir_monitor

../mir_monitor: monitor.cpp ../../libnet.a ../../liblog.a
	$(CXX) $(CXXFLAGS) $(LIBS) -o ../mir_monitor monitor.cpp -lnet -llog

global_clean: clean

clean:
	rm -f -- `ls -l | grep -e '^-[rw-]*x' | awk '{print $9}'`;
