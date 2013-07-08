#ifndef pid_h
#define pid_h

class PID {
  public:
    PID(int KP, int KI, int KD, int WORKING_BAND);
	
    void print();
    void setConstants(int KP, int KI, int KD);
    void setGoal(int goal);
	
    int run(int value);
  private:
    void init();
	
    int _goal;
    int _oldError;
    int _WORKING_BAND;
	
    double _proportional;
    double _integral;
    double _derivative;
    float _KP;
    float _KI;
    float _KD;
};

#endif
