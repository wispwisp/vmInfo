#include "Command.hpp"

#include <string>
#include <cstring>
#include <functional>

#include <libvirt/libvirt.h>

namespace ToolsForLibvirt {

  void collectDomainName(Result& result, virDomainPtr domain) noexcept {
    const char* name = virDomainGetName(domain);
    if (name)
      result.add("domain_name", name);
    else
      result.add("domain_name", "Undefined");
  }

  void appendBlockInfo(Result& result,
		       virDomainPtr domain,
		       virDomainFSInfoPtr info) noexcept {
    if (info->ndevAlias) {

      // what about the array of aliases ?
      const char *mountpoint = info->devAlias[0];

      virDomainBlockInfo blockInfo;
      if (virDomainGetBlockInfo(domain, mountpoint, &blockInfo, 0) != -1) {
	result.add("fs_capacity",
		   std::to_string(blockInfo.capacity).c_str());
	result.add("fs_allocation",
		   std::to_string(blockInfo.allocation).c_str());
	result.add("fs_physical",
		   std::to_string(blockInfo.physical).c_str());
      }
    }
  }

  void appendFSInfo(Result& result, virDomainFSInfoPtr info) noexcept {
    result.add("fs_name", info->name);
    result.add("fs_type", info->fstype);
    result.add("fs_mount", info->mountpoint);
  }

  void collectFSInfoFromDomain(Result& result, virDomainPtr domain) noexcept {

    // mount points
    virDomainFSInfoPtr *info;
    int numOfMountPoints = virDomainGetFSInfo(domain, &info, 0);
    if (numOfMountPoints == -1) {
      result.add("error", "virConnectListDomains()");
      return;
    }

    // collect fs`s
    for (int i = 0; i < numOfMountPoints; i++) {
      appendFSInfo(result, info[i]);
      appendBlockInfo(result, domain, info[i]);
      virDomainFSInfoFree(info[i]);
    }

    free(info);
  }

  void doWithAllDomains(virConnectPtr connection,
			Result& result,
			std::function<void(Result&, virDomainPtr)> action) noexcept {

    virDomainPtr *domains;
    int domainNumber = virConnectListAllDomains(connection, &domains, 0);
    if (domainNumber ) {
      if (domainNumber != -1) {
	for (int i = 0; i < domainNumber; i++) {
	  action(result, domains[i]);
	  virDomainFree(domains[i]);
	}
	free(domains);
      }
      else 
	result.add("error", "virConnectListAllDomains()");
    }
    else
      result.add("info", "There are no domains");
  }
}


namespace Commands {
  void list(virConnectPtr connection,
	    Result& result,
	    const Request& /*request*/) noexcept {
    ToolsForLibvirt::doWithAllDomains(connection,
				      result,
				      ToolsForLibvirt::collectDomainName);
  }

  void fileSystems(virConnectPtr connection,
		   Result& result,
		   const Request& /*request*/) noexcept {

    ToolsForLibvirt::doWithAllDomains(connection,
				      result,
				      ToolsForLibvirt::collectFSInfoFromDomain);
  }
}


Command::cmdType Command::decodeCommand(const Request& request) noexcept {

  if (request.getCommand() == "list")
    return cmdType::list;
  else if (request.getCommand() == "fsInfo")
    return cmdType::fsInfo;

  return cmdType::Unknown;
}


void Command::processComand(Result& result,
			    const Request& request,
			    cmdType c) noexcept {

  /* TODO !!!*/ std::string name = request.getHypervisor() == "qemu" ? "qemu:///session" : "test:///default";

  virConnectPtr connection = virConnectOpen(name.c_str());
  if (!connection) {
    result.add("error", "Domain undefined error");
    return;
  }

  switch(c) {

  case cmdType::list:
    Commands::list(connection, result, request);
    break;

  case cmdType::fsInfo:
    Commands::fileSystems(connection, result, request);
    break;

  case cmdType::Unknown:
  default:
    result.add("error", "No such command");
  }

  virConnectClose(connection);
}
