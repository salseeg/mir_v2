.PHONY: clean gloabal_clean

../m_player: music_player.cpp music.o
	$(CXX) $(CXXFLAGS) -o ../m_player music_player.cpp music.o
	
music.o: music.cpp music.h

clean:
	rm -f -- *.o
	rm -f -- ../m_player

gloabal_clean: clean


