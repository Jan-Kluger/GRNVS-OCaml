#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/unixsupport.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

/* Create a raw packet socket */
CAMLprim value ocaml_socket_raw(value unit) {
  int fd = socket(PF_PACKET, SOCK_RAW, htons(0x0003));
  if (fd < 0) caml_failwith("socket(PF_PACKET)");
  return Val_int(fd);
}

/* Host-to-network short conversion */
CAMLprim value ocaml_htons(value v) {
  int x = Int_val(v);
  return Val_int(htons(x));
}

/* Retrieve the MAC address for the given interface and return (string * bytes) */
CAMLprim value ocaml_get_mac(value v_ifname)
{
    CAMLparam1(v_ifname);
    CAMLlocal3(v_human, v_raw, v_tuple);
    const char *ifname = String_val(v_ifname);

    /* Open a socket for ioctl */
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) uerror("socket(AF_INET)", v_ifname);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    /* Copy interface name, ensuring null-termination */
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

    /* Perform the ioctl to get hardware address */
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        int err = errno;
        close(fd);
        errno = err;
        uerror("ioctl(SIOCGIFHWADDR)", v_ifname);
    }
    close(fd);

    /* Allocate the raw bytes string */
    v_raw = caml_alloc_string(6);
    /* Fill raw bytes (cast away const) */
    memcpy((char *)String_val(v_raw), ifr.ifr_hwaddr.sa_data, 6);

    /* Build human-readable hex string "aa:bb:cc:dd:ee:ff" */
    char buf[18];
    unsigned char *hw = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
             hw[0], hw[1], hw[2], hw[3], hw[4], hw[5]);
    v_human = caml_copy_string(buf);

    /* Return tuple (human, raw) */
    v_tuple = caml_alloc(2, 0);
    Store_field(v_tuple, 0, v_human);
    Store_field(v_tuple, 1, v_raw);

    CAMLreturn(v_tuple);
}
