open Unix
open Printf
open Ctypes
open Foreign

(* Declare Timeout Exception *)
exception Timeout

let af_packet = 17    (* PF_PACKET *)
let sock_raw  = 3     (* SOCK_RAW  *)

let socket = foreign "socket"  (int @-> int @-> int @-> returning int)
let htons   = foreign "htons"   (int @-> returning int)

type t = file_descr

type timeout = { mutable ns: int64 }

(* Some timout functions --------------- *)
               
let timeout_of_seconds (s : int) : timeout =
  { ns = Int64.mul (Int64.of_int s) 1_000_000_000L }

let subtract_elapsed (t : timeout) (delta : int64) : unit  =
  t.ns <- max 0L (Int64.sub t.ns delta)

let timeout_secs (t : timeout) : float =
  Int64.to_float t.ns /. 1e9

(* ------- *)
  
let open_raw (socket_type : int) : t =
  let proto = htons socket_type in
  let rawfd = socket af_packet sock_raw proto in
  if rawfd < 0 then failwith "socket() failed" else
    let fd = Obj.magic rawfd in
    set_nonblock fd;
    fd

(* ------- *)

let (close : t -> unit) = Unix.close

(* ------- *)

let read_frame (file_descr : t) (timeout : timeout) : bytes =
  if timeout.ns <= 0L then raise Timeout;
  let t0 = Unix.gettimeofday () in
  let ready, _, _ = select [file_descr] [] [] (timeout_secs timeout) in
  let dt = Unix.gettimeofday () -. t0 in
  subtract_elapsed timeout (Int64.of_float (dt *. 1e9));
  match ready with
  | [] -> raise Timeout
  | _  ->
    let buf = Bytes.create 4096 in
    let len = recv file_descr buf 0 4096 [] in
    Bytes.sub buf 0 len

(* ------- *)

let iterate (file_descr : t) (timeout : timeout) : bytes Seq.t =
  Seq.unfold
    (fun t ->
       match try Some (read_frame file_descr t) with Timeout -> None with
       | Some frame -> Some (frame, t)
       | None       -> None)
    timeout

(* ------- *)

let hexdump ?(width : int = 16) (data : bytes) : string =
  let len = Bytes.length data in
  let buf = Buffer.create (len * 3) in
  for i = 0 to len - 1 do
    if i mod width = 0 then bprintf buf "%04x: " i;
    bprintf buf "%02x " (Char.code (Bytes.get data i));
    if i mod width = width - 1 then bprintf buf "\n";
  done;
  Buffer.contents buf

(* ------- *)

let mac ~(interface : string) =
  (* check it exists on Linux *)
  let nets = Sys.readdir "/sys/class/net" in
  if not (Array.exists ((=) interface) nets) then
    failwith ("Unknown interface: " ^ interface);
  let path = Printf.sprintf "/sys/class/net/%s/address" interface in
  let ic = open_in path in
  let human =
    try
      let line = input_line ic in
      close_in ic;
      String.trim line
    with _ ->
      close_in_noerr ic;
      failwith ("Could not read MAC from " ^ path)
  in
  let parts = String.split_on_char ':' human in
  let b = Bytes.create (List.length parts) in
  List.iteri (fun i h ->
    Bytes.set b i (Char.chr (int_of_string ("0x" ^ h)))
  ) parts;
  human, b
