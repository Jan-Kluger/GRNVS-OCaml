(* main.ml *)
open Grnvs_ocaml.Grnvs

let interface  = ref "eth0"
let eth_p_all = 0x0003

(* Main function after passing args *)
let main : unit =
  print_endline "I am ready!";
  let timeout_time = 10.0 in

  let t = timeout_of_seconds (Float.to_int timeout_time) in
  let sock = open_raw eth_p_all in

  let result =
    try Some (read_frame sock t)
    with Timeout -> None
  in
  let elapsed_ms = (timeout_time -. (timeout_secs t)) *. 1000.0 in

  match result with
  | None ->
      print_endline ("Did not receive any data within 10 seconds ! :-(");
      close sock;
      exit 1
  | Some frame ->
    let t = (Printf.sprintf "Received %d bytes after %.2f ms" (Bytes.length frame) (elapsed_ms)) in
    print_endline t;

      (* Hexdump *)
      print_endline (hexdump frame);

      (* Entropy *)
    let counts = Hashtbl.create 256 in

    Bytes.iter (fun c ->
        let k = Char.code c in
        let v = try Hashtbl.find counts k with Not_found -> 0 in
        Hashtbl.replace counts k (v + 1)
      ) frame;
      let total = float_of_int (Bytes.length frame) in
      let entropy =
        Hashtbl.fold (fun _ cnt acc ->
          let p = float_of_int cnt /. total in
          acc -. p *. log p /. log 2.0
        ) counts 0.0
      in
      print_endline ("Estimated Entropy: "
                     ^ string_of_float entropy ^ " bits per byte");
      close sock;
      exit 0

let () =
    let speclist = [
    "-i",           Arg.Set_string interface, " network interface (default eth0)";
    "--interface",  Arg.Set_string interface, " same as -i";
  ] in

  Arg.parse speclist (fun _ -> ()) "Usage: hexdump [-i <iface> | --interface <iface>]";

  main
  
  
