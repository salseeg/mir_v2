(* Соединение через указания адреса и номера порта *)
(* let make_connection addres_string port_number = 
	let addr1 = Unix.inet_addr_of_string addres_string in
	let in_addr = Unix.ADDR_INET( addr1, port_number) in
	let my_sock = Unix.socket Unix.PF_INET Unix.SOCK_STREAM 0 in
	Unix.open_connection in_addr

let input,output = make_connection "127.0.0.1" 3003 
*)
open Tk
let com topp = 
		let top1  = Toplevel.create topp in
		Wm.transient_set topp top1;
		Wm.title_set top1 "Connection";
		let left  = Frame.create top1 in
		let right = Frame.create top1 in
		let down = Frame.create top1 in
		pack ~anchor:`Nw [
			(Label.create  ~text:"Address" left);
			(Label.create  ~text:"Port" left)];
		pack [
			(Entry.create ~width: 30 right);
			(Entry.create ~width: 30 right)
		];
		pack ~side:`Left  [left; right];
		pack ~side:`Bottom [down];
		pack  [
			(Button.create ~text:"OK" ~command:(fun () -> ()) down );
			(Button.create ~text:"Cancel" ~command:(fun () -> destroy top1) down)
		]
	


let base = openTk () 
let _ = 

	let bar = Frame.create ~borderwidth:2 ~relief:`Raised  base in 
	pack ~fill:`X [bar];



  (* Menu and Menubutton *)
  let meb = Menubutton.create ~text:"Menu" bar in
  let men = Menu.create meb in
  Menu.add_command ~label:"СоединениеConnection" ~command:(fun () -> com base) men;
  Menu.add_command ~label:"Quit" ~command:(fun () -> closeTk (); exit 0) men;
  pack ~side:`Left [meb];
  Menubutton.configure ~menu:men meb;
Printexc.print mainLoop  ()
