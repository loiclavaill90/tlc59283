#include "tlc59283_output.h"

namespace esphome {
namespace tlc59283 {

void TLC59283Channel::write_state(float state) {
  auto amount = static_cast<uint16_t>(state * 0xffff);
  this->parent_->set_channel_value(this->channel_, amount);
}

}  // namespace tlc59283
}  // namespace esphome
