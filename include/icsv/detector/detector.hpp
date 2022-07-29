#pragma once

#include "detector_manager.hpp"
#include "detector_report.hpp"
#include "report_center.hpp"
#include <iostream>
#include <json/json.h>
#include <string>

// TODO: Add interface documentation

using DetectorReport = icsv::detector::DetectorReport;

#define DETECTOR [](const Json::Value& arch) -> void
