/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <functional>

#include <jsi/jsi.h>
#include <react/renderer/uimanager/UIManager.h>
#include <react/renderer/uimanager/primitives.h>

namespace facebook::react {

using DispatchEvent = std::function<void(
    jsi::Runtime &runtime,
    const EventTarget *eventTarget,
    const std::string &type,
    ReactEventPriority priority,
    const EventPayload &payload)>;

using PointerIdentifier = int32_t;
using CaptureTargetOverrideRegistry =
    std::unordered_map<PointerIdentifier, ShadowNode::Weak>;

// Helper struct to package a PointerEvent and SharedEventTarget together
struct PointerEventTarget {
  PointerEvent event;
  SharedEventTarget target;
};

class PointerEventsProcessor final {
 public:
  void interceptPointerEvent(
      jsi::Runtime &runtime,
      EventTarget const *eventTarget,
      std::string const &type,
      ReactEventPriority priority,
      PointerEvent const &event,
      DispatchEvent const &eventDispatcher,
      UIManager const &uiManager);

  void setPointerCapture(
      PointerIdentifier pointerId,
      ShadowNode::Shared const &shadowNode);
  void releasePointerCapture(
      PointerIdentifier pointerId,
      ShadowNode const *shadowNode);
  bool hasPointerCapture(
      PointerIdentifier pointerId,
      ShadowNode const *shadowNode);

 private:
  void processPendingPointerCapture(
      PointerEvent const &event,
      jsi::Runtime &runtime,
      DispatchEvent const &eventDispatcher,
      UIManager const &uiManager);

  CaptureTargetOverrideRegistry pendingPointerCaptureTargetOverrides_;
  CaptureTargetOverrideRegistry activePointerCaptureTargetOverrides_;
};

} // namespace facebook::react
