/**
 * create by l on 2023.4.23
 */

#ifndef KUIPER_COURSE_DATA_BLOB_HPP_
#define KUIPER_COURSE_DATA_BLOB_HPP_
#include "op.hpp"
namespace kuiper_infer {
class ReluOperator : public Operator {
 private:
  float thresh_ = 0.f;

 public:
  ReluOperator(float thresh);
  float get_thresh() const;
  void set_thresh(const float& thresh);
};

}  // namespace kuiper_infer
#endif