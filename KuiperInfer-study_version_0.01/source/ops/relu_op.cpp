/**
 * create by l on 2023.4.23
 */

#include "ops/relu_op.hpp"
namespace kuiper_infer
{
    ReluOperator::ReluOperator(float thresh) : thresh_(thresh){

    }

    float ReluOperator::get_thresh() const{
        return thresh_;
    }
    void ReluOperator::set_thresh(const float &thresh){
        thresh_ = thresh;
    }


} // namespace kuiper_infer
