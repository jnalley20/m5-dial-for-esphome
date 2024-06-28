#pragma once

#include <memory>
#include <vector>

#include "esphome/core/log.h"
#include "esphome/core/helpers.h"


namespace esphome {
namespace nfc {

class NfcTag {
 public:
  NfcTag() {
    this->uid_ = {};
    this->tag_type_ = "Unknown";
  };
  NfcTag(std::vector<uint8_t> &uid) {
    this->uid_ = uid;
    this->tag_type_ = "Unknown";
  };
  NfcTag(std::vector<uint8_t> &uid, const std::string &tag_type) {
    this->uid_ = uid;
    this->tag_type_ = tag_type;
  };

  std::vector<uint8_t> &get_uid() { return this->uid_; };
  const std::string &get_tag_type() { return this->tag_type_; };
  
 protected:
  std::vector<uint8_t> uid_;
  std::string tag_type_;
};

}  // namespace nfc
}  // namespace esphome