(*****************************************)
(* Library Created by Yannick Schürmann  *)
(* Github: https://github.com/Jan-Kluger *)
(*****************************************)

[@@@ocaml.warning "-32"]

(** A contiguous buffer of bytes. *)
type buf = (int, Bigarray.int8_unsigned_elt, Bigarray.c_layout) Bigarray.Array1.t

(** [icmp6_checksum hdr_ptr payload len] returns a 4-tuple:
    - [checksum]: the 16-bit ICMPv6 checksum result,
    - [hdr_ptr]: the same IPv6 header pointer passed in,
    - [payload]: the same payload buffer (which may have been modified),
    - [len]: the original length argument. *)
val icmp6_checksum
  : nativeint                (** pointer to struct ip6_hdr *)
    -> buf                    (** payload buffer *)
    -> int                    (** payload length *)
    -> int * nativeint * buf * int

(** [get_crc32 buf len] returns a 3-tuple:
    - [crc]: the CRC32 result,
    - [buf]: the same buffer passed in,
    - [len]: the original length argument. *)
val get_crc32
  : buf                       (** input buffer *)
    -> int                    (** length *)
    -> int * buf * int

(** [hexdump_str buf len] returns a 3-tuple:
    - [s]: the formatted hexdump string,
    - [buf]: the same buffer passed in,
    - [len]: the original length argument. *)
val hexdump_str
  : buf                       (** input buffer *)
    -> int                    (** length *)
    -> string * buf * int

(** [hexdump buf len] returns a 3-tuple:
    - [()]: unit,
    - [buf]: the same buffer passed in,
    - [len]: the original length argument. *)
val hexdump
  : buf                       (** input buffer *)
    -> int                    (** length *)
    -> unit * buf * int

(** [grnvs_open ifname layer] returns a 3-tuple:
    - [fd]: the file descriptor of the raw socket,
    - [ifname]: the same interface name passed in,
    - [layer]: the same layer argument. *)
val grnvs_open
  : string                    (** interface name *)
    -> int                    (** layer, e.g. SOCK_RAW or SOCK_DGRAM *)
    -> int * string * int

(** [grnvs_read fd buf maxlen timeout] returns a 5-tuple:
    - [n]: the number of bytes read,
    - [fd]: the same file descriptor,
    - [buf]: the same buffer filled with data,
    - [maxlen]: the original maximum length requested,
    - [timeout]: the remaining timeout in milliseconds. *)
val grnvs_read
  : int                       (** file descriptor *)
    -> buf                    (** buffer to read into *)
    -> int                    (** maximum number of bytes to read *)
    -> int                    (** timeout in ms, will be updated *)
    -> int * int * buf * int * int

(** [grnvs_write fd buf len] returns a 4-tuple:
    - [n]: number of bytes written,
    - [fd]: same file descriptor,
    - [buf]: same buffer,
    - [len]: original length argument. *)
val grnvs_write
  : int                       (** file descriptor *)
    -> buf                    (** buffer to write from *)
    -> int                    (** length to write *)
    -> int * int * buf * int

(** [grnvs_close fd] returns a 2-tuple:
    - [r]: return code of close,
    - [fd]: the same file descriptor passed in. *)
val grnvs_close
  : int                       (** file descriptor *)
    -> int * int

(** [grnvs_get_hwaddr fd] returns a 2-tuple:
    - [mac]: MAC address as a string of length ETH_ALEN,
    - [fd]: the same file descriptor passed in. *)
val grnvs_get_hwaddr
  : int                       (** file descriptor *)
    -> string * int

(** [grnvs_get_ipaddr fd] returns a 2-tuple:
    - [ip]: IPv4 address as network-order int,
    - [fd]: the same file descriptor passed in. *)
val grnvs_get_ipaddr
  : int                       (** file descriptor *)
    -> int * int

(** [grnvs_get_ip6addr fd] returns a 2-tuple:
    - [ip6]: IPv6 address as a 16-byte string,
    - [fd]: the same file descriptor passed in. *)
val grnvs_get_ip6addr
  : int                       (** file descriptor *)
    -> string * int
