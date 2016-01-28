#include "Command.hpp"

#include <string>
#include <cstring>
#include <functional>

#include <libvirt/libvirt.h>

namespace ToolsForLibvirt {

  void domainList(Result& /*result*/, virDomainPtr /*domain*/) noexcept {
    ;// dummy example
  }

  void appendFSInfo(Result& result,
		    virDomainPtr domain,
		    virDomainFSInfoPtr info) noexcept {

    if (info->ndevAlias) {
      const char *mountpoint = info->devAlias[0];

      virDomainBlockInfo blockInfo;
      if (virDomainGetBlockInfo(domain, mountpoint, &blockInfo, 0) != -1) {
	result.addCurrentDomainFsInfo(info->name,
				      info->fstype,
				      info->mountpoint,
				      blockInfo.capacity,
				      blockInfo.allocation,
				      blockInfo.physical);
	return;
      }
    }

    result.addCurrentDomainFsInfo(info->name,
				  info->fstype,
				  info->mountpoint);
  }

  void collectFSInfoFromDomain(Result& result, virDomainPtr domain) noexcept {

    // mount points
    virDomainFSInfoPtr *info;
    int numOfMountPoints = virDomainGetFSInfo(domain, &info, 0);
    if (numOfMountPoints == -1) {
      result.error("Could not get FS info");
      return;
    }

    // collect fs`s
    for (int i = 0; i < numOfMountPoints; i++) {
      appendFSInfo(result, domain, info[i]);
      virDomainFSInfoFree(info[i]);
    }

    free(info);
  }

  void appendDomain(Result& result, virDomainPtr domain) noexcept {
    const char* name = virDomainGetName(domain);
    if (name)
      result.newDomain(name);
    else
      result.newDomain("Undefined");
  }

  void doWithAllDomains(virConnectPtr connection,
			Result& result,
			std::function<void(Result&, virDomainPtr)> action) noexcept {

    virDomainPtr *domains;
    int domainNumber = virConnectListAllDomains(connection, &domains, 0);
    if (domainNumber ) {
      if (domainNumber != -1) {
	for (int i = 0; i < domainNumber; i++) {
	  appendDomain(result, domains[i]);
	  action(result, domains[i]);
	  virDomainFree(domains[i]);
	}
	free(domains);
      }
      else
	result.error("Domains error");
    }
    else
      result.error("There are no domains");
  }

  const char* hypervisorName(const std::string& domainName) {
    if (domainName == "qemu")
      return "qemu:///session";
    else if (domainName == "test")
      return "test:///default";

    return nullptr;
  }
}


namespace Commands {
  void list(virConnectPtr connection,
	    Result& result,
	    const Request& /*request*/) noexcept {

    ToolsForLibvirt::doWithAllDomains(connection,
				      result,
				      ToolsForLibvirt::domainList);
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

  if (request.getDomain() == "all") {
    if (request.getCommand() == "list")
      return cmdType::list;
    else if (request.getCommand() == "fsInfo" ||
	     request.getCommand() == "df" )
      return cmdType::fsInfo;
  }

  return cmdType::Unknown;
}


void Command::processComand(Result& result,
			    const Request& request,
			    cmdType c) noexcept {

  auto name = ToolsForLibvirt::hypervisorName(request.getHypervisor());
  if (!name) {
    result.error("Unsupported Hypervisor");
    return;
  }
  
  virConnectPtr connection = virConnectOpen(name);
  if (!connection) {
    result.error("Hypervisor connection error");
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
    result.error("Undefined command");
  }

  virConnectClose(connection);
}
