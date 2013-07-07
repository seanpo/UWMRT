#ifndef pid_h
#define pid_h

class PID {
  public:
    PID(int KP, int KI, int KD, int MAX, int MIN);
	
    void print();
    void setConstants(int KP, int KI, int KD);
    void setGoal(int goal);
	
    int run(int value);
  private:
    void init();
	
    int _goal;
    int _proportional;
    int _integral;
    int _derivative;
    int _oldValue;
    int _oldError;
    int _MAX;
    int _MIN;
	
    float _KP;
    float _KI;
    float _KD;
};

#endif
