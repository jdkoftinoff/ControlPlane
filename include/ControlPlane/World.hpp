#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>
#include <typeinfo>
#include <array>
#include <tuple>
#include <valarray>
#include <algorithm>
#include <functional>
#include <limits>
#include <initializer_list>
#include <memory.h>
#include <map>
#include <atomic>
#include <set>
#include <mutex>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#if defined( WIN32 )
#define sprintf sprintf_s
#define snprintf _snprintf_s
#endif
namespace ControlPlane
{
using std::string;
using std::map;
using std::make_shared;
using std::weak_ptr;
using std::function;
using std::vector;
using std::shared_ptr;
using std::ostream;
using Milliseconds = std::chrono::milliseconds;

using SchemaAddressElement = string;
///
/// \brief Address
///
/// The Address type is a vector of AddressElements which describe a hierarchical address
///
using SchemaAddress = vector<SchemaAddressElement>;
using DescriptorType = uint16_t;
using DescriptorIndex = uint16_t;
using ControlType = uint64_t;
using ControlValueType = uint16_t;
}

#include "Util.hpp"
