#include "operation_proxy.hpp"

ADT OperationProxy::execute(const ADT& a, const ADT& b) const {
    return operation_->execute(a, b);
}

ADT OperationProxy::execute(const std::vector<ADT>& args) const {
    return operation_->execute(args);
}
