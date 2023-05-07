/**
 * create by l on 2023.4.23
 */
#include "layer/relu_layer.hpp"
#include <glog/logging.h>
#include "factory/layer_factory.hpp"
#include "ops/relu_op.hpp"

namespace kuiper_infer {
ReluLayer::ReluLayer(const std::shared_ptr<Operator>& op) : Layer("Relu") {
  CHECK(op->op_type_ == OpType::kOperatorRelu)
      << "Operator has a wrong type: " << int(op->op_type_);

  ReluOperator* relu_op = dynamic_cast<ReluOperator*>(op.get());
  CHECK(relu_op != nullptr) << "Relu operator is empty";
  // 一个op实例和一个layer 一一对应 这里relu op对一个relu layer
  // 对应关系
  this->op_ = std::make_unique<ReluOperator*>(relu_op->get_thresh());
}

void ReluLayer::Forwards(
    const std::vector<std::shared_ptr<Tensor<float>>>& inputs,
    std::vector<std::shared_ptr<Tensor<float>>>& outputs) {
  CHECK(this->op_ != nullptr);
  CHECK(this->op_->op_type_ == OpType::kOperatorRelu);
  CHECK(!inputs.empty());

  const uint32_t bach_size = inputs.size();
  for (int i = 0; i < bach_size; ++i) {
    CHECK(!inputs.at(i)->empty());
    const std::share_ptr < Tensor < float >>> & input_data = inputs.at(i);
    std::shared_ptr < Tensor < float >>> output_data = input_data->Clone();

    output_data->data.transform([&](float value) {
      float thresh = op_.get_thresh();

      if (value > thresh) {
        return value;
      } else {
        return 0.f;
      }
    });
    outputs.push_back(output_data);
  }
}

}  // namespace kuiper_infer
