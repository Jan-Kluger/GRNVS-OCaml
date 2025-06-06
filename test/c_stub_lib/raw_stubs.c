/* raw_stubs.c: OCaml C-stubs wrapping your gateway functions */
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>    /* Defines CAMLparam*, CAMLreturn*, etc. */
#include <caml/fail.h>
#include <caml/custom.h>
#include <caml/bigarray.h>

#include "raw.h"
#include "checksums.h"
#include "hexdump.h"
#include "timespec.h"

/* ICMPv6 checksum: returns (ret, hdr_ptr, payload, len) */
CAMLprim value caml_icmp6_checksum(value v_hdr_ptr, value v_payload, value v_len) {
  CAMLparam3(v_hdr_ptr, v_payload, v_len);
  CAMLlocal3(v_ret, v_tup, v_len_out);

  const struct ip6_hdr *hdr = (const struct ip6_hdr *)Nativeint_val(v_hdr_ptr);
  uint8_t *payload = (uint8_t *)Caml_ba_data_val(v_payload);
  size_t len = Int_val(v_len);
  uint16_t res = icmp6_checksum(hdr, payload, len);

  v_ret = Val_int(res);
  v_len_out = v_len;

  v_tup = caml_alloc(4, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_hdr_ptr);
  Store_field(v_tup, 2, v_payload);
  Store_field(v_tup, 3, v_len_out);

  CAMLreturn(v_tup);
}

/* CRC32: returns (ret, buf, len) */
CAMLprim value caml_get_crc32(value v_buf, value v_len) {
  CAMLparam2(v_buf, v_len);
  CAMLlocal3(v_ret, v_len_out, v_tup);

  void *buf = (void *)Caml_ba_data_val(v_buf);
  size_t len = Int_val(v_len);
  uint32_t crc = get_crc32(buf, len);

  v_ret = Val_int(crc);
  v_len_out = v_len;

  v_tup = caml_alloc(3, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_buf);
  Store_field(v_tup, 2, v_len_out);

  CAMLreturn(v_tup);
}

/* Hexdump string: returns (string, buf, len) */
CAMLprim value caml_hexdump_str(value v_buf, value v_len) {
  CAMLparam2(v_buf, v_len);
  CAMLlocal4(v_str, v_buf_out, v_len_out, v_tup);

  void *buf = (void *)Caml_ba_data_val(v_buf);
  ssize_t len = Int_val(v_len);
  char *s = hexdump_str(buf, len);

  v_str = caml_copy_string(s);
  v_buf_out = v_buf;
  v_len_out = v_len;

  v_tup = caml_alloc(3, 0);
  Store_field(v_tup, 0, v_str);
  Store_field(v_tup, 1, v_buf_out);
  Store_field(v_tup, 2, v_len_out);

  CAMLreturn(v_tup);
}

/* Printing hexdump: returns ((), buf, len) */
CAMLprim value caml_hexdump(value v_buf, value v_len) {
  CAMLparam2(v_buf, v_len);
  CAMLlocal3(v_unit, v_buf_out, v_len_out);

  void *buf = (void *)Caml_ba_data_val(v_buf);
  ssize_t len = Int_val(v_len);
  hexdump(buf, len);

  v_unit = Val_unit;
  v_buf_out = v_buf;
  v_len_out = v_len;

  value v_tup = caml_alloc(3, 0);
  Store_field(v_tup, 0, v_unit);
  Store_field(v_tup, 1, v_buf_out);
  Store_field(v_tup, 2, v_len_out);

  CAMLreturn(v_tup);
}

/* Raw socket open: returns (fd, ifname, layer) */
CAMLprim value caml_grnvs_open(value v_ifname, value v_layer) {
  CAMLparam2(v_ifname, v_layer);
  CAMLlocal3(v_fd, v_layer_out, v_tup);

  const char *ifname = String_val(v_ifname);
  int layer = Int_val(v_layer);
  int fd = grnvs_open(ifname, layer);
  if (fd < 0) caml_failwith("grnvs_open failed");

  v_fd = Val_int(fd);
  v_layer_out = v_layer;

  v_tup = caml_alloc(3, 0);
  Store_field(v_tup, 0, v_fd);
  Store_field(v_tup, 1, v_ifname);
  Store_field(v_tup, 2, v_layer_out);

  CAMLreturn(v_tup);
}

/* Read with timeout: returns (ret, fd, buf, maxlen, timeout_left) */
CAMLprim value caml_grnvs_read(value v_fd, value v_buf, value v_maxlen, value v_timeout) {
  CAMLparam4(v_fd, v_buf, v_maxlen, v_timeout);
  CAMLlocal5(v_ret, v_fd_out, v_buf_out, v_maxlen_out, v_timeout_out);

  int fd = Int_val(v_fd);
  void *buf = (void *)Caml_ba_data_val(v_buf);
  size_t maxlen = Int_val(v_maxlen);
  unsigned int timeout = Int_val(v_timeout);

  ssize_t ret = grnvs_read(fd, buf, maxlen, &timeout);
  if (ret < 0) caml_failwith("grnvs_read failed");

  v_ret = Val_int(ret);
  v_fd_out = v_fd;
  v_buf_out = v_buf;
  v_maxlen_out = v_maxlen;
  v_timeout_out = Val_int((int)timeout);

  value v_tup = caml_alloc(5, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);
  Store_field(v_tup, 2, v_buf_out);
  Store_field(v_tup, 3, v_maxlen_out);
  Store_field(v_tup, 4, v_timeout_out);

  CAMLreturn(v_tup);
}

/* Write: returns (ret, fd, buf, len) */
CAMLprim value caml_grnvs_write(value v_fd, value v_buf, value v_len) {
  CAMLparam3(v_fd, v_buf, v_len);
  CAMLlocal4(v_ret, v_fd_out, v_buf_out, v_len_out);

  int fd = Int_val(v_fd);
  void *buf = (void *)Caml_ba_data_val(v_buf);
  size_t len = Int_val(v_len);
  ssize_t ret = grnvs_write(fd, buf, len);
  if (ret < 0) caml_failwith("grnvs_write failed");

  v_ret = Val_int(ret);
  v_fd_out = v_fd;
  v_buf_out = v_buf;
  v_len_out = v_len;

  value v_tup = caml_alloc(4, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);
  Store_field(v_tup, 2, v_buf_out);
  Store_field(v_tup, 3, v_len_out);

  CAMLreturn(v_tup);
}

/* Close: returns (ret, fd) */
CAMLprim value caml_grnvs_close(value v_fd) {
  CAMLparam1(v_fd);
  CAMLlocal2(v_ret, v_fd_out);

  int fd = Int_val(v_fd);
  int r = grnvs_close(fd);

  v_ret = Val_int(r);
  v_fd_out = v_fd;

  value v_tup = caml_alloc(2, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);

  CAMLreturn(v_tup);
}

/* Get hardware address: returns (ret_str, fd) */
CAMLprim value caml_grnvs_get_hwaddr(value v_fd) {
  CAMLparam1(v_fd);
  CAMLlocal3(v_ret, v_fd_out, v_tup);

  int fd = Int_val(v_fd);
  const uint8_t *hw = grnvs_get_hwaddr(fd);
  if (!hw) caml_failwith("grnvs_get_hwaddr failed");

  v_ret = caml_copy_string((const char *)hw);
  v_fd_out = v_fd;

  v_tup = caml_alloc(2, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);

  CAMLreturn(v_tup);
}

/* Get IPv4 address: returns (ip, fd) */
CAMLprim value caml_grnvs_get_ipaddr(value v_fd) {
  CAMLparam1(v_fd);
  CAMLlocal2(v_ret, v_fd_out);

  int fd = Int_val(v_fd);
  struct in_addr addr = grnvs_get_ipaddr(fd);
  uint32_t ip = ntohl(addr.s_addr);

  v_ret = Val_int(ip);
  v_fd_out = v_fd;

  value v_tup = caml_alloc(2, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);

  CAMLreturn(v_tup);
}

/* Get IPv6 address: returns (ip6_str, fd) */
CAMLprim value caml_grnvs_get_ip6addr(value v_fd) {
  CAMLparam1(v_fd);
  CAMLlocal3(v_ret, v_fd_out, v_tup);

  int fd = Int_val(v_fd);
  const struct in6_addr *addr6 = grnvs_get_ip6addr(fd);
  if (!addr6) caml_failwith("grnvs_get_ip6addr failed");

  /* represent as a 16-byte string */
  v_ret = caml_copy_string((const char *)addr6->s6_addr);
  v_fd_out = v_fd;

  v_tup = caml_alloc(2, 0);
  Store_field(v_tup, 0, v_ret);
  Store_field(v_tup, 1, v_fd_out);

  CAMLreturn(v_tup);
}
