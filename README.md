# vmInfo

Requirements
  - libvirt (tested on 1.2.21 + QEMU API 1.2.21)
  - pugixml (tested on 1.5)
  - boost (1.56)

Usage
  * 'server [port]' (log output in stderr)
  * 'client host:port/hypervisor/domain/command'
    (Example: './client localhost:8001/qemu/all/fsInfo')

TODO:
  * Result add functions should be noexcept
  * signal handler
  * method GET, POST, etc. in request
  * unsigned long to pugi
  * percent