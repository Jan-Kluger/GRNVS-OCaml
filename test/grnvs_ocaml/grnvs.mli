(*****************************************)
(* Library Created by Yannick Schürmann  *)
(* Github: https://github.com/Jan-Kluger *)
(*****************************************)

type timeout

(** Raised by [read_frame] or [iterate] when the timeout expires. *)
exception Timeout
  
(** [timeout_of_seconds : s] - creates a timeout of [s] seconds. *)
val timeout_of_seconds : int -> timeout

(** [timeout_to_seconds : s] - gets time in timeout in [s] seconds. *)
val timeout_secs : timeout -> float

type t  (** opaque raw socket handle *)

(** [open_raw] - opens a socket @ eth_p_all = 0x0003 *)
val open_raw  : unit -> t

(** Close the socket and release resources. *)
val close     : t -> unit

(** [read_frame : sock to] -  waits up to [to] for the next frame and returns
    its raw bytes, or raises [Timeout] if none arrived before [to] ran out. *)
val read_frame: t -> timeout -> bytes

(** [iterate : sock to] - is a sequence of all frames read from [sock]
    until the timeout expires. *)
val iterate   : t -> timeout -> bytes Seq.t

(** [hexdump : ~width b] - returns a  hex dump (default 16 bytes/row). *)
val hexdump   : ?width:int -> bytes -> string

(** [mac : ~interface] - returns a pair of
    - readable MAC address (e.g. "aa:bb:cc:dd:ee:ff")
    - a 6-byte [bytes] value suitable for use in packet headers*)
val mac       : interface:string -> string * bytes

