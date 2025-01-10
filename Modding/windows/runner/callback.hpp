#pragma once


#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include <windows.h>

#include <memory>
#include "helper.hpp"

inline auto runLauncher (
  const flutter::MethodCall<>& call,
  std::unique_ptr<flutter::MethodResult<>>& result
) -> void
{
  auto& arguments = std::get<flutter::EncodableMap>(*call.arguments());
  auto argument = arguments.find(flutter::EncodableValue("argument"));
  if (argument == arguments.end()) {
    result->Error("EXCEPTION", "argument parameter is required");
    return;
  }
  auto arguments_str = std::get<std::string>(argument->second);
  try {
    execute_windows_command(utf8_to_utf16(arguments_str));
    result->Success();
  } catch (std::runtime_error &e) {
    result->Error("EXCEPTION", e.what());
  }
}