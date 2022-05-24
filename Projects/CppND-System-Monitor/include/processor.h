#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long PrevIdle{0};
  long PrevNonIdle{0};
  long Idle{0};
  long NonIdle{0};
};

#endif