.PHONY: clean global_clean

../m_player: music_player.cpp music.o
	$(CXX) $(CXXFLAGS) -o ../m_player music_player.cpp music.o
	
music.o: music.cpp music.h

clean:
	rm -f -- *.o
	rm -f -- ../m_player

global_clean: clean


