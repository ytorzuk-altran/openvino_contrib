// Copyright (C) 2021-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "single_layer_tests/select.hpp"

#include <vector>

#include "common_test_utils/test_constants.hpp"
#include "cuda_test_constants.hpp"

using namespace LayerTestsDefinitions;

const std::vector<InferenceEngine::Precision> inputPrecision = {
    InferenceEngine::Precision::U8,
    InferenceEngine::Precision::I16,
    InferenceEngine::Precision::FP16,
    InferenceEngine::Precision::FP32,
};

const std::vector<std::vector<std::vector<size_t>>> noneShapes = {{{1}, {1}, {1}},
                                                                  {{8}, {8}, {8}},
                                                                  {{4, 5}, {4, 5}, {4, 5}},
                                                                  {{3, 4, 5}, {3, 4, 5}, {3, 4, 5}},
                                                                  {{2, 3, 4, 5}, {2, 3, 4, 5}, {2, 3, 4, 5}},
                                                                  {{2, 3, 4, 5, 6}, {2, 3, 4, 5, 6}, {2, 3, 4, 5, 6}}};

const auto noneCases = ::testing::Combine(::testing::ValuesIn(noneShapes),
                                          ::testing::ValuesIn(inputPrecision),
                                          ::testing::Values(ov::op::AutoBroadcastSpec::NONE),
                                          ::testing::Values(CommonTestUtils::DEVICE_NVIDIA));

const std::vector<std::vector<std::vector<size_t>>> numpyShapes = {{{1}, {1}, {1}},
                                                                   {{1}, {16}, {1}},
                                                                   {{1}, {1}, {16}},
                                                                   {{1}, {8}, {8}},
                                                                   {{8}, {1}, {8}},
                                                                   {{8}, {8}, {8}},
                                                                   {{4, 1}, {1}, {4, 8}},
                                                                   {{3, 8}, {8}, {3, 1}},
                                                                   {{8, 1}, {8, 1}, {8, 1}},
                                                                   {{1}, {5, 8}, {5, 8}},
                                                                   {{8, 1, 1}, {8, 1, 1}, {2, 5}},
                                                                   {{8, 1}, {6, 8, 1}, {6, 1, 1}},
                                                                   {{5, 1}, {8, 1, 7}, {5, 7}},
                                                                   {{2, 8, 1}, {2, 8, 9}, {2, 1, 9}},
                                                                   {{1, 4}, {8, 1, 1, 1}, {4}},
                                                                   {{5, 4, 1}, {8, 5, 1, 1}, {4, 1}},
                                                                   {{1, 4}, {6, 1, 8, 1}, {6, 1, 8, 4}},
                                                                   {{7, 3, 1, 8}, {7, 1, 1, 8}, {3, 2, 8}},
                                                                   {{1, 3, 1}, {8, 2, 3, 1}, {3, 9}},
                                                                   {{5, 1, 8}, {2, 1, 9, 8}, {2, 5, 9, 8}},
                                                                   {{6, 1, 1, 8}, {6, 7, 1, 8}, {2, 1}},
                                                                   {{5, 1, 1, 1}, {5, 7, 8, 6}, {1, 8, 6}},
                                                                   {{8, 1, 5}, {8, 1, 1, 1, 1}, {8, 7, 5}},
                                                                   {{8, 1, 1, 9}, {4, 8, 1, 1, 1}, {1, 1, 9}},
                                                                   {{5, 1, 2, 1}, {8, 1, 9, 1, 1}, {5, 1, 2, 1}},
                                                                   {{8, 1}, {2, 1, 1, 8, 1}, {9, 1, 1}},
                                                                   {{8, 5, 5, 5, 1}, {8, 1, 1, 1, 8}, {5, 5, 5, 8}},
                                                                   {{4}, {8, 5, 6, 1, 1}, {2, 4}},
                                                                   {{9, 9, 2, 8, 1}, {9, 1, 2, 8, 1}, {9, 1, 1, 1}},
                                                                   {{5, 3, 3}, {8, 1, 1, 3, 3}, {5, 1, 3}},
                                                                   {{5, 1, 8, 1}, {5, 5, 1, 8, 1}, {1}},
                                                                   {{3}, {6, 8, 1, 1, 3}, {6, 1, 5, 3, 3}},
                                                                   {{5, 1}, {3, 1, 4, 1, 8}, {1, 4, 5, 8}},
                                                                   {{2, 1, 5}, {8, 6, 2, 3, 1}, {5}},
                                                                   {{6}, {2, 1, 9, 8, 6}, {2, 4, 9, 8, 6}},
                                                                   {{5, 7, 1, 8, 1}, {5, 7, 1, 8, 4}, {8, 1}},
                                                                   {{7, 6, 5, 8}, {4, 7, 6, 5, 8}, {6, 1, 8}}};

const auto numpyCases = ::testing::Combine(::testing::ValuesIn(numpyShapes),
                                           ::testing::ValuesIn(inputPrecision),
                                           ::testing::Values(ov::op::AutoBroadcastSpec::NUMPY),
                                           ::testing::Values(CommonTestUtils::DEVICE_NVIDIA));

const std::vector<std::vector<std::vector<size_t>>> tacotronShapes = {
    {{}, {1, 200}, {1, 200}},
    {{1}, {1, 200}, {1, 200}},
    {{1, 200}, {1, 200}, {1, 200}},
};

const auto tacotronCases = ::testing::Combine(::testing::ValuesIn(noneShapes),
                                              ::testing::ValuesIn(inputPrecision),
                                              ::testing::Values(ov::op::AutoBroadcastSpec::NUMPY),
                                              ::testing::Values(CommonTestUtils::DEVICE_NVIDIA));

INSTANTIATE_TEST_CASE_P(smoke_CUDA_TestsSelect_none, SelectLayerTest, noneCases, SelectLayerTest::getTestCaseName);

INSTANTIATE_TEST_CASE_P(smoke_CUDA_TestsSelect_numpy, SelectLayerTest, numpyCases, SelectLayerTest::getTestCaseName);

INSTANTIATE_TEST_CASE_P(smoke_CUDA_TestsSelect_tacotron,
                        SelectLayerTest,
                        tacotronCases,
                        SelectLayerTest::getTestCaseName);
