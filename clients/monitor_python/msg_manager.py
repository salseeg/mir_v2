from threading import *
import time

class P_message_manager:
	def __init__(self,sleep_time = 0.01):
		self.sleep_time = sleep_time
		self.queue = []
		self.exit = 0
		self.thread = Thread(target = self.process_queue)
		self.thread.start()
	
	def send(self, str):
		self.queue.append(str)

	def process_queue(self):
		while (not self.exit):
			if (self.queue):
				self.dispatch()
			if (not self.exit):
				time.sleep(self.sleep_time)

	def done(self):
		self.exit = 1;
		self.thread.join();
	
	def dispatch(self):
		s = self.queue.pop()
		if (s =="test"):
			print "Test_message executed"
		else:
			throw("Unhandled message")


#manager = P_message_manager()
#time.sleep(1)
#print "pre sending"
#manager.send("test")
#print "post sending"
#time.sleep(5)
#manager.done()
