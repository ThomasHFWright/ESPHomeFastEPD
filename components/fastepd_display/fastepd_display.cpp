#include "fastepd_display.h"

#include "esphome/core/log.h"

namespace esphome {
namespace fastepd_display {

static const char *const TAG = "fastepd_display";

void FastEPDDisplay::setup() {
  ESP_LOGI(TAG, "Initializing FastEPD panel");
  const int rc = this->fastepd_.initPanel(BB_PANEL_M5PAPERS3);
  if (rc != BBEP_SUCCESS) {
    ESP_LOGE(TAG, "FastEPD init failed (error=%d)", rc);
    this->mark_failed();
    return;
  }

  const int mode_rc = this->fastepd_.setMode(BB_MODE_1BPP);
  if (mode_rc != BBEP_SUCCESS) {
    ESP_LOGE(TAG, "FastEPD 1bpp mode setup failed (error=%d)", mode_rc);
    this->mark_failed();
    return;
  }

  this->fastepd_.fillScreen(BBEP_WHITE);
  ESP_LOGI(TAG, "Performing initial display render");
  this->do_update_();
  const int update_rc =
      this->fastepd_.fullUpdate(this->full_update_clear_mode_, false, nullptr);
  if (update_rc != BBEP_SUCCESS) {
    ESP_LOGE(TAG, "FastEPD initial full update failed (error=%d)", update_rc);
    this->mark_failed();
    return;
  }
  this->initialized_ = true;
}

void FastEPDDisplay::update() {
  if (!this->initialized_) {
    return;
  }

  this->do_update_();
  const int update_rc =
      this->fastepd_.fullUpdate(this->full_update_clear_mode_, false, nullptr);
  if (update_rc != BBEP_SUCCESS) {
    ESP_LOGE(TAG, "FastEPD full update failed (error=%d)", update_rc);
    this->status_set_warning();
  }
}

void FastEPDDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  this->fastepd_.drawPixel(x, y, color.is_on() ? BBEP_WHITE : BBEP_BLACK);
}

void FastEPDDisplay::dump_config() {
  LOG_DISPLAY("  ", "FastEPD Display", this);
  ESP_LOGCONFIG(TAG, "  Panel: M5Stack PaperS3");
  ESP_LOGCONFIG(TAG, "  Full Update Clear Mode: %u", this->full_update_clear_mode_);
  LOG_UPDATE_INTERVAL(this);
}

float FastEPDDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

} // namespace fastepd_display
} // namespace esphome
