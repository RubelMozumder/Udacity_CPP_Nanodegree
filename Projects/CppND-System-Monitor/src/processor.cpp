#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  Processor::Idle = LinuxParser::IdleJiffies("cpu");
  Processor::NonIdle = LinuxParser::ActiveJiffies("cpu");
  long Tot = Processor::Idle + Processor::NonIdle;
  long Prevtot = Processor::PrevIdle + Processor::PrevNonIdle;

  float usage_frac =
      abs((this->NonIdle - this->PrevNonIdle)) / (abs(Tot - Prevtot) * 1.0);

  this->PrevIdle = this->Idle;
  this->PrevNonIdle = this->NonIdle;
  return usage_frac;
};
