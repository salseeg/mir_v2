import mir_iface
import sys

iface = mir_iface.Mir_iface()

if not iface.connect():
	print "Failed to connect"
	sys.exit()
iface.send_command(0, "monitor.so:Python monitor\000")
iface.send_command(1, "")
l = iface.recive_command()
print "id = ", l[0]
print l[1]
iface.send_command(0xffff, "")
iface.disconnect()
