#pragma once

#include "esphome/core/helpers.h"

#include "esphome/components/output/float_output.h"

#include "../tlc59283.h"

namespace esphome {
namespace tlc59283 {

class TLC59283Channel : public output::FloatOutput, public Parented<TLC59283> {
 public:
  void set_channel(uint8_t channel) { this->channel_ = channel; }

 protected:
  void write_state(float state) override;
  uint8_t channel_;
};

}  // namespace tlc59283
}  // namespace esphome
