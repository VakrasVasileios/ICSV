#pragma once

#include "icsv/detector/arch/architecture_data.hpp"

namespace icsv::utils {

using ArchData  = icsv::detector::arch::ArchData;
using BasesList = std::list<std::string>;

/**
 * @brief Parses the dependency graph and returns a list of
 * the base classes signature name
 *
 * @param arch The deserialized architecture data
 * @return BasesList A list of strings
 */
auto get_base_structures(const ArchData& arch) -> BasesList;

/**
 * @brief Checks if the given method name is a constructor, destructor or
 * operator oveloading method
 *
 * @param meth_name Method name
 * @param strct_name Structure/Class name
 * @return true if the method is a constructor, destructor or
 * operator.
 * @return false otherwise
 */
bool is_standard_class_func(const std::string& meth_name,
                            const std::string& strct_name);

}  // namespace icsv::utils