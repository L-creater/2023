//
// Created by fss on 22-12-26.
//
#include <gtest/gtest.h>
#include <glog/logging.h>
#include "data/tensor.hpp"
#include "../../source/layer/details/cat.hpp"

TEST(test_layer, cat1) {
  using namespace kuiper_infer;
  int input_size = 4;
  int output_size = 2;
  int input_channels = 6;
  int output_channels = 12;

  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  for (int i = 0; i < input_size; ++i) {
    std::shared_ptr<Tensor<float>> input = std::make_shared<Tensor<float>>(input_channels, 32, 32);
    input->Rand();
    inputs.push_back(input);
  }
  std::vector<std::shared_ptr<Tensor<float>>> outputs(output_size);
  CatLayer cat_layer(1);
  const auto status = cat_layer.Forward(inputs, outputs);
  ASSERT_EQ(status, InferStatus::kInferSuccess);
  for (uint32_t i = 0; i < outputs.size(); ++i) {
    ASSERT_EQ(outputs.at(i)->channels(), output_channels);
  }

  for (int i = 0; i < input_size / 2; ++i) {
    for (int input_channel = 0; input_channel < input_channels; ++input_channel) {
      ASSERT_TRUE(arma::approx_equal(inputs.at(i)->slice(input_channel),
                                     outputs.at(i)->slice(input_channel), "absdiff", 0.01f));
    }
  }

  for (int i = input_size / 2; i < input_size; ++i) {
    for (int input_channel = input_channels; input_channel < input_channels * 2; ++input_channel) {
      ASSERT_TRUE(arma::approx_equal(
          inputs.at(i)->slice(input_channel - input_channels),
          outputs.at(i - output_size)->slice(input_channel), "absdiff", 0.01f));
    }
  }
}

TEST(test_layer, cat2) {
  using namespace kuiper_infer;
  int input_size = 8;
  int output_size = 4;
  int input_channels = 64;
  int output_channels = 128;

  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  for (int i = 0; i < input_size; ++i) {
    std::shared_ptr<Tensor<float>> input = std::make_shared<Tensor<float>>(input_channels, 32, 32);
    input->Rand();
    inputs.push_back(input);
  }
  std::vector<std::shared_ptr<Tensor<float>>> outputs(output_size);
  CatLayer cat_layer(1);
  const auto status = cat_layer.Forward(inputs, outputs);
  ASSERT_EQ(status, InferStatus::kInferSuccess);
  for (uint32_t i = 0; i < outputs.size(); ++i) {
    ASSERT_EQ(outputs.at(i)->channels(), output_channels);
  }

  for (int i = 0; i < input_size / 2; ++i) {
    for (int input_channel = 0; input_channel < input_channels; ++input_channel) {
      ASSERT_TRUE(arma::approx_equal(inputs.at(i)->slice(input_channel),
                                     outputs.at(i)->slice(input_channel), "absdiff", 0.01f));
    }
  }

  for (int i = input_size / 2; i < input_size; ++i) {
    for (int input_channel = input_channels; input_channel < input_channels * 2; ++input_channel) {
      ASSERT_TRUE(arma::approx_equal(
          inputs.at(i)->slice(input_channel - input_channels),
          outputs.at(i - output_size)->slice(input_channel), "absdiff", 0.01f));
    }
  }
}

TEST(test_layer, cat3) {
  using namespace kuiper_infer;
  int input_size = 3;
  int input_channels = 1;

  int output_size = 1;
  int output_channels = 3;

  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  for (int i = 0; i < input_size; ++i) {
    std::shared_ptr<Tensor<float>> input = std::make_shared<Tensor<float>>(input_channels, 4, 4);
    input->Fill(float(i) + 1.f);
    inputs.push_back(input);
  }
  std::vector<std::shared_ptr<Tensor<float>>> outputs(output_size);
  CatLayer cat_layer(1);
  const auto status = cat_layer.Forward(inputs, outputs);
  ASSERT_EQ(status, InferStatus::kInferSuccess);
  for (uint32_t i = 0; i < outputs.size(); ++i) {
    ASSERT_EQ(outputs.at(i)->channels(), output_channels);
  }

  for (int i = 0; i < input_size; ++i) {
    const arma::fmat &in_channel = inputs.at(i)->slice(0);
    const arma::fmat &out_channel = outputs.at(0)->slice(i);
    ASSERT_TRUE(arma::approx_equal(in_channel, out_channel, "absdiff", 0.01f));
  }
}

TEST(test_layer, cat4) {
  using namespace kuiper_infer;
  int input_size = 6;
  int input_channels = 1;

  int output_size = 1;
  int output_channels = 6;

  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  for (int i = 0; i < input_size; ++i) {
    std::shared_ptr<Tensor<float>> input = std::make_shared<Tensor<float>>(input_channels, 4, 4);
    input->Fill(float(i) + 1.f);
    inputs.push_back(input);
  }
  std::vector<std::shared_ptr<Tensor<float>>> outputs(output_size);
  CatLayer cat_layer(1);
  const auto status = cat_layer.Forward(inputs, outputs);
  ASSERT_EQ(status, InferStatus::kInferSuccess);
  for (uint32_t i = 0; i < outputs.size(); ++i) {
    ASSERT_EQ(outputs.at(i)->channels(), output_channels);
  }

  for (int i = 0; i < input_size; ++i) {
    const arma::fmat &in_channel = inputs.at(i)->slice(0);
    const arma::fmat &out_channel = outputs.at(0)->slice(i);
    ASSERT_TRUE(arma::approx_equal(in_channel, out_channel, "absdiff", 0.01f));
  }
}