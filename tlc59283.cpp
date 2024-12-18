#include "tlc59283.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tlc59283 {

static const char *const TAG = "tlc59283";

void TLC59283::setup() {
  this->data_pin_->setup();
  this->data_pin_->digital_write(true);
  this->clock_pin_->setup();
  this->clock_pin_->digital_write(true);

  this->pwm_amounts_.resize(this->num_chips_ * N_CHANNELS_PER_CHIP, 0);

  ESP_LOGCONFIG(TAG, "Done setting up TLC59283 output component.");
}
void TLC59283::dump_config() {
  ESP_LOGCONFIG(TAG, "TLC59283:");
  LOG_PIN("  Data Pin: ", this->data_pin_);
  LOG_PIN("  Clock Pin: ", this->clock_pin_);
  ESP_LOGCONFIG(TAG, "  Number of chips: %u", this->num_chips_);
}

void TLC59283::loop() {
  if (!this->update_)
    return;

  uint32_t command;

  // Magic word for write
  command = 0x25;

  command <<= 5;
  // OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
  command |= 0x16;

  command <<= 7;
  command |= 0x7F;  // default 100% brigthness

  command <<= 7;
  command |= 0x7F;  // default 100% brigthness

  command <<= 7;
  command |= 0x7F;  // default 100% brigthness

  for (uint8_t n = 0; n < num_chips_; n++) {
    this->transfer_(command >> 24);
    this->transfer_(command >> 16);
    this->transfer_(command >> 8);
    this->transfer_(command);

    // 24 channels per TLC59283
    for (int8_t c = 23; c >= 0; c--) {
      // 16 bits per channel, send MSB first
      this->transfer_(pwm_amounts_.at(n * 24 + c) >> 8);
      this->transfer_(pwm_amounts_.at(n * 24 + c));
    }
  }

  this->update_ = false;
}

void TLC59283::transfer_(uint8_t send) {
  uint8_t startbit = 0x80;

  bool towrite, lastmosi = !(send & startbit);
  uint8_t bitdelay_us = (1000000 / 1000000) / 2;

  for (uint8_t b = startbit; b != 0; b = b >> 1) {
    if (bitdelay_us) {
      delayMicroseconds(bitdelay_us);
    }

    towrite = send & b;
    if ((lastmosi != towrite)) {
      this->data_pin_->digital_write(towrite);
      lastmosi = towrite;
    }

    this->clock_pin_->digital_write(true);

    if (bitdelay_us) {
      delayMicroseconds(bitdelay_us);
    }

    this->clock_pin_->digital_write(false);
  }
}
void TLC59283::set_channel_value(uint16_t channel, uint16_t value) {
  if (channel >= this->num_chips_ * N_CHANNELS_PER_CHIP)
    return;
  if (this->pwm_amounts_[channel] != value) {
    this->update_ = true;
  }
  this->pwm_amounts_[channel] = value;
}

}  // namespace tlc59283
}  // namespace esphome
