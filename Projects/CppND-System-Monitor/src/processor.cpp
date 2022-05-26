#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  Processor::Idle = LinuxParser::IdleJiffies("cpu");
  Processor::NonIdle = LinuxParser::ActiveJiffies("cpu");
  float Tot = Processor::Idle + Processor::NonIdle;
  float Prevtot = Processor::PrevIdle + Processor::PrevNonIdle;

  this->PrevIdle = this->Idle;
  this->PrevNonIdle = this->NonIdle;
  float usage_frac =
      abs((this->NonIdle - this->PrevNonIdle)) / abs(Tot - Prevtot);
  return usage_frac;
};
