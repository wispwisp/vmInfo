# vmInfo

Requirements
  - libvirt (tested on 1.2.21 + QEMU API 1.2.21)
  - pugixml (tested on 1.5)
  - boost (1.56)

TODO:
  * parse hypervisor for virConnectOpen()
  * libvirt errors processing
  * better communication protocol? (now XML)
  * signal handler
  * asio errors processing
  * Makefile messy
  * method GET, POST, etc. in request
  * change Server`s std::cerr to log