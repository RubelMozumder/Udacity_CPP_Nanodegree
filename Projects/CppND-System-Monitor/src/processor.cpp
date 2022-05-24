#include "processor.h"

#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization

float Processor::Utilization() {
  Processor::Idle = LinuxParser::IdleJiffies("cpu");
  Processor::NonIdle = LinuxParser::ActiveJiffies("cpu");
  long Tot = Processor::Idle + Processor::NonIdle;
  long Prevtot = Processor::PrevIdle + Processor::PrevNonIdle;

  this->PrevIdle = this->Idle;
  this->PrevNonIdle = this->NonIdle;
  float usage_frac =
      abs((this->NonIdle - this->PrevNonIdle)) / abs(Tot - Prevtot);
  return usage_frac;
};