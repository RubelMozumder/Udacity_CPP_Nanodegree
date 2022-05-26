#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  float PrevIdle{0};
  float PrevNonIdle{0};
  float Idle{0};
  float NonIdle{0};
};

#endif