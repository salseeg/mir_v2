from threading import *

def test():
	i = 0
	while i < 1000:
		print "test :",i
		i = i + 1

th = Thread(target = test)
th.start()
i = 0
while i < 1000:
	print "main :",i
	i = i + 1
th.join()
