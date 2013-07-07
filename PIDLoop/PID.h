#ifndef pid_h
#define pid_h

class PID {
  public:
    PID(int KP, int KI, int KD);
    void setGoal(int goal)
    int run(int value);
  private:
    void init();

    int _goal;
    int _proportional;
    int _integral;
    int _derivative;
    int _oldValue;
    int _oldError;
    const int _KP;
    const int _KI;
    const int _KD;
};

#endif
