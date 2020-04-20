proc z a {
	set nn [.f$a.input get]
		pcall $a $nn
}

for {set i 0} {$i < 8} {incr i} \
{ set j $i; incr j; set nn 0;\
	frame .f$i; \
		button .f$i.up$i -text "Up $j" -command "pup $i"; \
		button .f$i.do$i -text "DOwn $j" -command "pdown $i";\
		entry .f$i.input; \
		button .f$i.call -text "Набери" -command "z $i" ;\
		pack  .f$i.up$i .f$i.do$i .f$i.call -side left; \
		pack .f$i.input; \
		pack .f$i -side top\
}
button .a -text "qiit" -command {set z 1; exit 0;}
pack .a;
raise .;
while {1} {update; pth_y;}
