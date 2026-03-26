#pragma once

#include "esphome/components/display/display_buffer.h"
#include "esphome/core/component.h"

#include <FastEPD.h>

namespace esphome {
namespace fastepd_display {

class FastEPDDisplay : public PollingComponent, public display::DisplayBuffer {
public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;

  void set_full_update_clear_mode(uint8_t clear_mode) {
    this->full_update_clear_mode_ = clear_mode;
  }

  display::DisplayType get_display_type() override {
    return display::DisplayType::DISPLAY_TYPE_BINARY;
  }

protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  int get_width_internal() override { return 960; }
  int get_height_internal() override { return 540; }

  FASTEPD fastepd_;
  bool initialized_{false};
  uint8_t full_update_clear_mode_{CLEAR_SLOW};
};

} // namespace fastepd_display
} // namespace esphome
