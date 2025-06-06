(*****************************************)
(* Library Created by Yannick Schürmann  *)
(* Github: https://github.com/Jan-Kluger *)
(*****************************************)

[@@@ocaml.warning "-32"]

type buf = (int, Bigarray.int8_unsigned_elt, Bigarray.c_layout) Bigarray.Array1.t

external icmp6_checksum
  : nativeint
    -> buf
    -> int
    -> (int * nativeint * buf * int)
  = "caml_icmp6_checksum"

external get_crc32
  : buf
    -> int
    -> (int * buf * int)
  = "caml_get_crc32"

external hexdump_str
  : buf
    -> int
    -> (string * buf * int)
  = "caml_hexdump_str"

external hexdump
  : buf
    -> int
    -> (unit * buf * int)
  = "caml_hexdump"

external grnvs_open
  : string
    -> int
    -> (int * string * int)
  = "caml_grnvs_open"

external grnvs_read
  : int
    -> buf
    -> int
    -> int
    -> (int * int * buf * int * int)
  = "caml_grnvs_read"

external grnvs_write
  : int
    -> buf
    -> int
    -> (int * int * buf * int)
  = "caml_grnvs_write"

external grnvs_close
  : int
    -> (int * int)
  = "caml_grnvs_close"

external grnvs_get_hwaddr
  : int
    -> (string * int)
  = "caml_grnvs_get_hwaddr"

external grnvs_get_ipaddr
  : int
    -> (int * int)
  = "caml_grnvs_get_ipaddr"

external grnvs_get_ip6addr
  : int
    -> (string * int)
  = "caml_grnvs_get_ip6addr"
