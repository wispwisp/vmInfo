# vmInfo

Requirements
  - libvirt (tested on 1.2.21 + QEMU API 1.2.21)
  - pugixml (tested on 1.5)
  - boost (1.56)

TODO:
  * parse hypervisor for virConnectOpen()
  * libvirt errors processing
  * better communication protocol? (now XML)
  * better api in Result
  * signal handler
  * asio errors processing
  * Makefile messy
  * testcase: "http://localhost//list"